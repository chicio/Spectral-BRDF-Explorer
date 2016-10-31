//
//  OpenGLSkyboxProgram.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 16/10/2016.
//  
//

#include "OpenGLSkyboxProgram.hpp"

bool OpenGLSkyboxProgram::startProgram(std::string& error) {
    
    //Load Skybox programs.
    std::string errors;

    std::string skyboxVertexShader = getFileContents(shadersBasePath + "SkyboxVertex.vsh");
    std::string skyboxFragmentShader = getFileContents(shadersBasePath + "SkyboxFragment.fsh");

    bool programLinked = loadProgram(skyboxVertexShader.c_str(), skyboxFragmentShader.c_str(), errors);
    
    if (!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return false;
    }
    
    _skyboxmvpLocation = glGetUniformLocation(program, "mvpMatrix");
    _skyBoxTextureSamplerLocation = glGetUniformLocation(program, "skyboxSampler");
    
    glGenBuffers(1, &(skyboxModel->_vboId));
    glBindBuffer(GL_ARRAY_BUFFER, skyboxModel->_vboId);
    glBufferData(GL_ARRAY_BUFFER,
                 skyboxModel->modelData().getVerticesDataSize(),
                 skyboxModel->modelData().getVerticesData().data(),
                 GL_STATIC_DRAW);

    skyboxTexture.loadCubeMapTexture(textureBasePath + "left.png",
                                     textureBasePath + "right.png",
                                     textureBasePath + "up.png",
                                     textureBasePath + "down.png",
                                     textureBasePath + "front.png",
                                     textureBasePath + "back.png",
                                     {OpenGLTextureParameter(GL_TEXTURE_MIN_FILTER, Int, {.intValue = GL_NEAREST}),
                                      OpenGLTextureParameter(GL_TEXTURE_MAG_FILTER, Int, {.intValue = GL_NEAREST})});

    return true;
}

void OpenGLSkyboxProgram::update(OpenGLCamera& openGLCamera, const glm::mat4& projectionMatrix) {
    
    skyboxModel->_modelViewProjectionMatrix = projectionMatrix * openGLCamera.lookAtMatrix()* skyboxModel->_modelMatrix;
}

void OpenGLSkyboxProgram::draw() {
    
    glUseProgram(program);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_POLYGON_OFFSET_FILL);
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    
    glUniform1i(_skyBoxTextureSamplerLocation, TEXTURE_UNIT_ID_0_SAMPLER);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture._textureId);
    
    glBindBuffer(GL_ARRAY_BUFFER, skyboxModel->_vboId);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX,
                          VERTEX_POS_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          skyboxModel->modelData().getStride(),
                          0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX,
                          VERTEX_NORMAL_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          skyboxModel->modelData().getStride(),
                          (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
    
    glUniformMatrix4fv(_skyboxmvpLocation,
                       1,
                       GL_FALSE,
                       glm::value_ptr(skyboxModel->_modelViewProjectionMatrix));
    
    glDrawArrays(GL_TRIANGLES, 0, skyboxModel->modelData().getNumberOfVerticesToDraw());
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
