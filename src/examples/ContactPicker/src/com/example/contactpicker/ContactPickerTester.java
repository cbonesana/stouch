package com.example.contactpicker;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class ContactPickerTester extends Activity {

	public static final int PICK_CONTACT = 1;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.contactpickertester);
		
		Button button = (Button)findViewById(R.id.pick_contact_button);
		
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View _view) {
				Intent intent = new Intent(Intent.ACTION_PICK, Uri.parse("content://contacts/"));
				startActivityForResult(intent, PICK_CONTACT);
			}
		});
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		switch (requestCode) {
		case PICK_CONTACT:
			if (resultCode == Activity.RESULT_OK){
				Uri contactData = data.getData();
				Cursor cursor = getContentResolver().query(contactData, null, null, null, null);
				cursor.moveToFirst();
				String name = cursor.getString(cursor.getColumnIndexOrThrow(ContactsContract.Contacts.DISPLAY_NAME_PRIMARY));
				cursor.close();
				TextView tv = (TextView)findViewById(R.id.selected_contact_textview);
				tv.setText(name);
			}
			break;

		default:
			break;
		}
	}
	
}
