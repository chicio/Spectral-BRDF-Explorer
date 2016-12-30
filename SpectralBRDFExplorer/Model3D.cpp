//
//  Obj3DModel.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  
//

#include "Model3D.hpp"

Model3D::Model3D(std::string fileBasePath, std::string modelFileName) {
    
    //Load model obj data.
    openglObj.parseObj(fileBasePath, modelFileName);
    
    //Set name.
    name = modelFileName;
}

std::string Model3D::getName() {
    
    return name;
}

void Model3D::setMaterial(Material* choosenMaterial) {
    
    //Set material.
    material = choosenMaterial;
}

OpenGLObjParser& Model3D::modelData() {
    
    return openglObj;
};

Material* Model3D::getMaterial() {
    
    return material;
};
