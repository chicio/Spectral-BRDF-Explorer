package it.chicio.spectralbrdfexplorer;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class OpenGLView extends GLSurfaceView {

    private int currentWidth;
    private int currentHeight;
    private AssetManager assetMgr;

    public OpenGLView(Context context) {

        super(context);

        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);

        //Setup renderer.
        setRenderer(new OpenGLAndroidRenderer());
    }

    public class OpenGLAndroidRenderer implements GLSurfaceView.Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {

            String vertexShaderSource = getShaderSource(getContext(), "PhongVertex", "vsh");
            String fragmentShaderSource = getShaderSource(getContext(), "PhongFragment", "fsh");
            String shadowMappingVertexShaderSource = getShaderSource(getContext(), "ShadowMapVertex", "vsh");
            String shadowMappingFragmentShaderSource = getShaderSource(getContext(), "ShadowMapFragment", "fsh");

            assetMgr = getResources().getAssets();

            LibOpenGL.defaultStartRender(assetMgr, vertexShaderSource, fragmentShaderSource, shadowMappingVertexShaderSource, shadowMappingFragmentShaderSource);
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

            currentWidth = width;
            currentHeight = height;
        }

        @Override
        public void onDrawFrame(GL10 gl) {

            LibOpenGL.update(currentWidth, currentHeight, 0);
            LibOpenGL.draw();
        }

        private String getShaderSource(Context context, String name, String extension) {

            InputStream input;
            String content = "";

            try {
                AssetManager assetManager = context.getAssets();
                input = assetManager.open("Shaders/" + name + "." + extension);

                int size = input.available();
                byte[] buffer = new byte[size];
                input.read(buffer);
                input.close();
                content = new String(buffer);
            } catch (IOException e) {
                e.printStackTrace();
            }

            return content;
        }
    }
}
