//
//  OpenGLModelProgramBuilder.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 31/12/2016.
//
//

#ifndef OpenGLModelProgramBuilder_hpp
#define OpenGLModelProgramBuilder_hpp

#include "Model3D.hpp"
#include "OpenGLCamera.hpp"
#include "OpenGLModelProgram.hpp"
#include "OpenGLModelRGBProgram.hpp"
#include "OpenGLModelSpectralProgram.hpp"

class OpenGLModelProgramBuilder {
public:
    
    /*!
     Build OpenGL model program.
     
     @param model the 3D model to be rendered.
     @param openGLCamera the current camera.
     
     @returns a OpenGLModelProgram subclass.
     */
    OpenGLModelProgram* buildOpenGLModelProgram(Model3D& model, OpenGLCamera& openGLCamera);
};

#endif /* OpenGLModelProgramBuilder_hpp */
