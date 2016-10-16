package it.chicio.spectralbrdfexplorer;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.Arrays;

public class ScenesActivity extends Activity {

    /**
     * ListView that contains the list of the available scenes.
     */
    private ListView sceneListView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_scenes);

        sceneListView = (ListView)findViewById(R.id.scenes_list_view);

        //Scene name list.
        ArrayList<String> sceneList = new ArrayList<String>(Arrays.asList(
            "RGB Scene",
            "Spectral Scene"
        ));

        //Set scene list adapter.
        sceneListView.setAdapter(new SceneListAdapter(this, sceneList));

        //Set click listener.
        final Context context = this;
        sceneListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                //TODO: dynamic selection.

                //Start OpenGL render activity.
                Intent openglRenderIntent = new Intent(context, OpenGLRenderActivity.class);
                startActivity(openglRenderIntent);
            }
        });
    }
}
