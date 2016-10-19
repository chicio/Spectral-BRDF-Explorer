#include <jni.h>
#include <android/asset_manager_jni.h>

#include "OpenGLRenderer.hpp"
#include "SceneLoader.h"
#include "LibOpenGLJNI.h"
#include "android_fopen.h"

static OpenGLRenderer* openGLRenderer;

JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_start(JNIEnv *env,
                                                                           jclass type,
                                                                           jobject assetManager) {

    //Get asset manager native reference.
    android_fopen_set_asset_manager(AAssetManager_fromJava(env, assetManager));

    //TODO: move outside (dynamic in listview).
    //Load scene.
    SceneLoader::loadRGBSceneAndroid();

    //Start render.
    std::string error;
    openGLRenderer = new OpenGLRenderer();
    openGLRenderer->startRenderer(OpenGLCamera(glm::vec3(0.0f, 0.0f, 0.0f),
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

    openGLRenderer->openGLCamera.setRotationFactors(rotationX * -0.02, rotationY * 0.01);
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
}