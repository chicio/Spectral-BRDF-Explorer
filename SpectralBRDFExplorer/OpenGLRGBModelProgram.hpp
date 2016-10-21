//
//  OpenGLRGBModelProgram.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 17/09/16.
//  
//

#ifndef OpenGLRGBProgram_hpp
#define OpenGLRGBProgram_hpp

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
    GLint _textureActive;
    GLint _textureSampler;
    GLint _shadowMapSamplerLoc;

    Model3D* model;
    OpenGLCamera* openGLCamera;
    OpenGLTexture* shadowTexture;
    
    OpenGLRGBModelProgram() : OpenGLProgram() {}
    
    OpenGLRGBModelProgram(std::string aShadersBasePath) : OpenGLProgram(aShadersBasePath) {}
    
    /*!
     Start a rgb program with its specific setup.
     
     @param error error message to be retur if the start fails.

     @returns true if the program starts correctly, else false.
     */
    bool startProgram(std::string& error);
    
    /*
     Draw method.
     Contains all the OpenGL ES step need to draw a model 
     in the draw renderer method.
     */
    void draw();
};

#endif /* OpenGLRGBProgram_hpp */
