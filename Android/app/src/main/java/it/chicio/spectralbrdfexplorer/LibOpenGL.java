package it.chicio.spectralbrdfexplorer;

import android.content.res.AssetManager;

/**
 * Created by chicio on 10/09/16.
 */
public class LibOpenGL {

    static {
        System.loadLibrary("LibOpenGLJNI");
    }

    public static native void defaultStartRender(AssetManager assetManager,
                                                 String vertexShaderSource,
                                                 String fragmentShaderSource,
                                                 String shadowMappingVertexShaderSource,
                                                 String shadowMappingFragmentShaderSource);
    public static native void update(int width, int height, int timeSinceLastUpdate);
    public static native void draw();
}
