//
//  OpenGLObj.hpp
//  OpenGL
//
//  Created by Fabrizio Duroni on 30/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLObj_hpp
#define OpenGLObj_hpp

#import <OpenGLES/ES3/gl.h>
#include <iostream>
#include <vector>

#define VERTEX_POS_SIZE       3
#define VERTEX_NORMAL_SIZE    3

struct vector3 {
    float x;
    float y;
    float z;
};

class OpenGLObj {
private:

    /// Vector of vertex indices.
    std::vector<unsigned int> verticesIndices;
    /// Vector of normal indices.
    std::vector<unsigned int> normalsIndices;
    /// Vector of vertices.
    std::vector<vector3> vertices;
    /// Vector of normals.
    std::vector<vector3> normals;

    /// Contains sequential vertex data: good for glDrawArray calls.
    std::vector<float> verticesData;
    /// verticesData size.
    int verticesDataSize;
    /// Number of vertices.
    int numberOfVerticesToDraw;
    /// Stride used in verticesDataArray.
    int stride;
    
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
     Parse obj file and retrive data in vectors.
     Each of them could be accessed using the related method.
     
     @param filePath path of the obj file.
     @param error error string if parsing fails, else empty.
     
     @returns true if parsing is fine, else false.
     */
    bool parseObj(const char* filePath, std::string& error);
};

#endif /* OpenGLObjParser_hpp */
