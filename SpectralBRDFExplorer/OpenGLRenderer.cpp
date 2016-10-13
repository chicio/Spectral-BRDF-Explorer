//
//  OpenGLRenderer.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include <iostream>

#include "Utils.hpp"
#include "OpenGLRenderer.hpp"

bool OpenGLRenderer::startRenderer(const OpenGLCamera& camera, std::string& error) {
    
    //Setup camera.
    openGLCamera = camera;
    openGLCamera.setSceneCenter(Scene::instance().sceneCenter);
    
    bool programLinked;
    std::string errors;
    
    //Load models programs.
    for (auto& currentModel : Scene::instance().models) {
        
        //Create program for model.
        OpenGLRGBModelProgram modelProgram;
        modelProgram.model = &currentModel;
        modelProgram.openGLCamera = &openGLCamera;
        
        programLinked = modelProgram.startProgram(errors);
        
        if (!programLinked || !errors.empty()) {
            
            //Return error from program loading.
            error = errors;
            
            return false;
        }
        
        //Add program to vector of program execution.
        openGLModelPrograms.push_back(modelProgram);
    }
    
    /********/
    //Load Skybox programs.
    std::string skyboxVertexShader = getFileContents("SkyboxVertex.vsh");
    std::string skyboxFragmentShader = getFileContents("SkyboxFragment.fsh");
    programLinked = openGLSkyboxProgram.loadProgram(skyboxVertexShader.c_str(),
                                                    skyboxFragmentShader.c_str(),
                                                    errors);
    
    if (!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return false;
    }
    
    _skyboxmvpLocation = glGetUniformLocation(openGLSkyboxProgram.program, "mvpMatrix");
    _skyBoxTextureSampler = glGetUniformLocation(openGLSkyboxProgram.program, "skyboxSampler");
    
    glGenBuffers(1, &(Scene::instance().skybox._vboId));
    glBindBuffer(GL_ARRAY_BUFFER, Scene::instance().skybox._vboId);
    glBufferData(GL_ARRAY_BUFFER,
                 Scene::instance().skybox.modelData().getVerticesDataSize(),
                 Scene::instance().skybox.modelData().getVerticesData().data(),
                 GL_STATIC_DRAW);
    
    skyboxTexture.loadCubeMapTexture("left.png",
                                     "right.png",
                                     "up.png",
                                     "down.png",
                                     "front.png",
                                     "back.png",
                                     {OpenGLTextureParameter(GL_TEXTURE_MIN_FILTER, Int, {.intValue = GL_NEAREST}),
                                      OpenGLTextureParameter(GL_TEXTURE_MAG_FILTER, Int, {.intValue = GL_NEAREST})});
    /********/
    
    //Load Shadow mapping programs.
    std::string shadowMappingVertexShader = getFileContents("ShadowMapVertex.vsh");
    std::string shadowMappingFragmentShader = getFileContents("ShadowMapFragment.fsh");
    programLinked = openGLShadowProgram.loadProgram(shadowMappingVertexShader.c_str(),
                                                    shadowMappingFragmentShader.c_str(),
                                                    errors);
    
    if (!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return false;
    }
    
    //Load shadow mapping uniform.
    _shadowMapMvpLoc = glGetUniformLocation(openGLShadowProgram.program, "mvpMatrix"); //shadow map
    _shadowMapMvpLightLoc = glGetUniformLocation(openGLShadowProgram.program, "mvpLightMatrix"); //shadow map
    
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
    shadowDepthFramebufferObject.attach2DTexture(shadowTexture._textureId, GL_DEPTH_ATTACHMENT);
    
    return true;
}

void OpenGLRenderer::update(float width, float height, double timeSinceLastUpdate) {
    
    //Projection matrix.
    float aspect = fabs(width / height);
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(65.0f),
                                                  aspect,
                                                  Scene::instance().nearPlane,
                                                  Scene::instance().farPlane);
    
    glm::mat4 orthoMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
    
    for (auto& currentModel : Scene::instance().models) {
        
        //Main scene matrix.
        currentModel._modelViewMatrix = openGLCamera.lookAtMatrix() * currentModel._modelMatrix;
        currentModel._modelViewProjectionMatrix = projectionMatrix * currentModel._modelViewMatrix;
        currentModel._normalMatrix = glm::inverseTranspose(currentModel._modelViewMatrix);
        
        //Shadow map matrix.
        currentModel._modelViewProjectionLightMatrix = orthoMatrix * glm::lookAt(Scene::instance().lightDirection,
                                                                                 openGLCamera.center,
                                                                                 glm::vec3(0.0f, 1.0f, 0.0f)) * currentModel._modelMatrix;
    }
    
    Scene::instance().skybox._modelViewProjectionMatrix = projectionMatrix * openGLCamera.lookAtMatrix() * Scene::instance().skybox._modelMatrix;
}

void OpenGLRenderer::draw() {
    
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    
    GLint defaultFramebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);
    
    /**************************/
    /********** SHADOW **********/
    glUseProgram(openGLShadowProgram.program);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthFramebufferObject.framebufferObjectId);
    glViewport(0, 0, shadowTexture.textureWidth, shadowTexture.textureHeight);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);     // reduce shadow rendering artifact
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);     // disable color rendering, only write to depth buffer
    glPolygonOffset(5.0f, 100.0f);
    
    for (auto& currentModel : Scene::instance().models) {
        
        glBindBuffer(GL_ARRAY_BUFFER, currentModel._vboId);
        glEnableVertexAttribArray(VERTEX_POS_INDX);
        glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
        glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, currentModel.modelData().getStride(), 0);
        glVertexAttribPointer(VERTEX_NORMAL_INDX,
                              VERTEX_NORMAL_SIZE,
                              GL_FLOAT,
                              GL_FALSE,
                              currentModel.modelData().getStride(),
                              (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
        
        //Load uniforms.
        glUniformMatrix4fv(_shadowMapMvpLightLoc, 1, GL_FALSE, glm::value_ptr(currentModel._modelViewProjectionLightMatrix));
        
        //Draw model.
        glDrawArrays(GL_TRIANGLES, 0, currentModel.modelData().getNumberOfVerticesToDraw());
        glDisableVertexAttribArray(VERTEX_POS_INDX);
        glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    /**************************/
    /********** DRAW **********/
    glUseProgram(openGLSkyboxProgram.program);

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glViewport(0, 0, m_viewport[2], m_viewport[3]);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_POLYGON_OFFSET_FILL);
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);

    glUniform1i(_skyBoxTextureSampler, TEXTURE_UNIT_ID_0_SAMPLER);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture._textureId);

    glBindBuffer(GL_ARRAY_BUFFER, Scene::instance().skybox._vboId);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX,
                          VERTEX_POS_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          Scene::instance().skybox.modelData().getStride(),
                          0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX,
                          VERTEX_NORMAL_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          Scene::instance().skybox.modelData().getStride(),
                          (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
    
    glUniformMatrix4fv(_skyboxmvpLocation,
                       1,
                       GL_FALSE,
                       glm::value_ptr(Scene::instance().skybox._modelViewProjectionMatrix));
    
    glDrawArrays(GL_TRIANGLES, 0, Scene::instance().skybox.modelData().getNumberOfVerticesToDraw());
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    //Draw models.
    for (auto& modelProgram : openGLModelPrograms) {
        
        //Set shadow texture.
        modelProgram.shadowTexture = &shadowTexture;

        //Draw current model.
        modelProgram.draw();
    }
}

void OpenGLRenderer::shutdown() {
    
    openGLShadowProgram.deleteProgram();
    
    //Clear vertex buffer objects.
    GLuint vbo[Scene::instance().models.size()];
    int i = 0;
    
    for (auto& program : openGLModelPrograms) {
        
        vbo[i++] = program.model->_vboId;
        program.deleteProgram();
    }
    
    glDeleteBuffers((int)Scene::instance().models.size(), vbo);
}
