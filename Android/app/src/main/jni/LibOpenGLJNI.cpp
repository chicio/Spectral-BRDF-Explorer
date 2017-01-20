#include <jni.h>
#include <android/asset_manager_jni.h>

#include "OpenGLRenderer.hpp"
#include "SceneLoader.h"
#include "LibOpenGLJNI.h"
#include "android_fopen.h"

static OpenGLRenderer* openGLRenderer;

JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_start(JNIEnv *env,
                                                                           jclass type) {

    //Start render.
    std::string error;
    openGLRenderer = new OpenGLRenderer();
    openGLRenderer->start(OpenGLCamera(glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 0.0f, -5.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f)),
                          error);
}

JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_update(JNIEnv *env,
                                                                            jclass type,
                                                                            jint width,
                                                                            jint height,
                                                                            jint timeSinceLastUpdate) {

    //Update.
    openGLRenderer->update(width, height, timeSinceLastUpdate);
}

JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_draw(JNIEnv *env, jclass type) {

    //Draw
    openGLRenderer->draw();
}

JNIEXPORT void JNICALL
Java_it_chicio_spectralbrdfexplorer_LibOpenGL_cameraRotation(JNIEnv *env, jclass type,
                                                             jfloat rotationX, jfloat rotationY) {

    //Set rotation factors.
    openGLRenderer->openGLCamera.setRotationFactors(rotationX * -0.02, rotationY * 0.01);

    //Update camera.
    openGLRenderer->openGLCamera.updateCamera();
}

JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_cameraZoom(JNIEnv *env,
                                                                                jclass type,
                                                                                jfloat scale) {

    if(scale < 1.0) {

        //if < 1.0 zoom out.
        openGLRenderer->openGLCamera.setZoomFactor(scale * -0.30f);
    } else {

        //if > 1.0 zoom in.
        openGLRenderer->openGLCamera.setZoomFactor(scale * 0.08f);
    }

    //Update camera.
    openGLRenderer->openGLCamera.updateCamera();
}

JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_loadAssetManager(JNIEnv *env,
                                                                                      jclass type,
                                                                                      jobject assetManager) {

    //Get assets manager native reference.
    android_fopen_set_asset_manager(AAssetManager_fromJava(env, assetManager));
}


JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_loadScene(JNIEnv *env,
                                                                               jclass type,
                                                                               jint sceneIdentifier) {

    //Load scene.
    if (sceneIdentifier == 1) {

        SceneLoader::loadSpectralScene();
    } else {

        SceneLoader::loadRGBScene();
    }
}
