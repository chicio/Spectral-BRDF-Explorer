package it.chicio.spectralbrdfexplorer;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

public class OpenGLRenderActivity extends Activity {

    private GLSurfaceView openGLView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        //Get scene selected.
        int sceneSelected = 0;
        Bundle extras = getIntent().getExtras();

        if (extras != null) {

            sceneSelected = extras.getInt(getString(R.string.sceneSelectedIntentExtra));
        }

        LibOpenGL.loadAssetManager(getAssets());
        LibOpenGL.loadScene(sceneSelected);

        //Setup OpenGL view.
        openGLView = new OpenGLView(this);
        setContentView(openGLView);
    }
}
