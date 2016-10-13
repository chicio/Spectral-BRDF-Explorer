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
        
        std::string vertexShader = getFileContents(currentModel.lighting + "Vertex.vsh");
        std::string fragmentShader = getFileContents(currentModel.lighting + "Fragment.fsh");
        
        programLinked = currentModel.openGLModelProgram.loadProgram(vertexShader.c_str(), fragmentShader.c_str(), errors);
        
        if(!programLinked || !errors.empty()) {
            
            //Return error from program loading.
            error = errors;
            
            return false;
        }
        
        //Prepare uniform to be loaded in shaders.
        currentModel.openGLModelProgram._mvLocation = glGetUniformLocation(currentModel.openGLModelProgram.program, "mvMatrix");
        currentModel.openGLModelProgram._mvpLocation = glGetUniformLocation(currentModel.openGLModelProgram.program, "mvpMatrix");
        currentModel.openGLModelProgram._mvpLightLocation = glGetUniformLocation(currentModel.openGLModelProgram.program, "mvpLightMatrix");
        currentModel.openGLModelProgram._normalLocation = glGetUniformLocation(currentModel.openGLModelProgram.program, "normalMatrix");
        currentModel.openGLModelProgram._viewPositionLocation = glGetUniformLocation(currentModel.openGLModelProgram.program, "viewPosition");
        currentModel.openGLModelProgram._lightDirection = glGetUniformLocation(currentModel.openGLModelProgram.program, "light.direction");
        currentModel.openGLModelProgram._lightColor = glGetUniformLocation(currentModel.openGLModelProgram.program, "light.color");
        currentModel.openGLModelProgram._materialAmbient = glGetUniformLocation(currentModel.openGLModelProgram.program, "surfaceMaterial.ka");
        currentModel.openGLModelProgram._materialDiffuse = glGetUniformLocation(currentModel.openGLModelProgram.program, "surfaceMaterial.kd");
        currentModel.openGLModelProgram._materialSpecular = glGetUniformLocation(currentModel.openGLModelProgram.program, "surfaceMaterial.ks");
        currentModel.openGLModelProgram._materialSpecularExponent = glGetUniformLocation(currentModel.openGLModelProgram.program, "surfaceMaterial.sh");
        currentModel.openGLModelProgram._textureActive = glGetUniformLocation(currentModel.openGLModelProgram.program, "textureActive");
        currentModel.openGLModelProgram._textureSampler = glGetUniformLocation(currentModel.openGLModelProgram.program, "textureSampler");
        currentModel.openGLModelProgram._shadowMapSamplerLoc = glGetUniformLocation(currentModel.openGLModelProgram.program, "shadowMapSampler");
        
        glGenBuffers(1, &(currentModel._vboId));
        glBindBuffer(GL_ARRAY_BUFFER, currentModel._vboId);
        glBufferData(GL_ARRAY_BUFFER,
                     currentModel.modelData().getVerticesDataSize(),
                     currentModel.modelData().getVerticesData().data(),
                     GL_STATIC_DRAW);
        
        //Prepare texture.
        if(currentModel.modelData().hasTexture()) {
            
            currentModel.loadTexture();
        }
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
    glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthFramebufferObject.framebufferObjectId);
    glViewport(0, 0, shadowTexture.textureWidth, shadowTexture.textureHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);     // disable color rendering, only write to depth buffer
    glEnable(GL_POLYGON_OFFSET_FILL);     // reduce shadow rendering artifact
    glPolygonOffset(5.0f, 100.0f);
    glUseProgram(openGLShadowProgram.program);
    
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
    glDisable(GL_POLYGON_OFFSET_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glViewport(0, 0, m_viewport[2], m_viewport[3]);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /********/
    glDepthMask(GL_FALSE);
    glUseProgram(openGLSkyboxProgram.program);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
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

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    /********/

    for (auto& currentModel : Scene::instance().models) {
        
        glUseProgram(currentModel.openGLModelProgram.program);
        
        //Set texture unit for each sampler (even if not used).
        glUniform1i(currentModel.openGLModelProgram._shadowMapSamplerLoc, TEXTURE_UNIT_ID_0_SAMPLER);
        glUniform1i(currentModel.openGLModelProgram._textureSampler, TEXTURE_UNIT_ID_1_SAMPLER);
        
        // Bind the shadow map texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowTexture._textureId);
        
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
        
        if (currentModel.modelData().hasTexture()) {
            
            glEnableVertexAttribArray(VERTEX_TEXCOORDINATE_INDX);
            glVertexAttribPointer(VERTEX_TEXCOORDINATE_INDX,
                                  VERTEX_TEXCOORDINATE_SIZE,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  currentModel.modelData().getStride(),
                                  (GLvoid *)((VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE) * sizeof(GLfloat)));
            
            glUniform1i(currentModel.openGLModelProgram._textureActive, 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, currentModel.openGLTexture._textureId);
        } else {
            
            //Set uniform flag for texture active to false.
            glUniform1i(currentModel.openGLModelProgram._textureActive, 0);
        }
        
        glUniformMatrix4fv(currentModel.openGLModelProgram._mvLocation, 1, GL_FALSE, glm::value_ptr(currentModel._modelViewMatrix));
        glUniformMatrix4fv(currentModel.openGLModelProgram._mvpLocation, 1, GL_FALSE, glm::value_ptr(currentModel._modelViewProjectionMatrix));
        glUniformMatrix4fv(currentModel.openGLModelProgram._mvpLightLocation, 1, GL_FALSE, glm::value_ptr(currentModel._modelViewProjectionLightMatrix));
        glUniformMatrix4fv(currentModel.openGLModelProgram._normalLocation, 1, GL_FALSE, glm::value_ptr(currentModel._normalMatrix));
        glUniform3f(currentModel.openGLModelProgram._viewPositionLocation,
                    openGLCamera.eye.x + openGLCamera.eyeOffset.x,
                    openGLCamera.eye.y + openGLCamera.eyeOffset.y,
                    openGLCamera.eye.z + openGLCamera.eyeOffset.z);
        glUniform3f(currentModel.openGLModelProgram._lightDirection,
                    Scene::instance().lightDirection.x,
                    Scene::instance().lightDirection.y,
                    Scene::instance().lightDirection.z);
        glUniform4f(currentModel.openGLModelProgram._lightColor, 1.0, 1.0, 1.0, 1.0);
        glUniform4f(currentModel.openGLModelProgram._materialAmbient,
                    currentModel.getMaterial().ka.red,
                    currentModel.getMaterial().ka.green,
                    currentModel.getMaterial().ka.blue,
                    currentModel.getMaterial().ka.alpha);
        glUniform4f(currentModel.openGLModelProgram._materialDiffuse,
                    currentModel.getMaterial().kd.red,
                    currentModel.getMaterial().kd.green,
                    currentModel.getMaterial().kd.blue,
                    currentModel.getMaterial().kd.alpha);
        glUniform4f(currentModel.openGLModelProgram._materialSpecular,
                    currentModel.getMaterial().ks.red,
                    currentModel.getMaterial().ks.green,
                    currentModel.getMaterial().ks.blue,
                    currentModel.getMaterial().ks.alpha);
        glUniform1f(currentModel.openGLModelProgram._materialSpecularExponent, currentModel.getMaterial().sh);
        
        glDrawArrays(GL_TRIANGLES, 0, currentModel.modelData().getNumberOfVerticesToDraw());
        glDisableVertexAttribArray(VERTEX_POS_INDX);
        glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
        
        if(currentModel.modelData().hasTexture()) {
            
            glDisableVertexAttribArray(VERTEX_TEXCOORDINATE_INDX);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void OpenGLRenderer::shutdown() {
    
    openGLShadowProgram.deleteProgram();
    
    //Clear vertex buffer objects.
    GLuint vbo[Scene::instance().models.size()];
    int i = 0;
    
    for (auto model : Scene::instance().models) {
        
        vbo[i++] = model._vboId;
        model.openGLModelProgram.deleteProgram();
    }
    
    glDeleteBuffers((int)Scene::instance().models.size(), vbo);
}
