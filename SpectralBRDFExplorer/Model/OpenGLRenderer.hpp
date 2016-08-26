//
//  OpenGLRenderer.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLRenderer_hpp
#define OpenGLRenderer_hpp

#include <OpenGLES/ES3/gl.h>
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

class OpenGLRenderer {
private:
    
    /// OpenGL ES program.
    OpenGLProgram openGLProgram;
    
    /// Models 3D.
    Model3D model;
    Model3D cornellBoxModel;
    
    /// Near plane.
    float nearPlane;
    /// Far Plane;
    float farPlane;
    
    /// Center of models.
    glm::vec3 modelCenter;

    //Program.
    GLuint _vboIds[2];
    GLuint _textureId;
    
    //Matrices.
    glm::mat4 _mvMatrix;
    glm::mat4 _mvpMatrix;
    glm::mat4 _normalMatrix;
    
    glm::mat4 _mvCornellBoxMatrix;
    glm::mat4 _mvpCornellBoxMatrix;
    glm::mat4 _normalCornellBoxMatrix;
    
    //Uniforms.
    GLint _mvLocation;
    GLint _mvpLocation;
    GLint _normalLocation;
    GLint _lightPosition;
    GLint _lightColor;
    GLint _materialAmbient;
    GLint _materialDiffuse;
    GLint _materialSpecular;
    GLint _materialSpecularExponent;
    GLint _textureSampler;
    GLint _textureActive;
    
public:
    
    /// OpenGL ES camera.
    OpenGLCamera openGLCamera;
    
    /*!
     Start OpenGL ES.
     
     @param vertexShaderSource source of the vertex shader.
     @param fragmentShaderSource source of the fragment shader.
     @param error log from error generated during setup.
     
     @returns true if OpenGL ES could start, else false.
     */
    bool startRenderer(const char* vertexShaderSource,
                       const char* fragmentShaderSource,
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
