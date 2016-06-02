//
//  Obj3DModel.cpp
//  OpenGL
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "Model3D.hpp"

Model3D::Model3D(const char* filePath, std::string modelName) {
    
    //Load model obj data.
    openglObj.parseObj(filePath);
    
    //Set name.
    name = modelName;
}

std::string Model3D::getName() {
    
    return name;
}

void Model3D::setMaterial(const Material& choosenMaterial) {
    
    //Set material.
    material = choosenMaterial;
}

OpenGLObjParser& Model3D::modelData() {
    
    return openglObj;
};

Material& Model3D::getMaterial() {
    
    return material;
};
