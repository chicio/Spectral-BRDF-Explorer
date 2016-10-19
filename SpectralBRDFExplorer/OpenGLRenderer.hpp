//
//  OpenGLRenderer.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLRenderer_hpp
#define OpenGLRenderer_hpp

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <string>

#include "Scene.hpp"
#include "OpenGLObjParser.hpp"
#include "OpenGLProgram.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLFramebufferObject.hpp"
#include "OpenGLCamera.hpp"
#include "OpenGLRGBModelProgram.hpp"
#include "OpenGLSkyboxProgram.hpp"

class OpenGLRenderer {
private:
    
    /// Models.
    std::vector<OpenGLRGBModelProgram> openGLModelPrograms;
    
    /// Shadow.
    OpenGLProgram openGLShadowProgram;
    OpenGLTexture shadowTexture;
    OpenGLFramebufferObject shadowDepthFramebufferObject;
    GLuint shadowMapBufferId;
    GLint _shadowMapMvpLoc;
    GLint _shadowMapMvpLightLoc;
    
    /// Skybox.
    OpenGLSkyboxProgram openGLSkyboxProgram;
    
public:
    
    /// OpenGL ES camera.
    OpenGLCamera openGLCamera;

    /*!
     Start OpenGL ES.

     @param openGLCamera the start camera.
     @param error log from error generated during setup.
     
     @returns true if OpenGL ES could start, else false.
     */
    bool startRenderer(const OpenGLCamera& openGLCamera, std::string& error);
    
    /*!
     Update data for draw.
     
     @param width viewport dimension.
     @param height viewport dimension.
     @param timeSinceLastUpdate
     */
    void update(float width, float height, double timeSinceLastUpdate);
    
    /*!
     Draw a specific model on screen.
     */
    void draw();
    
    /*!
     Shutdown renderer.
     */
    void shutdown();
};

#endif /* OpenGLRenderer_hpp */
