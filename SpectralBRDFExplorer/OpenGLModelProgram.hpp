//
//  OpenGLRGBModelProgram.hpp
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
    
    GLint _mvLocation;
    GLint _mvpLocation;
    GLint _mvpLightLocation;
    GLint _normalLocation;
    GLint _viewPositionLocation;
    GLint _lightDirection;
    
    GLint _shadowMapSamplerLoc;

    Model3D* model;
    OpenGLCamera* openGLCamera;
    OpenGLTexture* shadowTexture;
    
    OpenGLModelProgram() : OpenGLProgram() {}
    
    OpenGLModelProgram(std::string aShadersBasePath) : OpenGLProgram(aShadersBasePath) {}
};

#endif /* OpenGLModelProgram_hpp */
