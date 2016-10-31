//
//  OpenGLCamera.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 04/08/16.
//  
//

#include "OpenGLCamera.hpp"

OpenGLCamera::OpenGLCamera(glm::vec3 cameraEye, glm::vec3 cameraCenter, glm::vec3 cameraUp) {
 
    eye = cameraEye;
    center = cameraCenter;
    up = cameraUp;
    
    phi = 0.0f;
    theta = glm::radians(90.0f);
    
    phiRotation = 0.0f;
    thetaRotation = 0.0f;
    oldPhiRotation = 0.0f;
    oldThetaRotation = 0.0f;

    zoomFactor = 0.0f;
    oldZoomFactor = 0.0f;
    
    eyeOffset = glm::vec3(0.0f, 0.0f, 0.0f);
}

void OpenGLCamera::setSceneCenter(glm::vec3 newSceneCenter) {

    sceneCenter = newSceneCenter;
}

void OpenGLCamera::setZoomFactor(float zoom) {
    
    zoomFactor = zoom;
}

void OpenGLCamera::setRotationFactors(float newPhi, float newTheta) {
    
    phiRotation = newPhi;
    thetaRotation = newTheta;
}

void OpenGLCamera::updateCamera() {

    //Camera is zoomed using user gesture.
    //Update eye position based on zoom factor.
    if(zoomFactor != oldZoomFactor) {
        
        eye.z = eye.z + zoomFactor;
        oldZoomFactor = zoomFactor;
    }
    
    //Camera is rotated using spherical coordinate calculate with user gestures input.
    if(phiRotation != oldPhiRotation || thetaRotation != oldThetaRotation) {
        
        oldPhiRotation = phiRotation;
        oldThetaRotation = thetaRotation;
        
        phi = phi + glm::radians(phiRotation);
        theta = theta + glm::radians(thetaRotation);
        
        //Spherical coordinates (my loved Spectral Clara Lux Tracer, you are always in my heart :)).
        eyeOffset.x = sceneCenter.x + sin(phi) * sin(theta) * (sceneCenter.z * -1.0f);
        eyeOffset.y = sceneCenter.y + cos(theta) * (sceneCenter.z * -1.0f);
        eyeOffset.z = sceneCenter.z + cos(phi) * sin(theta) * (sceneCenter.z * -1.0f);
    }
}

glm::mat4 OpenGLCamera::lookAtMatrix() {

    //Create lookAt matrix.
    glm::mat4 lookAtMatrix = glm::lookAt(eye + eyeOffset, center, up);
    
    return lookAtMatrix;
}
