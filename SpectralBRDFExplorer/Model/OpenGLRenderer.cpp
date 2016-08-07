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
                                   const OpenGLCamera& camera,
                                   std::string& error) {
    
    std::string errors;
    bool programLinked = openGLProgram.loadProgram(vertexShaderSource, fragmentShaderSource, errors);
    
    if(!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return programLinked;
    }
    
    //Set model data
    //TODO: parametrical or calculated.
    nearPlane = 0.1f;
    farPlane = 20.0f;
    modelCenter = glm::vec3(0.0, 0.0f, -5.0f);
    
    //Setup camera.
    openGLCamera = camera;
    openGLCamera.setSceneCenter(modelCenter);
            
    //Prepare uniform to be loaded in shaders.
    _mvLocation = glGetUniformLocation(openGLProgram.program, "mvMatrix");
    _mvpLocation = glGetUniformLocation(openGLProgram.program, "mvpMatrix");
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
    
    //Generate buffer.
    glGenBuffers(1, _vboIds);
    
    //Start buffer for vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
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
}

void OpenGLRenderer::update(float width, float height, double timeSinceLastUpdate) {
        
    //Projection matrix.
    float aspect = fabs(width / height);
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(65.0f), aspect, nearPlane, farPlane);
    
    //Modelview matrix.
    _mvMatrix = openGLCamera.lookAtMatrix();
    _mvMatrix = glm::translate(_mvMatrix, modelCenter);
    
    //Set inverse transpose matrix for normal.
    _normalMatrix = glm::inverseTranspose(_mvMatrix);
    
    //Set uniform modelviewprojection matrix.
    _mvpMatrix = projectionMatrix * _mvMatrix;
}

void OpenGLRenderer::draw() {
    
    //Set states.
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Install program.
    glUseProgram(openGLProgram.program);
    
    //Bind buffers.
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    
    //Enable vertex attribute.
    GLvoid* offset = (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat));
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, model.modelData().getStride(), 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, model.modelData().getStride(), offset);

    if(model.modelData().hasTexture()) {

        offset = (GLvoid *)((VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE) * sizeof(GLfloat));
        glEnableVertexAttribArray(VERTEX_TEXCOORDINATE_INDX);
        glVertexAttribPointer(VERTEX_TEXCOORDINATE_INDX, VERTEX_TEXCOORDINATE_SIZE, GL_FLOAT, GL_FALSE, model.modelData().getStride(), offset);
    }
    
    //Load uniforms.
    glUniformMatrix4fv(_mvLocation, 1, GL_FALSE, glm::value_ptr(_mvMatrix));
    glUniformMatrix4fv(_mvpLocation, 1, GL_FALSE, glm::value_ptr(_mvpMatrix));
    glUniformMatrix4fv(_normalLocation, 1, GL_FALSE, glm::value_ptr(_normalMatrix));
    
    glUniform3f(_lightPosition, 1.0, 1.0, 1.0);
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
    
    if(model.modelData().hasTexture()) {
        
        //Set uniform flag for texture active to true.
        glUniform1i(_textureActive, 1);
    } else {
        
        //Set uniform flag for texture active to false.
        glUniform1i(_textureActive, 0);
    }
    
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
    glDeleteBuffers(sizeof(_vboIds)/sizeof(_vboIds[0]), _vboIds);
}
