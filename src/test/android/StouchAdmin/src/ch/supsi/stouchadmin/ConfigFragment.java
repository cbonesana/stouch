package ch.supsi.stouchadmin;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import com.actionbarsherlock.app.SherlockFragment;

public class ConfigFragment extends SherlockFragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        final View view = inflater.inflate(R.layout.config_fragment, container, false);
        final SharedPreferences pref = getActivity().getSharedPreferences(MainActivity.PREF_NAME, 0);
        Button b = (Button) view.findViewById(R.id.saveButton);
        EditText et = (EditText) view.findViewById(R.id.usernameEditText);
        et.setText(pref.getString("username", ""));
        et = (EditText) view.findViewById(R.id.passwordEditText);
        et.setText(pref.getString("password", ""));
        b.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				SharedPreferences.Editor editor = pref.edit();
				EditText et = (EditText) view.findViewById(R.id.usernameEditText);
				String user = et.getText().toString();
				editor.putString("username", user);
				et = (EditText) view.findViewById(R.id.passwordEditText);
				String pass = et.getText().toString();
				editor.putString("password", pass);
				editor.apply();
			}
		});
        return view;
    }
}
