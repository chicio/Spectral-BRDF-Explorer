//
//  OpenGLObjParser.cpp
//  OpenGL
//
//  Created by Fabrizio Duroni on 30/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLObjParser.hpp"

const std::vector<float>& OpenGLObjParser::getVerticesData() {
    
    return verticesData;
}

const int OpenGLObjParser::getVerticesDataSize() {

    return verticesDataSize;
}

int OpenGLObjParser::getStride() {
    
    return stride;
}

int OpenGLObjParser::getNumberOfVerticesToDraw() {
    
    return numberOfVerticesToDraw;
}

bool OpenGLObjParser::parseObj(const char* filePath) {
    
    FILE* file = fopen(filePath, "r");
    
    if(file == nullptr) {
        
        return false;
    }
    
    while(1) {
        
        char lineHeader[128];
        
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        
        if(res == EOF) {
            
            //End of file, exit parser.
            break;
        }
        
        if (strcmp(lineHeader, "v") == 0) {
            
            openglVector3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            vertices.push_back(vertex);
        }else if(strcmp(lineHeader, "vn") == 0) {
            
            openglVector3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            normals.push_back(normal);
        }else if(strcmp(lineHeader, "f") == 0) {
            
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], normalIndex[3];
            int matches = fscanf(file,
                                 "%d//%d %d//%d %d//%d\n",
                                 &vertexIndex[0],
                                 &normalIndex[0],
                                 &vertexIndex[1],
                                 &normalIndex[1],
                                 &vertexIndex[2],
                                 &normalIndex[2]);
            
            if (matches != 6){
                
                return false;
            }
            
            //Indices array.
            verticesIndices.push_back(vertexIndex[0]);
            verticesIndices.push_back(vertexIndex[1]);
            verticesIndices.push_back(vertexIndex[2]);
            normalsIndices.push_back(normalIndex[0]);
            normalsIndices.push_back(normalIndex[1]);
            normalsIndices.push_back(normalIndex[2]);
            
            //Vertex data.
            for (int i = 0 ; i < 3; i++) {
                
                openglVector3 vertex = vertices[vertexIndex[i] - 1];
                openglVector3 normal = normals[normalIndex[i] - 1];
                
                verticesData.push_back(vertex.x);
                verticesData.push_back(vertex.y);
                verticesData.push_back(vertex.z);
                
                verticesData.push_back(normal.x);
                verticesData.push_back(normal.y);
                verticesData.push_back(normal.z);
            }
        }
    }
    
    //Set number of vertices to draw.
    numberOfVerticesToDraw = ((int)verticesData.size() / (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE));;
    
    //Set stride.
    stride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE);
    
    //Set vertices data size.
    verticesDataSize = stride * numberOfVerticesToDraw;
    
    return true;
}
