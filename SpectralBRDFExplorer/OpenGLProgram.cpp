//
//  OpenGLProgram.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLProgram.hpp"

bool OpenGLProgram::loadProgram(const char* vertexShaderSource, const char* fragmentShaderSource, std::string& errors) {
    
    std::string error;
    
    //Load shaders.
    OpenGLShader shader;
    
    vertexShader = shader.loadShader(GL_VERTEX_SHADER, vertexShaderSource, error);
    
    if(vertexShader == OPENGL_SHADER_ERROR) {
        
        errors = "Vertex Shader: " + error;
        
        return false;
    }
    
    fragmentShader = shader.loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource, error);
    
    if(fragmentShader == OPENGL_SHADER_ERROR) {
        
        errors = "Fragment Shader: " + error;
        
        return false;
    }
    
    //Create program.
    program = glCreateProgram();
    
    //Attach shader to program.
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    //Link the program.
    glLinkProgram(program);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    // Check the link status
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    
    if (!linked) {
        
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        
        if(infoLen > 1) {
            
            GLchar* programError = (GLchar *)malloc(sizeof(GLchar) * infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, programError);
            errors = "Program error: " + std::string(programError);;
            free(programError);
        }
        
        glDeleteProgram(program);
        
        return false;
    }
    
    //Program linked.
    return true;
}

void OpenGLProgram::deleteProgram() {
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
}
