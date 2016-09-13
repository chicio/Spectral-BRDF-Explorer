//
//  OpenGLShader.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLShader.hpp"

GLuint OpenGLShader::loadShader(const GLenum type, const char *shaderSource, std::string& error) {
    
    //Create the shader object.
    GLuint shader = glCreateShader(type);
    
    if(shader == 0) {
        
        return 0;
    }
    
    //Load the shader source.
    glShaderSource(shader, 1, &shaderSource, NULL);
    
    //Compile the shader.
    glCompileShader(shader);
    
    //Check the compile status.
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    
    //Shader not compiled.
    if(!compiled) {
        
        //Extract info log length.
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        
        if(infoLen > 1) {
            
            //Get info log.
            GLchar *infoLog = (GLchar *)malloc(sizeof(GLchar) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            error = std::string(infoLog);
        }
        
        glDeleteShader(shader);
        
        return OPENGL_SHADER_ERROR;
    }
    
    return shader;
}
