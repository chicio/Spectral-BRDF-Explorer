//
//  Scene.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  
//

#include "Scene.hpp"

void Scene::clearScene() {

    //Clear Light direction.
    lightDirection = glm::vec3(0.0f, 0.0f, 0.0f);

    //Clear skybox.
    skybox = nullptr;

    //Clear n/f planes.
    nearPlane = 0;
    farPlane = 0;
    
    //Clear scene center.
    sceneCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    //Clear models addition.
    models.clear();
}
