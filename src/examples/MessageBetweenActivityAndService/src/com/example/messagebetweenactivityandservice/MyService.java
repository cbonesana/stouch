package com.example.messagebetweenactivityandservice;

import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

import android.annotation.SuppressLint;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.location.GpsStatus.NmeaListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;

public class MyService extends Service {

	private NotificationManager notificationManager;
	private Timer timer = new Timer();

	private int counter = 0;
	private int incrementBy = 1;

	private static boolean isRunning = false;

	public ArrayList<Messenger> clients = new ArrayList<Messenger>();
	public int value = 0;

	public static final int MSG_REGISTER_CLIENT = 1;
	public static final int MSG_UNREGISTER_CLIENT = 2;
	public static final int MSG_SET_INT_VALUE = 3;
	public static final int MSG_SET_STRING_VALUE = 4;

	public final Messenger messenger = new Messenger(new IncomingHandler());

	@Override
	public IBinder onBind(Intent intent) {
		return messenger.getBinder();
	}

	@Override
	public void onCreate() {
		super.onCreate();
		Log.i("MyService", "Service Started.");
		showNotification();
		timer.scheduleAtFixedRate(
				new TimerTask(){
					public void run(){
						onTimerTick();
					}
				}, 0, 100L);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.i("MyService", "Received start id " + startId + ": " + intent);
		return START_STICKY;
	}
	
	@Override
	public void onDestroy() {
		super.onDestroy();
		if (timer != null){
			timer.cancel();
		}
		counter = 0;
		// Cancel the persistent notification.
        notificationManager.cancel(R.string.service_started);
        Log.i("MyService", "Service Stopped.");
        isRunning = false;
	}
	
	public static boolean isRunning(){
		return isRunning;
	}
	
	private void showNotification(){
		notificationManager = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        // In this sample, we'll use the same text for the ticker and the expanded notification
		CharSequence text = getText(R.string.service_started);
        // Set the icon, scrolling text and timestamp
        Notification notification = new Notification(R.drawable.ic_launcher, text, System.currentTimeMillis());
		// The PendingIntent to launch our activity if the user selects this notification
        PendingIntent contentIntent = PendingIntent.getActivity(this, 0, new Intent(this, MainActivity.class), 0);
        // Set the info for the views that show in the notification panel.
        notification.setLatestEventInfo(this, getText(R.string.service_label), text, contentIntent);
        // Send the notification.
        // We use a layout id because it is a unique number.  We use it later to cancel.
        notificationManager.notify(R.string.service_started, notification);
	}
	
	private void onTimerTick(){
		Log.i("TimerTick", "Timer doing work. " + counter);
		try {
			counter += incrementBy;
			sendMessageToUI(counter);
		} catch (Throwable t){
			Log.e("TimerTick", "Timer Tick Failed.", t);
		}
	}
	
	private void sendMessageToUI(int intValueToSend) {
		for (int i = clients.size()-1; i>=0; i--) {
			try {
				// Send data as an Integer
				clients.get(i).send(Message.obtain(null, MSG_SET_INT_VALUE, intValueToSend, 0));

				// Send data as a String
				Bundle b = new Bundle();
				b.putString("str1", "ab" + intValueToSend + "cd");
				Message msg = Message.obtain(null, MSG_SET_STRING_VALUE);
				msg.setData(b);
				clients.get(i).send(msg);
			} catch (Exception e) {
				// The client is dead. Remove it from the list; we are going through the list from back to front so this is safe to do inside the loop.
				clients.remove(i);
			}
		}
	}

	@SuppressLint("HandlerLeak")
	public class IncomingHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_REGISTER_CLIENT:
				clients.add(msg.replyTo);
				break;
			case MSG_UNREGISTER_CLIENT:
				clients.remove(msg.replyTo);
				break;
			case MSG_SET_INT_VALUE:
				incrementBy = msg.arg1;
				break;
			default:
				super.handleMessage(msg);
			}
		}
	}

}
