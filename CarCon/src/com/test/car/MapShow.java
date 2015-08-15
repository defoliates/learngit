package com.test.car;

import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.DatePickerDialog.OnDateSetListener;
import android.app.TabActivity;
import android.app.TimePickerDialog;
import android.app.TimePickerDialog.OnTimeSetListener;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TabHost;
import android.widget.TabWidget;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;
import android.widget.ZoomControls;

import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.MapStatusUpdate;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.MyLocationData;
import com.baidu.mapapi.model.LatLng;
import com.baidu.mapapi.overlayutil.PoiOverlay;
import com.baidu.mapapi.search.core.CityInfo;
import com.baidu.mapapi.search.core.PoiInfo;
import com.baidu.mapapi.search.core.SearchResult;
import com.baidu.mapapi.search.poi.OnGetPoiSearchResultListener;
import com.baidu.mapapi.search.poi.PoiDetailResult;
import com.baidu.mapapi.search.poi.PoiNearbySearchOption;
import com.baidu.mapapi.search.poi.PoiResult;
import com.baidu.mapapi.search.poi.PoiSearch;

public class MapShow extends TabActivity implements OnDateSetListener,OnTimeSetListener, OnClickListener{
	
	// ��ͼ���
	private ZoomControls zoomControls;
	private MapView mMapView = null; // ��ͼView
	private BaiduMap  mBaiduMap=null;
	private LocationClient mLocationClient = null,mLocClient;
	public MyLocationListenner myListener = new MyLocationListenner();
	boolean isFirstLoc = true;
	private PoiSearch  mPoiSearch;;
	private LatLng nearPos;
	
	private TabHost myTabhost;
	private TabWidget tabWidget;
	
	private SharedPreferencesHelper sp;
	public final static String POS_LONGITUDE="longitude";
	public final static String POS_LATITUDE="latitude";
	public final static String MAP_DATE="date";
	public final static String MAP_TIME="time";
	private final String LIST_STORE_INFO="list_store_info"; 
	
	private MyApplication app;
	
	private ImageView searchImg,orderConfirm,selectDes;
	private RelativeLayout maplayout,maplayout1;
	private TextView placeName,placeTel,orderBack;
	private EditText date,time,orderName,orderTel,placeStart,placeDes;
	
	private String mapShop="",mapTel="";
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		app=(MyApplication)getApplication();
		app.addActivity((Activity)this);
		this.requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		SDKInitializer.initialize(getApplicationContext());  
		initTab();		
		
		placeName=(TextView)this.findViewById(R.id.map_info_up);
		placeTel=(TextView)this.findViewById(R.id.map_info_down);
		orderBack=(TextView)this.findViewById(R.id.map_back);
		//
		orderName=(EditText)this.findViewById(R.id.map_account);
		orderTel=(EditText)this.findViewById(R.id.map_tel);
		date=(EditText)this.findViewById(R.id.map_date);
		time=(EditText)this.findViewById(R.id.map_time);
		placeStart=(EditText)this.findViewById(R.id.map_source);
		placeDes=(EditText)this.findViewById(R.id.map_des);
		//
		orderConfirm=(ImageView)this.findViewById(R.id.map_confirm);
		selectDes=(ImageView)this.findViewById(R.id.map_info_img);
				
		maplayout=(RelativeLayout)this.findViewById(R.id.map_info_layout);
		maplayout1=(RelativeLayout)this.findViewById(R.id.map_info_layout1);
		//��ȡ�����������û���
		sp=new SharedPreferencesHelper(this,"positionInfo");
		
		// ��ͼ��ʼ��
		mMapView = (MapView) findViewById(R.id.bmapsView1);
		mBaiduMap=mMapView.getMap();
		zoomControls = (ZoomControls) mMapView.getChildAt(2); 
		zoomControls.hide();
		//�������ſؼ���λ��  
        //zoomControls.setPadding(0, 0, 0, 100);  
        //zoomControls.setOrientation(1);
		// ������λͼ��
		mBaiduMap.setMyLocationEnabled(true);
		
		// ��λ��ʼ��
		mLocClient = new LocationClient(getApplicationContext());
		mLocClient.registerLocationListener(myListener);
		LocationClientOption option = new LocationClientOption();
		option.setOpenGps(true);// ��gps
		option.setCoorType("bd09ll"); // ������������
		option.setScanSpan(1000);
		option.setPoiExtraInfo(true);
		//��ѯ��ַ��Ϣ����ʹ�ܵ�һ������
		option.setAddrType("all");
		mLocClient.setLocOption(option);
		mLocClient.start();
		
		//������ť��ת�¼�
		searchImg=(ImageView)this.findViewById(R.id.start_search);
		searchImg.setOnClickListener(new OnClickListener(){
			public void onClick(View v) {
				Intent intent=new Intent();
				intent.setClass(MapShow.this, Search.class);
				//startActivity(intent);
				startActivityForResult(intent, 0);
			}
        });
		
		//
		date.setOnClickListener(this);
		time.setOnClickListener(this);
		orderBack.setOnClickListener(this);
		orderConfirm.setOnClickListener(this);
		//
		initDate();
		initAccount();
	}
	
	public void onClick(View v) {//��ͨ��ť�¼�
		switch(v.getId())
		{
		case R.id.map_time:
			Calendar c = Calendar.getInstance(Locale.CHINA);
			//����һ����������d��ͨ����̬����getInstance() ��ָ��ʱ�� Locale.CHINA ���һ������ʵ��
			int hour = c.get(Calendar.HOUR_OF_DAY);
			int minute = c.get(Calendar.MINUTE);
			
			//��������е� year month day
			TimePickerDialog dlg1=new TimePickerDialog(this,this,hour,minute,true);
			//�½�һ��DatePickerDialog ���췽����         
//			     ���豸�����ģ�OnDateSetListenerʱ�����ü�������Ĭ���꣬Ĭ���£�Ĭ���գ�
			dlg1.show();
			//��DatePickerDialog��ʾ����
			break;
		case R.id.map_date:
			Calendar d = Calendar.getInstance(Locale.CHINA);
			//����һ����������d��ͨ����̬����getInstance() ��ָ��ʱ�� Locale.CHINA ���һ������ʵ��
			Date myDate=new Date();
			//����һ��Dateʵ��
			d.setTime(myDate);
			//����������ʱ�䣬��һ���½�Dateʵ��myDate����
			int year=d.get(Calendar.YEAR);
			int month=d.get(Calendar.MONTH);
			int day=d.get(Calendar.DAY_OF_MONTH);
			//��������е� year month day
			DatePickerDialog dlg=new DatePickerDialog(this,this,year,month,day);
			//�½�һ��DatePickerDialog ���췽����         
//			     ���豸�����ģ�OnDateSetListenerʱ�����ü�������Ĭ���꣬Ĭ���£�Ĭ���գ�
			dlg.show();
			//��DatePickerDialog��ʾ����
			break;
		case R.id.map_back:
			Intent intent=new Intent();
			intent.setClass(MapShow.this, MileShow.class);
			startActivity(intent);
			finish();
			break;
		case R.id.map_confirm:
			//�洢���ں�ʱ��
			sp.putValue(MAP_DATE, date.getText().toString());
			sp.putValue(MAP_TIME, time.getText().toString());
			//���¶�����Ϣ
			String str=mapShop+","+mapTel+","+"0"+"#"+sp.getValue(LIST_STORE_INFO);
			sp.putValue(LIST_STORE_INFO, str);
			//�洢�û��͵绰
			app.updateUserInfo(orderName.getText().toString(), orderTel.getText().toString());
			Intent intent1=new Intent();
			intent1.setClass(MapShow.this, MileShow.class);
			startActivity(intent1);
			finish();
			break;
					
		}
	}
	
	public void onDateSet(DatePicker view, int year, int monthOfYear,int dayOfMonth){
		//DatePickerDialog �а�ťSet����ʱ�Զ�����
		
		date.setText(Integer.toString(year) + "-" +
		Integer.toString(monthOfYear+1) + "-" +
		Integer.toString(dayOfMonth));
		//����text
		}
	
	public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
		// Do something with the time chosen by the user
			time.setText(""+hourOfDay+":"+minute);
		}
	
	public void initTab()
	{
		myTabhost = this.getTabHost();  
        tabWidget = myTabhost.getTabWidget();     
        LayoutInflater.from(this).inflate(R.layout.mapshow,  
                myTabhost.getTabContentView(), true);  
        //myTabhost.setBackgroundColor(Color.argb(80, 80, 0, 0));  
        this.getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.maptitlebar);
        
        myTabhost.addTab(myTabhost.newTabSpec("One")// make a new Tab  
                .setIndicator("�ص�")  
                // set the Title  
                .setContent(R.id.widget_layout_place));  
        // set the layout  
  
        myTabhost.addTab(myTabhost  
                .newTabSpec("Two")  
                // make a new Tab  
                .setIndicator("ʱ��")  
                // set the Title   
                .setContent(R.id.widget_layout_time));  
        // set the layout  
  
        myTabhost.addTab(myTabhost  
                .newTabSpec("Three")  
                // make a new Tab  
                .setIndicator("�˻�")  
                // set the Title and Icon  
                .setContent(R.id.widget_layout_account)); 
        
        int count = tabWidget.getChildCount();//TabHost����һ��getTabWidget()�ķ���
        for (int i = 0; i < count; i++) {
           View view = tabWidget.getChildTabViewAt(i);   
           view.getLayoutParams().height = 80; //tabWidget.getChildAt(i)
           view.setBackgroundDrawable(getResources().getDrawable(  
                   R.drawable.tab_bg)); 
           //�����������ɫ
           final TextView tv = (TextView) view.findViewById(android.R.id.title);
             tv.setTextColor(this.getResources().getColorStateList(
             android.R.color.white));
        }
        //
        
        //���õ�һ��ΪĬ����
        getTabHost().setCurrentTab(0);
	}
	
	public void initAccount()
	{
		//String name="",tel="";
		orderName.setText(app.getUserAccount());
		orderTel.setText(app.getUserTel());

	}
	
	public void initDate()
	{
		date.setText(sp.getValue(MAP_DATE));
		time.setText(sp.getValue(MAP_TIME));
	}
	/*
	public void initPosition()
	{
		 	mLocationClient = new LocationClient(getApplicationContext()); 
	       // locData = new LocationData();
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
	    				//autoPostion.setChecked(false);
	    				String citystr=location.getAddrStr();
	    				sp.putValue(POS_LONGITUDE, location.getLongitude()+"");
		    			sp.putValue(POS_LATITUDE, location.getLatitude()+"");
		            	Toast.makeText(MapShow.this, citystr, Toast.LENGTH_LONG)
						.show();
		            	//��λ����ǰ����λ��
		            	LatLng ll = new LatLng(location.getLatitude(),
	    						location.getLongitude());
		    			MapStatusUpdate u = MapStatusUpdateFactory.newLatLng(ll);
	    				mBaiduMap.animateMapStatus(u);
	    			
	    				
	    			}
	    			
	            } 
	             
	            public void onReceivePoi(BDLocation location){ 
	                //return ; 
	            	String citystr=location.getCity()+location.getProvince()+location.getCityCode();
	            	Toast.makeText(MapShow.this, citystr, Toast.LENGTH_LONG)
					.show();
	            } 
	        }); 
	        mLocationClient.start();
	}
	*/
	public void findNearBy(String searchStr)
	{
		 //����λ������
        mPoiSearch = PoiSearch.newInstance();
        nearPos=new LatLng(Float.parseFloat(sp.getValue(POS_LATITUDE)),Float.parseFloat(sp.getValue(POS_LONGITUDE)));
		mPoiSearch.searchNearby((new PoiNearbySearchOption()).keyword(searchStr).location(nearPos).pageNum(10).radius(5000));
		//
        OnGetPoiSearchResultListener poiListener = new OnGetPoiSearchResultListener(){  
            public void onGetPoiResult(PoiResult result){  
            //��ȡPOI�������  
            	if (result == null
        				|| result.error == SearchResult.ERRORNO.RESULT_NOT_FOUND) {
        			return;
        		}
        		if (result.error == SearchResult.ERRORNO.NO_ERROR) {
        			mBaiduMap.clear();
        			PoiOverlay overlay = new MyPoiOverlay(mBaiduMap);
        			mBaiduMap.setOnMarkerClickListener(overlay);
        			//��ʾ��һ����ѯ���        		
    				for (PoiInfo poi : result.getAllPoi()) {  
    					placeName.setText(poi.name);
        				placeTel.setText(poi.address+"       "+poi.phoneNum);
        				break;
    			        //sb.append("���ƣ�").append(poiInfo.name).append("/n");  
    			        //sb.append("��ַ��").append(poiInfo.address).append("/n");  
    			        //sb.append("���ȣ�").append(poiInfo.pt.getLongitudeE6() / 1000000.0f).append("/n");  
    			        //sb.append("γ�ȣ�").append(poiInfo.pt.getLatitudeE6() / 1000000.0f).append("/n");  
    			
    				/*
    				PoiInfo poi=(PoiInfo)result.getAllPoi().get(0);
    				Toast.makeText(MapShow.this, ((PoiInfo)result.getAllPoi().get(0)).name+"", Toast.LENGTH_LONG)
					.show();
    				placeName.setText(poi.name);
    				placeTel.setText(poi.address+" "+poi.phoneNum);
    				*/
    			}
        			overlay.setData(result);
        			overlay.addToMap();
        			overlay.zoomToSpan();
        			return;
        		}
        		if (result.error == SearchResult.ERRORNO.AMBIGUOUS_KEYWORD) {

        			// ������ؼ����ڱ���û���ҵ����������������ҵ�ʱ�����ذ����ùؼ�����Ϣ�ĳ����б�
        			String strInfo = "��";
        			for (CityInfo cityInfo : result.getSuggestCityList()) {
        				strInfo += cityInfo.city;
        				strInfo += ",";
        			}
        			strInfo += "�ҵ����";
        			Toast.makeText(MapShow.this, strInfo, Toast.LENGTH_LONG)
        					.show();
        		}
            	//Toast.makeText(MapShow.this, result.getAllPoi().toString(), 5).show();
            }  
            public void onGetPoiDetailResult(PoiDetailResult result){  
            //��ȡPlace����ҳ�������  
            	if (result.error != SearchResult.ERRORNO.NO_ERROR) {
        			Toast.makeText(MapShow.this, "��Ǹ��δ�ҵ����", Toast.LENGTH_SHORT)
        					.show();
        		} else {
        			Toast.makeText(MapShow.this, "�ɹ����鿴����ҳ��", Toast.LENGTH_SHORT)
        					.show();
        		}
            	//Toast.makeText(MapShow.this, result.toString(), 5).show();
            }  
        }; 
        mPoiSearch.setOnGetPoiSearchResultListener(poiListener);
   
	}
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (resultCode) { //resultCodeΪ�ش��ı�ǣ�����B�лش�����RESULT_OK
         case RESULT_OK:
            Bundle b=data.getExtras();  //dataΪB�лش���Intent
             String str=b.getString("searchInfo");//str��Ϊ�ش���ֵ
             //maplayout.setVisibility(View.GONE);
             maplayout1.setVisibility(View.VISIBLE);
             findNearBy(str);
            
             /*Toast.makeText(MapShow.this,str, Toast.LENGTH_LONG)
				.show();
             */
                      break;
        default:
               break;
        }
    }
	
	public class MyLocationListenner implements BDLocationListener {

		@Override
		public void onReceiveLocation(BDLocation location) {
			// map view ���ٺ��ڴ����½��յ�λ��
			if (location == null || mMapView == null)
				return;
			MyLocationData locData = new MyLocationData.Builder()
					.accuracy(location.getRadius())
					// �˴����ÿ����߻�ȡ���ķ�����Ϣ��˳ʱ��0-360
					.direction(100).latitude(location.getLatitude())
					.longitude(location.getLongitude()).build();
			mBaiduMap.setMyLocationData(locData);
			//
			placeStart.setText(location.getAddrStr());
			if (isFirstLoc) {
				isFirstLoc = false;
			
				String citystr=location.getAddrStr();
				placeStart.setText(citystr);
				//
				sp.putValue(POS_LONGITUDE, location.getLongitude()+"");
    			sp.putValue(POS_LATITUDE, location.getLatitude()+"");
            	/*Toast.makeText(MapShow.this, location.getCity(), Toast.LENGTH_LONG)
				.show();
            	*/
				LatLng ll = new LatLng(location.getLatitude(),
						location.getLongitude());
				MapStatusUpdate u = MapStatusUpdateFactory.newLatLng(ll);
				mBaiduMap.animateMapStatus(u);
				nearPos=ll;
			}
		}

		public void onReceivePoi(BDLocation poiLocation) {
		}
	}
	//
	private class MyPoiOverlay extends PoiOverlay {

		public MyPoiOverlay(BaiduMap baiduMap) {
			super(baiduMap);
		}

		@Override
		public boolean onPoiClick(int index) {
			super.onPoiClick(index);
			PoiInfo poi = getPoiResult().getAllPoi().get(index);
			placeName.setText(poi.name);
			placeTel.setText(poi.address+"     "+poi.phoneNum);
			//
			placeDes.setText(poi.name);
			mapShop=poi.name;
			mapTel=poi.phoneNum;
			
			//info.setText(poi.address+","+poi.name+","+poi.phoneNum);
			//Toast.makeText(MapShow.this, "����"+index+"�Ҳݣ��ұ�����", Toast.LENGTH_LONG)
			//.show();
			/*if (poi.hasCaterDetails) {
				mPoiSearch.searchPoiDetail((new PoiDetailSearchOption())
						.poiUid(poi.uid));
			}
			*/
			return true;
		}
		
	}
	
	 @Override 
	    public void onDestroy(){ 
	        if (mLocationClient != null && mLocationClient.isStarted()){ 
	            mLocationClient.stop(); 
	            mLocationClient = null; 
	        } 
	        super.onDestroy(); 
	    } 
}
