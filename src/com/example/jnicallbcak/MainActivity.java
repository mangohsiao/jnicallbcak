package com.example.jnicallbcak;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener{

	TextView main_txv_str = null;
	Button btn_stop = null;
	Button btn_send = null;
	Button btn_recon = null;
	
	WorkingJni work = null;
	
	
	Mango mango = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		main_txv_str = (TextView)findViewById(R.id.main_txv_str);
		btn_stop = (Button)findViewById(R.id.btn_stop);
		btn_send = (Button)findViewById(R.id.btn_send);
		btn_recon = (Button)findViewById(R.id.btn_recon);

		btn_stop.setOnClickListener(this);
		btn_send.setOnClickListener(this);
		btn_recon.setOnClickListener(this);
		
		mango = new Mango(this, mHandler);
		mango.mangoInit();
		
//		work = new WorkingJni(mHandler);
//		work.startWorking();
		
		Intent intent = new Intent(MainActivity.this,HeartBreakReceiver.class);
		intent.setAction("com.mango.action.HEART_BREAK");
		PendingIntent sender = PendingIntent.getBroadcast(MainActivity.this, 0, intent, 0);
		AlarmManager am = (AlarmManager)getSystemService(ALARM_SERVICE);
		am.setRepeating(AlarmManager.ELAPSED_REALTIME_WAKEUP, 15*1000, 15*1000, sender);
		Log.v("MainActivity", "setting ok.");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	private Handler mHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			Bundle bd = msg.getData();
//			String str = bd.getString("CMD");
			main_txv_str.setText(Integer.toString(bd.getInt("CMD")));
		}
		
	};

	@Override
	protected void onPause() {
		super.onPause();
//		work.stop();
		if(mango != null){
//			mango.mangoStop();
		}
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if(null != mango){
//			mango.mangoContinue();
		}
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		cancelBroadcast();
		System.out.println("on destroyed.");
	}

	private void cancelBroadcast() {
		Intent intent = new Intent(MainActivity.this,HeartBreakReceiver.class);
		intent.setAction("com.mango.action.HEART_BREAK");
		PendingIntent sender = PendingIntent.getBroadcast(MainActivity.this, 0, intent, 0);
		AlarmManager am = (AlarmManager)getSystemService(ALARM_SERVICE);
		am.cancel(sender);
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		System.out.println("on stop.");
		if(mango != null){
//			mango.mangoStop();
		}
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.btn_stop:
			if(mango!=null){
				mango.mangoStop();
			}
			break;
		case R.id.btn_send:
			if(mango!=null){
				mango.mangoSend();
			}
			break;
		case R.id.btn_recon:
			if(mango!=null){
				mango.mangoRecon();
			}
			break;

		default:
			break;
		}
	}
	
	
}
