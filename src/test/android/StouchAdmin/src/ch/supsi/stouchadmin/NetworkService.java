package ch.supsi.stouchadmin;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Scanner;
import java.util.regex.Pattern;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Rect;
import android.net.Uri;
import android.util.Log;
import android.widget.Toast;

public class NetworkService extends IntentService {
	private static final String TAG = NetworkService.class.getSimpleName();

	public static final int CODE_UPLOAD_CROP = 1;
	public static final int CODE_REGISTER_USER = 2;

	private static String identifier = new String();

	public NetworkService() {
		super(TAG);
	}

	@Override
	protected void onHandleIntent(Intent intent) {
		final int opcode = intent.getIntExtra("opcode", 0);
		
		if (identifier.isEmpty()) {
			identifier = getId(getApplicationContext());
			if (identifier.isEmpty()) {
				Toast.makeText(getApplicationContext(), "Login fallito!", Toast.LENGTH_LONG).show();
				return;
			}
		}
		
		switch (opcode) {
		case CODE_UPLOAD_CROP:
		{	
			Rect r = intent.getParcelableExtra("crop");
			putCrop(r);
		}
		break;
		case CODE_REGISTER_USER:
		{
			String email = intent.getStringExtra("email");
			if (email == null) {
				Toast.makeText(getApplicationContext(), "Email non fornita!", Toast.LENGTH_LONG).show();
				return;
			}
			
			int lang = intent.getIntExtra("lang", 0);
			
			if (registerUser(email, lang) == null) {
				Toast.makeText(getApplicationContext(), "Registrazione fallita!", Toast.LENGTH_LONG).show();
			}
			
		}
		break;
		default:
			Log.e(TAG, "Unknown opcode: " + opcode);
		}

	}
	
	private String registerUser(String email, int lang) {
		JSONObject replyItems = null;
		String reply = null;
		
		if (email==null || lang <= 0) {
			return null;
		}
		
		try {
			HashMap<String, String> pairs = new HashMap<String, String>();
			pairs.put("idUser", identifier);
			pairs.put("comando", "REG_RFID");
			pairs.put("mac", email);
			pairs.put("param", String.valueOf(lang));
			pairs.put("risposta", "");
			
			replyItems = getJsonObject(pairs);
			
			String errore = replyItems.getString("errore");
			if (errore.equals("ERR")) {
				return null;
			}
			reply = replyItems.getString("risposta");
			
		} catch (JSONException e) {
			Log.e(TAG, "JSONException in putCrop()", e);
		}
		
		return reply;
	}
	
	private void putCrop(Rect r) {
		if (r == null) {
			return;
		}
		try {
			HashMap<String, String> pairs = new HashMap<String, String>();
			pairs.put("idUser", identifier);
			pairs.put("comando", "SET_CROP");
			pairs.put("param", "STOUCH=" + r.left + "," + r.top + ";" + r.right + "," + r.bottom);
			
			getJsonObject(pairs);
		} catch (JSONException e) {
			Log.e(TAG, "JSONException in putCrop()", e);
		}
	}

	public static String getId(Context c) {
		JSONObject replyItems = null;
		String id = new String();
		String mac = getMac(c);

		if (mac.isEmpty()) {
			return id;
		}

		//http://www.tv-surf.com/3D-Enter/servlet/P13_19_CommandXML?idUser=00012e230458&comando=CHECK_USER_DET&mac=00012e230458&param=&risposta=risposta 

		try {
			HashMap<String, String> pairs = new HashMap<String, String>();
			pairs.put("idUser", mac);
			pairs.put("comando", "CHECK_USER_DET");
			pairs.put("mac", mac);
			pairs.put("param", "");
			pairs.put("risposta", "");

			replyItems = getJsonObject(pairs);
			if (replyItems == null) {
				return id;
			}
			String errore = replyItems.getString("errore");
			if (errore.equals("DTT")) {
				id = replyItems.getString("risposta");
			}
		}
		catch(JSONException e) {
			Log.e(TAG, "JSONException in getId()", e);
		}
		Log.d(TAG, "id: " + id);
		return id;
	}

	private static String getMac(Context c) {
		JSONObject replyItems = null;
		String mac = new String();
		SharedPreferences pref = c.getSharedPreferences(MainActivity.PREF_NAME, 0);

		if (!pref.contains("username")) {
			Toast.makeText(c, "Username not set!", Toast.LENGTH_LONG).show();
			return mac;
		}

		try {
			HashMap<String, String> pairs = new HashMap<String, String>();
			pairs.put("idUser", "-1");
			pairs.put("comando", "CHECK_USER_JS");
			pairs.put("mac", "");

			String user = pref.getString("username", "");
			String pass = pref.getString("password", "");
			pairs.put("param", user + "|" + pass);
			pairs.put("risposta", "");

			replyItems = getJsonObject(pairs);
			if (replyItems == null) {
				return mac;
			}
			String errore = replyItems.getString("errore");
			if (errore.equals("OK")) {
				String[] tmp = replyItems.getString("risposta").split("mac=");
				mac = tmp[tmp.length - 1];
			}
		}
		catch(JSONException e) {
			Log.e(TAG, "JSONException in getMac()", e);
		}
		Log.d(TAG, "mac: " + mac);
		return mac;
	}

	private static JSONObject getJsonObject(HashMap<String, String> params) throws JSONException {
		JSONObject replyItems = null;
		InputStream is = null;
		HttpURLConnection connection = null;
		final String serverUrl = "http://www.tv-surf.com/";

		try {
			Uri.Builder b = Uri.parse(serverUrl).buildUpon();
			b.path("/3D-Enter/servlet/P13_19_CommandXML");
			for (String p:params.keySet()) {
				b.appendQueryParameter(p, params.get(p));
			}
			String url = b.build().toString();

			URL u = new URL(url);

			connection = (HttpURLConnection) u.openConnection();

			if (connection.getResponseCode() != 200) {
				Log.e(TAG, "Invalid response from server: " + connection.getResponseMessage() + "\nUrl: "+ url);
				return null;
			}

			is = connection.getInputStream();
			String input = convertStreamToString(is);
			Log.d(TAG, "Input: " + input);
			if (input.isEmpty()) {
				return replyItems;
			}
			replyItems = new JSONObject(input.substring(1, input.length() - 2));
		} catch (IOException e) {
			Log.e(TAG, "IOException in getJsonObject()", e);
		} catch (Exception e) {
			Log.e(TAG, "Exception in getJsonObject()", e);
		}
		finally {
			try {
				if(is != null)
					is.close();
				if(connection != null)
					connection.disconnect();
			} catch (IOException ioex) {
				// ignore
			}
		}
		return replyItems;
	}

	private static final Pattern p = Pattern.compile("\\A"); //Regex for next begin of file (= read everything)
	private static String convertStreamToString(InputStream is) {
		Scanner s = new Scanner(is).useDelimiter(p);
		return s.hasNext() ? s.next() : "";
	}

}
