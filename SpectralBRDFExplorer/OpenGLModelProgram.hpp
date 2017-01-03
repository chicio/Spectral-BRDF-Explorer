//
//  OpenGLModelProgram.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 17/09/16.
//  
//

#ifndef OpenGLModelProgram_hpp
#define OpenGLModelProgram_hpp

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "OpenGLProgram.hpp"
#include "Scene.hpp"
#include "Model3D.hpp"

class OpenGLModelProgram : public OpenGLProgram {
public:
    
    /// Model view matrix uniform location.
    GLint mvLocation;
    /// Model view projection matrix uniform location.
    GLint mvpLocation;
    /// Model view projection matrix from light view point uniform location.
    GLint mvpLightLocation;
    /// Normal uniform location.
    GLint normalLocation;
    /// View position uniform location.
    GLint viewPositionLocation;
    /// Light direction uniform location.
    GLint lightDirection;
    /// Texture active flag uniform location.
    GLint textureActive;
    /// Texture sampler uniform location.
    GLint textureSampler;
    /// Shadow map sampler uniform location.
    GLint shadowMapSamplerLoc;

    /// Model reference.
    Model3D* model;
    /// Scene camera.
    OpenGLCamera* openGLCamera;
    /// Shadow texture.
    OpenGLTexture* shadowTexture;
    
    /*!
     Constructor.
     */
    OpenGLModelProgram() : OpenGLProgram() {}
    
    /*!
     Constructor.
     
     @param aShadersBasePath shader base path.
     */
    OpenGLModelProgram(std::string aShadersBasePath) : OpenGLProgram(aShadersBasePath) {}
    
    /*!
     Load common uniforms to all type of model program.
     Declare as property in this base class.
     */
    void loadModelBaseUniform();
    
    /*!
     Load model vertex VBO.
     */
    void loadModelVertexBufferObject();
};

#endif /* OpenGLModelProgram_hpp */
