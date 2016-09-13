#include <jni.h>
#include <android/asset_manager_jni.h>
#include <OpenGLRenderer.hpp>
#include "LibOpenGLJNI.h"
#include "android_fopen.h"

static OpenGLRenderer* renderer;

JNIEXPORT void JNICALL Java_it_chicio_spectralbrdfexplorer_LibOpenGL_update(JNIEnv *env,
                                                                            jclass type,
                                                                            jint width,
                                                                            jint height,
                                                                            jint timeSinceLastUpdate) {

    //Update.
    renderer->update(width, height, timeSinceLastUpdate);
}

JNIEXPORT void JNICALL
Java_it_chicio_spectralbrdfexplorer_LibOpenGL_draw(JNIEnv *env, jclass type) {

    //Draw
    renderer->draw();
}

JNIEXPORT void JNICALL
Java_it_chicio_spectralbrdfexplorer_LibOpenGL_defaultStartRender(JNIEnv *env,
                                                                 jclass type,
                                                                 jobject assetMgr,
                                                                 jstring vertexShaderSource_,
                                                                 jstring fragmentShaderSource_,
                                                                 jstring shadowMappingVertexShaderSource_,
                                                                 jstring shadowMappingFragmentShaderSource_) {

    const char *vertexShaderSource = env->GetStringUTFChars(vertexShaderSource_, 0);
    const char *fragmentShaderSource = env->GetStringUTFChars(fragmentShaderSource_, 0);
    const char *shadowMappingVertexShaderSource = env->GetStringUTFChars(shadowMappingVertexShaderSource_, 0);
    const char *shadowMappingFragmentShaderSource = env->GetStringUTFChars(shadowMappingFragmentShaderSource_, 0);

    android_fopen_set_asset_manager(AAssetManager_fromJava(env, assetMgr));

    renderer = OpenGLRenderer::defaultStartRender(vertexShaderSource,
                                                  fragmentShaderSource,
                                                  shadowMappingVertexShaderSource,
                                                  shadowMappingFragmentShaderSource);

    renderer->loadScene();


    env->ReleaseStringUTFChars(vertexShaderSource_, vertexShaderSource);
    env->ReleaseStringUTFChars(fragmentShaderSource_, fragmentShaderSource);
    env->ReleaseStringUTFChars(shadowMappingVertexShaderSource_, shadowMappingVertexShaderSource);
    env->ReleaseStringUTFChars(shadowMappingFragmentShaderSource_, shadowMappingFragmentShaderSource);
}