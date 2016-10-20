//
//  OpenGLTexture.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/10/2016.
//  
//

#ifndef OpenGLTexture_hpp
#define OpenGLTexture_hpp

#include <OpenGLESHeaders.h>
#include <string>
#include <vector>

#include "lodepng.h"

/// Parameter value data type classification.
enum ParameterDataType {

    Float,
    Int
};

/// Parameter texture data type.
struct OpenGLTextureParameter {
    
    /// Parameter name.
    GLenum parameterName;
    /// Parameter value data type.
    ParameterDataType parameterDataType;
    /// Parameter value.
    union ParameterValue {
        GLfloat floatValue;
        GLint intValue;
    } parameterValue;
    
    OpenGLTextureParameter(GLenum name,
                           ParameterDataType type,
                           ParameterValue value) : parameterName{name}, parameterDataType{type}, parameterValue{value} {}
};

class OpenGLTexture {
public:
    
    /// OpenGL Texture id.
    GLuint _textureId;
    /// Texture width.
    unsigned textureWidth;
    //// Texture height.
    unsigned textureHeight;
    
    void startTexture(GLenum target);
    
    void createTexture(std::vector<OpenGLTextureParameter> parametersList,
                       GLint internalformat,
                       GLenum format,
                       GLenum type,
                       unsigned char* texturePixels = NULL);
    
    void setTextureParameters(GLenum target, std::vector<OpenGLTextureParameter> parametersList);
    
    bool loadTexture(std::string textureFileName, std::vector<OpenGLTextureParameter> parametersList);
    
    bool loadCubeMapTexture(std::string left,
                            std::string right,
                            std::string up,
                            std::string down,
                            std::string front,
                            std::string back,
                            std::vector<OpenGLTextureParameter> parametersList);
};

#endif /* OpenGLTexture_hpp */
