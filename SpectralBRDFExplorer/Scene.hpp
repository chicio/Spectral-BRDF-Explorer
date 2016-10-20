//
//  Scene.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  
//

#ifndef Scene_hpp
#define Scene_hpp

#include "Model3D.hpp"

class Scene {
private:

    /*!
     Private constructor for Scene singleton.
     */
    Scene() {};
    
public:
    
    /// Light direction.
    glm::vec3 lightDirection;
    /// Models contained in the scene.
    std::vector<Model3D> models;
    /// Skybox, if there's one.
    Model3D skybox;
    /// Near plane.
    float nearPlane;
    /// Far Plane;
    float farPlane;
    /// Center of models.
    glm::vec3 sceneCenter;
    
    static Scene& instance() {
        
        static Scene scene;
        return scene;
    }
    
    /*!
     Clear scene.
     Remove all models and reset all setup.
     */
    void clearScene();
};

#endif /* Scene_hpp */
