//
// Created by Fabrizio Duroni on 10/09/16.
//

#include <jni.h>

extern "C" {
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_defaultStartRender(JNIEnv *env,
                                                                                            jclass type,
                                                                                            jobject assetMgr,
                                                                                            jstring vertexShaderSource_,
                                                                                            jstring fragmentShaderSource_,
                                                                                            jstring shadowMappingVertexShaderSource_,
                                                                                            jstring shadowMappingFragmentShaderSource_);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_update(JNIEnv *env,
                                                                                jclass type,
                                                                                jint width,
                                                                                jint height,
                                                                                jint timeSinceLastUpdate);
    JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_draw(JNIEnv *env, jclass type);
}
