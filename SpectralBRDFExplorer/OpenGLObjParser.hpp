//
//  OpenGLObj.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 30/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLObj_hpp
#define OpenGLObj_hpp

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif


#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "lodepng.h"

#define VERTEX_POS_SIZE              3
#define VERTEX_NORMAL_SIZE           3
#define VERTEX_TEXCOORDINATE_SIZE    2

class OpenGLObjParser {
private:

    /// Vector of vertex indices.
    std::vector<GLuint> verticesIndices;
    /// Vector of normal indices.
    std::vector<GLuint> normalsIndices;
    /// Vector of texture indices.
    std::vector<GLuint> textureCoordinatesIndices;
    /// Vector of vertices.
    std::vector<glm::vec3> vertices;
    /// Vector of normals.
    std::vector<glm::vec3> normals;
    /// Vector of texture coordinates.
    std::vector<glm::vec2> textureCoordinates;
    /// Vector of color pixel of texture image.
    unsigned char* texturePixels;

    /// Contains sequential vertex data: good for glDrawArray calls.
    std::vector<GLfloat> verticesData;
    /// verticesData size.
    int verticesDataSize;
    /// Number of vertices.
    int numberOfVerticesToDraw;
    /// Stride used in verticesDataArray.
    int stride;
    /// Flag used to check if the model has a texture.
    bool textureActive;
    /// Texture width.
    unsigned textureWidth;
    //// Texture height.
    unsigned textureHeight;
    
public:
    
    /*!
     Get vertices data.
     
     @returns reference to vertice vector data.
     */
    const std::vector<float>& getVerticesData();
    
    /*!
     Get vertice data size, in byte.
     
     @returns vertices data size.
     */
    const int getVerticesDataSize();
    
    /*!
     Get verticesData stride.
     
     @returns stride of verticesDataArray.
     */
    int getStride();
    
    /*!
     Get number of vertices to draw.
     
     @returns int that corresponds to the number of vertices to draw.
    */
    int getNumberOfVerticesToDraw();
    
    /*!
     Get flag hasTexture, used to check if the model has a texture.

     @returns true if the model has a texture, else false.
     */
    bool hasTexture();
    
    /*!
     Get texture width.
     
     @returns texture width.
     */
    int getTextureWidth();
    
    /*!
     Get texture height.
     
     @returns texture height.
     */
    int getTextureHeight();
    
    /*!
     Get texture data.
     
     @returns pointer to texture data.
     */
    unsigned char* getTexturePixels();
    
    /*!
     Load texture from image.
     
     @param filePath path of the image to be used as texture.
     */
    void loadTexture(const char* filePath);

    /*!
     Parse obj file and retrive data in vectors.
     Each of them could be accessed using the related method.
     
     @param filePath path of the obj file.
     
     @returns true if parsing is fine, else false.
     */
    bool parseObj(const char* filePath);
};

#endif /* OpenGLObjParser_hpp */
