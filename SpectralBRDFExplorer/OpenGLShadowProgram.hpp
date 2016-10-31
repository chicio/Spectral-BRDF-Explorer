//
//  OpenGLShadowProgram.hpp
//  SpectralBRDFExplorer
//
//  Created by Duroni Fabrizio on 25/10/2016.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLShadowProgram_hpp
#define OpenGLShadowProgram_hpp

#include <glm/gtc/type_ptr.hpp>

#include "OpenGLProgram.hpp"
#include "OpenGLFramebufferObject.hpp"
#include "Model3D.hpp"

class OpenGLShadowProgram : public OpenGLProgram {
public:
    
    OpenGLTexture shadowTexture;
    OpenGLFramebufferObject shadowDepthFramebufferObject;
    GLuint shadowMapBufferId;
    GLint _shadowMapMvpLoc;
    GLint _shadowMapMvpLightLoc;
    
    glm::vec3 lightDirection;
    std::vector<Model3D>* models;
    
    OpenGLShadowProgram() : OpenGLProgram() {}
    OpenGLShadowProgram(std::string aShadersBasePath) : OpenGLProgram(aShadersBasePath) {}
    
    /*!
     Start a shadow program with its specific setup.
     
     @param error error message to be return if the start fails.
     
     @returns true if the program starts correctly, else false.
     */
    bool startProgram(std::string& error);
    
    /*!
     Update models using the current camera and projection matrix.
     
     @param camera the current camera.
     @param projectionMatrix the projection matrix (it is usually a orthogonal matrix).
     */
    void update(OpenGLCamera& openGLCamera, const glm::mat4& projectionMatrix);
    
    /*
     Draw method.
     Contains all the OpenGL ES step need to draw a model
     in the draw renderer method.
     */
    void draw();
};

#endif /* OpenGLShadowProgram_hpp */
