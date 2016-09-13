package it.chicio.spectralbrdfexplorer;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class OpenGLRenderActivity extends Activity {

    private GLSurfaceView openGLView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        //Setup OpenGL view.
        openGLView = new OpenGLView(this);
        setContentView(openGLView);
    }
}
