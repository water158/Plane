/****************************************************************************
Copyright (c) 2010-2011 ferrisgame.com
 ****************************************************************************/

package com.ferrisgame.ambition;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.easyndk.classes.AndroidNDKHelper;

import org.json.JSONObject;
import org.json.JSONException;

public class ambition extends Cocos2dxActivity
{
	// UMeng Analytics���瑕���ㄧ��涓�涓����
	public static Context STATIC_REF = null;

	// 璁惧��娓告�����Native���Lib
	static
	{
		System.loadLibrary("game");
	}

	// UMeng Analytics
	public static Context getContext()
	{
		return STATIC_REF;
	}

	protected void onCreate(Bundle savedInstanceState)
	{
		Log.e("ambition", "ambition onCreate");

		super.onCreate(savedInstanceState);

		// UMeng Analytics ���瑕���ㄧ��涓�涓����������濮����
		STATIC_REF = this;

		// NDK HELPER������璋�
		AndroidNDKHelper.SetNDKReciever(this);
	}

	// ���濮����OpenGL Surface
	public Cocos2dxGLSurfaceView onCreateView()
	{
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);

		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}

	protected void onDestroy()
	{
		super.onDestroy();
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		super.onActivityResult(requestCode, resultCode, data);
	}

	// ���浠���ㄧ�����璋���ュ��
	public void ProcessPurchaseRequest(JSONObject prms)
	{
		try
		{
			final String strCallbackTag = prms.getString("callback_tag");
			final String strFakeCode = String.valueOf(prms.getInt("fake_code"));

			JSONObject paramCallback = new JSONObject();
			paramCallback.put("fake_code", strFakeCode);
			AndroidNDKHelper.SendMessageWithParameters(strCallbackTag, paramCallback);
		}
		catch (JSONException e)
		{
			e.printStackTrace();
		}
	}

	//娓告�������哄��璋�
	public void ExitGame(JSONObject prms)
	{
//		String func = null;
//		
//		try
//		{
//			func = prms.getString("to_call");
//		}
//		catch(JSONException e)
//		{
//			e.printStackTrace();
//		}
		
		AlertDialog.Builder builder = new AlertDialog.Builder(STATIC_REF);
		builder.setMessage("").setNegativeButton("取消", new OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				// TODO Auto-generated method stub
				
			}
		}).setTitle("确认退出游戏？").setPositiveButton("确认", new OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				// TODO Auto-generated method stub
				//AndroidNDKHelper.SendMessageWithParameters(func, null);
				System.exit(0);
			}
		}).show();

	}
}
