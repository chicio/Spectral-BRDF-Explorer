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
    
    /// Cornell Box model.
    Model3D cornellBoxModel;
    /// Current model to be rendered.
    Model3D model;
    /// Current lighting method selected.
    std::string lighting;
    
    static Scene& instance() {
        
        static Scene scene;
        return scene;
    }
};

#endif /* Scene_hpp */
