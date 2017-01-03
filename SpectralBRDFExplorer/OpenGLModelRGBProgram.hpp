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
#include <glm/gtc/matrix_inverse.hpp>

#include "OpenGLModelProgram.hpp"
#include "Scene.hpp"
#include "Model3D.hpp"

class OpenGLModelRGBProgram : public OpenGLModelProgram {
public:
    
    /// Light color.
    GLint lightColor;
    /// Ambiente component.
    GLint materialAmbient;
    /// Diffuse component.
    GLint materialDiffuse;
    /// Specular component.
    GLint materialSpecular;
    /// Specular exponent component.
    GLint materialSpecularExponent;
    
    /*!
     Constructor.
     */
    OpenGLModelRGBProgram() : OpenGLModelProgram() {}
    
    /*!
     Constructor.
     
     @param aShadersBasePath the shader path.
     */
    OpenGLModelRGBProgram(std::string aShadersBasePath) : OpenGLModelProgram(aShadersBasePath) {}
    
    /*!
     Start a rgb program with its specific setup.
     
     @param error error message to be retur if the start fails.

     @returns true if the program starts correctly, else false.
     */
    bool startProgram(std::string& error);
    
    /*!
     Update method.
     Ina model program the update function calculate 
     the new matrices needed to update the position of
     the object in scene (used in shaders).
     
     @param openGLCamera the current camera scene.
     @param projectionMatrix the projectionMatrix used for the current scene.
     */
    void update(OpenGLCamera& openGLCamera, const glm::mat4& projectionMatrix);
    
    /*
     Draw method.
     Contains all the OpenGL ES step need to draw a model 
     in the draw renderer method.
     */
    void draw();
};

#endif /* OpenGLRGBModelProgram_hpp */
