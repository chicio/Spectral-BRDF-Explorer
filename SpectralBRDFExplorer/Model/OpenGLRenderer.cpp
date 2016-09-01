//
//  OpenGLRenderer.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include <iostream>
#include "OpenGLRenderer.hpp"

bool OpenGLRenderer::startRenderer(const char* vertexShaderSource,
                                   const char* fragmentShaderSource,
                                   const char* shadowMappingVertexShaderSource,
                                   const char* shadowMappingFragmentShaderSource,
                                   const OpenGLCamera& camera,
                                   std::string& error) {
    
    std::string errors;
    bool programLinked = openGLProgram.loadProgram(vertexShaderSource,
                                                   fragmentShaderSource,
                                                   errors);
    
    if(!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return programLinked;
    }

    programLinked = openGLShadowProgram.loadProgram(shadowMappingVertexShaderSource,
                                                    shadowMappingFragmentShaderSource,
                                                    errors);
    
    if(!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return programLinked;
    }
    
    //Set data
    //TODO: parametrical or calculated.
    nearPlane = 0.1f;
    farPlane = 100.0f;
    modelCenter = glm::vec3(0.0, 0.0f, -12.0f);
    lightPosition = glm::vec3(1.0, 1.0, 1.0);
    
    //Setup camera.
    openGLCamera = camera;
    openGLCamera.setSceneCenter(modelCenter);
            
    //Prepare uniform to be loaded in shaders.
    _mvLocation = glGetUniformLocation(openGLProgram.program, "mvMatrix");
    _mvpLocation = glGetUniformLocation(openGLProgram.program, "mvpMatrix");
    _mvpLightLocation = glGetUniformLocation(openGLProgram.program, "mvpLightMatrix");
    _normalLocation = glGetUniformLocation(openGLProgram.program, "normalMatrix");
    _lightPosition = glGetUniformLocation(openGLProgram.program, "light.position");
    _lightColor = glGetUniformLocation(openGLProgram.program, "light.color");
    _materialAmbient = glGetUniformLocation(openGLProgram.program, "surfaceMaterial.ka");
    _materialDiffuse = glGetUniformLocation(openGLProgram.program, "surfaceMaterial.kd");
    _materialSpecular = glGetUniformLocation(openGLProgram.program, "surfaceMaterial.ks");
    _materialSpecularExponent = glGetUniformLocation(openGLProgram.program, "surfaceMaterial.sh");
    _textureActive = glGetUniformLocation(openGLProgram.program, "textureActive");
    
    return programLinked;
}

void OpenGLRenderer::loadScene() {
    
    model = Scene::instance().model;
    cornellBoxModel = Scene::instance().cornellBoxModel;
    
    //CORNELL BOX 3D.
    glGenBuffers(1, &_vboIds[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 cornellBoxModel.modelData().getVerticesDataSize(),
                 cornellBoxModel.modelData().getVerticesData().data(),
                 GL_STATIC_DRAW);
    
    //MODEL 3D.
    glGenBuffers(1, &_vboIds[1]);
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 model.modelData().getVerticesDataSize(),
                 model.modelData().getVerticesData().data(),
                 GL_STATIC_DRAW);
    
    //Prepare texture.
    if(model.modelData().hasTexture()) {
        
        //Get uniform sampler location.
        //Done here because we don't know if the model has a texture.
        _textureSampler = glGetUniformLocation(openGLProgram.program, "textureSampler");
     
        //Generate texture.
        glGenTextures(1, &_textureId);
        
        //Bind texture.
        glBindTexture(GL_TEXTURE_2D, _textureId);
        
        //Load texture pixels.
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     model.modelData().getTextureWidth(),
                     model.modelData().getTextureHeight(),
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     model.modelData().getTexturePixels());
        
        //Set filtering.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    
    //SHADOW MAP.
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    shadowMapTextureWidth = 1024;
    shadowMapTextureHeight = 1024;
    
    //Get uniform location.
    _shadowMapMvpLoc = glGetUniformLocation(openGLShadowProgram.program, "mvpMatrix");
    _shadowMapMvpLightLoc = glGetUniformLocation(openGLShadowProgram.program, "mvpLightMatrix");
    _shadowMapSamplerLoc = glGetUniformLocation ( openGLShadowProgram.program, "shadowMapSampler" );
    
    //Setup texture
    glGenTextures(1, &shadowMapTextureId);
    glBindTexture(GL_TEXTURE_2D, shadowMapTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    //Load texture (no pixel because we will attach it to a framebuffer object).
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT24,
                 shadowMapTextureWidth,
                 shadowMapTextureHeight,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_UNSIGNED_INT,
                 NULL);
    
    //Bind default texture unit.
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //Get default framebuffer handle.
    GLint defaultFramebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);
    
    //Setup FBO.
    GLenum none = GL_NONE;
    glGenFramebuffers(1, &shadowMapBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapBufferId);
    glDrawBuffers(1, &none);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTextureId, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMapTextureId);
    
    if(GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        
        std::cout << "ERROR FRAMEBUFFER OBJECT " << glCheckFramebufferStatus(GL_FRAMEBUFFER);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
}

void OpenGLRenderer::update(float width, float height, double timeSinceLastUpdate) {
    
    //Projection matrix.
    float aspect = fabs(width / height);
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(65.0f), aspect, nearPlane, farPlane);
    
    //MODEL.
    //Modelview matrix.
    _mvMatrix = openGLCamera.lookAtMatrix();
    _mvMatrix = glm::translate(_mvMatrix, modelCenter);
    _normalMatrix = glm::inverseTranspose(_mvMatrix);
    _mvpMatrix = projectionMatrix * _mvMatrix;

    //CORNELL BOX.
    //Modelview matrix.
    _mvCornellBoxMatrix = openGLCamera.lookAtMatrix();
    _mvCornellBoxMatrix = glm::translate(_mvCornellBoxMatrix, modelCenter);
    _normalCornellBoxMatrix = glm::inverseTranspose(_mvCornellBoxMatrix);
    _mvpCornellBoxMatrix = projectionMatrix * _mvCornellBoxMatrix;
    
    /******** SHADOW MAP. *********/
    glm::mat4 biasMatrix(
                         0.5, 0.0, 0.0, 0.0,
                         0.0, 0.5, 0.0, 0.0,
                         0.0, 0.0, 0.5, 0.0,
                         0.5, 0.5, 0.5, 1.0
                         );
    
    _mvpLightMatrix = glm::lookAt(lightPosition, openGLCamera.center, openGLCamera.up);
    _mvpLightMatrix = glm::translate(_mvpLightMatrix, modelCenter);
    _mvpLightMatrix = projectionMatrix * _mvpLightMatrix;
    
    
    _mvpCornellBoxLightMatrix = glm::lookAt(lightPosition, openGLCamera.center, openGLCamera.up);
    _mvpCornellBoxLightMatrix = glm::translate(_mvpCornellBoxLightMatrix, modelCenter);
    _mvpCornellBoxLightMatrix = projectionMatrix * _mvpCornellBoxLightMatrix;
}

void OpenGLRenderer::drawScene() {
    
}

void OpenGLRenderer::draw() {
    
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    
    GLint defaultFramebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);
    
    /**************************/
    /********** SHADOW **********/
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapBufferId);
    glViewport(0, 0, shadowMapTextureWidth, shadowMapTextureHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);     // disable color rendering, only write to depth buffer
    glEnable(GL_POLYGON_OFFSET_FILL);     // reduce shadow rendering artifact
    glPolygonOffset( 5.0f, 100.0f);
    glUseProgram(openGLShadowProgram.program);

    /********* CORNELL BOX **********/
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);  //Bind buffers.
    glEnableVertexAttribArray(VERTEX_POS_INDX); //Enable vertex attribute.
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, cornellBoxModel.modelData().getStride(), 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX,
                          VERTEX_NORMAL_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          cornellBoxModel.modelData().getStride(),
                          (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
    
    //Load uniforms.
    glUniformMatrix4fv(_shadowMapMvpLightLoc, 1, GL_FALSE, glm::value_ptr(_mvpCornellBoxLightMatrix));
    
    //Draw model.
    glDrawArrays(GL_TRIANGLES, 0, cornellBoxModel.modelData().getNumberOfVerticesToDraw());
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /********************************/

    /********* MODEL 3D ********/
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[1]);  //Bind buffers.
    glEnableVertexAttribArray(VERTEX_POS_INDX); //Enable vertex attribute.
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, model.modelData().getStride(), 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX,
                          VERTEX_NORMAL_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          model.modelData().getStride(),
                          (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
    
    //Load uniforms.
    glUniformMatrix4fv(_shadowMapMvpLightLoc, 1, GL_FALSE, glm::value_ptr(_mvpLightMatrix));

    //Draw model.
    glDrawArrays(GL_TRIANGLES, 0, model.modelData().getNumberOfVerticesToDraw());
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    /**************************/
    /********** DRAW **********/
    glDisable( GL_POLYGON_OFFSET_FILL );
    glBindFramebuffer ( GL_FRAMEBUFFER, defaultFramebuffer );
    glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glViewport(0, 0, m_viewport[2], m_viewport[3]);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(openGLProgram.program);
    
    // Bind the shadow map texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMapTextureId);
    glUniform1i(_shadowMapSamplerLoc, TEXTURE_UNIT_ID_0_SAMPLER);
    
    /********* CORNELL BOX **********/
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);  //Bind buffers.
    glEnableVertexAttribArray(VERTEX_POS_INDX); //Enable vertex attribute.
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, cornellBoxModel.modelData().getStride(), 0);    
    glVertexAttribPointer(VERTEX_NORMAL_INDX,
                          VERTEX_NORMAL_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          cornellBoxModel.modelData().getStride(),
                          (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
    
    //Load uniforms.
    glUniformMatrix4fv(_mvLocation, 1, GL_FALSE, glm::value_ptr(_mvCornellBoxMatrix));
    glUniformMatrix4fv(_mvpLocation, 1, GL_FALSE, glm::value_ptr(_mvpCornellBoxMatrix));
    glUniformMatrix4fv(_mvpLightLocation, 1, GL_FALSE, glm::value_ptr(_mvpCornellBoxLightMatrix));
    glUniformMatrix4fv(_normalLocation, 1, GL_FALSE, glm::value_ptr(_normalCornellBoxMatrix));
    glUniform3f(_lightPosition, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform4f(_lightColor, 1.0, 1.0, 1.0, 1.0);
    glUniform1i(_textureActive, 0);
    glUniform4f(_materialAmbient,
                cornellBoxModel.getMaterial().ka.red,
                cornellBoxModel.getMaterial().ka.green,
                cornellBoxModel.getMaterial().ka.blue,
                cornellBoxModel.getMaterial().ka.alpha);
    glUniform4f(_materialDiffuse,
                cornellBoxModel.getMaterial().kd.red,
                cornellBoxModel.getMaterial().kd.green,
                cornellBoxModel.getMaterial().kd.blue,
                cornellBoxModel.getMaterial().kd.alpha);
    glUniform4f(_materialSpecular,
                cornellBoxModel.getMaterial().ks.red,
                cornellBoxModel.getMaterial().ks.green,
                cornellBoxModel.getMaterial().ks.blue,
                cornellBoxModel.getMaterial().ks.alpha);
    glUniform1f(_materialSpecularExponent, cornellBoxModel.getMaterial().sh);
    
    //Draw model.
    glDrawArrays(GL_TRIANGLES, 0, cornellBoxModel.modelData().getNumberOfVerticesToDraw());
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /********************************/
    
    /********* MODEL 3D ********/
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[1]);  //Bind buffers.
    glEnableVertexAttribArray(VERTEX_POS_INDX); //Enable vertex attribute.
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, model.modelData().getStride(), 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX,
                          VERTEX_NORMAL_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          model.modelData().getStride(),
                          (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));

    if(model.modelData().hasTexture()) {

        glEnableVertexAttribArray(VERTEX_TEXCOORDINATE_INDX);
        glVertexAttribPointer(VERTEX_TEXCOORDINATE_INDX,
                              VERTEX_TEXCOORDINATE_SIZE,
                              GL_FLOAT,
                              GL_FALSE,
                              model.modelData().getStride(),
                              (GLvoid *)((VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE) * sizeof(GLfloat)));
    }
    
    if(model.modelData().hasTexture()) {
        
        //Set uniform flag for texture active to true.
        glUniform1i(_textureActive, 1);
        
        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        
        // Set the sampler texture unit to 0 (pag. 256)
        glUniform1i(_textureSampler, TEXTURE_UNIT_ID_0_SAMPLER);
    } else {
        
        //Set uniform flag for texture active to false.
        glUniform1i(_textureActive, 0);
    }
    
    //Load uniforms.
    glUniformMatrix4fv(_mvLocation, 1, GL_FALSE, glm::value_ptr(_mvMatrix));
    glUniformMatrix4fv(_mvpLocation, 1, GL_FALSE, glm::value_ptr(_mvpMatrix));
    glUniformMatrix4fv(_mvpLightLocation, 1, GL_FALSE, glm::value_ptr(_mvpLightMatrix));
    glUniformMatrix4fv(_normalLocation, 1, GL_FALSE, glm::value_ptr(_normalMatrix));
    glUniform3f(_lightPosition, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform4f(_lightColor, 1.0, 1.0, 1.0, 1.0);
    glUniform4f(_materialAmbient,
                model.getMaterial().ka.red,
                model.getMaterial().ka.green,
                model.getMaterial().ka.blue,
                model.getMaterial().ka.alpha);
    glUniform4f(_materialDiffuse,
                model.getMaterial().kd.red,
                model.getMaterial().kd.green,
                model.getMaterial().kd.blue,
                model.getMaterial().kd.alpha);
    glUniform4f(_materialSpecular,
                model.getMaterial().ks.red,
                model.getMaterial().ks.green,
                model.getMaterial().ks.blue,
                model.getMaterial().ks.alpha);
    glUniform1f(_materialSpecularExponent, model.getMaterial().sh);
    /********************************/

    //Draw model.
    glDrawArrays(GL_TRIANGLES, 0, model.modelData().getNumberOfVerticesToDraw());
    
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    
    if(model.modelData().hasTexture()) {
        
        glDisableVertexAttribArray(VERTEX_TEXCOORDINATE_INDX);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::shutdown() {
    
    openGLProgram.deleteProgram();
    
    //Clear vertex buffer objects.
    glDeleteBuffers(sizeof(_vboIds)/sizeof(_vboIds[1]), _vboIds);
}
