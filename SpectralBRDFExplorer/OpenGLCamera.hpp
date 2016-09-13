//
//  OpenGLCamera.hpp
//  OpenGL
//
//  Created by Fabrizio Duroni on 04/08/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLCamera_hpp
#define OpenGLCamera_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class OpenGLCamera {
private:
    
    /// Camera position.
    glm::vec3 eye;
    /// Near plane.
    float nearPlane;
    /// Far plane.
    float farPlane;
    ///  Current eye offset.
    glm::vec3 eyeOffset;
    /// Zoom factor.
    float zoomFactor;
    /// Old zoom factor;
    float oldZoomFactor;
    /// Azimuth angle.
    float phi;
    /// Azimuth angle of rotation.
    float phiRotation;
    /// Old azimuth angle of rotation.
    float oldPhiRotation;
    ///  Polar angle.
    float theta;
    /// Polar angle of rotation.
    float thetaRotation;
    /// Old polar angle of rotation.
    float oldThetaRotation;
    /// Current scene center.
    glm::vec3 sceneCenter;
    
public:
    
    /// Camera up orientation.
    glm::vec3 up;    
    /// Camera look at point.
    glm::vec3 center;
    
    /*!
     Default constructor.
     */
    OpenGLCamera() : eye{glm::vec3(0.0f, 0.0f, 0.0f)},
                     center{glm::vec3(0.0f, 0.0f, 0.0f)},
                     up{glm::vec3(0.0f, 0.0f, 0.0f)} {};
    
    /*!
     Constructor.
     
     @param cameraEye camera position.
     @param cameraCenter camera look at point.
     @param cameraUp camera up direction.
     */
    OpenGLCamera(glm::vec3 cameraEye, glm::vec3 cameraCenter, glm::vec3 cameraUp);
    
    /*!
     Set center of the camera.
     
     @param newSceneCenter center of the scene.
     */
    void setSceneCenter(glm::vec3 newSceneCenter);
    
    /*!
     Set camera zoom factor.
     
     @param zoom set camera zoom.
     */
    void setZoomFactor(float zoom);
    
    /*!
     Set rotation factors (degrees).
     
     @param newPhiRotation azimuth angle of rotation.
     @param newThetaRotation polar angle of rotation.
     */
    void setRotationFactors(float newPhiRotation, float newThetaRotation);
    
    /*!
     Update camera position.
     
     @returns camera matrix updated.
     */
    glm::mat4 lookAtMatrix();
};

#endif /* OpenGLCamera_hpp */
