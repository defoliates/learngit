             
package com.travel.map;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.location.Location;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;


import com.baidu.mapapi.BMapManager;
import com.baidu.mapapi.GeoPoint;
import com.baidu.mapapi.LocationListener;
import com.baidu.mapapi.MKAddrInfo;
import com.baidu.mapapi.MKBusLineResult;
import com.baidu.mapapi.MKDrivingRouteResult;
import com.baidu.mapapi.MKLocationManager;
import com.baidu.mapapi.MKPoiResult;
import com.baidu.mapapi.MKSearch;
import com.baidu.mapapi.MKSearchListener;
import com.baidu.mapapi.MKSuggestionResult;
import com.baidu.mapapi.MKTransitRouteResult;
import com.baidu.mapapi.MKWalkingRouteResult;
import com.travel.R;

public class WeatherScreen extends Activity implements OnClickListener {
	BMapManager mBMapMan = null;
    LocationListener mLocationListener = null;
    MKSearch mSearch = null;
    String npCityId="";
    EditText dialogCity;
    String provinceName, cityName;
    boolean flag =true;
    ProgressDialog progressDialog;
    LinearLayout ll_yes,ll_no;
    TextView tv_city,tv_today,tv_attr1,tv_attr2,tv_attr3,tv_noresult;
    TextView tv_date1,tv_date2,tv_wd1,tv_wd2;              
    ImageView ima,ima1,ima2;
    Button btn_return,btn_other;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.weather_screen);
		initView();
		initBaiDuMap();
		
	}
	@Override
	protected void onPause() {
		mBMapMan.getLocationManager().removeUpdates(mLocationListener);
		mBMapMan.stop();
		super.onPause();
	}
	
	@Override
	protected void onResume() {
		mBMapMan.getLocationManager().requestLocationUpdates(mLocationListener);
		mBMapMan.getLocationManager().enableProvider(MKLocationManager.MK_GPS_PROVIDER);
		mBMapMan.start();
		super.onResume();
	}
	
	/**
	 * 
	 * ��������initBaiDuMap 
	 * ���ܣ���ʼ���ٶȵ�ͼ
	 * ������
	 * 
	 */
	private void initBaiDuMap(){
		mBMapMan = new BMapManager(getApplication());
        mBMapMan.init("14A97FC2DDF678193F61C19C0A20EA29C49DEF5C", null);
        mBMapMan.start();
        initMyLocation();
	}
	
	
	/**
	 * 
	 * ��������initMyLocation 
	 * ���ܣ�������λ
	 * ������
	 */
	private void initMyLocation(){
		progressDialog = ProgressDialog.show(this,null, "���ж�λ��...",true, true);
		mLocationListener = new LocationListener(){
			public void onLocationChanged(Location location) {
				if(location != null&& flag){
					progressDialog.dismiss();
					flag = false;
					GeoPoint myPt = new GeoPoint((int)(location.getLatitude()*1e6),
							(int)(location.getLongitude()*1e6));
					initMapSerach();
					//����ǰ����ת��Ϊ��ַ��ȡ��ǰ��������
					mSearch.reverseGeocode(myPt);
				}else{
				}
			}
        };
	}
	private void initMapSerach(){
		  // ��ʼ������ģ�飬ע���¼�����
      mSearch = new MKSearch();
      mSearch.init(mBMapMan, new MKSearchListener(){

			public void onGetPoiResult(MKPoiResult res, int type, int error) {
			    
			}
			public void onGetDrivingRouteResult(MKDrivingRouteResult res,
					int error) {
			}
			public void onGetTransitRouteResult(MKTransitRouteResult res,
					int error) {
			}
			public void onGetWalkingRouteResult(MKWalkingRouteResult res,
					int error) {
			}
			public void onGetAddrResult(MKAddrInfo res, int error) {
				if (error != 0 || res == null) {
				}else{
					String city = res.addressComponents.city;
					String pro = res.addressComponents.province;
					if(city!=null){
						provinceName = pro.substring(0, pro.length()-1);
						cityName =  city.substring(0, city.length()-1);
						progressDialog = ProgressDialog.show(WeatherScreen.this,null, "������ѯ��...",true, true);
						QueryAsyncTask asyncTask = new QueryAsyncTask();
						asyncTask.execute("");
					}else{
						Toast.makeText(WeatherScreen.this, "��λ������ǰ���У��޷���ѯ����", Toast.LENGTH_SHORT).show();
					}
				}
			}
			public void onGetBusDetailResult(MKBusLineResult arg0, int arg1) {
				
			}
			public void onGetRGCShareUrlResult(String arg0, int arg1) {
				// TODO Auto-generated method stub
				
			}
			public void onGetSuggestionResult(MKSuggestionResult arg0, int arg1) {
				// TODO Auto-generated method stub
				
			}
      });

	}
	
	/**
	 * 
	 * ��������initView 
	 * ���ܣ���ʼ���ؼ�
	 * ������
	 */
	private void initView(){
		 ll_yes = (LinearLayout)this.findViewById(R.id.ws2_ll_yes);
		 ll_no= (LinearLayout)this.findViewById(R.id.ws2_ll_no);
		 
		 tv_city= (TextView)this.findViewById(R.id.ws2_tv_city);
		 ima= (ImageView)this.findViewById(R.id.ws2_iv_image);
		 tv_attr1= (TextView)this.findViewById(R.id.ws2_tv_attr1);
		 tv_attr2= (TextView)this.findViewById(R.id.ws2_tv_attr2);
		 tv_attr3= (TextView)this.findViewById(R.id.ws2_tv_attr3);
		 
		 tv_noresult = (TextView)this.findViewById(R.id.ws2_tv_noresult);
		 
		 tv_date1= (TextView)this.findViewById(R.id.ws2_tv_1_date);
		 tv_date2= (TextView)this.findViewById(R.id.ws2_tv_2_date);
		 tv_wd1= (TextView)this.findViewById(R.id.ws2_tv_1_wd);
		 tv_wd2= (TextView)this.findViewById(R.id.ws2_tv_2_wd);
		 
		 ima1= (ImageView)this.findViewById(R.id.ws2_iv_1_image);
		 ima2= (ImageView)this.findViewById(R.id.ws2_iv_2_image);
		 
		 btn_return= (Button)this.findViewById(R.id.ws2_btn_return);
		 btn_return.setOnClickListener(this);
		 btn_other= (Button)this.findViewById(R.id.ws2_btn_submit);
		 btn_other.setOnClickListener(this);
	}
	
	private class QueryAsyncTask extends AsyncTask {
		@Override
		protected void onPostExecute(Object result) {
			progressDialog.dismiss();
			if(result!=null){
				String weatherResult = (String)result;
				if(weatherResult.split(";").length>1){
					String a  = weatherResult.split(";")[1];
					if(a.split("=").length>1){                               
						String b = a.split("=")[1];
						String c = b.substring(0,b.length()-1);
						String[] resultArr = c.split("\\}");
						if(resultArr.length>0){
								todayParse(resultArr[0]);
								tommrowParse(resultArr[1]);                           
								thirddayParse(resultArr[2]);
								ll_yes.setVisibility(View.VISIBLE);
								tv_city.setText(cityName);                             
						}
						
					}else{
						DataUtil.Alert(WeatherScreen.this,"����������Ϣ");
					}
				}else{
					DataUtil.Alert(WeatherScreen.this,"����������Ϣ");
				}
			}else{
				DataUtil.Alert(WeatherScreen.this,"����������Ϣ");
			}
			super.onPostExecute(result);			
		}
			
		@Override
		protected Object doInBackground(Object... params) {
			return HttpService.getWeather(cityName);
		}
	}

	/**
	 * 
	 * ��������todayParse 
	 * ���ܣ���������
	 * ������
	 * @param weather                      
	 */
	private void todayParse(String weather){
		String temp = weather.replace("'", "");
		String[] tempArr = temp.split(",");
		String wd="";
		String tq="";
		String fx="";
		if(tempArr.length>0){
			for(int i=0;i<tempArr.length;i++){
				if(tempArr[i].indexOf("t1:")!=-1){
					wd=tempArr[i].substring(3,tempArr[i].length())+"��";
				}else if(tempArr[i].indexOf("t2:")!=-1){
					wd=wd+"~"+tempArr[i].substring(3,tempArr[i].length())+"��";
				}else if(tempArr[i].indexOf("d1:")!=-1){
					fx=tempArr[i].substring(3,tempArr[i].length());
				}else if(tempArr[i].indexOf("s1:")!=-1){
					tq=tempArr[i].substring(4,tempArr[i].length());
				}
			}
			
			tv_attr1.setText("���£�"+wd);
			tv_attr2.setText("���������"+tq);
			tv_attr3.setText("����"+fx);
			ima.setImageResource(imageResoId(tq));
			
		}
	}
	
	/**
	 * 
	 * ��������tommrowParse 
	 * ���ܣ���������
	 * ������
	 * @param weather
	 */
	private void tommrowParse(String weather){
		String temp = weather.replace("'", "");
		String[] tempArr = temp.split(",");
		String wd="";
		String tq="";
		String fx="";
		if(tempArr.length>0){
			for(int i=0;i<tempArr.length;i++){
				if(tempArr[i].indexOf("t1:")!=-1){
					wd=tempArr[i].substring(3,tempArr[i].length())+"��";
				}else if(tempArr[i].indexOf("t2:")!=-1){
					wd=wd+"~"+tempArr[i].substring(3,tempArr[i].length())+"��";
				}else if(tempArr[i].indexOf("d1:")!=-1){
					fx=tempArr[i].substring(3,tempArr[i].length());
				}else if(tempArr[i].indexOf("s1:")!=-1){
					tq=tempArr[i].substring(4,tempArr[i].length());
				}
			}
			tv_date1.setText("����    "+tq);
			tv_wd1.setText(wd);
			ima1.setImageResource(imageResoId(tq));
			
			
		}
	}
	
	/**
	 * 
	 * ��������thirddayParse 
	 * ���ܣ���ȡ��������
	 * ������
	 * @param weather
	 */
	private void thirddayParse(String weather){
		String temp = weather.replace("'", "");
		String[] tempArr = temp.split(",");
		String wd="";
		String tq="";
		String fx="";
		if(tempArr.length>0){
			for(int i=0;i<tempArr.length;i++){
				if(tempArr[i].indexOf("t1:")!=-1){
					wd=tempArr[i].substring(3,tempArr[i].length())+"��";
				}else if(tempArr[i].indexOf("t2:")!=-1){
					wd=wd+"~"+tempArr[i].substring(3,tempArr[i].length())+"��";
				}else if(tempArr[i].indexOf("d1:")!=-1){
					fx=tempArr[i].substring(3,tempArr[i].length());
				}else if(tempArr[i].indexOf("s1:")!=-1){
					tq=tempArr[i].substring(4,tempArr[i].length());
				}
			}
			
			tv_date2.setText("����    "+tq);
			tv_wd2.setText(wd);
			ima2.setImageResource(imageResoId(tq));
			
		}
	}
	/**
	 * 
	 * ��������imageResoId 
	 * ���ܣ���ȡͼƬ
	 * ������
	 * @param weather
	 * @return
	 */
	private int imageResoId(String weather){
		int resoid=R.drawable.s_2;
		if(weather.indexOf("����")!=-1||weather.indexOf("��")!=-1){//����ת�磬������ͬ indexOf:�����ִ�
            resoid=R.drawable.s_1;}
        else if(weather.indexOf("����")!=-1&&weather.indexOf("��")!=-1){
            resoid=R.drawable.s_2;}
        else if(weather.indexOf("��")!=-1&&weather.indexOf("��")!=-1){
            resoid=R.drawable.s_3;}
        else if(weather.indexOf("��")!=-1&&weather.indexOf("��")!=-1){
            resoid=R.drawable.s_12;}
        else if(weather.indexOf("��")!=-1&&weather.indexOf("��")!=-1){
            resoid=R.drawable.s_12;}
        else if(weather.indexOf("��")!=-1){resoid=R.drawable.s_13;}
        else if(weather.indexOf("����")!=-1){resoid=R.drawable.s_2;}
        else if(weather.indexOf("����")!=-1){resoid=R.drawable.s_3;}
        else if(weather.indexOf("С��")!=-1){resoid=R.drawable.s_3;}
        else if(weather.indexOf("����")!=-1){resoid=R.drawable.s_4;}
        else if(weather.indexOf("����")!=-1){resoid=R.drawable.s_5;}
        else if(weather.indexOf("����")!=-1){resoid=R.drawable.s_5;}
        else if(weather.indexOf("����")!=-1){resoid=R.drawable.s_6;}
        else if(weather.indexOf("������")!=-1){resoid=R.drawable.s_7;}
        else if(weather.indexOf("Сѩ")!=-1){resoid=R.drawable.s_8;}
        else if(weather.indexOf("��ѩ")!=-1){resoid=R.drawable.s_9;}
        else if(weather.indexOf("��ѩ")!=-1){resoid=R.drawable.s_10;}
        else if(weather.indexOf("��ѩ")!=-1){resoid=R.drawable.s_10;}
        else if(weather.indexOf("��ɳ")!=-1){resoid=R.drawable.s_11;}
        else if(weather.indexOf("ɳ��")!=-1){resoid=R.drawable.s_11;}
        else if(weather.indexOf("��")!=-1){resoid=R.drawable.s_12;}
		return resoid;
	}
	public void onClick(View v) {
		switch(v.getId()){
		case R.id.ws2_btn_return:
			finish();
			break;
		case R.id.ws2_btn_submit:
			showOtherCity();
			break;
		}
	}
	
	/**
	 * 
	 * ��������showOtherCity 
	 * ���ܣ�����������������
	 * ������
	 */
	private void showOtherCity(){
		LayoutInflater inflater = getLayoutInflater();
		   View layout = inflater.inflate(R.layout.weather_other_city,(ViewGroup) findViewById(R.id.ws_dialog));
		    dialogCity = (EditText)layout.findViewById(R.id.ws_city_name);
		   new AlertDialog.Builder(this).setTitle("�������������").setView(layout)
		     .setPositiveButton("ȷ��",new DialogInterface.OnClickListener() {
		         public void onClick(DialogInterface dialog, int id) {
		        	 cityName=dialogCity.getText().toString();
		        	 
		        	 if(cityName!=null&&cityName.length()>0){
		        		 progressDialog = ProgressDialog.show(WeatherScreen.this,null, "������ѯ��...",true, true);
						 QueryAsyncTask asyncTask = new QueryAsyncTask();
						 asyncTask.execute("");
		        	 }
		           }
		       })
		     .setNegativeButton("ȡ��", new DialogInterface.OnClickListener() {
		         public void onClick(DialogInterface dialog, int id) {
		        	   dialog.cancel();
		           }
		       }).show();
	}
}
