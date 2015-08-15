package com.test.car;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

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
import com.baidu.mapapi.search.poi.PoiDetailSearchOption;
import com.baidu.mapapi.search.poi.PoiNearbySearchOption;
import com.baidu.mapapi.search.poi.PoiResult;
import com.baidu.mapapi.search.poi.PoiSearch;



public class MapInfo extends Activity{

		// ��ͼ���
		private MapView mMapView = null; // ��ͼView
		BaiduMap  mBaiduMap;
		PoiSearch  mPoiSearch;
		private LocationClient mLocationClient = null;
		private PoiNearbySearchOption mNearbySearch;
		LatLng nearPos;
		//LocationData locData = null;
		boolean isFirstLoc = true;
		EditText search;
		ImageView startSearch;
		private Button mapOk;
		private CheckBox autoPostion;
		
		private SharedPreferencesHelper sp;
		public final static String POS_LONGITUDE="longitude";
		public final static String POS_LATITUDE="latitude";

		protected void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);
			MyApplication app=(MyApplication)getApplication();
			app.addActivity(this);
			this.requestWindowFeature(Window.FEATURE_NO_TITLE);
			//SDKInitializer.initialize(getApplicationContext());  
			setContentView(R.layout.mapinfo);

			search=(EditText)this.findViewById(R.id.map_search);
			startSearch=(ImageView)this.findViewById(R.id.start_search);
			autoPostion=(CheckBox)this.findViewById(R.id.map_autoGetPosition);
			//��ȡ�����������û���
			sp=new SharedPreferencesHelper(this,"positionInfo");
			
			// ��ͼ��ʼ��
			mMapView = (MapView) findViewById(R.id.bmapsView);
			mBaiduMap=mMapView.getMap();
			
			 mLocationClient = new LocationClient(this); 
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
	    				autoPostion.setChecked(false);
	    				String citystr=location.getAddrStr();
	    				sp.putValue(POS_LONGITUDE, location.getLongitude()+"");
		    			sp.putValue(POS_LATITUDE, location.getLatitude()+"");
		            	Toast.makeText(MapInfo.this, citystr, Toast.LENGTH_LONG)
						.show();
		            	//��λ����ǰ����λ��
		            	LatLng ll = new LatLng(location.getLatitude(),
	    						location.getLongitude());
		    			MapStatusUpdate u = MapStatusUpdateFactory.newLatLng(ll);
	    				mBaiduMap.animateMapStatus(u);
	    				/*
	    				//����Markerͼ��
	    		        BitmapDescriptor bitmap = BitmapDescriptorFactory
	    		            .fromResource(R.drawable.icon_marka);
	    		        //����MarkerOption�������ڵ�ͼ�����Marker
	    		        OverlayOptions option1 = new MarkerOptions()
	    		            .position(ll)
	    		            .icon(bitmap);
	    		        //�ڵ�ͼ�����Marker������ʾ
	    		        mBaiduMap.addOverlay(option1);
	    		        */
	    				
	    			}
	    			
	            } 
	             
	            public void onReceivePoi(BDLocation location){ 
	                //return ; 
	            	String citystr=location.getCity()+location.getProvince()+location.getCityCode();
	            	Toast.makeText(MapInfo.this, citystr, Toast.LENGTH_LONG)
					.show();
	            } 
	        }); 
	        mLocationClient.start();
	        //����λ������
	        mPoiSearch = PoiSearch.newInstance();
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
	        			Toast.makeText(MapInfo.this, strInfo, Toast.LENGTH_LONG)
	        					.show();
	        		}
	            	//Toast.makeText(MapInfo.this, result.getAllPoi().toString(), 5).show();
	            }  
	            public void onGetPoiDetailResult(PoiDetailResult result){  
	            //��ȡPlace����ҳ�������  
	            	if (result.error != SearchResult.ERRORNO.NO_ERROR) {
	        			Toast.makeText(MapInfo.this, "��Ǹ��δ�ҵ����", Toast.LENGTH_SHORT)
	        					.show();
	        		} else {
	        			Toast.makeText(MapInfo.this, "�ɹ����鿴����ҳ��", Toast.LENGTH_SHORT)
	        					.show();
	        		}
	            	//Toast.makeText(MapInfo.this, result.toString(), 5).show();
	            }  
	        }; 
	        mPoiSearch.setOnGetPoiSearchResultListener(poiListener);
	        
	        //Ϊ��ת��ť��Ӽ����¼�
	        mapOk=(Button)this.findViewById(R.id.map_ok);
	        mapOk.setOnClickListener(new Button.OnClickListener(){
				public void onClick(View v) {
					Intent intent=new Intent();
					intent.setClass(MapInfo.this, PlaceOrder.class);
					startActivity(intent);					
				}
	        });
	        //
	        startSearch.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					String searchStr=search.getText().toString();
					if(searchStr.equals(""))
					{
						Toast.makeText(MapInfo.this, "���������Ϊ�գ����������룡", Toast.LENGTH_LONG)
						.show();
					}
					else
					{
						nearPos=new LatLng(Float.parseFloat(sp.getValue(POS_LATITUDE)),Float.parseFloat(sp.getValue(POS_LONGITUDE)));
						mPoiSearch.searchNearby((new PoiNearbySearchOption()).keyword(searchStr).location(nearPos).pageNum(10).radius(5000));
					}
				}
				});
	        //
	        autoPostion.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					isFirstLoc=true;
					//autoPostion.setChecked(false);
				}
				});
	        
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
			super.onDestroy();
		}
			        
		private class MyPoiOverlay extends PoiOverlay {

			public MyPoiOverlay(BaiduMap baiduMap) {
				super(baiduMap);
			}

			@Override
			public boolean onPoiClick(int index) {
				super.onPoiClick(index);
				PoiInfo poi = getPoiResult().getAllPoi().get(index);
				//info.setText(poi.address+","+poi.name+","+poi.phoneNum);
				//Toast.makeText(MapInfo.this, "����"+index+"�Ҳݣ��ұ�����", Toast.LENGTH_LONG)
				//.show();
				if (poi.hasCaterDetails) {
					mPoiSearch.searchPoiDetail((new PoiDetailSearchOption())
							.poiUid(poi.uid));
				}
				return true;
			}
			
		}
}
