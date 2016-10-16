//
//  OpenGLProgram.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLProgram_hpp
#define OpenGLProgram_hpp

#include <OpenGLESHeaders.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>

#include "OpenGLShader.hpp"
#include "Utils.hpp"

#define VERTEX_POS_INDX             0
#define VERTEX_NORMAL_INDX          1
#define VERTEX_TEXCOORDINATE_INDX   2

#define TEXTURE_UNIT_ID_0_SAMPLER   0
#define TEXTURE_UNIT_ID_1_SAMPLER   1

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
     Start a program.
     Each subclass will implements this method with
     the specific setup needed for what it must draw.
     
     @param error error message to be retur if the start fails.
     
     @returns true if the program starts correctly, else false.
     */
    virtual bool startProgram(std::string& error) { return false; };
    
    /*
     Draw method.
     Must contain all the opengl step need to draw with a program.
     */
    virtual void draw() {};
    
    /*!
     Delete OpenGL ES program.
     Clean any vertex or fragment shader created.
     */
    void deleteProgram();    
};

#endif /* OpenGLProgram_hpp */
