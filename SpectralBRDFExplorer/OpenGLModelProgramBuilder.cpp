//
//  OpenGLModelProgramBuilder.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 31/12/2016.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLModelProgramBuilder.hpp"

OpenGLModelProgram* OpenGLModelProgramBuilder::buildOpenGLModelProgram(Model3D& model, OpenGLCamera& openGLCamera) {
    
    OpenGLModelProgram* program;
    
    //Init program based on material of the model.
    MaterialSpectral* material = dynamic_cast<MaterialSpectral*>(model.getMaterial());
    
    if (material != nullptr) {
        
        program = new OpenGLModelSpectralProgram(OpenGLESConfig::shadersBasePath);
    } else {
        
        program = new OpenGLModelRGBProgram(OpenGLESConfig::shadersBasePath);
    }
    
    //Setup model and camera.
    program->model = &model;
    program->openGLCamera = &openGLCamera;
    
    return program;
}
