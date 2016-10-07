//
//  OpenGLProgram.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLProgram_hpp
#define OpenGLProgram_hpp

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>

#include "OpenGLShader.hpp"

class OpenGLProgram {
public:
    
    /// Program handle.
    GLuint program;
    /// Vertex shader handle.
    GLuint vertexShader;
    /// Fragment shader handle.
    GLuint fragmentShader;
        
    /*!
     Load a OpenGL ES program with its shaders and option.
     
     @param vertexShaderSource vertex shader source code.
     @param fragmentShaderSource vertex shader source code.
     @param errors log from OpenGL ES program loading, compiling and linking.
     
     @returns true if program setup is completed correctly, else false.
     */
    bool loadProgram(const char* vertexShaderSource, const char* fragmentShaderSource, std::string& errors);
    
    /*!
     Delete OpenGL ES program.
     Clean any vertex or fragment shader created.
     */
    void deleteProgram();
};

#endif /* OpenGLProgram_hpp */
