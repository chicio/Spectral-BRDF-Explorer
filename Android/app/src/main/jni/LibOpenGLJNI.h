//
// Created by Fabrizio Duroni on 10/09/16.
//

#include <jni.h>

extern "C" {
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_startOpenGLESRender(JNIEnv *env,
                                                                                             jclass type,
                                                                                             jobject assetManager);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_update(JNIEnv *env,
                                                                                jclass type,
                                                                                jint width,
                                                                                jint height,
                                                                                jint timeSinceLastUpdate);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_draw(JNIEnv *env, jclass type);
}
