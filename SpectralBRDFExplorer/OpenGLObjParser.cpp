//
//  OpenGLObjParser.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 30/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "lodepng.h"
#include "OpenGLObjParser.hpp"

#ifdef __ANDROID__
#include "android_fopen.h"
#endif

#ifdef __APPLE__
#include "iOSFileOpen.hpp"
#endif

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

bool OpenGLObjParser::hasTexture() {
    
    return textureActive;
}

int OpenGLObjParser::getTextureWidth() {

    return textureWidth;
}

int OpenGLObjParser::getTextureHeight() {
    
    return textureHeight;
}

unsigned char* OpenGLObjParser::getTexturePixels() {
    
    return texturePixels;
}

void OpenGLObjParser::loadTexture(const char* filePath) {

    unsigned error;
    
    error = lodepng_decode32_file(&texturePixels, &textureWidth, &textureHeight, filePath);
    
    if(error) {
        
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }
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
            
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        }else if(strcmp(lineHeader, "vn") == 0) {
            
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        }else if(strcmp(lineHeader, "vt") == 0) {
            
            glm::vec2 texel;
            fscanf(file, "%f %f\n", &texel.s, &texel.t);
            textureCoordinates.push_back(texel);
        }else if(strcmp(lineHeader, "f") == 0) {
            
            if(textureCoordinates.size() > 0) {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], normalIndex[3], textureIndex[3];
                int matches = fscanf(file,
                                     "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                     &vertexIndex[0],
                                     &textureIndex[0],
                                     &normalIndex[0],
                                     &vertexIndex[1],
                                     &textureIndex[1],
                                     &normalIndex[1],
                                     &vertexIndex[2],
                                     &textureIndex[2],
                                     &normalIndex[2]);
                
                if (matches != 9){
                    
                    return false;
                }
                
                //Indices array.
                verticesIndices.push_back(vertexIndex[0]);
                verticesIndices.push_back(vertexIndex[1]);
                verticesIndices.push_back(vertexIndex[2]);
                normalsIndices.push_back(normalIndex[0]);
                normalsIndices.push_back(normalIndex[1]);
                normalsIndices.push_back(normalIndex[2]);
                textureCoordinatesIndices.push_back(textureIndex[0]);
                textureCoordinatesIndices.push_back(textureIndex[1]);
                textureCoordinatesIndices.push_back(textureIndex[2]);
                
                //Vertex data.
                for (int i = 0 ; i < 3; i++) {
                    
                    glm::vec3 vertex = vertices[vertexIndex[i] - 1];
                    glm::vec3 normal = normals[normalIndex[i] - 1];
                    glm::vec2 texel = textureCoordinates[textureIndex[i] - 1];
                    
                    verticesData.push_back(vertex.x);
                    verticesData.push_back(vertex.y);
                    verticesData.push_back(vertex.z);
                    
                    verticesData.push_back(normal.x);
                    verticesData.push_back(normal.y);
                    verticesData.push_back(normal.z);
                    
                    verticesData.push_back(texel.s);
                    verticesData.push_back(texel.t);
                }
            } else {
                
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
                    
                    glm::vec3 vertex = vertices[vertexIndex[i] - 1];
                    glm::vec3 normal = normals[normalIndex[i] - 1];
                    
                    verticesData.push_back(vertex.x);
                    verticesData.push_back(vertex.y);
                    verticesData.push_back(vertex.z);
                    
                    verticesData.push_back(normal.x);
                    verticesData.push_back(normal.y);
                    verticesData.push_back(normal.z);
                }
            }
        }
    }
    
    fclose(file);
    
    if(textureCoordinates.size() > 0) {
        
        //Model has texture.
        textureActive = true;
        
        //Set number of vertices to draw.
        numberOfVerticesToDraw = ((int)verticesData.size() / (VERTEX_POS_SIZE +
                                                              VERTEX_NORMAL_SIZE +
                                                              VERTEX_TEXCOORDINATE_SIZE));;
        
        //Set stride.
        stride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE + VERTEX_TEXCOORDINATE_SIZE);
    } else {

        //Model has texture.
        textureActive = false;
        
        //Set number of vertices to draw.
        numberOfVerticesToDraw = ((int)verticesData.size() / (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE));;
    
        //Set stride.
        stride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE);
    }
        
    //Set vertices data size.
    verticesDataSize = stride * numberOfVerticesToDraw;
    
    return true;
}
