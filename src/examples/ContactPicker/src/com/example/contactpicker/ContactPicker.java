package com.example.contactpicker;

import android.app.Activity;
import android.content.ContentUris;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.provider.ContactsContract.Contacts;
import android.support.v4.widget.CursorAdapter;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

public class ContactPicker extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		final Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
		String[] from = new String[] { Contacts.DISPLAY_NAME_PRIMARY };
		int[] to = new int[] { R.id.itemTextView };

		SimpleCursorAdapter adapter = new SimpleCursorAdapter(this, R.layout.listitemlayout, cursor, from, to, CursorAdapter.NO_SELECTION);
		ListView lv = (ListView)findViewById(R.id.contactListview);
		lv.setAdapter(adapter);

		lv.setOnItemClickListener(new ListView.OnItemClickListener(){
			@Override
			public void onItemClick(AdapterView<?> parent, View view, int pos, long id) {
				// Move the cursor to the selected item
				cursor.moveToPosition(pos);
				// Extract the row id.
				int rowId = cursor.getInt(cursor.getColumnIndexOrThrow("_id"));
				Uri outURI = ContentUris.withAppendedId(ContactsContract.Contacts.CONTENT_URI, rowId);
				Intent outData = new Intent();
				outData.setData(outURI);
				setResult(Activity.RESULT_OK, outData);
				finish();
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
