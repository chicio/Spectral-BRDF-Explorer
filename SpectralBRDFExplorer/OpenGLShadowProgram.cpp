//
//  OpenGLShadowProgram.cpp
//  SpectralBRDFExplorer
//
//  Created by Duroni Fabrizio on 25/10/2016.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLShadowProgram.hpp"

bool OpenGLShadowProgram::startProgram(std::string& error) {

    //Load Skybox programs.
    std::string errors;
    
    std::string shadowMappingVertexShader = getFileContents(shadersBasePath + "ShadowMapVertex.vsh");
    std::string shadowMappingFragmentShader = getFileContents(shadersBasePath + "ShadowMapFragment.fsh");
    
    bool programLinked = loadProgram(shadowMappingVertexShader.c_str(), shadowMappingFragmentShader.c_str(), errors);
    
    if (!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return false;
    }
    
    //Load shadow mapping uniform.
    _shadowMapMvpLoc = glGetUniformLocation(program, "mvpMatrix"); //shadow map
    _shadowMapMvpLightLoc = glGetUniformLocation(program, "mvpLightMatrix"); //shadow map
    
    //Shadow depth texture.
    shadowTexture.textureWidth = 1024;
    shadowTexture.textureHeight = 1024;
    shadowTexture.createTexture({
        OpenGLTextureParameter(GL_TEXTURE_MIN_FILTER, Int, {.intValue = GL_NEAREST}),
        OpenGLTextureParameter(GL_TEXTURE_MAG_FILTER, Int, {.intValue = GL_NEAREST}),
        OpenGLTextureParameter(GL_TEXTURE_WRAP_S, Int, {.intValue = GL_CLAMP_TO_EDGE}),
        OpenGLTextureParameter(GL_TEXTURE_WRAP_T, Int, {.intValue = GL_CLAMP_TO_EDGE}),
        OpenGLTextureParameter(GL_TEXTURE_COMPARE_MODE, Int, {.intValue = GL_COMPARE_REF_TO_TEXTURE}),
        OpenGLTextureParameter(GL_TEXTURE_COMPARE_FUNC, Int, {.intValue = GL_LEQUAL}),
    }, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
    
    //Setup shadow depth framebuffer object.
    bool framebufferCreation = shadowDepthFramebufferObject.attach2DTexture(shadowTexture._textureId,
                                                                            GL_DEPTH_ATTACHMENT,
                                                                            GL_NONE);
    
    if (!framebufferCreation) {
        
        error = "Framebuffer object creation error.";
        
        return false;
    }
    
    return true;
}

void OpenGLShadowProgram::update(OpenGLCamera& openGLCamera, const glm::mat4& projectionMatrix) {
    
    //Construct a new camera matrix using light position as point of view.
    glm::mat4 cameraMatrix = glm::lookAt(lightDirection,
                                         openGLCamera.center,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
    
    for (auto& currentModel : *models) {
        
        //Update shadow map matrix using camera defined below and projection matrix received as input.
        currentModel._modelViewProjectionLightMatrix = projectionMatrix * cameraMatrix * currentModel._modelMatrix;
    }
}

void OpenGLShadowProgram::draw() {
    
    glUseProgram(program);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL); // reduce shadow rendering artifact
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // disable color rendering, only write to depth buffer
    glPolygonOffset(5.0f, 100.0f);
    
    for (auto& currentModel : *models) {
        
        glBindBuffer(GL_ARRAY_BUFFER, currentModel._vboId);
        glEnableVertexAttribArray(VERTEX_POS_INDX);
        glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
        glVertexAttribPointer(VERTEX_POS_INDX,
                              VERTEX_POS_SIZE,
                              GL_FLOAT,
                              GL_FALSE,
                              currentModel.modelData().getStride(),
                              0);
        glVertexAttribPointer(VERTEX_NORMAL_INDX,
                              VERTEX_NORMAL_SIZE,
                              GL_FLOAT,
                              GL_FALSE,
                              currentModel.modelData().getStride(),
                              (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
        
        //Load uniforms.
        glUniformMatrix4fv(_shadowMapMvpLightLoc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(currentModel._modelViewProjectionLightMatrix));
        
        //Draw model.
        glDrawArrays(GL_TRIANGLES, 0, currentModel.modelData().getNumberOfVerticesToDraw());
        glDisableVertexAttribArray(VERTEX_POS_INDX);
        glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
