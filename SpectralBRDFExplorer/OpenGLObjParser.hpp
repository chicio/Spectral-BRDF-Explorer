//
//  OpenGLObj.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 30/05/16.
//  
//

#ifndef OpenGLObj_hpp
#define OpenGLObj_hpp

#include <OpenGLESHeaders.h>
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
    //unsigned char* texturePixels;

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
     Parse obj file and retrive data in vectors.
     Each of them could be accessed using the related method.
     
     @param fileBasePath base path of the obj file.
     @param fileName obj file name.
     
     @returns true if parsing is fine, else false.
     */
    bool parseObj(std::string fileBasePath, std::string fileName);
};

#endif /* OpenGLObjParser_hpp */
