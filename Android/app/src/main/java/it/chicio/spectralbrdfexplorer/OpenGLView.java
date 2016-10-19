package it.chicio.spectralbrdfexplorer;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class OpenGLView extends GLSurfaceView {

    /**
     * Current OpenGL ES Viewport.
     */
    private int currentWidth;

    /**
     * Current OpenGL ES Viewport.
     */
    private int currentHeight;

    /**
     * Gesture detector.
     */
    private GestureDetector gestureDetector;

    public OpenGLView(Context context) {

        super(context);

        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);

        //Setup gesture detector.
        gestureDetector = new GestureDetector(context, new OpenGLGestureListener());

        //Setup renderer.
        setRenderer(new OpenGLAndroidRenderer());
    }

    public class OpenGLAndroidRenderer implements GLSurfaceView.Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {

            LibOpenGL.startOpenGLESRender(getResources().getAssets());
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
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {

        this.gestureDetector.onTouchEvent(event);

        return true;
    }

    class OpenGLGestureListener extends GestureDetector.SimpleOnGestureListener {

        private static final String DEBUG_TAG = "Gestures";

        @Override
        public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {

            Log.d(DEBUG_TAG, "onScroll: " + distanceX + " - " + distanceY);

            LibOpenGL.cameraRotation(distanceX, distanceY);

            return true;
        }
    }
}
