//
//  OpenGLRGBModelProgram.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 17/09/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLRGBModelProgram.hpp"

bool OpenGLRGBModelProgram::startProgram(std::string& error) {
    
    std::string errors;
    std::string vertexShader = getFileContents(model->lighting + "Vertex.vsh");
    std::string fragmentShader = getFileContents(model->lighting + "Fragment.fsh");
    
    bool programLinked = loadProgram(vertexShader.c_str(), fragmentShader.c_str(), errors);
    
    if (!programLinked || !errors.empty()) {
        
        //Return error from program loading.
        error = errors;
        
        return false;
    }
    
    //Prepare uniform to be loaded in shaders.
    _mvLocation = glGetUniformLocation(program, "mvMatrix");
    _mvpLocation = glGetUniformLocation(program, "mvpMatrix");
    _mvpLightLocation = glGetUniformLocation(program, "mvpLightMatrix");
    _normalLocation = glGetUniformLocation(program, "normalMatrix");
    _viewPositionLocation = glGetUniformLocation(program, "viewPosition");
    _lightDirection = glGetUniformLocation(program, "light.direction");
    _lightColor = glGetUniformLocation(program, "light.color");
    _materialAmbient = glGetUniformLocation(program, "surfaceMaterial.ka");
    _materialDiffuse = glGetUniformLocation(program, "surfaceMaterial.kd");
    _materialSpecular = glGetUniformLocation(program, "surfaceMaterial.ks");
    _materialSpecularExponent = glGetUniformLocation(program, "surfaceMaterial.sh");
    _textureActive = glGetUniformLocation(program, "textureActive");
    _textureSampler = glGetUniformLocation(program, "textureSampler");
    _shadowMapSamplerLoc = glGetUniformLocation(program, "shadowMapSampler");
    
    glGenBuffers(1, &(model->_vboId));
    glBindBuffer(GL_ARRAY_BUFFER, model->_vboId);
    glBufferData(GL_ARRAY_BUFFER,
                 model->modelData().getVerticesDataSize(),
                 model->modelData().getVerticesData().data(),
                 GL_STATIC_DRAW);
    
    //Prepare texture.
    if (model->modelData().hasTexture()) {
        
        model->loadTexture();
    }
    
    return true;
}

void OpenGLRGBModelProgram::draw() {
 
    glUseProgram(program);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    //Set texture unit for each sampler (even if not used).
    glUniform1i(_shadowMapSamplerLoc, TEXTURE_UNIT_ID_0_SAMPLER);
    glUniform1i(_textureSampler, TEXTURE_UNIT_ID_1_SAMPLER);
    
    // Bind the shadow map texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowTexture->_textureId);
    
    glBindBuffer(GL_ARRAY_BUFFER, model->_vboId);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, model->modelData().getStride(), 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX,
                          VERTEX_NORMAL_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          model->modelData().getStride(),
                          (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
    
    if (model->modelData().hasTexture()) {
        
        glEnableVertexAttribArray(VERTEX_TEXCOORDINATE_INDX);
        glVertexAttribPointer(VERTEX_TEXCOORDINATE_INDX,
                              VERTEX_TEXCOORDINATE_SIZE,
                              GL_FLOAT,
                              GL_FALSE,
                              model->modelData().getStride(),
                              (GLvoid *)((VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE) * sizeof(GLfloat)));
        
        glUniform1i(_textureActive, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, model->openGLTexture._textureId);
    } else {
        
        //Set uniform flag for texture active to false.
        glUniform1i(_textureActive, 0);
    }
    
    glUniformMatrix4fv(_mvLocation, 1, GL_FALSE, glm::value_ptr(model->_modelViewMatrix));
    glUniformMatrix4fv(_mvpLocation, 1, GL_FALSE, glm::value_ptr(model->_modelViewProjectionMatrix));
    glUniformMatrix4fv(_mvpLightLocation, 1, GL_FALSE, glm::value_ptr(model->_modelViewProjectionLightMatrix));
    glUniformMatrix4fv(_normalLocation, 1, GL_FALSE, glm::value_ptr(model->_normalMatrix));
    glUniform3f(_viewPositionLocation,
                openGLCamera->eye.x + openGLCamera->eyeOffset.x,
                openGLCamera->eye.y + openGLCamera->eyeOffset.y,
                openGLCamera->eye.z + openGLCamera->eyeOffset.z);
    glUniform3f(_lightDirection,
                Scene::instance().lightDirection.x,
                Scene::instance().lightDirection.y,
                Scene::instance().lightDirection.z);
    glUniform4f(_lightColor, 1.0, 1.0, 1.0, 1.0);
    glUniform4f(_materialAmbient,
                model->getMaterial().ka.red,
                model->getMaterial().ka.green,
                model->getMaterial().ka.blue,
                model->getMaterial().ka.alpha);
    glUniform4f(_materialDiffuse,
                model->getMaterial().kd.red,
                model->getMaterial().kd.green,
                model->getMaterial().kd.blue,
                model->getMaterial().kd.alpha);
    glUniform4f(_materialSpecular,
                model->getMaterial().ks.red,
                model->getMaterial().ks.green,
                model->getMaterial().ks.blue,
                model->getMaterial().ks.alpha);
    glUniform1f(_materialSpecularExponent, model->getMaterial().sh);
    
    glDrawArrays(GL_TRIANGLES, 0, model->modelData().getNumberOfVerticesToDraw());
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    
    if(model->modelData().hasTexture()) {
        
        glDisableVertexAttribArray(VERTEX_TEXCOORDINATE_INDX);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
