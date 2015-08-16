package com.travel.map;

import com.travel.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class NearActivity extends Activity implements OnClickListener {
//NearActivity��Ϊ�ڵ���������Һ󵯳����ı�
	TextView select;

	TextView poi_in;

	TextView poi_poi;

	ImageButton pop;

	EditText word;

	ImageButton search;

	ListView poiList;
	// λ�����
	ArrayAdapter<String> adapter;
	// POI����������
	ArrayAdapter<String> adapterPoi;

	String selectMode;

	Intent i;

	int poiPickPointLat;

	int poiPickPointLon;

	String poiCity;

	String poiDistrict;

	String poiStreet;

	String poiCompleteAdd;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		// full screen
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		setContentView(R.layout.activity_poi);
		findViews();
	}

	public void findViews() {
		select = (TextView) findViewById(R.id.select);
		poi_in = (TextView) findViewById(R.id.poi_in);
		poi_poi = (TextView) findViewById(R.id.poi_poi);
		poi_in.setText(getResources().getString(R.string.poi_in));
		poi_poi.setText(getResources().getString(R.string.poi_poi));
		pop = (ImageButton) findViewById(R.id.popupwindow);
		word = (EditText) findViewById(R.id.word);
		word.setHint(R.string.poi_type_edit_tip);
		search = (ImageButton) findViewById(R.id.search);
		poiList = (ListView) findViewById(R.id.poiList);

		selectMode = getResources().getString(R.string.menu_my_loc);
		select.setText(selectMode);
		pop.setOnClickListener(this);

		String[] arr = { "��ݾƵ�", "�Ǽ��Ƶ�", "ҽԺ", "ѧУ", "����վ", "��ʳ", "����", "����",
				"ATM", "����վ", "KFC", "����", "��԰" };
		adapterPoi = new ArrayAdapter<String>(NearActivity.this,
				R.layout.poi_sortlist_item, R.id.item, arr);

		poiList.setAdapter(adapterPoi);

		poiList.setOnItemClickListener(listItemOnClick);

		search.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				if (("").equals(word.getText().toString())) {
					Toast.makeText(NearActivity.this, R.string.poi_type_tip,
							Toast.LENGTH_SHORT).show();
				} else {
					i = new Intent(NearActivity.this, DropDownListActivity.class);
					i.putExtra("type", word.getText().toString());
					i.putExtra("selectMode", selectMode);
					i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
					startActivity(i);
				}

			}
		});

	}

	OnItemClickListener listItemOnClick = new OnItemClickListener() {

		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
				long arg3) {
			i = new Intent(NearActivity.this, DropDownListActivity.class);
			if ((getResources().getString(R.string.menu_my_loc)
					.equals(selectMode))) {

			} else if (getResources().getString(R.string.poi_type_map).equals(
					selectMode)) {
				i.putExtra("poiLat", poiPickPointLat);
				i.putExtra("poiLon", poiPickPointLon);
				i.putExtra("poiCity", poiCity);
				i.putExtra("poiDistrict", poiDistrict);
				i.putExtra("poiStreet", poiStreet);
				i.putExtra("poiCompleteAdd", poiCompleteAdd);
			}
			i.putExtra("selectMode", selectMode);
			i.putExtra("type", adapterPoi.getItem(arg2).toString());
			i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
			startActivity(i);

		}
	};

	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == 0) {
			selectMode = getResources().getString(R.string.poi_type_map);
			select.setText(selectMode);
			Bundle bundle = data.getExtras();
			poiPickPointLat = bundle.getInt("poiLat");
			poiPickPointLon = bundle.getInt("poiLon");
			poiCity = bundle.getString("poiCity");
			poiDistrict = bundle.getString("poiDistrict");
			poiStreet = bundle.getString("poiStreet");
			poiCompleteAdd = bundle.getString("poiCompleteAdd");
		}

	}

	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.popupwindow: {
			openSelect();
			break;
		}

		default:
			break;
		}

	};

	//�ж��ڸ������ҵ�λ�ã��Ƿ�Ϊ��ǰλ�ú��ڵ�ͼ��ѡȡ��
	private void openSelect() {
		String[] items = { getResources().getString(R.string.menu_my_loc),
				getResources().getString(R.string.poi_type_map) };

		AlertDialog dialog = new AlertDialog.Builder(NearActivity.this)
				.setItems(items, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int position) {
						switch (position) {
						case 0: {

							selectMode = getResources().getString(
									R.string.menu_my_loc);
							select.setText(selectMode);
							break;
						}
						case 1: {
							selectMode = getResources().getString(
									R.string.poi_type_map);
							i = new Intent(NearActivity.this,
									SeleOnMapActivity.class);
							i.putExtra("poi", "poiPick");
							i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
							startActivityForResult(i, 0);

							break;							
						}

						default:
							break;
						}

					}
				}).create();
		dialog.show();
	}
}
