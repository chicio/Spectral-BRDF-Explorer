//
//  OpenGLRGBProgram.hpp
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

#include "OpenGLProgram.hpp"

class OpenGLRGBProgram : public OpenGLProgram {
public:
    
    GLint _mvLocation;
    GLint _mvpLocation;
    GLint _mvpLightLocation;
    
    GLint _normalLocation;
    
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
};

#endif /* OpenGLRGBProgram_hpp */
