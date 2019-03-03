package ch.denter.TVKeyboard.BTService;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

import ch.denter.TVKeyboard.Constants;
import ch.denter.TVKeyboard.MainActivity;
import ch.denter.TVKeyboard.R;

/**
 * This class does all the work for setting up and managing Bluetooth
 * connections with device named BLUROSSO. It has a thread for connecting with a
 * device, and a thread for performing data transmissions when connected.
 */
public class BluetoothService extends Service {
	// Debugging
	private static final String TAG = "BluetoothService";
	private static final boolean D = true;

	// Member fields
	private BluetoothAdapter mBluetoothAdapter;

	private ConnectThread mConnectThread;
	private ConnectedThread mConnectedThread;
	private int mState;

	private NotificationManager mNotificationManager;

	private static boolean isRunning = false;
	// Keeps track of all current registered clients.
	ArrayList<Messenger> mClients = new ArrayList<Messenger>();

	// Target we publish for clients to send messages to IncomingHandler.
	final Messenger mMessenger = new Messenger(new IncomingHandler());

	@Override
	public IBinder onBind(Intent intent) {
		return mMessenger.getBinder();
	}

	class IncomingHandler extends Handler {
		// Handler of incoming messages from
		// clients.
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case Constants.BT_REGISTER_CLIENT:
				mClients.add(msg.replyTo);
				// inform clients about our state
				Message mg = Message.obtain(null, Constants.BT_STATE_CHANGE);
				Bundle bundle = new Bundle();
				bundle.putInt(Constants.BT_TAG, mState);
				mg.setData(bundle);
				sendMessageToUI(mg);
				break;
			case Constants.BT_UNREGISTER_CLIENT:
				mClients.remove(msg.replyTo);
				break;
			case Constants.BT_MSG_FOR_SEND:
				try {
					byte[] b = (byte[]) msg.obj;
					mConnectedThread.write(b);
				} catch (Exception e) {
					Log.e(TAG, "Can't write in mConnectedThread");
					e.printStackTrace();

					Message m = Message.obtain(null,
							Constants.BT_ERROR_IN_WRITE_MSG);
					sendMessageToUI(m);
				}

				break;
			default:
				super.handleMessage(msg);
			}
		}
	}

	private void sendMessageToUI(Message msg) {
		Log.i(TAG, mClients.size() + "");
		for (int i = mClients.size() - 1; i >= 0; --i) {
			try {
				Message m = new Message();
				m.copyFrom(msg);
				mClients.get(i).send(m);
			} catch (RemoteException re) {
				// The client is dead. Remove it from the list; we are going
				// through the list from back to front so this is safe to do
				// inside the loop.
				mClients.remove(i);
				re.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}

		}
	}

	@Override
	public void onCreate() {
		super.onCreate();
		if (D)
			Log.d(TAG, "Create");
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		// If BT is not on activity will request that it be enabled.
		setState(Constants.STATE_NONE);

		Log.i(TAG, "Service Started.");
		showNotification();
		regRecevers();

		// start search
		doDiscovery();
		isRunning = true;

	}

	void regRecevers() {
		// Register for broadcasts when a device is discovered
		IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
		BluetoothService.this.registerReceiver(mReceiver, filter);

		// Register for broadcasts when discovery has finished
		filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
		BluetoothService.this.registerReceiver(mReceiver, filter);
	}

	/**
	 * Start device discover with the BluetoothAdapter
	 */
	private void doDiscovery() {
		if (D)
			Log.d(TAG, "doDiscovery()");

		// If we're already discovering, stop it
		if (mBluetoothAdapter.isDiscovering()) {
			mBluetoothAdapter.cancelDiscovery();
		}

		// Request discover from BluetoothAdapter
		mBluetoothAdapter.startDiscovery();
		setState(Constants.STATE_SEARCHING);
		showNotification();
	}

	// The BroadcastReceiver that listens for discovered devices and
	// changes the title when discovery is finished
	private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			if (D)
				Log.d(TAG, "onReceive");
			String action = intent.getAction();
			// When discovery finds a device
			if (BluetoothDevice.ACTION_FOUND.equals(action)) {
				// Get the BluetoothDevice object from the Intent

				BluetoothDevice device = intent
						.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				if (device != null)
					if (D)
						Log.d(TAG, "Find " + device.getName());
				// If it's already paired, skip it, because it's been listed
				// already
				try {
					if ((device != null) && (device.getName() != null))

						if ((device.getName().equals(Constants.NEED_ADDRESS))) {
							Log.d(TAG, "Find device   "
									+ device.getName().toString());
							mBluetoothAdapter.cancelDiscovery();
							connect(device);
						}
				} catch (Exception e) {
					e.printStackTrace();
				}

			}
			// When discovery is finished and we don't find need device
			else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
				if (mState != Constants.STATE_CONNECTING) {
					doDiscovery();
				}
			}

		}
	};

	private void showNotification() {
		Log.d(TAG, "Notification");
		mNotificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

		String notify = BTServiceManager.getDescriptionOfBTState(mState,
				BluetoothService.this);

		Notification notification = new Notification(R.drawable.icon, notify,
				System.currentTimeMillis());
		// The PendingIntent to launch our activity if the user selects this
		// notification
		PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
				new Intent(this, MainActivity.class), 0);
		// Set the info for the views that show in the notification panel.
		notification.setLatestEventInfo(this, getText(R.string.service_label),
				notify, contentIntent);

		// Send the notification.
		mNotificationManager.notify(R.string.service_started, notification);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.d(TAG, "Received start id " + startId + ": " + intent);
		return START_STICKY; // run until explicitly stopped.
	}

	public static boolean isRunning() {
		return isRunning;
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		unRegRecever();
		mNotificationManager.cancel(R.string.service_started); // Cancel the
		// persistent
		stop(); // notification.
		Log.d(TAG, "Service Stopped.");
		isRunning = false;
	}

	void unRegRecever() {
		unregisterReceiver(mReceiver);
	}

	/**
	 * Set the current state of the connection
	 * 
	 * @param state
	 *            An integer defining the current connection state
	 */
	private synchronized void setState(int state) {
		if (D)
			Log.d(TAG, "setState() " + mState + " -> " + state);
		mState = state;
		// Give the new state to the Handler so the UI Activity can update
		Message msg = Message.obtain(null, Constants.BT_STATE_CHANGE);
		Bundle b = new Bundle();
		b.putInt(Constants.BT_TAG, state);
		msg.setData(b);
		sendMessageToUI(msg);
	}

	/**
	 * Start the ConnectThread to initiate a connection to a remote device.
	 * 
	 * @param device
	 *            The BluetoothDevice to connect
	 * @param secure
	 *            Socket Security type - Secure (true) , Insecure (false)
	 */
	public synchronized void connect(BluetoothDevice device) {
		if (D)
			Log.d(TAG, "connect to: " + device);

		// Cancel any thread attempting to make a connection
		if (mState == Constants.STATE_CONNECTING) {
			if (mConnectThread != null) {
				mConnectThread.cancel();
				mConnectThread = null;
			}
		}

		// Cancel any thread currently running a connection
		if (mConnectedThread != null) {
			mConnectedThread.cancel();
			mConnectedThread = null;
		}

		// Start the thread to connect with the given device
		mConnectThread = new ConnectThread(device);
		mConnectThread.start();
	}

	/**
	 * Start the ConnectedThread to begin managing a Bluetooth connection
	 * 
	 * @param socket
	 *            The BluetoothSocket on which the connection was made
	 * @param device
	 *            The BluetoothDevice that has been connected
	 */
	public synchronized void connected(BluetoothSocket socket,
			BluetoothDevice device) {
		if (D)
			Log.d(TAG, "connected");

		// Cancel the thread that completed the connection
		if (mConnectThread != null) {
			mConnectThread.cancel();
			mConnectThread = null;
		}

		// Cancel any thread currently running a connection
		if (mConnectedThread != null) {
			mConnectedThread.cancel();
			mConnectedThread = null;
		}

		// Start the thread to manage the connection and perform transmissions
		mConnectedThread = new ConnectedThread(socket);
		mConnectedThread.start();

	}

	/**
	 * Stop all threads
	 */
	public synchronized void stop() {
		if (D)
			Log.d(TAG, "stop");

		if (mConnectThread != null) {
			mConnectThread.cancel();
			mConnectThread = null;
		}

		if (mConnectedThread != null) {
			mConnectedThread.cancel();
			mConnectedThread = null;
		}

		setState(Constants.STATE_NONE);
	}

	//
	// /**
	// * Write to the ConnectedThread in an unsynchronized manner
	// *
	// * @param out
	// * The bytes to write
	// * @see ConnectedThread#write(byte[])
	// */
	// public void write(byte[] out) {
	// // Create temporary object
	// ConnectedThread r;
	// // Synchronize a copy of the ConnectedThread
	// synchronized (this) {
	// if (mState != Constants.STATE_CONNECTED)
	// return;
	// r = mConnectedThread;
	//
	// }
	// // Perform the write unsynchronized
	// r.write(out);
	// //for click sound
	// Message msg = Message.obtain(null, Constants.BT_MSG_SENDED);
	// sendMessageToUI(msg);
	//
	// }

	/**
	 * Indicate that the connection attempt failed and notify the UI Activity.
	 */
	private void connectionFailed() {

		Message msg = Message.obtain(null, Constants.BT_ERROR_CONNECTION_FAIL);
		sendMessageToUI(msg);
		// reconnection
		unRegRecever();
		regRecevers();
		doDiscovery();
		showNotification();
	}

	/**
	 * Indicate that the connection was lost and notify the UI Activity.
	 */
	private void connectionLost() {
		// Send a failure message back to the Activity
		Message msg = Message.obtain(null, Constants.BT_ERROR_CONNECTION_LOST);
		sendMessageToUI(msg);
		// reconnection
		unRegRecever();
		regRecevers();
		doDiscovery();
		showNotification();

	}

	/**
	 * This thread runs while attempting to make an outgoing connection with a
	 * device. It runs straight through; the connection either succeeds or
	 * fails.
	 */
	private class ConnectThread extends Thread {
		private final BluetoothSocket mmSocket;
		private final BluetoothDevice mmDevice;

		public ConnectThread(BluetoothDevice device) {
			mmDevice = device;
			BluetoothSocket tmp = null;
			setState(Constants.STATE_CONNECTING);
			// Get a BluetoothSocket for a connection with the
			// given BluetoothDevice
			try {
				tmp = device
						.createRfcommSocketToServiceRecord(Constants.MY_UUID_SECURE);
			} catch (IOException e) {
				Log.e(TAG, "Create() failed", e);
				e.printStackTrace();
			} catch (Exception ex) {
				ex.printStackTrace();
			}
			mmSocket = tmp;

		}

		public void run() {
			if (D)
				Log.d(TAG, "BEGIN mConnectThread ");
			setName("ConnectThread");

			// Make a connection to the BluetoothSocket
			try {
				// This is a blocking call and will only return on a
				// successful connection or an exception
				mmSocket.connect();

				setState(Constants.STATE_CONNECTING);
				showNotification();
			} catch (IOException e) {
				// Close the socket
				e.printStackTrace();
				Log.e(TAG, " to close() " + e);
				connectionFailed();
				return;
			}
			// send password
			OutputStream tmpOut = null;
			try {
				tmpOut = mmSocket.getOutputStream();
			} catch (IOException e) {
				Log.e(TAG, "temp sockets not created", e);
			}
			try {
				char[] ch = Constants.PASSWORD.toCharArray();
				byte[] buffer = new byte[ch.length];
				for (int i = 0; i < ch.length; i++)
					buffer[i] = (byte) ch[i];
				tmpOut.write(buffer);
			} catch (IOException e) {
				Log.e(TAG, "Exception during write password", e);
			}
			// Reset the ConnectThread because we're done
			synchronized (BluetoothService.this) {
				mConnectThread = null;
			}
			// Start the connected thread
			connected(mmSocket, mmDevice);

		}

		public void cancel() {
			try {
				mmSocket.close();
			} catch (IOException e) {
				Log.e(TAG, "close() of connect " + " socket failed", e);
			} catch (Exception e1) {
				e1.printStackTrace();
			}
		}
	}

	/**
	 * This thread runs during a connection with a remote device. It handles all
	 * incoming and outgoing transmissions.
	 */
	private class ConnectedThread extends Thread {
		private final BluetoothSocket mmSocket;
		private final InputStream mmInStream;
		private final OutputStream mmOutStream;

		public ConnectedThread(BluetoothSocket socket) {
			Log.d(TAG, "create ConnectedThread: ");
			mmSocket = socket;
			InputStream tmpIn = null;
			OutputStream tmpOut = null;

			// Get the BluetoothSocket input and output streams
			try {
				tmpIn = socket.getInputStream();
				tmpOut = socket.getOutputStream();
				setState(Constants.STATE_CONNECTED);
				showNotification();
			} catch (IOException e) {
				Log.e(TAG, "temp sockets not created", e);
			}

			mmInStream = tmpIn;
			mmOutStream = tmpOut;

			Log.i(TAG + "tmpIn", tmpIn.toString());
			Log.i(TAG + "tmpOut", tmpOut.toString());
		}

		public void run() {
			if (D)
				Log.d(TAG, "BEGIN mConnectedThread");
			byte[] buffer = new byte[1024];
			setState(Constants.STATE_CONNECTED);
			int bytes;

			// Keep listening to the InputStream while connected
			while (true) {
				try {
					// Read from the InputStream
					bytes = mmInStream.read(buffer);

					// // Send the obtained bytes to the UI Activity
					Message msg = Message.obtain(null, Constants.BT_READ,
							bytes, -1, buffer);
					sendMessageToUI(msg);

					/*
					 * public static String readInputStreamAsString(InputStream
					 * in) throws IOException {
					 * 
					 * BufferedInputStream bis = new BufferedInputStream(in);
					 * ByteArrayOutputStream buf = new ByteArrayOutputStream();
					 * int result = bis.read(); while(result != -1) { byte b =
					 * (byte)result; buf.write(b); result = bis.read(); } return
					 * buf.toString(); }
					 */

				} catch (IOException e) {
					Log.e(TAG, "disconnected", e);
					connectionLost();
					break;
				}
			}
		}

		/**
		 * Write to the connected OutStream.
		 * 
		 * @param buffer
		 *            The bytes to write
		 */
		public void write(byte[] buffer) {
			try {
				setState(Constants.STATE_SEND);
				mmOutStream.write(buffer);
				mmOutStream.flush();
				// for click sound
				Message msg = Message.obtain(null, Constants.BT_MSG_SENDED);
				sendMessageToUI(msg);
				setState(Constants.STATE_CONNECTED);
			} catch (IOException e) {
				Log.e(TAG, "Exception during write", e);
				connectionLost();
			}

		}

		public void cancel() {
			try {
				mmSocket.close();
			} catch (IOException e) {
				Log.e(TAG, "close() of connect socket failed", e);
			}
		}
	}

}
