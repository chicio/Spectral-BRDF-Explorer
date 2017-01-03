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
    
    OpenGLModelProgramBuilder openGLModelProgramBuilder;
    
    //Load models programs.
    for (auto& currentModel : Scene::instance().models) {
        
        OpenGLModelProgram* modelProgram = openGLModelProgramBuilder.buildOpenGLModelProgram(currentModel, openGLCamera);
        
        programLinked = modelProgram->startProgram(errors);
        
        if (!programLinked || !errors.empty()) {
            
            //Return error from program loading.
            error = errors;
            
            return false;
        }
        
        //Add program to vector of program execution.
        openGLModelPrograms.push_back(modelProgram);
    }
    
    //Skybox program.
    if (Scene::instance().skybox != nullptr) {
        
        openGLSkyboxProgram = OpenGLSkyboxProgram(OpenGLESConfig::shadersBasePath, OpenGLESConfig::textureFileBasePath);
        openGLSkyboxProgram.skyboxModel = Scene::instance().skybox;
        
        programLinked  = openGLSkyboxProgram.startProgram(errors);
        
        if (!programLinked) {
            
            //Return error from program loading.
            error = errors;
            
            return false;
        }
    }
    
    //Load Shadow mapping programs.
    openGLShadowProgram = OpenGLShadowProgram(OpenGLESConfig::shadersBasePath);
    openGLShadowProgram.lightDirection = Scene::instance().lightDirection;
    openGLShadowProgram.models = &Scene::instance().models;
    
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
    
    for (auto& modelProgram : openGLModelPrograms) {
        
        //        modelProgram.update(openGLCamera, projectionMatrix);
        modelProgram->update(openGLCamera, projectionMatrix);
    }
    
    openGLShadowProgram.update(openGLCamera, orthoMatrix);
    
    if (Scene::instance().skybox != nullptr) {
        
        Scene::instance().skybox->_modelViewProjectionMatrix = projectionMatrix *
                                                               openGLCamera.lookAtMatrix() *
                                                               Scene::instance().skybox->_modelMatrix;
    }
}

void OpenGLRenderer::draw() {
    
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    
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

    //Activate all color components (have been disabled for shadow rendering).
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw skybox.
    if (Scene::instance().skybox != nullptr) {

        openGLSkyboxProgram.draw();
    }
    
    //Draw models.
    for (auto& modelProgram : openGLModelPrograms) {
        
        //Set shadow texture.
        modelProgram->shadowTexture = &openGLShadowProgram.shadowTexture;
        
        //Draw current model.
        modelProgram->draw();
    }
}

void OpenGLRenderer::shutdown() {
    
    openGLShadowProgram.deleteProgram();
    
    //Clear vertex buffer objects.
    std::vector<GLuint> vbo;
    
    //Delete programs.
    for (auto& program : openGLModelPrograms) {
        
        vbo.push_back(program->model->_vboId);
        program->deleteProgram();
    }
    
    //Delete buffers.
    if (openGLModelPrograms.size() > 0) {
        
        glDeleteBuffers((int)Scene::instance().models.size(), vbo.data());
    }
}
