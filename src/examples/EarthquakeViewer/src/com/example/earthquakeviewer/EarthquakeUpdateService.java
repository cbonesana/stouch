package com.example.earthquakeviewer;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Timer;
import java.util.TimerTask;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import android.app.Service;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.location.Location;
import android.os.IBinder;
import android.preference.PreferenceManager;
import android.util.Log;

public class EarthquakeUpdateService extends Service {

	public static String TAG = "EARTHQUAKE_UPDATE_SERVICE";

	private Timer updateTimer;

	private TimerTask doRefresh = new TimerTask() {
		public void run() {
			refreshEarthquakes();
		}
	};
	
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		// Retrieve the shared preferences
		Context context = getApplicationContext();
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
		
		int updateFreq = Integer.parseInt(prefs.getString(PreferencesActivity.PREF_UPDATE_FREQ, "60"));
		boolean autoUpdateChecked = prefs.getBoolean(PreferencesActivity.PREF_AUTO_UPDATE, false);
		
		updateTimer.cancel();
		if (autoUpdateChecked) {
			updateTimer = new Timer("earthquakeUpdates");
			updateTimer.scheduleAtFixedRate(doRefresh, 0, updateFreq*60*1000);
		} else {
			Thread thread = new Thread(new Runnable() {
				@Override
				public void run() {
					refreshEarthquakes();
				}
			});
			thread.start();
		}
		
		return Service.START_STICKY;
	}

	
	
	@Override
	public void onCreate() {
		super.onCreate();
		updateTimer = new Timer("earthquakUpdates");
	}
	
	private void refreshEarthquakes() {
		// Get the XML
		URL url;
		try {
			String quakeFeed = getString(R.string.quake_feed);
			url = new URL(quakeFeed);

			URLConnection connection;
			connection = url.openConnection();

			HttpURLConnection httpConnection = (HttpURLConnection)connection;
			int responseCode = httpConnection.getResponseCode();

			if (responseCode == HttpURLConnection.HTTP_OK) {
				InputStream in = httpConnection.getInputStream();

				DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
				DocumentBuilder db = dbf.newDocumentBuilder();

				// Parse the earthquake feed.
				Document dom = db.parse(in);
				Element docEle = dom.getDocumentElement();

				// Get a list of each earthquake entry.
				NodeList nl = docEle.getElementsByTagName("entry");
				if (nl != null && nl.getLength() > 0) {
					for (int i = 0 ; i < nl.getLength(); i++) {
						Element entry = (Element)nl.item(i);
						Element title = (Element)entry.getElementsByTagName("title").item(0);
						Element g = (Element)entry.getElementsByTagName("georss:point").item(0);
						Element when = (Element)entry.getElementsByTagName("updated").item(0);
						Element link = (Element)entry.getElementsByTagName("link").item(0);

						String details = title.getFirstChild().getNodeValue();
						String hostname = "http://earthquake.usgs.gov";
						String linkString = hostname + link.getAttribute("href");

						String point = g.getFirstChild().getNodeValue();
						String dt = when.getFirstChild().getNodeValue(); 
						SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd'T'hh:mm:ss'Z'");
						Date qdate = new GregorianCalendar(0,0,0).getTime();
						try {
							qdate = sdf.parse(dt);
						} catch (java.text.ParseException e) {
							Log.d(TAG, "Date parsing exception.", e);
						}

						String[] location = point.split(" ");
						Location l = new Location("dummyGPS");
						l.setLatitude(Double.parseDouble(location[0]));
						l.setLongitude(Double.parseDouble(location[1]));

						String magnitudeString = details.split(" ")[1];
						int end =  magnitudeString.length()-1;
						double magnitude = Double.parseDouble(magnitudeString.substring(0, end));

						details = details.split(",")[1].trim();

						final Quake quake = new Quake(qdate, details, l, magnitude, linkString);

						// Process a newly found earthquake
						addNewQuake(quake);
					}
				}
			}
		} catch (MalformedURLException e) {
			Log.d(TAG, "MalformedURLException");
		} catch (IOException e) {
			Log.d(TAG, "IOException");
		} catch (ParserConfigurationException e) {
			Log.d(TAG, "Parser Configuration Exception");
		} catch (SAXException e) {
			Log.d(TAG, "SAX Exception");
		}
		finally {
		}
	}

	private void addNewQuake(Quake quake) {
		ContentResolver cr = getContentResolver();

		//  Construct a where clause to make sure we don’t already have this earthquake in the provider.
		String w = EarthquakeProvider.KEY_DATE + "=" + quake.getDate().getTime();

		// If the earthquake is new, insert it into the provider.
		Cursor query = cr.query(EarthquakeProvider.CONTENT_URI, null, w, null, null);
		if (query.getCount() == 0){
			ContentValues values = new ContentValues();

			values.put(EarthquakeProvider.KEY_DATE, quake.getDate().getTime());
			values.put(EarthquakeProvider.KEY_DETAILS, quake.getDetails());
			values.put(EarthquakeProvider.KEY_SUMMARY, quake.toString());

			double lat = quake.getLocation().getLatitude();
			double lng = quake.getLocation().getLongitude();
			values.put(EarthquakeProvider.KEY_LOCATION_LAT, lat);
			values.put(EarthquakeProvider.KEY_LOCATION_LNG, lng);
			values.put(EarthquakeProvider.KEY_LINK, quake.getLink());
			values.put(EarthquakeProvider.KEY_MAGNITUDE, quake.getMagnitude());

			cr.insert(EarthquakeProvider.CONTENT_URI, values);
		}
		query.close();
	}
}
