package com.example.earthquakeviewer;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceActivity;

public class PreferencesActivity extends PreferenceActivity {
	
	public static final String USER_PREFERENCE = "USER_PREFERENCE";
	public static final String PREF_AUTO_UPDATE = "PREF_AUTO_UPDATE";
	public static final String PREF_MIN_MAG = "PREF_UPDATE_FREQ";
	public static final String PREF_UPDATE_FREQ = "PREF_UPDATE_FREQ";
	
	SharedPreferences prefs;
	
	@SuppressWarnings("deprecation")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		addPreferencesFromResource(R.xml.userpreferences);
	}
}
