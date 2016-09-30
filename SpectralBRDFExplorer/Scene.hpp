//
//  Scene.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
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
    
    /// Models contained in the scene.
    std::vector<Model3D> models;
    Model3D skybox;
        
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
