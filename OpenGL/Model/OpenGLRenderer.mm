//
//  OpenGLRenderer.cpp
//  OpenGL
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLRenderer.h"

bool OpenGLRenderer::startRenderer(const char* vertexShaderSource,
                                   const char* fragmentShaderSource,
                                   std::string& error) {
    
    std::string errors;
    bool programLinked = openGLProgram.loadProgram(vertexShaderSource, fragmentShaderSource, errors);
    
    if(!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return programLinked;
    }
    
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
    
    return programLinked;
}

void OpenGLRenderer::loadScene() {
    
    model = Scene::instance().model;
    
    //Generate buffer objects: one for vertex data and one for vertex indices.
    glGenBuffers(1, _vboIds);
    
    //Start buffer for vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 model.modelData().getVerticesDataSize(),
                 model.modelData().getVerticesData().data(),
                 GL_STATIC_DRAW);
}

void OpenGLRenderer::update(float width, float height, double timeSinceLastUpdate) {
    
    //Projection matrix.
    float aspect = fabs(width / height);
    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 10.0f);
    
    //Modelview matrix.
    _mvMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -5.0f);
    _mvMatrix = GLKMatrix4Rotate(_mvMatrix, _rotation, 0.0f, 1.0f, 0.0f);
    
    //Set inverse transpose matrix for normal.
    _normalMatrix = GLKMatrix4InvertAndTranspose(_mvMatrix, NULL);
    
    //Set uniform modelviewprojection matrix.
    _mvpMatrix = GLKMatrix4Multiply(projectionMatrix, _mvMatrix);
    
    _rotation += timeSinceLastUpdate * 0.5f;
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
    GLvoid *offset = (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat));
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, model.modelData().getStride(), 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, model.modelData().getStride(), offset);
    
    //Load uniforms.
    glUniformMatrix4fv(_mvLocation, 1, GL_FALSE, (GLfloat *)_mvMatrix.m);
    glUniformMatrix4fv(_mvpLocation, 1, GL_FALSE, (GLfloat *)_mvpMatrix.m);
    glUniformMatrix4fv(_normalLocation, 1, GL_FALSE, (GLfloat *)_normalMatrix.m);
    
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
    
    //Draw model.
    glDrawArrays(GL_TRIANGLES, 0, model.modelData().getNumberOfVerticesToDraw());
    
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::shutdown() {
    
    openGLProgram.deleteProgram();
    
    //Clear vertex buffer objects.
    glDeleteBuffers(sizeof(_vboIds)/sizeof(_vboIds[0]), _vboIds);
}
