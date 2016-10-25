//
//  OpenGLRenderer.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  
//

#ifndef OpenGLRenderer_hpp
#define OpenGLRenderer_hpp

#include <OpenGLESHeaders.h>
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
#include "OpenGLShadowProgram.hpp"

class OpenGLRenderer {
private:
    
    /// Models.
    std::vector<OpenGLRGBModelProgram> openGLModelPrograms;
    
    /// Shadow.
    OpenGLShadowProgram openGLShadowProgram;
    
//    OpenGLProgram openGLShadowProgram;
//    OpenGLTexture shadowTexture;
//    OpenGLFramebufferObject shadowDepthFramebufferObject;
//    GLuint shadowMapBufferId;
//    GLint _shadowMapMvpLoc;
//    GLint _shadowMapMvpLightLoc;
    
    /// Skybox.
    OpenGLSkyboxProgram openGLSkyboxProgram;
    
public:
    
    /// OpenGL ES camera.
    OpenGLCamera openGLCamera;

    /*!
     Start OpenGL ES renderer.

     @param openGLCamera the start camera.
     @param error log from error generated during setup.
     
     @returns true if OpenGL ES could start, else false.
     */
    bool start(const OpenGLCamera& openGLCamera, std::string& error);
    
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
