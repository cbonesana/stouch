package com.example.messagebetweenactivityandservice;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

	private Button buttonStart;
	private Button buttonStop;
	private Button buttonBind;
	private Button buttonUnbind;
	private Button buttonUpBy1;
	private Button buttonUpBy10;
	
	private TextView textStatus;
	private TextView textIntValue;
	private TextView textStringValue;
	
	private boolean isBound;
	
	private Messenger service = null;
	private final Messenger messenger = new Messenger (new IncomingHandler());
	
	private ServiceConnection connection = new ServiceConnection() {
		public void onServiceConnected(android.content.ComponentName name, android.os.IBinder _service) {
			service = new Messenger(_service);
			textStatus.setText("Attached.");
			try {
				Message msg = Message.obtain(null, MyService.MSG_REGISTER_CLIENT);
				msg.replyTo = messenger;
				service.send(msg);
			} catch (RemoteException e) {
                // In this case the service has crashed before we could even do anything with it
			}
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
            // This is called when the connection with the service has been unexpectedly disconnected - process crashed.
            service = null;
            textStatus.setText("Disconnected.");
		};
	};
	
	private class IncomingHandler extends Handler{
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MyService.MSG_SET_INT_VALUE:
				textIntValue.setText("Int Message: " + msg.arg1);
				break;
			case MyService.MSG_SET_STRING_VALUE:
				String str1 = msg.getData().getString("str1");
				textStringValue.setText("Str Message: " + str1);
				break;
			default:
				super.handleMessage(msg);
			}
		}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		buttonStart = (Button)findViewById(R.id.buttonStart);
		buttonStop = (Button)findViewById(R.id.buttonStop);
		buttonBind = (Button)findViewById(R.id.buttonBind);
		buttonUnbind = (Button)findViewById(R.id.buttonUnbind);
		textStatus = (TextView)findViewById(R.id.textStatus);
		textIntValue = (TextView)findViewById(R.id.textIntValue);
		textStringValue = (TextView)findViewById(R.id.textStringValue);
		buttonUpBy1 = (Button)findViewById(R.id.buttonUpy1);
		buttonUpBy10 = (Button)findViewById(R.id.buttonUpy10);
		
		buttonStart.setOnClickListener(buttonStartListener);
		buttonStop.setOnClickListener(buttonStopListener);
		buttonBind.setOnClickListener(buttonBindListener);
		buttonUnbind.setOnClickListener(buttonUnbindListener);
		buttonUpBy1.setOnClickListener(buttonUpby1Listener);
		buttonUpBy10.setOnClickListener(buttonUpby10Listener);
		
		restoreMe(savedInstanceState);
		
		checkIfServiceIsRunning();
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		outState.putString("textStatus", textStatus.getText().toString());
		outState.putString("textIntValue", textIntValue.getText().toString());
		outState.putString("textStrValue", textStringValue.getText().toString());
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	private void restoreMe(Bundle state){
		if (state != null){
			textStatus.setText(state.getString("textStatus"));
			textIntValue.setText(state.getString("textIntValue"));
			textStringValue.setText(state.getString("textStrValue"));
		}
	}

	private void checkIfServiceIsRunning() {
        // If the service is running when the activity starts, we want to automatically bind to it.
		if (MyService.isRunning()){
			doBindService();
		}
	}
	
	private OnClickListener buttonStartListener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			startService(new Intent(MainActivity.this, MyService.class));
		}
	};
	
	private OnClickListener buttonStopListener = new OnClickListener() {
        public void onClick(View v){
            doUnbindService();
            stopService(new Intent(MainActivity.this, MyService.class));
        }
    };
    
	private OnClickListener buttonBindListener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			doBindService();
		}
	};
	
	private OnClickListener buttonUnbindListener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			doUnbindService();
		}
	};
	
	private OnClickListener buttonUpby1Listener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			sendMessageToService(1);
		}
	};
	
	private OnClickListener buttonUpby10Listener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			sendMessageToService(10);
		}
	};
	
	private void sendMessageToService(int intValueToSend) {
		if (isBound){
			if (service != null){
				try {
				Message msg = Message.obtain(null, MyService.MSG_SET_INT_VALUE, intValueToSend, 0);
				msg.replyTo = messenger;
				service.send(msg);
				} catch (RemoteException ignored){}
			} 
		}
	}
	
	void doBindService(){
		bindService(new Intent(this, MyService.class), connection, Context.BIND_AUTO_CREATE);
		isBound = true;
		textStatus.setText("Binding...");
	}
	
	void doUnbindService(){
		if (isBound){
            // If we have received the service, and hence registered with it, then now is the time to unregister.
			if ( service != null) {
				try {
					Message msg = Message.obtain(null, MyService.MSG_UNREGISTER_CLIENT);
					msg.replyTo = messenger;
					service.send(msg);
				} catch (RemoteException ignored) {
                    // There is nothing special we need to do if the service has crashed.
				}
			}
			unbindService(connection);
			isBound = false;
			textStatus.setText("Unbinding...");
		}
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		try {
			doUnbindService();
		} catch (Throwable t) {
			Log.e("MainActivity", "Failed to unbind from the service", t);
		}
	}
}
