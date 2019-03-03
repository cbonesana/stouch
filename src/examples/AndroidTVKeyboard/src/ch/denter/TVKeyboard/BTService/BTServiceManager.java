package ch.denter.TVKeyboard.BTService;


import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;

import ch.denter.TVKeyboard.Constants;
import ch.denter.TVKeyboard.R;

public class BTServiceManager {
	private Context mContext;
	// Debugging
	private static final String TAG = "BTServiceManager";
	private static final boolean D = true;

	public BTServiceManager(Context context) {
		this.mContext = context;
	}

	private Messenger mService = null;
	private boolean isBound = false;
	private int mState = -2;

	private boolean isTouchPadMessage = false;

	// sounds
	private SoundPool mSoundPool;
	int mSoundID = -1;

	void getSound() {

		mSoundPool = new SoundPool(20, AudioManager.STREAM_MUSIC, 0);
		try {
			AssetManager assetManager = mContext.getAssets();
			AssetFileDescriptor descriptor = assetManager
					.openFd("sound_bt_send.ogg");
			mSoundID = mSoundPool.load(descriptor, 1);
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	private final Messenger mMessenger = new Messenger(new IncomingHandler());

	public class IncomingHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case Constants.BT_ERROR_IN_WRITE_MSG:

				break;

			case Constants.BT_ERROR_CONNECTION_LOST:
				Toast.makeText(
						mContext,
						mContext.getResources().getString(
								R.string.bt_connection_lost),
						Toast.LENGTH_SHORT).show();
				break;

			case Constants.BT_ERROR_CONNECTION_FAIL:
				Toast.makeText(
						mContext,
						mContext.getResources().getString(
								R.string.bt_connection_lost),
						Toast.LENGTH_SHORT).show();
				break;

			case Constants.BT_STATE_CHANGE:

				mState = msg.getData().getInt(Constants.BT_TAG);

				break;

			case Constants.BT_READ:
				break;

			case Constants.BT_WRITE:
				break;

			case Constants.BT_TOAST:
				Toast.makeText(mContext,
						msg.getData().getString(Constants.BT_TAG),
						Toast.LENGTH_SHORT).show();
				break;

			case Constants.BT_MSG_SENDED:
				if (!isTouchPadMessage)
					if (mSoundID != -1)
						mSoundPool.play(mSoundID, 1.0f, 1.0f, 0, 0, 1);
				break;

			default:
				break;
			}
		}
	}

	public static String getDescriptionOfBTState(int state, Context context) {
		String result = "";
		switch (state) {
		case Constants.STATE_CONNECTED:
			result = context.getResources().getString(R.string.state_connected);
			break;
		case Constants.STATE_CONNECTING:
			result = context.getResources().getString(R.string.state_conecting);
			break;
		case Constants.STATE_SEND:
			result = context.getResources().getString(R.string.state_send);
			break;
		case Constants.STATE_NONE:
			result = context.getResources().getString(R.string.state_none);
			break;
		case Constants.STATE_SEARCHING:
			result = context.getResources().getString(R.string.state_search);
			break;

		default:
			break;
		}
		return result;
	}

	public Messenger getService() {
		return mService;
	}

	public Messenger getMessenger() {
		return mMessenger;
	}

	public boolean isBound() {
		return isBound;
	}

	public int getState() {
		return mState;
	}

	public void setTouchPadAcces(boolean isTouchMassege) {
		this.isTouchPadMessage = isTouchMassege;
	}

	// ----------------------------------------

	/**
	 * Sends a message.
	 * 
	 * @param message
	 *            A string of text to send.
	 */
	public void sendMessage(byte[] message) {

		if (isBound()) {
			if (mService != null) {
				if (mState == Constants.STATE_CONNECTED) {
					if (message.length > 0) {
						try {

							Message msg = Message.obtain(null,
									Constants.BT_MSG_FOR_SEND, message);
							msg.replyTo = mMessenger;
							mService.send(msg);
							// need t send release event
							if (message[0] == Constants.TYPE_K) {
								msg = Message.obtain(
										null,
										Constants.BT_MSG_FOR_SEND,
										messageCreator(Constants.TYPE_CHECKSUM,
												Constants.RELEASE_EVENT));
								msg.replyTo = mMessenger;
								mService.send(msg);
							}

						} catch (RemoteException e) {
							e.printStackTrace();
						}
					}
				}

			} else {
				Toast.makeText(mContext, R.string.bt_not_available,
						Toast.LENGTH_SHORT);
				return;
			}
		}
	}

	/**
	 * Create message to send.
	 * 
	 * @param command
	 *            A symbol of the command.
	 * @param data
	 *            A char array for send;.
	 * @return message A byte array for send.
	 */
	public byte[] messageCreator(char command, byte[] data) {
		byte[] message = new byte[6];
		// Zero out
		for (int i = 0; i < message.length; i++)
			message[i] = 0;

		switch (command) {
		case Constants.TYPE_PC_BOARD_MOUSE:
			message[0] = (byte) command;

			message[2] = data[0];
			message[3] = data[1];

			break;
		case Constants.TYPE_K:
			message[0] = (byte) command;

			int i = 0;
			int j = 1;
			while (i < data.length) {
				message[j] = data[i];
				i++;
				j++;
			}
			break;
		case Constants.TYPE_CHECKSUM:
			for (i = 0; i < data.length; i++)
				message[i] = data[i];
			break;

		default:
			break;
		}

		message[5] = (byte) checkSum(message);

		return message;
	}

	/**
	 * Formation of post checksum.
	 * 
	 * @param data
	 *            The array of bytes by which you want to count checksum.
	 * @return check sum
	 */
	public byte checkSum(byte[] data) {
		byte sum = 0;
		for (byte i = 0; i < data.length - 1; i++)
			sum += data[i];
		return sum;
	}

	public byte[] fromCharToByte(char[] data) {
		byte[] res = new byte[data.length];
		for (int i = 0; i < data.length; i++) {
			res[i] = (byte) data[i];
		}
		return res;
	}

	public char[] fromByteToChar(byte[] data) {
		char[] res = new char[data.length];
		for (int i = 0; i < data.length; i++) {
			res[i] = (char) data[i];
		}
		return res;
	}

	public void sendSkypeMessage(String data) {

		// at first sending F10 command
		sendMessage(messageCreator(Constants.TYPE_CHECKSUM, Constants.F10));

		char[] charData = data.toCharArray();
		int index;// for position
		byte[] mes;// Massage 2 bytes modifier + value

		for (int i = 0; i < data.length(); i++) {
			index = -1;// out of indexes
			for (int j = 0; j < Constants.CAPTIN_SYMBOLS.length; j++) {
				if (charData[i] == Constants.CAPTIN_SYMBOLS[j]) {
					index = j;
					break;
				}
			}

			mes = new byte[2];

			if (index != -1) { // in existing variables
				if ((index >= 0) && (index < Constants.ALPHABET_SIZE)) {
					// low cast symbol
					mes[0] = 0x00;
					mes[1] = Constants.ALPHABET_CODS[index];

				} else if ((index >= Constants.ALPHABET_SIZE)
						&& (index < Constants.ALPHABET_SIZE * 2)) {
					// high cast symbols
					mes[0] = Constants.SHIFT;
					mes[1] = Constants.ALPHABET_CODS[index-Constants.ALPHABET_SIZE];

				} else if ((index >= Constants.ALPHABET_SIZE * 2)
						&& (index < Constants.ALPHABET_SIZE * 3)) {
					// numbers
					mes[0] = Constants.SYMBOL_CODES[index-Constants.ALPHABET_SIZE*2][0];
					mes[1] = Constants.SYMBOL_CODES[index-Constants.ALPHABET_SIZE*2][1];
				}
				sendMessage(messageCreator(Constants.TYPE_K, mes)); // send
			} else {// not in existing variables

				if (charData[i] == ' ') {
					sendMessage(messageCreator(Constants.TYPE_CHECKSUM,
							Constants.SPACE));
				}
			}
		}

		sendMessage(messageCreator(Constants.TYPE_CHECKSUM,
				Constants.NAVIGATE_OK_TV)); // send enter massage. End
	}

	// -----------------------------------
	private ServiceConnection mConnection = new ServiceConnection() {
		public void onServiceConnected(ComponentName className, IBinder service) {
			mService = new Messenger(service);
			if (D)
				Log.d(TAG, "onServiceConnected()");
			try {
				Message msg = Message
						.obtain(null, Constants.BT_REGISTER_CLIENT);
				msg.replyTo = mMessenger;
				mService.send(msg);

			} catch (RemoteException e) {
				// In this case the service has crashed before we could even do
				// anything with it
				e.printStackTrace();
			}
		}

		public void onServiceDisconnected(ComponentName className) {
			// This is called when the connection with the service has been
			// unexpectedly disconnected - process crashed.
			if (D)
				Log.d(TAG, "onServiceDisconnected()");
			mService = null;
			Toast.makeText(
					mContext,
					mContext.getResources().getString(
							R.string.disconnected_with_service),
					Toast.LENGTH_SHORT).show();
			doBindService();
		}
	};

	// Connect to service
	public void doBindService() {
		if (D)
			Log.d(TAG, "doBindService()");

		isBound = mContext.bindService(new Intent(mContext,
				BluetoothService.class), mConnection, Context.BIND_AUTO_CREATE);
		if (!isBound) {
			new Timer().schedule(new TimerTask() {

				@Override
				public void run() {
					if (D)
						Log.e(TAG, "doBindService()");

					doBindService();

				}
			}, Constants.BIND_TIMEOUT);

		}
		// still not load
		if (mSoundID == -1)
			getSound();

	}

	// disconnect from service
	public void doUnbindService() {
		if (D)
			Log.d(TAG, "doUnbindService()");
		if (isBound) {
			// If we have received the service, and hence registered with it,
			// then now is the time to unregister.
			if (mService != null) {
				try {
					Message msg = Message.obtain(null,
							Constants.BT_UNREGISTER_CLIENT);
					msg.replyTo = mMessenger;
					mService.send(msg);
				} catch (RemoteException e) {
					// There is nothing special we need to do if the service has
					// crashed.
					e.printStackTrace();
				}
			}
			// Detach our existing connection.
			mContext.unbindService(mConnection);
			isBound = false;

			mSoundPool.unload(mSoundID);
		}
	}

}
