package it.chicio.spectralbrdfexplorer;

import android.content.Context;
import android.content.res.AssetManager;
import android.gesture.Gesture;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

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

    /**
     * Scale gesture detector.
     */
    private ScaleGestureDetector scaleGestureDetector;

    public OpenGLView(Context context) {

        super(context);

        //Setup EGL surface.
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);

        //Setup gesture detectors.
        gestureDetector = new GestureDetector(context, new GestureListener());
        scaleGestureDetector = new ScaleGestureDetector(context, new ScaleGestureListener());

        //Setup renderer.
        setRenderer(new OpenGLAndroidRenderer());
    }

    public class OpenGLAndroidRenderer implements GLSurfaceView.Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {

            LibOpenGL.start(getResources().getAssets());
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

            //Update view size properties (used by renderer in rendering calculation).
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

        //Get number of fingers touching.
        int numberOfFingers = event.getPointerCount();

        if (numberOfFingers == 1) {

            //1 finger => scroll gesture.
            this.gestureDetector.onTouchEvent(event);
        } else {

            this.scaleGestureDetector.onTouchEvent(event);
        }

        return true;
    }

    private class GestureListener extends GestureDetector.SimpleOnGestureListener {

        private static final String DEBUG_TAG = "Gestures";

        @Override
        public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {

            Log.d(DEBUG_TAG, "onScroll: " + distanceX + " - " + distanceY);

            //Rotate camera on scroll.
            LibOpenGL.cameraRotation(distanceX, distanceY);

            return true;
        }
    }

    private class ScaleGestureListener implements ScaleGestureDetector.OnScaleGestureListener {

        private static final String DEBUG_TAG = "Scale";

        @Override
        public boolean onScale(ScaleGestureDetector detector) {

            Log.d(DEBUG_TAG, "scale: " + detector.getScaleFactor());

            //Zoom camera on scale.
            LibOpenGL.cameraZoom(detector.getScaleFactor());

            return true;
        }

        @Override
        public boolean onScaleBegin(ScaleGestureDetector detector) {

            return true;
        }

        @Override
        public void onScaleEnd(ScaleGestureDetector detector) { }
    }
}
