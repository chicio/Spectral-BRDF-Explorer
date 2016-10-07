//
//  Obj3DModel.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef Obj3DModel_hpp
#define Obj3DModel_hpp

#include <stdio.h>
#include <glm/glm.hpp>

#include "OpenGLRGBProgram.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLObjParser.hpp"
#include "Material.hpp"

class Model3D {
private:
    
    /// Model name.
    std::string name;
    /// Model Material.
    Material material;
    /// Obj parser that incapsulate model data about vertices and normal.
    OpenGLObjParser openglObj;
    
public:
    
    GLuint _vboId;
    OpenGLRGBProgram openGLModelProgram;
    OpenGLTexture openGLTexture;
    
    glm::mat4 _modelMatrix;
    glm::mat4 _modelViewMatrix;
    glm::mat4 _modelViewProjectionMatrix;
    glm::mat4 _modelViewProjectionLightMatrix;
    glm::mat4 _normalMatrix;
    
    std::string textureName;
    
    std::string lighting;
    
    /*!
     Default constructor.
     */
    Model3D() {};
    
    /*!
     Constructor.
     Load the 3D Obj model and its material.
     
     @param filePath path to the obj 3D model file.
     @param modelName model name.
     */
    Model3D(const char* filePath, std::string modelName);
    
    /*!
     Get model name.
     
     @returns model name.
     */
    std::string getName();
    
    /*!
     Get model data.
     Model data is encapsulated in the obj parser.
     
     @returns model normal and vertex data.
     */
    OpenGLObjParser& modelData();
    
    /*!
     Set material.
     
     @param choosenMaterial material to be loaded.
     */
    void setMaterial(const Material& choosenMaterial);
    
    /*!
     Get model material.
     
     @returns model material
     */
    Material& getMaterial();
        
    void loadTexture() {
        
        openGLTexture.loadTexture(textureName,
                                  {OpenGLTextureParameter(GL_TEXTURE_MIN_FILTER, Int, {.intValue = GL_NEAREST}),
                                   OpenGLTextureParameter(GL_TEXTURE_MAG_FILTER, Int, {.intValue = GL_NEAREST})});
    }
    
    void setTextureName(std::string name) {
        
        textureName = name;
    }
};

#endif /* Obj3DModel_hpp */
