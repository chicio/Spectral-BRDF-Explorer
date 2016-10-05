//
//  OpenGLTexture.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/10/2016.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLTexture.hpp"

void OpenGLTexture::startTexture(GLenum target) {
    
    //Generate texture.
    glGenTextures(1, &(_textureId));
    
    //Bind texture.
    glBindTexture(target, _textureId);
}

void OpenGLTexture::setTextureParameters(GLenum target, std::vector<OpenGLTextureParameter> parametersList) {
    
    for (auto parameter : parametersList) {
        
        switch (parameter.parameterDataType) {
            case Float:
                glTexParameterf(target, parameter.parameterName, parameter.parameterValue.floatValue);
                break;
            case Int:
                glTexParameteri(target, parameter.parameterName, parameter.parameterValue.intValue);
                break;
            default:
                break;
        }
    }
}

bool OpenGLTexture::loadTexture(std::string textureFileName, std::vector<OpenGLTextureParameter> parametersList) {
    
    unsigned error;
    unsigned char* texturePixels;
    
    
    error = lodepng_decode32_file(&texturePixels, &textureWidth, &textureHeight, textureFileName.c_str());
    
    if (error) {
        
        printf("error %u: %s\n", error, lodepng_error_text(error));
        return error;
    }
    
    createTexture(parametersList,
                  GL_RGBA,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  texturePixels);
    
    //    //Create texture.
    //    startTexture(GL_TEXTURE_2D);
    //
    //    //Load texture pixels.
    //    glTexImage2D(GL_TEXTURE_2D,
    //                 0,
    //                 GL_RGBA,
    //                 textureWidth,
    //                 textureHeight,
    //                 0,
    //                 GL_RGBA,
    //                 GL_UNSIGNED_BYTE,
    //                 texturePixels);
    //
    //    //Set parameters.
    //    setTextureParameters(GL_TEXTURE_2D, parametersList);
    //
    return true;
}

void OpenGLTexture::createTexture(std::vector<OpenGLTextureParameter> parametersList,
                                  GLint internalformat,
                                  GLenum format,
                                  GLenum type,
                                  unsigned char* texturePixels) {
    
    //Create texture.
    startTexture(GL_TEXTURE_2D);
    
    //Load texture pixels.
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 internalformat,
                 textureWidth,
                 textureHeight,
                 0,
                 format,
                 type,
                 texturePixels);
    
    //Set parameters.
    setTextureParameters(GL_TEXTURE_2D, parametersList);
}

bool OpenGLTexture::loadCubeMapTexture(std::string left,
                                       std::string right,
                                       std::string up,
                                       std::string down,
                                       std::string front,
                                       std::string back,
                                       std::vector<OpenGLTextureParameter> parametersList) {
    
    //Create cubemap texture.
    startTexture(GL_TEXTURE_CUBE_MAP);
    
    unsigned error;
    
    unsigned char* texturePixels1;
    unsigned textureWidth;
    unsigned textureHeight;
    error = lodepng_decode32_file(&texturePixels1, &textureWidth, &textureHeight, left.c_str());
    
    //Load the cube face - Positive X
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                 0,
                 GL_RGBA,
                 textureWidth,
                 textureHeight,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texturePixels1);
    
    unsigned char* texturePixels2;
    error = lodepng_decode32_file(&texturePixels2, &textureWidth, &textureHeight, right.c_str());
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                 0,
                 GL_RGBA,
                 textureWidth,
                 textureHeight,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texturePixels2);
    
    unsigned char* texturePixels3;
    error = lodepng_decode32_file(&texturePixels3, &textureWidth, &textureHeight, up.c_str());
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                 0,
                 GL_RGBA,
                 textureWidth,
                 textureHeight,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texturePixels3);
    
    unsigned char* texturePixels4;
    error = lodepng_decode32_file(&texturePixels4, &textureWidth, &textureHeight, down.c_str());
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                 0,
                 GL_RGBA,
                 textureWidth,
                 textureHeight,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texturePixels4);
    
    unsigned char* texturePixels5;
    error = lodepng_decode32_file(&texturePixels5, &textureWidth, &textureHeight, front.c_str());
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                 0,
                 GL_RGBA,
                 textureWidth,
                 textureHeight,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texturePixels5);
    
    unsigned char* texturePixels6;
    error = lodepng_decode32_file(&texturePixels6, &textureWidth, &textureHeight, back.c_str());
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                 0,
                 GL_RGBA,
                 textureWidth,
                 textureHeight,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texturePixels6);
    
    //Set parameters.
    setTextureParameters(GL_TEXTURE_CUBE_MAP, parametersList);
    
    return true;
}
