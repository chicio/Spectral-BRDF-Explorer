//
//  OpenGLTexture.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/10/2016.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLTexture_hpp
#define OpenGLTexture_hpp

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif

#include <string>
#include <vector>

#include "lodepng.h"

class OpenGLTexture {
public:
    
    /// OpenGL Texture id.
    GLuint _textureId;
    /// Texture width.
    unsigned textureWidth;
    //// Texture height.
    unsigned textureHeight;
    
    void createTexture(GLenum target) {
        
        //Generate texture.
        glGenTextures(1, &(_textureId));
        
        //Bind texture.
        glBindTexture(target, _textureId);
    }
    
    bool loadTexture(std::string textureFileName) {
        
        unsigned error;
        unsigned char* texturePixels;
        
        error = lodepng_decode32_file(&texturePixels, &textureWidth, &textureHeight, textureFileName.c_str());
        
        if(error) {
            
            printf("error %u: %s\n", error, lodepng_error_text(error));
            return error;
        }
        
        //Create texture.
        createTexture(GL_TEXTURE_2D);
        
        //Load texture pixels.
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     textureWidth,
                     textureHeight,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     texturePixels);
        
        //Set filtering.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        return true;
    }
    
    bool loadCubeMapTexture(std::string left,
                            std::string right,
                            std::string up,
                            std::string down,
                            std::string front,
                            std::string back) {
        
        //Create cubemap texture.
        createTexture(GL_TEXTURE_CUBE_MAP);
        
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
        
        // Set the filtering mode
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        return true;
    }
};


#endif /* OpenGLTexture_hpp */
