//
//  Obj3DModel.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  
//

#ifndef Obj3DModel_hpp
#define Obj3DModel_hpp

#include <stdio.h>
#include <glm/glm.hpp>

#include "OpenGLTexture.hpp"
#include "OpenGLObjParser.hpp"
#include "Material.hpp"
#include "MaterialRGB.hpp"
#include "MaterialSpectral.hpp"

class Model3D {
private:
    
    /// Model name.
    std::string name;
    /// Model Material.
    Material* material;
    /// Obj parser that incapsulate model data about vertices and normal.
    OpenGLObjParser openglObj;
    
public:
    
    GLuint _vboId;

    OpenGLTexture openGLTexture;
    
    glm::mat4 _modelMatrix;
    glm::mat4 _modelViewMatrix;
    glm::mat4 _modelViewProjectionMatrix;
    glm::mat4 _modelViewProjectionLightMatrix;
    glm::mat4 _normalMatrix;
    
    std::string textureName;
    std::string textureBasePath;
    
    std::string lighting;
    
    /*!
     Default constructor.
     */
    Model3D() {};
    
    /*!
     Constructor.
     Load the 3D Obj model and its material.
     
     @param fileBasePath path to the 3D model file.
     @param modelFileName model file name.
     */
    Model3D(std::string fileBasePath, std::string modelFileName);
    
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
    void setMaterial(Material* choosenMaterial);
    
    /*!
     Get model material.
     
     @returns model material
     */
    Material* getMaterial();
        
    void loadTexture() {

        //Create texture.
        openGLTexture = OpenGLTexture(textureBasePath);

        //Load texture.
        openGLTexture.loadTexture(textureName,
                                  {OpenGLTextureParameter(GL_TEXTURE_MIN_FILTER, Int, {.intValue = GL_NEAREST}),
                                   OpenGLTextureParameter(GL_TEXTURE_MAG_FILTER, Int, {.intValue = GL_NEAREST})});
    }
    
    void setTexture(std::string aTextureBasePath, std::string name) {

        textureBasePath = aTextureBasePath;
        textureName = name;
    }
};

#endif /* Obj3DModel_hpp */
