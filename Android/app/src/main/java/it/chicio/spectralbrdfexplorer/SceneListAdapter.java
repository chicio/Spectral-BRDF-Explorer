package it.chicio.spectralbrdfexplorer;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by chicio on 16/10/2016.
 */

public class SceneListAdapter extends BaseAdapter {

    /**
     * Current context.
     */
    private Context context;

    /**
     * Layout inflater for the adapter.
     */
    private LayoutInflater inflater;

    /**
     * Data source with the list of available scenes.
     */
    private ArrayList<String> scenesList;

    /**
     * Constructor.
     *
     * @param aContext
     * @param aSceneList
     */
    public SceneListAdapter(Context aContext, ArrayList<String> aSceneList) {

        context = aContext;
        scenesList = aSceneList;
        inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public int getCount() {

        return scenesList.size();
    }

    @Override
    public Object getItem(int position) {

        return scenesList.get(position);
    }

    @Override
    public long getItemId(int position) {

        //No id in scene list.
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        //Inflate scene item layout.
        View listItemView = inflater.inflate(R.layout.list_item_scene, parent, false);

        //Set scene name.
        TextView sceneNameTextView = (TextView)listItemView.findViewById(R.id.scene_name_textview);
        sceneNameTextView.setText(scenesList.get(position));

        return listItemView;
    }
}
