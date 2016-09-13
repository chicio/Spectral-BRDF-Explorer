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
#include "OpenGLCamera.hpp"

#define VERTEX_POS_INDX             0
#define VERTEX_NORMAL_INDX          1
#define VERTEX_TEXCOORDINATE_INDX   2
#define TEXTURE_UNIT_ID_0_SAMPLER   0
#define TEXTURE_UNIT_ID_1_SAMPLER   1

class OpenGLRenderer {
private:
    
    /// OpenGL ES program.
    OpenGLProgram openGLProgram;
    
    /// Light direction.
    glm::vec3 lightDirection;
        
    /// Near plane.
    float nearPlane;
    /// Far Plane;
    float farPlane;
    
    /// Center of models.
    glm::vec3 sceneCenter;

    //Program.
    GLuint _textureId;
    
    //Uniforms.
    GLint _mvLocation;
    GLint _mvpLocation;
    GLint _mvpLightLocation; //Shadow map...
    GLint _normalLocation;
    GLint _lightDirection;
    GLint _lightColor;
    GLint _materialAmbient;
    GLint _materialDiffuse;
    GLint _materialSpecular;
    GLint _materialSpecularExponent;
    GLint _textureSampler;
    GLint _textureActive;
    
    //Shadow map.
    OpenGLProgram openGLShadowProgram;
    
    float shadowMapTextureWidth;
    float shadowMapTextureHeight;
    GLuint shadowMapTextureId;
    GLuint shadowMapBufferId;
    
    GLint _shadowMapMvpLoc;
    GLint _shadowMapMvpLightLoc;
    GLint _shadowMapSamplerLoc;
        
public:
    
    /// OpenGL ES camera.
    OpenGLCamera openGLCamera;

    static OpenGLRenderer * defaultStartRender(const char* vertexShaderSource,
                                               const char* fragmentShaderSource,
                                               const char* shadowMappingVertexShaderSource,
                                               const char* shadowMappingFragmentShaderSource) {


        /***** MODELS ******/
        Model3D model3D("Objs/Dragon.obj", "Sphere");
        Model3D cornellBoxModel3D("Objs/CornellBox.obj", "Cornell Box");

        model3D._modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0f, -12.0f));
        cornellBoxModel3D._modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0f, -12.0f));;

        Scene::instance().models.push_back(model3D);
        Scene::instance().models.push_back(cornellBoxModel3D);

        /****** MATERIALS *******/
        Scene::instance().models[0].setMaterial(Material::createBronzeMaterial());
        Scene::instance().models[1].setMaterial(Material::createMatteMaterial());

        /****** LIGHITNG *******/
        Scene::instance().lighting = "Phong";

        /****** START *******/
        std::string error;
        OpenGLRenderer* openGLRenderer = new OpenGLRenderer();
        bool rendererStarted = openGLRenderer->startRenderer(vertexShaderSource,
                                                            fragmentShaderSource,
                                                            shadowMappingVertexShaderSource,
                                                            shadowMappingFragmentShaderSource,
                                                            OpenGLCamera(glm::vec3(0.0f, 0.0f, 0.0f),
                                                                         glm::vec3(0.0f, 0.0f, -5.0f),
                                                                         glm::vec3(0.0f, 1.0f, 0.0f)),
                                                            error);

        return openGLRenderer;
    }
    
    /*!
     Start OpenGL ES.
     
     @param vertexShaderSource source of the vertex shader.
     @param fragmentShaderSource source of the fragment shader.
     @param error log from error generated during setup.
     
     @returns true if OpenGL ES could start, else false.
     */
    bool startRenderer(const char* vertexShaderSource,
                       const char* fragmentShaderSource,
                       const char* shadowMappingVertexShaderSource,
                       const char* shadowMappingFragmentShaderSource,
                       const OpenGLCamera& openGLCamera,
                       std::string& error);
    
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
