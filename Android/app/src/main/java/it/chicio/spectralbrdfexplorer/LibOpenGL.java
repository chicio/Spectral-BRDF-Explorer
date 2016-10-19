package it.chicio.spectralbrdfexplorer;

import android.content.res.AssetManager;

/**
 * Created by chicio on 10/09/16.
 */
public class LibOpenGL {

    static {
        System.loadLibrary("LibOpenGLJNI");
    }

    public static native void startOpenGLESRender(AssetManager assetManager);
    public static native void update(int width, int height, int timeSinceLastUpdate);
    public static native void draw();
    public static native void cameraRotation(float rotationX, float rotationY);
}
