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
    
    glm::mat4 _mvMatrix;
    glm::mat4 _mvpMatrix;
    glm::mat4 _mvpLightMatrix;
    glm::mat4 _normalMatrix;
    
    


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
};

#endif /* Obj3DModel_hpp */
