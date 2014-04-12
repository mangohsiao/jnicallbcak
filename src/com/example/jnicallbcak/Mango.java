package com.example.jnicallbcak;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class Mango {

	Handler mHandler;
	Context ctx;
	
	public Mango(Context ctx, Handler Handler) {
		super();
		this.ctx = ctx;
		this.mHandler = Handler;
		ins = this;
	}
	
	private static Mango ins = null;
	
	public static Mango getIns(){
		return ins;
	}

	public native void mangoInit();
	public native void mangoStop();
	public native void mangoContinue();
	public native void mangoSend();
	public native void mangoRecon();
	public native boolean mangoCheck();
	
	public void mangoCallBack(int num){
		Bundle bd = new Bundle();
		bd.putInt("CMD", num);
		Message msg = new Message();
		msg.setData(bd);
		mHandler.sendMessage(msg);
	}
	
	static {
//		System.loadLibrary("jnicallbcak");
		System.loadLibrary("jniNet");
	}
}
