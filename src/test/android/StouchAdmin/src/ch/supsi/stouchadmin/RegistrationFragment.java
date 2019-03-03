package ch.supsi.stouchadmin;

import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;

import com.actionbarsherlock.app.SherlockFragment;

public class RegistrationFragment extends SherlockFragment {
	
	private int lang = 1;
	
	public int getLang() {
		return lang;
	}

	public void setLang(int lang) {
		this.lang = lang;
	}

	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    /**
     * The Fragment's UI is just a simple text view showing its
     * instance number.
     */
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.registration_fragment, container, false);
        
        final EditText et = (EditText) v.findViewById(R.id.email_input);
        
        RadioGroup rg = (RadioGroup) v.findViewById(R.id.radio_lang);
        rg.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				// TODO Auto-generated method stub
				switch(checkedId) {
				case R.id.radio_it:
					setLang(1);
					break;
				case R.id.radio_en:
					setLang(2);
					break;
				case R.id.radio_de:
					setLang(5);
					break;
				case R.id.radio_fr:
					setLang(4);
					break;
				}
			}
		});
        
        Button b = (Button) v.findViewById(R.id.email_submit);
        b.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent i = new Intent(getActivity().getBaseContext(), NetworkService.class);
				i.putExtra("email", et.getEditableText().toString());
				i.putExtra("lang", getLang());
				getActivity().startService(i);
				
			}
		});
        
        return v;
    }
    
}
