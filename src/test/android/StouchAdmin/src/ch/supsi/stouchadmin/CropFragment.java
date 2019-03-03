package ch.supsi.stouchadmin;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;
import java.net.URLConnection;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;

import com.actionbarsherlock.app.SherlockFragment;

public class CropFragment extends SherlockFragment {
	private final static String TAG = "CropFragment";
	private String bmpPath = null;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.cropimage_fragment, container, false);
		Button b = ((Button) v.findViewById(R.id.crop_button));
		b.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				Intent i = new Intent(getActivity().getBaseContext(), CropImage.class);
				i.putExtra("data", bmpPath);
				startActivityForResult(i, 0);
			}
		});
		
		new ImageDownloader().execute();
		return v;
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (resultCode == Activity.RESULT_OK) {
			Rect r = data.getParcelableExtra("crop");
			Log.d(TAG, "Crop: " + r.left + "," + r.top + ";" + r.right + "," + r.bottom);
			Intent i = new Intent(getActivity().getBaseContext(), NetworkService.class);
			i.putExtra("opcode", NetworkService.CODE_UPLOAD_CROP);
			r.set(r.left*2,r.top*2,r.right*2,r.bottom*2);
			i.putExtra("crop", r);
			getActivity().startService(i);
		}
	}
	
	private class ImageDownloader extends AsyncTask<Void, Void, Bitmap> {
		@Override
		protected Bitmap doInBackground(Void... params) {
			String id = NetworkService.getId(getActivity().getBaseContext());
			Log.d(TAG, "ID: " + id);
			return downloadImage(id);
		}
		
		@Override
		protected void onPostExecute(Bitmap result) {
			Bitmap bmp;
			if (result != null) {
				ImageView iv = (ImageView) getView().findViewById(R.id.currentImage);
				bmp = Bitmap.createScaledBitmap(result, result.getWidth()/2, result.getHeight()/2, false);
				File cacheDir = getActivity().getApplicationContext().getExternalCacheDir();
				if (!cacheDir.isDirectory()) {
					cacheDir.mkdirs();
				}
				try {
					File tmpFile = File.createTempFile("crop", null, cacheDir);
					FileOutputStream fos = new FileOutputStream(tmpFile);
					bmp.compress(Bitmap.CompressFormat.JPEG, 90, fos);
					fos.close();
					bmpPath = tmpFile.getAbsolutePath();
				} catch (IOException e) {
					Log.e(TAG, "Unable to create temporary image file", e);
				}
				iv.setImageBitmap(bmp);
			}
		}
		
		private Bitmap downloadImage(String id) {
		    Bitmap bmp = null;
		    try {
		        URL url = new URL("http://www.tv-surf.com/img/stouch/camera_" + id + ".jpg");
		        URLConnection con = url.openConnection();
		        con.connect();
		        BufferedInputStream bis = new BufferedInputStream(con.getInputStream());
		        bmp = BitmapFactory.decodeStream(bis);
		        bis.close();
		    } catch (Exception e) {
		        Log.e(TAG, "Error getting the image from server : ", e);
		    } 
		    return bmp;
		}
		
	}
}
