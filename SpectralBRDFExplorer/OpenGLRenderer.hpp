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

#ifdef __APPLE__
#include "iOSFileOpen.hpp"
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

#define VERTEX_POS_INDX             0
#define VERTEX_NORMAL_INDX          1
#define VERTEX_TEXCOORDINATE_INDX   2
#define TEXTURE_UNIT_ID_0_SAMPLER   0
#define TEXTURE_UNIT_ID_1_SAMPLER   1

class OpenGLRenderer {
private:
    
    /// Shadow.
    OpenGLProgram openGLShadowProgram;
    OpenGLTexture shadowTexture;
    OpenGLFramebufferObject shadowDepthFramebufferObject;
    GLuint shadowMapBufferId;
    GLint _shadowMapMvpLoc;
    GLint _shadowMapMvpLightLoc;
    
    /// Skybox.
    OpenGLProgram openGLSkyboxProgram;
    OpenGLTexture skyboxTexture;
    GLint _skyboxmvpLocation;
    GLint _skyBoxTextureSampler;
    
public:
    
    /// OpenGL ES camera.
    OpenGLCamera openGLCamera;

    static OpenGLRenderer* defaultStartRender() {

        /***** MODELS ******/
        Model3D model3D("Objs/Dragon.obj", "Sphere");
        model3D._modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0f, -12.0f));
        model3D.setMaterial(Material::createBronzeMaterial());
        model3D.lighting = "BlinnPhong";
        Scene::instance().models.push_back(model3D);
        
        Model3D cornellBoxModel3D("Objs/CornellBox.obj", "Cornell Box");
        cornellBoxModel3D._modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0f, -12.0f));;
        cornellBoxModel3D.setMaterial(Material::createMatteMaterial());
        cornellBoxModel3D.lighting = "Phong";
        Scene::instance().models.push_back(cornellBoxModel3D);

        /****** START *******/
        std::string error;
        OpenGLRenderer* openGLRenderer = new OpenGLRenderer();
        openGLRenderer->startRenderer(OpenGLCamera(glm::vec3(0.0f, 0.0f, 0.0f),
                                                   glm::vec3(0.0f, 0.0f, -5.0f),
                                                   glm::vec3(0.0f, 1.0f, 0.0f)),
                                      error);

        return openGLRenderer;
    }
    
    /*!
     Start OpenGL ES.

     @param openGLCamera the start camera.
     @param error log from error generated during setup.
     
     @returns true if OpenGL ES could start, else false.
     */
    bool startRenderer(const OpenGLCamera& openGLCamera, std::string& error);
    
    /*!
     Load current scene.
     */
    void loadScene();
    
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
