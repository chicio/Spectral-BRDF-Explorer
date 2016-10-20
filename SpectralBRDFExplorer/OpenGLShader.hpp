//
//  OpenGLShader.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  
//

#ifndef OpenGLShader_hpp
#define OpenGLShader_hpp

#include <OpenGLESHeaders.h>
#include <stdio.h>
#include <cstdlib>
#include <string>

#define OPENGL_SHADER_ERROR -1

class OpenGLShader {
public:
    
    /*!
     Load a OpenGL ES shader.
     
     @param type type of shader to be loaded (Vertex or Fragment).
     @param shaderSource source code of the shader.
     @param error log returned if shader is not compiled succesfully.
     
     @returns handle for shader.
     */
    GLuint loadShader(const GLenum type, const char *shaderSource, std::string& error);
};

#endif /* OpenGLShader_hpp */
