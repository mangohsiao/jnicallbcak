/**
 * 
 */
package com.example.jnicallbcak;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

/**
 * @author xiao
 *
 */
public class HeartBreakReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		Log.v("Receiver", "onReceive()");
		Mango mango = Mango.getIns();
		if(mango != null){
			Log.v("Receiver","Mango.getIns() != null");
			if(mango.mangoCheck())
				mango.mangoSend();
		}
	}

}
