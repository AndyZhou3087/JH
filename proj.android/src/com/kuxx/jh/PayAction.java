package com.kuxx.jh;

import android.app.Activity;
import android.os.Message;


public class PayAction {
	static Activity context;

	static int buyprice[] = { 0, 6, 12, 30, 6, 12, 30, 68,128, 6, 30, 68, 18};
	static String goodsDesc[] = { "", "三少爷角色", "江小白角色", "阿青角色", "60元宝", "140元宝", "400元宝", "1080元宝", "2388元宝", "荣耀月卡", "贵族月卡", "尊贵月卡", "限时礼包"};
	public static void pay(String payCode, int payindex)
	{ 
		 if (!Utils.getNetworkAvailable())
		 {
	        Message msg = AppActivity.handler.obtainMessage();
	        msg.what = -1;
	        msg.sendToTarget();
			return;
		 }
		 WXPay.pay(payCode, buyprice[payindex] * 100, goodsDesc[payindex]);
		 //WXPay.pay(payCode, 1, goodsDesc[payindex]);
	}

	public static void init(Activity act)
	{
		context = act;
		WXPay.init(act);
	}
}

















