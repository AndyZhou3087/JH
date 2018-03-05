package com.kuxx.jh;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.Window;



public class LauncherActivity extends Activity {
	static Context mContext;
	int count = 0;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);      
		setContentView(R.layout.launcheractivity);
		
		final Timer timer = new Timer();
		TimerTask timerTask = new TimerTask() {
		 @Override
		 public void run() {

			 count++;
			 if (count == 2)
			 {
				 timer.cancel();
				 Intent intent = new  Intent();
				 intent.setClass(LauncherActivity.this, AppActivity.class);
				 LauncherActivity.this.startActivity(intent);
				 LauncherActivity.this.finish();
			 }

		 }
		};
		timer.schedule(timerTask, 100, 1000);	
	}
}
