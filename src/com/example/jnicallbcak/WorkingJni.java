package com.example.jnicallbcak;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class WorkingJni {

	Handler mHandler;
	Thread thread = null;

	public WorkingJni(Handler mHandler) {
		super();
		this.mHandler = mHandler;
	}
	
	public void callBackFunc(String msg) {
		System.out.println("callBackFunc. msg: " + msg);
		Message tMsg = new Message();
		Bundle tBundle = new Bundle();
		tBundle.putString("CMD", msg);
		tMsg.setData(tBundle);
		
		mHandler.sendMessage(tMsg);
	}

	static{
		System.loadLibrary("WorkingJni");
	}
	public native void myJni();
	
	public void startWorking(){
		if(thread == null){
			thread = new Thread(new WorkingThread());
		}
		thread.start();
	}
	
	public class WorkingThread implements Runnable{

		@Override
		public void run() {
			myJni();
		}
		
	}

	public void stop() {
		thread.interrupt();
	}
}
