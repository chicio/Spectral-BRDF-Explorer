//
//  OpenGLRGBModelProgram.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 17/09/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLRGBProgram_hpp
#define OpenGLRGBProgram_hpp

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif

#include <glm/gtc/type_ptr.hpp>

#include "OpenGLProgram.hpp"
#include "OpenGLCamera.hpp"
#include "Scene.hpp"
#include "Model3D.hpp"

class OpenGLRGBModelProgram : public OpenGLProgram {
public:
    
    GLint _mvLocation;
    GLint _mvpLocation;
    GLint _mvpLightLocation;
    GLint _normalLocation;
    GLint _viewPositionLocation;
    GLint _lightDirection;
    GLint _lightColor;
    GLint _materialAmbient;
    GLint _materialDiffuse;
    GLint _materialSpecular;
    GLint _materialSpecularExponent;
    GLuint _textureId;
    GLint _textureActive;
    GLint _textureSampler;
    GLint _shadowMapSamplerLoc;

    Model3D* model;
    OpenGLCamera* openGLCamera;
    OpenGLTexture* shadowTexture;
    
    /*!
     Start a rgb program with its specific setup.
     
     @param error error message to be retur if the start fails.

     @returns true if the program starts correctly, else false.
     */
    bool startProgram(std::string& error);
    
    /*
     Draw method.
     Contains all the opengl step need to draw a model.
     */
    void draw();
};

#endif /* OpenGLRGBProgram_hpp */
