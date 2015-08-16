package com.travel.map;

import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.location.Location;
import android.os.Bundle;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.TextView;

import com.baidu.mapapi.GeoPoint;
import com.baidu.mapapi.LocationListener;
import com.baidu.mapapi.MKAddrInfo;
import com.baidu.mapapi.MKBusLineResult;
import com.baidu.mapapi.MKDrivingRouteResult;
import com.baidu.mapapi.MKPoiResult;
import com.baidu.mapapi.MKSearch;
import com.baidu.mapapi.MKSearchListener;
import com.baidu.mapapi.MKSuggestionResult;
import com.baidu.mapapi.MKTransitRouteResult;
import com.baidu.mapapi.MKWalkingRouteResult;
import com.baidu.mapapi.MapActivity;
import com.baidu.mapapi.MapController;
import com.baidu.mapapi.MapView;
import com.baidu.mapapi.Overlay;
import com.baidu.mapapi.Projection;
import com.travel.R;
import com.travel.util.MapApplication;

public class SeleOnMapActivity extends MapActivity implements LocationListener,
		OnClickListener {

	MapController mMapController;// �õ�mMapView�Ŀ���Ȩ,�����������ƺ�����ƽ�ƺ�,����SeleOnMapActivityΪ���ж��ǵ�ǰλ�û����ڵ�ͼ�����ѹ�ѡȡ

	GeoPoint geoPoint;

	MKSearch mSearch;

	RelativeLayout relative;

	Button back;

	Bitmap bitmap;

	GeoPoint gept;

	LocationListener mLocationListener;

	Projection projection;

	GeoPoint pickpt;

	View mPopView = null; //

	TextView popmsg;

	String startOrendFlag = "";

	SharedPreferences pickPointSharePre;

	SharedPreferences.Editor editor;

	PickupMapView mapview;

	String map = "";

	String poi = "";// ��������ѡȡ��ͼ�ϵĵ�ı�ʶ

	Intent i;

	int poiPickPointLat;

	int poiPickPointLon;

	String poiCity = null;

	String poiDistrict = null;

	String poiStreet = null;

	String poiCompleteAdd = null;

	int pickPointLat;

	int pickPointLon;

	String city = null;

	String district = null;

	String street = null;

	String completeAdd = null;

	@Override
	protected void onCreate(Bundle arg0) {
		// TODO Auto-generated method stub
		super.onCreate(arg0);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_chmap);
		findViews();
		MapApplication.getInstance().getMapManager().start();
		super.initMapActivity(MapApplication.getInstance().getMapManager());
		initMap();

	}

	public void findViews() {
		mapview = new PickupMapView(SeleOnMapActivity.this);
		projection = mapview.getProjection();
		relative = (RelativeLayout) findViewById(R.id.relative);
		relative.addView(mapview, new RelativeLayout.LayoutParams(
				LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		back = (Button) findViewById(R.id.back);

		back.setOnClickListener(this);

	}

	public void initMap() {
		bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.pin);
		mMapController = mapview.getController();
		mapview.setBuiltInZoomControls(true); // �����������õ����ſؼ�
		mapview.setDrawOverlayWhenZooming(true);
		mapview.setSatellite(false);// ��������ͼ
		mapview.setDragMode(0);// ����0����ק�ж���
		mMapController.setZoom(16); // ���õ�ͼzoom����

		mSearch = new MKSearch();
		mSearch.init(MapApplication.getInstance().getMapManager(),
				new MySearchListener());
		mLocationListener = new LocationListener() {

			public void onLocationChanged(Location loc) {
				if (loc != null) {
					geoPoint = new GeoPoint((int) (loc.getLatitude() * 1E6),
							(int) (loc.getLongitude() * 1E6));

					mMapController.animateTo(geoPoint);
					mMapController.setCenter(geoPoint); // ���õ�ͼ���ĵ�

					mapview.getOverlays().clear();

					addOverLay(geoPoint);

				}

			}
		};

		// ����
		mPopView = super.getLayoutInflater()
				.inflate(R.layout.pickmap_pop, null);
		mapview.addView(mPopView, new MapView.LayoutParams(
				LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, null,
				MapView.LayoutParams.CENTER));
		mPopView.setVisibility(View.GONE);
		pickPointSharePre = getSharedPreferences("pickPoint", 0);
		editor = pickPointSharePre.edit();

	}

	public class MySearchListener implements MKSearchListener {

		public void onGetAddrResult(MKAddrInfo info, int error) {
			if (error != 0) {
				String str = String.format("����ţ�%d", error);
				Log.e("error", str);
				return;
			}
			if (("").equals(poi)) {

				if (info.addressComponents.city == null) {
					city = "";
				} else {
					city = info.addressComponents.city;

				}
				if (info.addressComponents.district == null) {
					district = "";
				} else {
					district = info.addressComponents.district;

				}
				if (info.addressComponents.street == null) {
					street = "";
				} else {
					street = info.addressComponents.street;
				}
				pickPointLat = info.geoPt.getLatitudeE6();
				pickPointLon = info.geoPt.getLongitudeE6();
				completeAdd = city + district + street;

			} else if (("poiPick").equals(poi)) {
				poiPickPointLat = info.geoPt.getLatitudeE6();
				poiPickPointLon = info.geoPt.getLongitudeE6();
				if (info.addressComponents.city == null) {
					poiCity = "";
				} else {
					poiCity = info.addressComponents.city;

				}
				if (info.addressComponents.district == null) {
					poiDistrict = "";
				} else {
					poiDistrict = info.addressComponents.district;

				}
				if (info.addressComponents.street == null) {
					poiStreet = "";
				} else {
					poiStreet = info.addressComponents.street;
				}
				poiCompleteAdd = poiCity + poiDistrict + poiStreet;

			}

		}

		public void onGetBusDetailResult(MKBusLineResult arg0, int arg1) {
			// TODO Auto-generated method stub

		}

		public void onGetDrivingRouteResult(MKDrivingRouteResult arg0, int arg1) {
			// TODO Auto-generated method stub

		}

		public void onGetPoiResult(MKPoiResult arg0, int arg1, int arg2) {
			// TODO Auto-generated method stub

		}

		public void onGetRGCShareUrlResult(String arg0, int arg1) {
			// TODO Auto-generated method stub

		}

		public void onGetSuggestionResult(MKSuggestionResult arg0, int arg1) {
			// TODO Auto-generated method stub

		}

		public void onGetTransitRouteResult(MKTransitRouteResult result,
				int error) {
		}

		public void onGetWalkingRouteResult(MKWalkingRouteResult arg0, int arg1) {
			// TODO Auto-generated method stub

		}
	}

	@Override
	protected void onStart() {
		Bundle bundle = getIntent().getExtras();
		if (bundle.containsKey("flag")) {
			startOrendFlag = bundle.getString("flag");
		}
		if (bundle.containsKey("mapFlag")) {
			map = bundle.getString("mapFlag");
		}
		if (bundle.containsKey("poi")) {
			poi = bundle.getString("poi");
		}
		super.onStart();
	}

	@Override
	protected void onPause() {
		MapApplication.getInstance().getMapManager().getLocationManager()
				.removeUpdates(mLocationListener);
		MapApplication.getInstance().getMapManager().stop();
		super.onPause();
	}

	@Override
	protected void onResume() {
		MapApplication.getInstance().getMapManager().getLocationManager()
				.requestLocationUpdates(mLocationListener);
		MapApplication.getInstance().getMapManager().start();
		super.onResume();
	}

	void addOverLay(GeoPoint gept) {
		List<Overlay> overlays = mapview.getOverlays();
		MyOverlay overlay = new MyOverlay(mapview, bitmap);
		overlay.setGeoPoint(gept);
		overlays.add(overlay);// ����Զ���overlay
	}

	public class MyOverlay extends Overlay {
		private MapView mapView;
		private Projection projection;
		private Point point;// ��Ļ����ĵ�
		private Bitmap bitmap;

		private GeoPoint geoPoint;// ��γ�ȵ�

		public void setGeoPoint(GeoPoint geoPoint) {
			this.geoPoint = geoPoint;
		}

		public MyOverlay(MapView mapView, Bitmap bitmap) {
			super();
			this.mapView = mapView;
			this.bitmap = bitmap;
		}

		@Override
		public void draw(Canvas canvas, MapView arg1, boolean arg2) {
			projection = mapView.getProjection();
			point = new Point();
			projection.toPixels(geoPoint, point);// ��GeoPoint ת����point.
			int x = point.x - bitmap.getWidth() / 2;
			int y = point.y - bitmap.getHeight();

			canvas.drawBitmap(bitmap, x, y, new Paint());
		}

		@Override
		public boolean onTap(GeoPoint point, MapView arg1) {
			mSearch.reverseGeocode(point);
			mapview.updateViewLayout(mPopView, new MapView.LayoutParams(
					MapView.LayoutParams.WRAP_CONTENT,
					MapView.LayoutParams.WRAP_CONTENT, point,
					MapView.LayoutParams.BOTTOM_CENTER));
			mPopView.setVisibility(View.VISIBLE);
			popmsg = (TextView) mPopView.findViewById(R.id.popmsg);
			if (startOrendFlag.equals("start")) {
				 popmsg.setText(R.string.pick_start);
			} else if (startOrendFlag.equals("end")) {
				 popmsg.setText(R.string.pick_end);
			}
			if (poi.equals("poiPick")) {
				 popmsg.setText(R.string.pick_point);
			}

			popmsg.setOnClickListener(new OnClickListener() {

				public void onClick(View v) {
					if (poi.equals("poiPick")) {
						i = new Intent(SeleOnMapActivity.this,
								NearActivity.class);
						i.putExtra("poiLat", poiPickPointLat);
						i.putExtra("poiLon", poiPickPointLon);
						i.putExtra("poiCity", poiCity);
						i.putExtra("poiDistrict", poiDistrict);
						i.putExtra("poiStreet", poiStreet);
						i.putExtra("poiCompleteAdd", poiCompleteAdd);
						setResult(0, i);
						finish();
					} else {
						i = new Intent(SeleOnMapActivity.this,
								SearchWayActivity.class);
						i.putExtra("flag", "pick");
						i.putExtra("startOrendDialogFlag", startOrendFlag);
						i.putExtra("map", map);
						i.putExtra("lat", pickPointLat);
						i.putExtra("lon", pickPointLon);
						i.putExtra("completeAdd", completeAdd);
						i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
						startActivity(i);
					}

				}
			});

			return true;
		}
	}

	@Override
	protected boolean isRouteDisplayed() {
		// TODO Auto-generated method stub
		return false;
	}

	public void onLocationChanged(Location location) {
		location.getLatitude();
		location.getLongitude();

	}

	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.back: {
			finish();
			break;
		}

		default:
			break;
		}

	}

	class PickupMapView extends MapView {

		public PickupMapView(Context arg0, AttributeSet arg1, int arg2) {
			super(arg0, arg1, arg2);
			// TODO Auto-generated constructor stub
		}

		public PickupMapView(Context arg0, AttributeSet arg1) {
			super(arg0, arg1);
			// TODO Auto-generated constructor stub
		}

		public PickupMapView(Context arg0) {
			super(arg0);
			// TODO Auto-generated constructor stub
		}

		@Override
		public boolean onTouchEvent(MotionEvent event) {
			// �����Ļ�����λ��
			int x = (int) event.getX();
			int y = (int) event.getY();
			// ����������תΪ��ַ����
			pickpt = projection.fromPixels(x, y);
			mapview.getOverlays().clear();
			addOverLay(pickpt);
			return super.onTouchEvent(event);
		}

	}

}
