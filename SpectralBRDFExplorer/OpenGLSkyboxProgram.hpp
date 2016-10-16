//
//  OpenGLSkyboxProgram.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 16/10/2016.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLSkyboxProgram_hpp
#define OpenGLSkyboxProgram_hpp

#include <glm/gtc/type_ptr.hpp>

#include "OpenGLProgram.hpp"
#include "Model3D.hpp"

class OpenGLSkyboxProgram : public OpenGLProgram {
private:
    
    /// Skybox cubemap texture.
    OpenGLTexture skyboxTexture;
    
public:
    
    /// Skybox model view projection matrix shader location.
    GLint _skyboxmvpLocation;
    /// Skybox texture sampler shader location.
    GLint _skyBoxTextureSamplerLocation;
    /// Skybox 3D model.
    Model3D* skyboxModel;
    
    /*!
     Start a skybox program with its specific setup.
     
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

#endif /* OpenGLSkyboxProgram_hpp */
