//
// Created by Fabrizio Duroni on 10/09/16.
//

#include <jni.h>

extern "C" {
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_start(JNIEnv *env,
                                                                               jclass type);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_update(JNIEnv *env,
                                                                                jclass type,
                                                                                jint width,
                                                                                jint height,
                                                                                jint timeSinceLastUpdate);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_draw(JNIEnv *env, jclass type);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_cameraRotation(JNIEnv *env,
                                                                                        jclass type,
                                                                                        jfloat rotationX,
                                                                                        jfloat rotationY);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_cameraZoom(JNIEnv *env,
                                                                                    jclass type,
                                                                                    jfloat scale);

    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_loadAssetManager(JNIEnv *env,
                                                                                          jclass type,
                                                                                          jobject assetManager);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_loadScene(JNIEnv *env,
                                                                                   jclass type,
                                                                                   jint sceneIdentifier);
}
