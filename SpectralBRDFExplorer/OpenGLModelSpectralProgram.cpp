//
//  OpenGLRGBModelProgram.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 17/09/16.
//
//

#include "OpenGLModelSpectralProgram.hpp"

bool OpenGLModelSpectralProgram::startProgram(std::string& error) {
    
    std::string errors;
    std::string vertexShader = getFileContents(shadersBasePath + model->lighting + "SpectralVertex.vsh");
    std::string fragmentShader = getFileContents(shadersBasePath + model->lighting + "SpectralFragment.fsh");
    
    bool programLinked = loadProgram(vertexShader.c_str(), fragmentShader.c_str(), errors);
    
    if (!programLinked || !errors.empty()) {
        
        //Return error from program loading.
        error = errors;
        
        return false;
    }
    
    //Load standard uniform.
    loadModelBaseUniform();
    
    //Load spectral unifrom.
    _materialAmbientLocation = glGetUniformLocation(program, "surfaceMaterial.ambientPercentage");
    _materialDiffuseLocation = glGetUniformLocation(program, "surfaceMaterial.diffusePercentage");
    _materialSpecularLocation = glGetUniformLocation(program, "surfaceMaterial.specularPercentage");
    _materialSpecularExponentLocation = glGetUniformLocation(program, "surfaceMaterial.sh");
    _lightSpectrumLocation = glGetUniformLocation(program, "light.spectrum");
    _objectLocation = glGetUniformLocation(program, "surfaceMaterial.spectrum");

    //Load vertex buffer.
    loadModelVertexBufferObject();
    
    return true;
}

void OpenGLModelSpectralProgram::update(OpenGLCamera& openGLCamera, const glm::mat4& projectionMatrix) {
    
    //Main scene matrix.
    model->_modelViewMatrix = openGLCamera.lookAtMatrix() * model->_modelMatrix;
    model->_modelViewProjectionMatrix = projectionMatrix * model->_modelViewMatrix;
    model->_normalMatrix = glm::inverseTranspose(model->_modelViewMatrix);
}

void OpenGLModelSpectralProgram::draw() {
    
    glUseProgram(program);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    //Set texture unit for each sampler (even if not used).
    glUniform1i(shadowMapSamplerLoc, TEXTURE_UNIT_ID_0_SAMPLER);
    glUniform1i(textureSampler, TEXTURE_UNIT_ID_1_SAMPLER);

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
        
        glUniform1i(textureActive, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, model->openGLTexture._textureId);
    } else {
        
        //Set uniform flag for texture active to false.
        glUniform1i(textureActive, 0);
    }
    
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, glm::value_ptr(model->_modelViewMatrix));
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(model->_modelViewProjectionMatrix));
    glUniformMatrix4fv(mvpLightLocation, 1, GL_FALSE, glm::value_ptr(model->_modelViewProjectionLightMatrix));
    glUniformMatrix4fv(normalLocation, 1, GL_FALSE, glm::value_ptr(model->_normalMatrix));
    glUniform3f(viewPositionLocation,
                openGLCamera->eye.x + openGLCamera->eyeOffset.x,
                openGLCamera->eye.y + openGLCamera->eyeOffset.y,
                openGLCamera->eye.z + openGLCamera->eyeOffset.z);
    glUniform3f(lightDirection,
                Scene::instance().lightDirection.x,
                Scene::instance().lightDirection.y,
                Scene::instance().lightDirection.z);
    
    MaterialSpectral* material = static_cast<MaterialSpectral*>(model->getMaterial());
    
    glUniform1f(_materialAmbientLocation, material->ambientPercentage);
    glUniform1f(_materialDiffuseLocation, material->diffusePercentage);
    glUniform1f(_materialSpecularLocation, material->specularPercentage);
    glUniform1f(_materialSpecularExponentLocation, material->sh);
    glUniform1fv(_lightSpectrumLocation, SBEConfiguration::numberOfSamples, Illuminant::d65);
    glUniform1fv(_objectLocation, SBEConfiguration::numberOfSamples, material->spectrumSamples);
    
    glDrawArrays(GL_TRIANGLES, 0, model->modelData().getNumberOfVerticesToDraw());
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_NORMAL_INDX);
    
    if(model->modelData().hasTexture()) {
        
        glDisableVertexAttribArray(VERTEX_TEXCOORDINATE_INDX);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
