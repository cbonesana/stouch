package ch.supsi.dti.bluetoothserver;

import java.util.ArrayList;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

public class MainActivity extends Activity {
	
	private static final int ENABLE_BLUETOOTH = 1;
	private static final int SCANNING = 2;
	private static final int NOSCAN = 3;
	
	BluetoothAdapter bluetoothAdapter;
	
	ArrayAdapter<String> arrayAdapter;
	
	int currentState;
	int previousState;
	int currentScanMode;
	int previousScanMode;
	int scanStatus = NOSCAN;
	
	String tt = "";
	
	TextView textEnabled;
	TextView textScanMode;
	TextView textCurrentState;
	TextView textPreviousState;
	
	ListView listView;
	
	Button buttonScan;
	
	private ArrayList<BluetoothDevice> deviceList = new ArrayList<BluetoothDevice>();
	private ArrayList<String> deviceNameList = new ArrayList<String>();
	
	BroadcastReceiver bluetoothState = new BroadcastReceiver() {
		
		@Override
		public void onReceive(Context context, Intent intent) {
			String prevStateExtra = BluetoothAdapter.EXTRA_PREVIOUS_STATE;
			String stateExtra = BluetoothAdapter.EXTRA_STATE;
			currentState = intent.getIntExtra(stateExtra, -1);
			previousState = intent.getIntExtra(prevStateExtra, -1);
			textPreviousState.setText("Previous state: " + tt);
			switch (currentState) {
			case BluetoothAdapter.STATE_TURNING_ON:
				tt = "Bluetooth turning on";
				break;
			case BluetoothAdapter.STATE_ON:
				tt = "Bluetooth on";
				break;
			case BluetoothAdapter.STATE_TURNING_OFF:
				tt = "Bluetooth turning off";
				break;
			case BluetoothAdapter.STATE_OFF:
				tt = "Bluetooth off";
				break;
			default:
				break;
			}
			if (textEnabled != null){
				textCurrentState.setText("Current state: " + tt);
			}
			Log.d("BLUETOOTH", tt);
		}
	};

	BroadcastReceiver bluetoothScanMode = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			currentScanMode = intent.getIntExtra(BluetoothAdapter.EXTRA_SCAN_MODE, -1);
			previousScanMode = intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_SCAN_MODE, -1);
			
			String txt = "Scan Mode: " + currentScanMode + ". Previous: "+ previousScanMode;
			
			if (textScanMode != null){
				textScanMode.setText(txt);
			}
			
			Log.d("BLUETOOTH", txt);
		}
	};
	
	BroadcastReceiver bluetoothDiscoveryMonitor = new BroadcastReceiver() {
		String discoveryStarted = BluetoothAdapter.ACTION_DISCOVERY_STARTED;
		String discoveryFinished = BluetoothAdapter.ACTION_DISCOVERY_FINISHED;
		
		@Override
		public void onReceive(Context context, Intent intent) {
			if (discoveryStarted.equals(intent.getAction())){
				Log.d("BLUETOOTH", "Discovery started...");
			} else if (discoveryFinished.equals(intent.getAction())){
				stopDiscovery();
				Log.d("BLUETOOTH", "Discovery completed.");
			}
		}
	};
	
	BroadcastReceiver discoveryResult = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String remoteDeviceName = intent.getStringExtra(BluetoothDevice.EXTRA_NAME);
			BluetoothDevice remoteDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
			deviceList.add(remoteDevice);
			
			Log.d("BLUETOOTH", "Discovered " + remoteDeviceName);
		}
	};
	
	OnClickListener discoveryScanStart = new OnClickListener() {
		@Override
		public void onClick(View v) {
			if (bluetoothAdapter.isEnabled()){
				if (scanStatus == NOSCAN){
					startDiscovery();
				} else if (scanStatus == SCANNING){
					stopDiscovery();
				}
			}
		}
	};
	
	private void startDiscovery(){
		scanStatus = SCANNING;
		buttonScan.setText(getString(R.string.stop_scan));
		
		registerReceiver(discoveryResult, new IntentFilter(BluetoothDevice.ACTION_FOUND));
		if (!bluetoothAdapter.isDiscovering()){
			deviceList.clear();
		}
		bluetoothAdapter.startDiscovery();
	}
	
	private void stopDiscovery(){
		scanStatus = NOSCAN;
		buttonScan.setText(getString(R.string.perform_scan));
		bluetoothAdapter.cancelDiscovery();
		
		unregisterReceiver(discoveryResult);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		textEnabled = (TextView)findViewById(R.id.textViewEnabled);
		textScanMode = (TextView)findViewById(R.id.textViewScanMode);
		textCurrentState = (TextView)findViewById(R.id.textViewCurrentState);
		textPreviousState = (TextView)findViewById(R.id.textViewPreviousState);
		
		buttonScan = (Button)findViewById(R.id.buttonScan);
		buttonScan.setOnClickListener(discoveryScanStart);
		
		listView = (ListView)findViewById(R.id.discoveryResults);
		
		arrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1,deviceNameList);
		
		listView.setAdapter(arrayAdapter);
		
		registerReceiver(bluetoothState, new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED));
		registerReceiver(bluetoothScanMode, new IntentFilter(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED));
		registerReceiver(bluetoothDiscoveryMonitor, new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_STARTED));
		registerReceiver(bluetoothDiscoveryMonitor, new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED));
		
		initBluetooth();
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		if (bluetoothState != null){
			unregisterReceiver(bluetoothState);
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	private void initBluetooth() {
		if (!bluetoothAdapter.isEnabled()){
			Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(intent, ENABLE_BLUETOOTH);
		} else {
			initBluetoothUI();
		}
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == ENABLE_BLUETOOTH){
			if (resultCode == RESULT_OK){
				initBluetoothUI();
			}
		}
	}

	private void initBluetoothUI() {
		textEnabled.setText("Bluetooth enabled.");
	}

}
