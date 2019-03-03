package com.example.todolist;

import java.text.SimpleDateFormat;
import java.util.Date;

public class ToDoItem {

	String task;
	Date created;
	
	public String getTask() {
		return task;
	}
	public Date getCreated() {
		return created;
	}
	
	public ToDoItem(String task){
		this(task, new Date(System.currentTimeMillis()));
	}
	
	public ToDoItem(String task, Date created) {
		super();
		this.task = task;
		this.created = created;
	}
	
	@Override
	public String toString() {
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat("dd/MM/yy");
		String dateString = simpleDateFormat.format(created);
		return "(" + dateString + ") " + task;
	}
	
}
