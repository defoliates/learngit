package com.test.pptcontrol;
/****
 * ����һ��PPT�������棬����ʹ���ֶ����ƺ���������
 */
import java.io.IOException;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.iflytek.cloud.speech.RecognizerResult;
import com.iflytek.cloud.speech.SpeechConstant;
import com.iflytek.cloud.speech.SpeechError;
import com.iflytek.cloud.speech.SpeechListener;
import com.iflytek.cloud.speech.SpeechUser;
import com.iflytek.cloud.ui.RecognizerDialog;
import com.iflytek.cloud.ui.RecognizerDialogListener;
import com.msc.util.JsonParser;

public class MainActivity extends Activity implements OnTouchListener,OnGestureListener{

	private Button start;
	private Button escape;
	private Button forward;
	private Button back;
	private Button iatButton;
	ImageButton settingButton;
	
	private final static int RIGHT = 1;
	private final static int LEFT = 2;
	private final static int SHIFTF5 = 0;
	private final static int ESC = 3;
	
	private Handler mHandler;
	//private MyApplication app;
	//
	//��־TAG.
	private static final String TAG = "IatDemoActivity";
	// Tip
	private Toast mToast;
	//ʶ������ʾ
	private String mResultText;
	private String ipAddress;
	private String mPort;
	//ʶ�𴰿�
	private RecognizerDialog iatDialog;
	//���棬���浱ǰ�������������һ������Ӧ�ó���ʹ��.
	private SharedPreferences mSharedPreferences;
	//
	GestureDetector gd;
	final int FLING_MIN_DISTANCE = 100, FLING_MIN_VELOCITY = 200; 
		
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//�û��Զ��崰����ʾ״̬
		requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.main);
		getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.titlebar);
		//��ȡApplication����
		//app=(MyApplication)getApplication();
		((TextView) findViewById(R.id.titlebar_text)).setText(R.string.text_iat_demo);
		
		LinearLayout viewSnsLayout=(LinearLayout)findViewById(R.id.viewSnsLayout);
		//��֤�ܹ����������¼�
		viewSnsLayout.setOnTouchListener(this);
		viewSnsLayout.setLongClickable(true);
		//
		new CustomThread().start();//�½�������CustomThreadʵ��
		//����Ƿ�������
		checkNetwork();
		//�û���¼
		SpeechUser.getUser().login(MainActivity.this, null, null
				, "appid=" + getString(R.string.app_id), listener);
		//����PPT���Ƶ��ĸ���ť
		 start = (Button)this.findViewById(R.id.start);
        escape = (Button)this.findViewById(R.id.escape);
        forward = (Button)this.findViewById(R.id.froward);
        back = (Button)this.findViewById(R.id.back);
        //
		mToast = Toast.makeText(this, "", Toast.LENGTH_LONG);	
		//��ʼ���������.
		mSharedPreferences = getSharedPreferences(getPackageName(),
						MODE_PRIVATE);
		//����һ������ʶ�������
		gd = new GestureDetector((OnGestureListener)this);
		//��ʶ�𡱰�ť��ʼ��
		iatButton = (Button) findViewById(R.id.speech);	
		//��д�������
		//mResultText = (EditText) findViewById(R.id.txt_result);
		iatButton.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
			//��ʾ������дDialog.
			showIatDialog();			
			
			}
        });
		iatButton.setText(R.string.text_iat);
		//
		//�����á���ť��ʼ��
		settingButton=(ImageButton)findViewById(R.id.tts_setting_btn);
		settingButton.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
			//��ת�����ý���.
			Intent localIntent = new Intent();
			localIntent.setClass(MainActivity.this, SpeechControl.class);
			startActivity(localIntent);				
			}
        });
		//��ʼ����дDialog,���ֻʹ����UI��д����,���贴��SpeechRecognizer
		iatDialog =new RecognizerDialog(this);
		//ΪPPT���Ű�ť��Ӽ����¼�
        start.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View v) {
				String str ="start";
	            Log.d("Test", "MainThread is ready to send msg:" + str);
	            mHandler.obtainMessage(SHIFTF5, str).sendToTarget();//������Ϣ��CustomThreadʵ��  
			}
        	
        });
        //ΪPPT�˳���ť��Ӽ����¼�
        escape.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View arg0) {
				//mHandler=app.getHandler();
				String str ="esc";
	            Log.d("Test", "MainThread is ready to send msg:" + str);
	            mHandler.obtainMessage(ESC, str).sendToTarget();//������Ϣ��CustomThreadʵ��  
			}
        });
        //ΪPPT�·���ť��Ӽ����¼�
        forward.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View v) {
				//mHandler=app.getHandler();
				String str ="RIGHT";
	            Log.d("Test", "MainThread is ready to send msg:" + str);
	            mHandler.obtainMessage(RIGHT, str).sendToTarget();//������Ϣ��CustomThreadʵ��  
				
			}
        	
        });
      //ΪPPT�Ϸ���ť��Ӽ����¼�
        back.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View v) {
				//mHandler=app.getHandler();
				String str ="LEFT";
	            Log.d("Test", "MainThread is ready to send msg:" + str);
	            mHandler.obtainMessage(LEFT, str).sendToTarget();//������Ϣ��CustomThreadʵ��  
			}
        });
    }
	
	/*********************************
	 * ��Ӵ�������
	 */
	public boolean onTouch(View v, MotionEvent event) {  
        // TODO Auto-generated method stub  
        return gd.onTouchEvent(event);  
    }
	/***********
	 * ��������ʶ����
	 */
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,  
            float velocityY) {  
        // TODO Auto-generated method stub  
        if(e1.getX() - e2.getX() > FLING_MIN_DISTANCE&&Math.abs(velocityX) > FLING_MIN_VELOCITY)  
        {  
            Intent intent = new Intent(MainActivity.this,Music.class);  
            startActivity(intent);  
            overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
             //Toast.makeText(this, "��������", Toast.LENGTH_SHORT).show();   
  
        }  
        /*else
        if (e2.getX()-e1.getX() > FLING_MIN_DISTANCE && Math.abs(velocityX) >FLING_MIN_VELOCITY) {  
              
            //�л�Activity  
            Intent intent = new Intent(MainActivity.this, Music.class);  
            startActivity(intent);  
            //Toast.makeText(this, "��������", Toast.LENGTH_SHORT).show();  
        }  
          */
        return false;  
    }  
	/**
	 * ��ʾ��д�Ի���.
	 * @param
	 */
	public void showIatDialog()
	{
		if(null == iatDialog) {
		//��ʼ����дDialog	
		iatDialog =new RecognizerDialog(this);
		}
		//���Grammar_ID����ֹʶ��������дʱGrammar_ID�ĸ���
		iatDialog.setParameter(SpeechConstant.CLOUD_GRAMMAR, null);
		//������дDialog������
		iatDialog.setParameter(SpeechConstant.DOMAIN, "iat");
		//���ò����ʲ�����֧��8K��16K 
		String rate =mSharedPreferences.getString(
				getString(R.string.preference_key_iat_rate),
				getString(R.string.preference_default_iat_rate));
				
		if(rate.equals("rate8k"))
		{
			iatDialog.setParameter(SpeechConstant.SAMPLE_RATE, "8000");
		}
		else 
		{
			iatDialog.setParameter(SpeechConstant.SAMPLE_RATE, "16000");
		}
		//iatDialog.setParameter(SpeechConstant.SAMPLE_RATE, "8000");
		//��ս����ʾ��.
		mResultText="";
		//��ʾ��д�Ի���
		iatDialog.setListener(recognizerDialogListener);
		iatDialog.show();
		showTip(getString(R.string.text_iat_begin));
	}
	//������Ϣ
	private void showTip(String str)
	{
		if(!TextUtils.isEmpty(str))
		{
			mToast.setText(str);
			mToast.show();
		}
	}
	//
	/**
	 * ʶ��ص�������
	 */
	RecognizerDialogListener recognizerDialogListener=new RecognizerDialogListener()
	{
		@Override
		public void onResult(RecognizerResult results, boolean isLast) {
			String text = JsonParser.parseIatResult(results.getResultString());
			mResultText=text.trim();
			mResultText.replaceAll("\\p{P}", "");
			if(!mResultText.equals(""))
			{
			showTip(mResultText);
			//mHandler=app.getHandler();
			if(mResultText.indexOf("����")>-1)
				mHandler.obtainMessage(SHIFTF5, mResultText).sendToTarget();
			else if(mResultText.indexOf("�˳�")>-1)
				mHandler.obtainMessage(ESC, mResultText).sendToTarget();
			else if(mResultText.indexOf("��һҳ")>-1)
				mHandler.obtainMessage(LEFT, mResultText).sendToTarget();
			else if(mResultText.indexOf("��һҳ")>-1)
				mHandler.obtainMessage(RIGHT, mResultText).sendToTarget();
			else
				showTip("����ָ��������������룡");
			}
			//mResultText.append(text);
			//mResultText.setSelection(mResultText.length());
		}

		/**
		 * ʶ��ص�����.
		 */
		public void onError(SpeechError error) {
			
		}
		
	};
	//
	/**
	 * ��дActivity��onStart����
	 * @param
	 */
		@Override
	protected void onStop() {
		mToast.cancel();
		if (null != iatDialog) {
			iatDialog.cancel();
		}
		super.onStop();
	}
	
	/**
	 * �û���¼�ص�������.
	 */
	private SpeechListener listener = new SpeechListener()
	{

		@Override
		public void onData(byte[] arg0) {
		}

		@Override
		public void onCompleted(SpeechError error) {
			if(error != null) {
				Toast.makeText(MainActivity.this, getString(R.string.text_login_fail)
						, Toast.LENGTH_SHORT).show();
				
			}			
		}

		@Override
		public void onEvent(int arg0, Bundle arg1) {
		}		
	};
	//���˰�ť��Ӧ�¼�
	public boolean onKeyDown(int keyCode, KeyEvent event) 
    {	
		if ( event.getKeyCode() == KeyEvent.KEYCODE_BACK){
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setTitle("�˳�Ӧ��");
			builder.setMessage("׼���˳�...");
			//builder.setIcon(R.drawable.stat_sys_warning);
			builder.setPositiveButton("ȷ��",new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialog, int which) {
					Intent startMain = new Intent(Intent.ACTION_MAIN);
					startMain.addCategory(Intent.CATEGORY_HOME); 
					startMain.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK); 
					startActivity(startMain);
					System.exit(0);
				}
			});
			builder.setNegativeButton("ȡ��",new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					
				}
			});
			builder.show();
		}
		return super.onKeyDown(keyCode, event);
	}
	//�趨�����������ȼ�
	public void setPreferredNetwork(int networkType) {
		ConnectivityManager connMgr = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		if (networkType == ConnectivityManager.TYPE_WIFI) {//��Ϊ2G/3G�������ȣ�����wifi���ӵ�AP��ϵͳ��Ȼͨ��2G/3G��������
			connMgr.setNetworkPreference(0);
		} else if (networkType == ConnectivityManager.TYPE_MOBILE) {
			connMgr.setNetworkPreference(1);
		}
	}
	//
	/** �ж��Ƿ������� */
	private void checkNetwork() {
		boolean flag = false;
		ConnectivityManager cwjManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		if (cwjManager.getActiveNetworkInfo() != null)
			flag = cwjManager.getActiveNetworkInfo().isAvailable();
		if (!flag) {
			Builder b = new AlertDialog.Builder(this).setTitle("û�п��õ�����").setMessage("�뿪��GPRS��WIFI��������");
			b.setPositiveButton("ȷ��", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int whichButton) {
					Intent intent=null;
	                //�ж��ֻ�ϵͳ�İ汾  ��API����10 ����3.0�����ϰ汾 
	                if(android.os.Build.VERSION.SDK_INT>10){
	                    intent = new Intent(android.provider.Settings.ACTION_WIRELESS_SETTINGS);
	                }else{
	                    intent = new Intent();
	                    ComponentName component = new ComponentName("com.test.pptcontrol","com.test.pptcontrol.WirelessSettings");
	                    intent.setComponent(component);
	                    intent.setAction("android.intent.action.VIEW");
	                }
	                startActivity(intent);
	            }
			}).setNeutralButton("ȡ��", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int whichButton) {
					dialog.cancel();
				}
			}).create();
			b.show();
		}

		//return flag;
	}
	//

	 class CustomThread extends Thread {
			 //
	         String str="";
	        @Override
	        public void run() {

	            //������Ϣѭ���Ĳ���

	            Looper.prepare();//1����ʼ��Looper

	            mHandler = new Handler(){//2����handler��CustomThreadʵ����Looper����

	                public void handleMessage (Message msg) {//3�����崦����Ϣ�ķ���

	                    switch(msg.what) {

	                    case SHIFTF5:
	                        Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);
	                        str=""+SHIFTF5;
	                        break;
	                    case RIGHT:
	                        Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);
	                        str=""+RIGHT;
	                        break;
	                    case LEFT:
	                        Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);
	                        str=""+LEFT;
	                        break;
	                    case ESC:
	                        Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);
	                        str=""+ESC;
	                        break;
	                    }	                    
                        //
                        DatagramSocket ds;
                        ipAddress=mSharedPreferences.getString(
                				getString(R.string.set_ip),
                				getString(R.string.ip_default));
                        mPort=mSharedPreferences.getString(
                				getString(R.string.set_port),
                				getString(R.string.port_default));
                        try {
                            //�׽���
                            ds = new DatagramSocket();                           
                            //�������ݱ�������+��ַ��
                            DatagramPacket dp = new DatagramPacket(str.getBytes(),str.length(),InetAddress.getByName(ipAddress),Integer.parseInt(mPort));//"192.168.185.1"
                            //�����ݱ����ͳ�ȥ
                            ds.send(dp);
                            //�ر��׽���
                            ds.close();
                            
                        } catch (IOException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }

	                }

	            };

	            Looper.loop();//4��������Ϣѭ��

	        }

	    }
	    
	@Override
	public boolean onDown(MotionEvent e) {
		// TODO Auto-generated method stub
		return false;
	}
	@Override
	public void onShowPress(MotionEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public boolean onSingleTapUp(MotionEvent e) {
		// TODO Auto-generated method stub
		return false;
	}
	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX,
			float distanceY) {
		// TODO Auto-generated method stub
		return false;
	}
	@Override
	public void onLongPress(MotionEvent e) {
		// TODO Auto-generated method stub
		
	}
}
