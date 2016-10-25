//
//  OpenGLRenderer.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  
//

#include "Utils.hpp"
#include "OpenGLRenderer.hpp"

bool OpenGLRenderer::start(const OpenGLCamera& camera, std::string& error) {
    
    //Setup camera.
    openGLCamera = camera;
    openGLCamera.setSceneCenter(Scene::instance().sceneCenter);
    
    bool programLinked;
    std::string errors;
    
    //Load models programs.
    for (auto& currentModel : Scene::instance().models) {
        
        //Create program for model.
        OpenGLRGBModelProgram modelProgram(OpenGLESConfiguration::shadersBasePath);
        modelProgram.model = &currentModel;
        modelProgram.openGLCamera = &openGLCamera;
        
        programLinked = modelProgram.startProgram(errors);
        
        if (!programLinked || !errors.empty()) {
            
            //Return error from program loading.
            error = errors;
            
            return false;
        }
        
        //Add program to vector of program execution.
        openGLModelPrograms.push_back(modelProgram);
    }
    
    //Skybox program.
    openGLSkyboxProgram = OpenGLSkyboxProgram(OpenGLESConfiguration::shadersBasePath,
                                              OpenGLESConfiguration::textureFileBasePath);
    openGLSkyboxProgram.skyboxModel = &Scene::instance().skybox;
    
    programLinked  = openGLSkyboxProgram.startProgram(errors);
    
    if (!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return false;
    }
    
    //Load Shadow mapping programs.
    openGLShadowProgram = OpenGLShadowProgram(OpenGLESConfiguration::shadersBasePath);
    
    programLinked = openGLShadowProgram.startProgram(errors);
    
    if (!programLinked) {
        
        //Return error from program loading.
        error = errors;
        
        return false;
    }
    
    return true;
}

void OpenGLRenderer::update(float width, float height, double timeSinceLastUpdate) {
    
    //Projection matrix.
    float aspect = fabs(width / height);
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(65.0f),
                                                  aspect,
                                                  Scene::instance().nearPlane,
                                                  Scene::instance().farPlane);
    
    glm::mat4 orthoMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
    
    for (auto& currentModel : Scene::instance().models) {
        
        //Main scene matrix.
        currentModel._modelViewMatrix = openGLCamera.lookAtMatrix() * currentModel._modelMatrix;
        currentModel._modelViewProjectionMatrix = projectionMatrix * currentModel._modelViewMatrix;
        currentModel._normalMatrix = glm::inverseTranspose(currentModel._modelViewMatrix);
        
        //Shadow map matrix.
        currentModel._modelViewProjectionLightMatrix = orthoMatrix * glm::lookAt(Scene::instance().lightDirection,
                                                                                 openGLCamera.center,
                                                                                 glm::vec3(0.0f, 1.0f, 0.0f)) * currentModel._modelMatrix;
    }
    
    Scene::instance().skybox._modelViewProjectionMatrix = projectionMatrix * openGLCamera.lookAtMatrix() * Scene::instance().skybox._modelMatrix;
}

void OpenGLRenderer::draw() {
    
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    
    GLint defaultFramebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);
    
    //Change viewport to shadow texture size.
    glBindFramebuffer(GL_FRAMEBUFFER, openGLShadowProgram.shadowDepthFramebufferObject.framebufferObjectId);
    glViewport(0, 0, openGLShadowProgram.shadowTexture.textureWidth, openGLShadowProgram.shadowTexture.textureHeight);

    //Draw shadow.
    openGLShadowProgram.draw();
   
    //Restore default viewport and start drawing.
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glViewport(0, 0, m_viewport[2], m_viewport[3]);

    //Draw skybox.
    openGLSkyboxProgram.draw();
    
    //Draw models.
    for (auto& modelProgram : openGLModelPrograms) {
        
        //Set shadow texture.
        modelProgram.shadowTexture = &openGLShadowProgram.shadowTexture;

        //Draw current model.
        modelProgram.draw();
    }
}

void OpenGLRenderer::shutdown() {
    
    openGLShadowProgram.deleteProgram();
    
    //Clear vertex buffer objects.
    std::vector<GLuint> vbo;
    
    for (auto& program : openGLModelPrograms) {
        
        vbo.push_back(program.model->_vboId);
        program.deleteProgram();
    }
    
    glDeleteBuffers((int)Scene::instance().models.size(), vbo.data());
}
