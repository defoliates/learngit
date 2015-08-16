package com.hurry.client;

import java.util.Timer;
import java.util.TimerTask;

import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.XMPPException;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.BaiduMap.OnMarkerClickListener;
import com.baidu.mapapi.map.BitmapDescriptor;
import com.baidu.mapapi.map.BitmapDescriptorFactory;
import com.baidu.mapapi.map.InfoWindow;
import com.baidu.mapapi.map.InfoWindow.OnInfoWindowClickListener;
import com.baidu.mapapi.map.MapStatusUpdate;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.Marker;
import com.baidu.mapapi.map.MarkerOptions;
import com.baidu.mapapi.map.MyLocationData;
import com.baidu.mapapi.map.OverlayOptions;
import com.baidu.mapapi.model.LatLng;
import com.hurry.client.util.MyApplication;
import com.hurry.client.util.SharedPreferencesHelper;
import com.hurry.client.util.XmppTool;



public class HurryMapPosition extends Activity{
	// ��ͼ���
	private MapView mMapView = null; // ��ͼView
	private BaiduMap  mBaiduMap;
	private LocationClient mLocationClient = null;
	LatLng nearPos=null;
	//LocationData locData = null;
	boolean isFirstLoc = true;
	//���ݴ洢��ر���
	private SharedPreferencesHelper sp;
	public final static String POS_LONGITUDE="longitude";
	public final static String POS_LATITUDE="latitude";
	
	private LayoutInflater inflater=null;
	private View view=null;
	private MyApplication app;
	private MediaPlayer mediaPlayer;
	private Button hurryBtn,easyBtn;
	//����ߴ���Ϣָ��
	public final static String HURRY_INFO="##hurry##";
	public final static String NO_HURRY_INFO="##no hurry##";
	public final static String HURRY_HEAD_INFO="##head is clicked##";
	
	private final String BROADCAST_HURRY_INFO="action.hurry.info";
	private final String FRIEND_ID="friend_id";
	private final String FRIEND_MESSAGE="friend_message";
	    
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		app=(MyApplication)getApplication();
		app.addActivity(this);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		SDKInitializer.initialize(getApplicationContext());  
		setContentView(R.layout.hurry_map);
		
		//ע��һ���㲥
		IntentFilter intentFilter = new IntentFilter();  
	      intentFilter.addAction(BROADCAST_HURRY_INFO);  
	      registerReceiver(mRefreshBroadcastReceiver, intentFilter);
		//����洢��ǰλ�õı���
		sp=new SharedPreferencesHelper(this,"positionInfo");
		
		//���Ƶ�ǰ�û�����Ϣ��
        inflater=(LayoutInflater)getSystemService(LAYOUT_INFLATER_SERVICE);
		view=inflater.inflate(R.layout.map_windowinfo, null);
		view.setMinimumHeight(150);
		view.setMinimumWidth(200);
		
		// ��ͼ��ʼ��
		mMapView = (MapView) findViewById(R.id.bmapsView);
		mBaiduMap=mMapView.getMap();
		//ʵ��λ�õĶ�λ			
		mLocationClient = new LocationClient(this); 
        mBaiduMap = mMapView.getMap();
		// ������λͼ��
		mBaiduMap.setMyLocationEnabled(true);
        LocationClientOption option = new LocationClientOption(); 
        option.setOpenGps(true); 
        option.setAddrType("all");
        option.setCoorType("bd09ll");                           //������������Ϊbd09ll 
        option.setPoiExtraInfo(true);
        option.setPriority(LocationClientOption.NetWorkFirst);  //������������ 
        //option.setProdName("locSDKDemo2");                      //���ò�Ʒ������ 
        option.setScanSpan(5000);                               //��ʱ��λ��ÿ��5���Ӷ�λһ�Ρ� 
        mLocationClient.setLocOption(option); 
        mLocationClient.registerLocationListener(new BDLocationListener() { 
            @Override 
            public void onReceiveLocation(BDLocation location) { 
            	
            	if (location == null || mMapView == null)
    				return;
    			MyLocationData locData = new MyLocationData.Builder()
    					.accuracy(location.getRadius())
    					// �˴����ÿ����߻�ȡ���ķ�����Ϣ��˳ʱ��0-360
    					.direction(100).latitude(location.getLatitude())
    					.longitude(location.getLongitude()).build();
    			mBaiduMap.setMyLocationData(locData);
    			if (isFirstLoc) {
    				isFirstLoc = false;
    				String citystr=location.getAddrStr();
    				sp.putValue(POS_LONGITUDE, location.getLongitude()+"");
	    			sp.putValue(POS_LATITUDE, location.getLatitude()+"");
	            	Toast.makeText(HurryMapPosition.this, citystr, Toast.LENGTH_LONG)
					.show();
	            	//��λ����ǰ����λ��
	            	LatLng ll = new LatLng(location.getLatitude(),
    						location.getLongitude());
	    			MapStatusUpdate u = MapStatusUpdateFactory.newLatLng(ll);
    				mBaiduMap.animateMapStatus(u);
    				
    				//nearPos=ll;
    				/*
    				//���Ƶ�ǰ�û�����Ϣ��
    		        inflater=(LayoutInflater)getSystemService(LAYOUT_INFLATER_SERVICE);
    				view=inflater.inflate(R.layout.map_windowinfo, null);
    				view.setMinimumHeight(150);
    				view.setMinimumWidth(200);
    				initPositionInfo(view,ll);
    				*/
    				//
    				LatLng objPos = new LatLng(location.getLatitude()+0.1,
    						location.getLongitude()+0.1);
    				//initPositionInfo(view,objPos);
    		
    				nearPos=objPos;
    				//����Markerͼ��
    		        BitmapDescriptor bitmap = BitmapDescriptorFactory
    		            .fromResource(R.drawable.icon_marka);
    		        //����MarkerOption�������ڵ�ͼ�����Marker
    		        OverlayOptions option1 = new MarkerOptions()
    		            .position(objPos)
    		            .icon(bitmap);
    		        //�ڵ�ͼ�����Marker������ʾ
    		        mBaiduMap.addOverlay(option1);
    		       
    				
    			}
    			
            } 
             
            public void onReceivePoi(BDLocation location){ 
                //return ; 
            	String citystr=location.getCity()+location.getProvince()+location.getCityCode();
            	Toast.makeText(HurryMapPosition.this, citystr, Toast.LENGTH_LONG)
				.show();
            } 
        }); 
        mLocationClient.start();
        //Ϊmarker��ǩ��Ӽ����¼�
        mBaiduMap.setOnMarkerClickListener(new OnMarkerClickListener() {
            public boolean onMarkerClick(Marker arg0) {
            	/*Toast.makeText(HurryMapPosition.this, "hello", Toast.LENGTH_LONG)
				.show();
            	*/
            	setPositionInfo(arg0.getPosition());
		       // arg0.setIcon(bitmap);
            	//arg0.setTitle("A");
                return true;
            }
        });
        
        //Ϊ������ť��Ӽ����¼�
        hurryBtn=(Button)this.findViewById(R.id.map_hurry);
        hurryBtn.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				 //
				sendHurryInfo(HURRY_INFO);
			}
        });
        easyBtn=(Button)this.findViewById(R.id.map_nohurry);
        easyBtn.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				 //
				sendHurryInfo(NO_HURRY_INFO);
			}
        });
        
	}
	//���ú�������λ��
	public void setPositionInfo(LatLng postion)
	{
		if(view==null)
		{
			//���Ƶ�ǰ�û�����Ϣ��
	        inflater=(LayoutInflater)getSystemService(LAYOUT_INFLATER_SERVICE);
			view=inflater.inflate(R.layout.map_windowinfo, null);
			view.setMinimumHeight(150);
			view.setMinimumWidth(200);
		}
		((LinearLayout)this.findViewById(R.id.map_userinfo_layout)).setVisibility(View.VISIBLE);
		((TextView)this.findViewById(R.id.map_username)).setText(app.currentTalkObj);
		//����InfoWindow  
        InfoWindow mInfoWindow = new InfoWindow(view, postion, listener);  
        //��ʾInfoWindow  
        mBaiduMap.showInfoWindow(mInfoWindow); 
	}
	
	//����InfoWindow�ĵ���¼�������  
    private OnInfoWindowClickListener listener = new OnInfoWindowClickListener() {  
        public void onInfoWindowClick() {  
        //��ӵ������¼���Ӧ����  
        	sendHurryInfo(HURRY_HEAD_INFO);
        	if(nearPos!=null)
        		setPositionInfo(nearPos);
        	/*
        Toast.makeText(HurryMapPosition.this, "hello", Toast.LENGTH_LONG)
			.show();
        	*/
        }  
    };  
    //
    public void sendHurryInfo(String info)
    {
    	//�����뵱ǰ�����Ա�Ự
    	Chat newchat = XmppTool.getConnection().getChatManager().createChat(app.currentTalkObj+"@hnu-pc", null);
    	try {
			newchat.sendMessage(info);
		} catch (XMPPException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
	//��ȡ�����û���λ����Ϣ
	public void getUserPosition()
	{
		
	}
	
	@Override
	protected void onPause() {
		mMapView.onPause();
		super.onPause();
	}

	@Override
	protected void onResume() {
		mMapView.onResume();
		super.onResume();
	}

	@Override
	protected void onDestroy() {
		mMapView.onDestroy();
		unregisterReceiver(mRefreshBroadcastReceiver);
		super.onDestroy();
	}
	
	// �����Ƿ����ݿ����ݷ����仯��ˢ��ҳ��  
	protected BroadcastReceiver mRefreshBroadcastReceiver = new BroadcastReceiver() {  
	  
	      @Override  
	      public void onReceive(Context context, Intent intent) {  
	          String action = intent.getAction();
	          String userName=intent.getStringExtra(FRIEND_ID);
	          String[] userId=userName.split("@");
	          String info=intent.getStringExtra(FRIEND_MESSAGE);
	          
	          if (action.equals(BROADCAST_HURRY_INFO))  
	          {  
	        	  //updateList(intent.getStringExtra(FRIEND_ID),intent.getStringExtra(FRIEND_MESSAGE));
	        	  	Log.v("--tags--", "--tags-form--"+intent.getStringExtra(FRIEND_ID));
					Log.v("--tags--", "--tags-message--"+intent.getStringExtra(FRIEND_MESSAGE));
					if(info.equals(HURRY_INFO))
					{
						//��������
						mediaPlayer = MediaPlayer.create(HurryMapPosition.this,R.raw.hurry);
						mediaPlayer.start();
					}
					else if(info.equals(NO_HURRY_INFO))
					{
						//��������
						mediaPlayer = MediaPlayer.create(HurryMapPosition.this,R.raw.nohurry);
						mediaPlayer.start();
					}
					else if(info.equals(HURRY_HEAD_INFO))
					{
						if(view!=null)
						{	
							((ImageView)view.findViewById(R.id.map_window_head)).setImageDrawable(getResources().getDrawable(R.drawable.profile));
							//�ػ�
							setPositionInfo(nearPos);
						
						}
					}
					setMyTimer(10);
	          }  
	      }  
	  }; 
	  //
	  private Timer  timer;
	  public void setMyTimer(int second)
	  {
		  // �����ʱ��   
			timer = new Timer();   
          // ����ƻ����񣬸��ݲ����Ĳ�ͬ���������������Ĺ������ڹ̶�ʱ��ִ��ĳ�����ڹ̶�ʱ�俪ʼ�ظ�ִ��ĳ�����ظ�ʱ�����ɿأ����ӳٶ�ú�ִ��ĳ�������ӳٶ�ú��ظ�ִ��ĳ�����ظ�ʱ�����ɿ�   
          timer.schedule(new TimerTask() {   
              // TimerTask �Ǹ�������,ʵ�ֵ���Runable��   
              @Override  
              public void run() {   
                  Log.i("TIMER:", Thread.currentThread().getName());   
                  //����һ����Ϣ����ȥ   
                  Message msg = new Message();   
                  msg.what = 1;   
                  handler.sendMessage(msg);   
              }   

          }, second*1000, 200);
	  }
	// ����Handler   
      final Handler handler = new Handler() {   

          @Override  
          public void handleMessage(Message msg) {   
              super.handleMessage(msg);   
              //handler������Ϣ   
              ((ImageView)view.findViewById(R.id.map_window_head)).setImageDrawable(getResources().getDrawable(R.drawable.h001));
              setPositionInfo(nearPos);
              ((LinearLayout)HurryMapPosition.this.findViewById(R.id.map_userinfo_layout)).setVisibility(View.GONE);
              timer.cancel(); 
          }   
      };   
}
