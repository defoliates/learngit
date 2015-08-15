package com.test.pptcontrol;
/**
 * ����һ��������ʾ���棬������ʾ�����б�Ϳ��Ʋ���
 */
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.HashMap;

import com.iflytek.cloud.speech.RecognizerResult;
import com.iflytek.cloud.speech.SpeechConstant;
import com.iflytek.cloud.speech.SpeechError;
import com.iflytek.cloud.ui.RecognizerDialog;
import com.iflytek.cloud.ui.RecognizerDialogListener;
import com.msc.util.JsonParser;
import com.test.pptcontrol.R.id;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class Music extends Activity implements OnTouchListener,OnGestureListener{
	/** Called when the activity is first created. */
	//
	private GestureDetector gd;
	private MyApplication app;
	final int FLING_MIN_DISTANCE = 100, FLING_MIN_VELOCITY = 20; 
	//
	private ListView list;
	//DBAdapter db;
	private ImageButton SetUpButton;
	private ImageButton forwardButton;
	private ImageButton playButton;
	private ImageButton backButton;
	private ImageButton soundButton;
	
	private boolean playCheck;
	private boolean alarmCheck;
	private boolean lightCheck;
	//
	private Handler mHandler;
	private String ipAddress;
	private String mPort;
	//���棬���浱ǰ�������������һ������Ӧ�ó���ʹ��.
	private SharedPreferences mSharedPreferences;
	final private String song[]={"��«��","����"};
	//ʶ�𴰿�
	private RecognizerDialog iatDialog;
	private Toast mToast;
	private String mResultText;
	
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//
		requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.music);
		//���Ĵ�����ʽ
		getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.musicbar);
		
		new CustomThread().start();//�½�������CustomThreadʵ��
		//��ʼ���������.
		mSharedPreferences = getSharedPreferences(getPackageName(),
						MODE_PRIVATE);
		
		LinearLayout viewSnsLayout=(LinearLayout)findViewById(R.id.musicview);
		//��֤�ܹ����������¼�
		viewSnsLayout.setOnTouchListener(this);
		viewSnsLayout.setLongClickable(true);
		//��ȡApplication����
		app=(MyApplication)getApplication();
		gd = new GestureDetector((OnGestureListener)this);
		//
		mToast = Toast.makeText(this, "", Toast.LENGTH_LONG);
		list=(ListView) findViewById(R.id.users);
		 //db= new DBAdapter(this);
		
		alarmCheck=true;
		lightCheck=true;
		//ע��һ���㲥
		IntentFilter intentFilter = new IntentFilter();  
	      intentFilter.addAction("action.refreshList");  
	      registerReceiver(mRefreshBroadcastReceiver, intentFilter);  
	      //���adapter������
		SimpleAdapter useradapter = new SimpleAdapter(this, app.getData(),
				R.layout.user, new String[] { "img", "code","singer", "name" },
				new int[] { R.id.img,R.id.code, R.id.singer,
						R.id.name});
		list.setAdapter(useradapter);
		
		//�����á���ť��ʼ��
		SetUpButton=(ImageButton)findViewById(R.id.music_setting_btn);
		SetUpButton.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
			//��ת�����ý���.
			Intent localIntent = new Intent();
			localIntent.setClass(Music.this, SQLSong.class);
			startActivity(localIntent);				
			}
        });
		//
		forwardButton=(ImageButton)findViewById(R.id.music_forward);
		forwardButton.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
			//��ת�����ý���.
				mHandler.obtainMessage(207, "weizhi").sendToTarget();			
			}
        });
		//
		playCheck=false;
		playButton=(ImageButton)findViewById(R.id.music_start);
		playButton.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				//��ת�����ý���.
				if(playCheck)
				{
					mHandler.obtainMessage(200, "weizhi").sendToTarget();
					playButton.setImageDrawable(getResources().getDrawable(R.drawable.play));
				}
				else
				{
					mHandler.obtainMessage(201, "weizhi").sendToTarget();
					playButton.setImageDrawable(getResources().getDrawable(R.drawable.pause));
				}
				playCheck=!playCheck;
			}
        });
		//
		backButton=(ImageButton)findViewById(R.id.music_next);
		backButton.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
			//��ת�����ý���.
				mHandler.obtainMessage(206, "weizhi").sendToTarget();			
			}
        });
		//
		//��ʼ����дDialog,���ֻʹ����UI��д����,���贴��SpeechRecognizer
		iatDialog =new RecognizerDialog(this);
		soundButton=(ImageButton)findViewById(R.id.music_sound);
		soundButton.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				//��ʾ������дDialog.
				showIatDialog();		
			}
        });
		//Ϊlistview��ÿһ����Ӽ����¼�
		list.setOnItemClickListener(new OnItemClickListener()
		{
		       @Override
		      public void onItemClick(AdapterView<?> parent, View view, int position, long id)
		      {
		    	   ListView listView = (ListView)parent;
		    	    HashMap<String, String> map = (HashMap<String, String>) listView.getItemAtPosition(position);
		    	    String userid = map.get("code");
		    	    String singer = map.get("singer");
		    	    String name = map.get("name");
		    	    //Toast.makeText(Music.this, userid +" , "+ singer +" , "+ name ,Toast.LENGTH_LONG).show();
		    	    if(name.equals("��«��"))
		    	    	mHandler.obtainMessage(1, "weizhi").sendToTarget();
		    	    else if(name.equals("����"))
		    	    	mHandler.obtainMessage(0, "weizhi").sendToTarget();
		    	    else if(name.equals("���־�"))
		    	    	mHandler.obtainMessage(2, "weizhi").sendToTarget();
		    	    else if(name.equals("������"))
		    	    	mHandler.obtainMessage(3, "weizhi").sendToTarget();
		    	    else if(name.equals("��ֻ�ϻ�"))
		    	    	mHandler.obtainMessage(4, "weizhi").sendToTarget();
		    	    else if(name.equals("�����"))
		    	    	mHandler.obtainMessage(5, "weizhi").sendToTarget();
		    	    else
		    	    	Toast.makeText(Music.this, "��������ĸ�����Ƭ������û�У�������ѡ��" ,Toast.LENGTH_LONG).show();
		    }
		});
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
		//mResultText="";
		//��ʾ��д�Ի���
		iatDialog.setListener(recognizerDialogListener);
		iatDialog.show();
		showTip(getString(R.string.text_iat_begin));
	}
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
				//
				if(mResultText.indexOf("��«��")>-1)
	    	    	mHandler.obtainMessage(1, "weizhi").sendToTarget();
	    	    else if(mResultText.indexOf("����")>-1)
	    	    	mHandler.obtainMessage(0, "weizhi").sendToTarget();
	    	    else if(mResultText.indexOf("���־�")>-1)
	    	    	mHandler.obtainMessage(2, "weizhi").sendToTarget();
	    	    else if(mResultText.indexOf("������")>-1)
	    	    	mHandler.obtainMessage(3, "weizhi").sendToTarget();
	    	    else if(mResultText.indexOf("��ֻ�ϻ�")>-1)
	    	    	mHandler.obtainMessage(4, "weizhi").sendToTarget();
	    	    else if(mResultText.indexOf("�����")>-1)
	    	    	mHandler.obtainMessage(5, "weizhi").sendToTarget();
	    	    else if(mResultText.indexOf("��һ��")>-1)
	    	    	mHandler.obtainMessage(207, "weizhi").sendToTarget();
	    	    else if(mResultText.indexOf("��һ��")>-1)
	    	    	mHandler.obtainMessage(206, "weizhi").sendToTarget();
	    	    else if(mResultText.indexOf("����")>-1)
	    	    	{
	    	    	mHandler.obtainMessage(200, "weizhi").sendToTarget();
					playButton.setImageDrawable(getResources().getDrawable(R.drawable.play));    	    	
	    	    	}
	    	    else if(mResultText.indexOf("��ͣ")>-1)
	    	    	{
	    	    	mHandler.obtainMessage(201, "weizhi").sendToTarget();
					playButton.setImageDrawable(getResources().getDrawable(R.drawable.pause));    	    	
	    	    	}
	    	    else
	    	    	Toast.makeText(Music.this, "����ѡ��ĸ�����Ƭ������û�У�������ѡ��" ,Toast.LENGTH_LONG).show();
			}
		}

		/**
		 * ʶ��ص�����.
		 */
		public void onError(SpeechError error) {
			
		}
		
	};
	//������Ϣ
	private void showTip(String str)
	{
		if(!TextUtils.isEmpty(str))
		{
			mToast.setText(str);
			mToast.show();
		}
	}
	
	public boolean onTouch(View v, MotionEvent event) {  
        // TODO Auto-generated method stub  
        return gd.onTouchEvent(event);  
    }
	/***********
	 * 
	 */
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,  
            float velocityY) {  
        // TODO Auto-generated method stub  
        if (e2.getX()-e1.getX() > FLING_MIN_DISTANCE && Math.abs(velocityX) >FLING_MIN_VELOCITY) {  
              
            //�л�Activity  
            Intent intent = new Intent(Music.this, MainActivity.class);  
            startActivity(intent);  
            //Toast.makeText(this, "��������", Toast.LENGTH_SHORT).show();  
        }  
          
        return false;  
    }
	//
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	 @Override
	    public boolean onMenuItemSelected(int featureId, MenuItem item) {
	        switch (item.getItemId()) {
	        case id.buzzer:
	        	if(alarmCheck)
	        		mHandler.obtainMessage(202, "weizhi").sendToTarget();
	        	else
	        		mHandler.obtainMessage(203, "weizhi").sendToTarget();
	        	alarmCheck=!alarmCheck;
	            //Toast.makeText(getApplicationContext(), string.itemAddSelect, Toast.LENGTH_SHORT).show();
	            break;
	        case id.light:
	        	if(lightCheck)
	        		mHandler.obtainMessage(204, "weizhi").sendToTarget();
	        	else
	        		mHandler.obtainMessage(205, "weizhi").sendToTarget();
	        	lightCheck=!lightCheck;
	            //Toast.makeText(getApplicationContext(), string.itemEditSelect, Toast.LENGTH_SHORT).show();
	            break;
	        case id.about:
	            //Toast.makeText(getApplicationContext(), string.itemDelSelect, Toast.LENGTH_SHORT).show();
	        	Toast.makeText(Music.this, "��ƭ�˰ɣ�����û�й��ڣ�������" ,Toast.LENGTH_LONG).show();
	            break;       
	        }        
	        return false;
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
	 @Override
	protected void onDestroy() {
	        super.onDestroy();
	        unregisterReceiver(mRefreshBroadcastReceiver);
	    }
	 
	 protected void onStop() {
			mToast.cancel();
			if (null != iatDialog) {
				iatDialog.cancel();
			}
			super.onStop();
		}
	 
	 class CustomThread extends Thread {
		 //
         String str="";
        @Override
        public void run() {

            //������Ϣѭ���Ĳ���

            Looper.prepare();//1����ʼ��Looper

            mHandler = new Handler(){//2����handler��CustomThreadʵ����Looper����

                public void handleMessage (Message msg) {//3�����崦����Ϣ�ķ���
                	str=""+msg.what;
                	/*	
                    switch(msg.what) {

                    case 0:
                        Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);
                        str=""+0;
                        break;
                    case 1:
                        Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);
                        str=""+1;
                        break;
                    case 2:
                        Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);
                        str=""+2;
                        break;
                    case 3:
                        Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);
                        str=""+3;
                        break;
                    }
                    */
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
    
	//����һ��ˢ�º���
	 private void refresh() {
	        finish();
	        Intent intent = new Intent(Music.this, Music.class);
	        startActivity(intent);
	    }
	// �����Ƿ����ݿ����ݷ����仯��ˢ��ҳ��  
	  private BroadcastReceiver mRefreshBroadcastReceiver = new BroadcastReceiver() {  
	  
	      @Override  
	      public void onReceive(Context context, Intent intent) {  
	          String action = intent.getAction();  
	          if (action.equals("action.refreshList"))  
	          {  
	        	  refresh();  
	          }  
	      }  
	  };  
	 
}
