package com.example.todolist;

import java.util.ArrayList;

import android.app.Activity;
import android.app.FragmentManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;

public class ToDoActivity extends Activity implements OnNewItemAddedListener{
		
	private ToDoItemAdapter arrayAdapter;
	private ArrayList<ToDoItem> toDoItems;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		// Inflate your View
		setContentView(R.layout.activity_to_do);
		
		// Get references to the Fragments
		FragmentManager fragmentManager = getFragmentManager();
		ToDoListFragment toDoListFragment = (ToDoListFragment)fragmentManager.findFragmentById(R.id.ToDoListFragment);

		// Create the ArrayList of items
		toDoItems = new ArrayList<ToDoItem>();
	
		// Create the array adapter to bind the array to the List View
		int resID = R.layout.todolist_item;
		arrayAdapter = new ToDoItemAdapter(this, resID, toDoItems);
		
		// Bind the Array Adapter to the ListView
		toDoListFragment.setListAdapter(arrayAdapter);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.to_do, menu);
		return true;
	}

	@Override
	public void onNewItemAdded(String newItem) {
		ToDoItem newToDoItem = new ToDoItem(newItem);
		Log.d("NewItem", newItem.toString());
		toDoItems.add(newToDoItem);
		arrayAdapter.notifyDataSetChanged();
	}

}
