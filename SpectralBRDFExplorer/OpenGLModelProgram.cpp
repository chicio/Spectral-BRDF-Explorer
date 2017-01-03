//
//  OpenGLModelProgram.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 03/01/2017.
//  
//

#include "OpenGLModelProgram.hpp"

void OpenGLModelProgram::loadModelBaseUniform() {
    
    mvLocation = glGetUniformLocation(program, "mvMatrix");
    mvpLocation = glGetUniformLocation(program, "mvpMatrix");
    mvpLightLocation = glGetUniformLocation(program, "mvpLightMatrix");
    normalLocation = glGetUniformLocation(program, "normalMatrix");
    viewPositionLocation = glGetUniformLocation(program, "viewPosition");
    lightDirection = glGetUniformLocation(program, "light.direction");
    shadowMapSamplerLoc = glGetUniformLocation(program, "shadowMapSampler");
    textureActive = glGetUniformLocation(program, "textureActive");
    textureSampler = glGetUniformLocation(program, "textureSampler");
}

void OpenGLModelProgram::loadModelVertexBufferObject() {
    
    glGenBuffers(1, &(model->_vboId));
    glBindBuffer(GL_ARRAY_BUFFER, model->_vboId);
    glBufferData(GL_ARRAY_BUFFER,
                 model->modelData().getVerticesDataSize(),
                 model->modelData().getVerticesData().data(),
                 GL_STATIC_DRAW);
}
