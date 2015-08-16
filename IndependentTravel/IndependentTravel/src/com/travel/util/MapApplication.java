package com.travel.util;

import android.app.Application;
import android.util.Log;
import android.widget.Toast;

import com.baidu.mapapi.BMapManager;
import com.baidu.mapapi.MKEvent;
import com.baidu.mapapi.MKGeneralListener;

public final class MapApplication extends Application {

	private static MapApplication instance;
	BMapManager mBMapMan = null;
	String mStrKey = "873544E597B06B687F74082479C1E4A78F7B286C";
	
	public BMapManager getMapManager() {
		return mBMapMan;
	}

	// �ڳ�ʼ����ͼActivityʱ��ע��һ���¼���������ʵ��MKGeneralListener�Ľӿڴ�����Ӧ�¼�
	public static class MyGeneralListener implements MKGeneralListener {
		// �����������ͨ����������ж�ԭ��MKEvent�г���ֵ��
		public void onGetNetworkState(int arg0) {
			Log.d("MyGeneralListener", "onGetNetworkState error is " + arg0);
			Toast.makeText(MapApplication.getInstance(), "GetNetworkState",
					Toast.LENGTH_LONG).show();
		}

		// ������Ȩ��֤����ͨ����������ж�ԭ��MKEvent�г���ֵ��
		public void onGetPermissionState(int iError) {
			Log.d("MyGeneralListener", "onGetPermissionState error is " + iError);
			if (iError == MKEvent.ERROR_PERMISSION_DENIED) {
				Toast.makeText(MapApplication.getInstance(),
						"permissionDenied", Toast.LENGTH_LONG).show();
			}
		}

	}

	@Override
	public void onTerminate() {
		// TODO Auto-generated method stub
		if (mBMapMan != null) {
			mBMapMan.destroy();
			mBMapMan = null;
		}
		super.onTerminate();
	}

	@Override
	public void onCreate() {
		super.onCreate();
		instance = this;
		mBMapMan = new BMapManager(this);
		mBMapMan.init(this.mStrKey, new MyGeneralListener());
	}

	public static MapApplication getInstance() {
		return instance;
	}

}
