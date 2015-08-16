package com.travel.map;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import com.travel.R;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

public class shangchuan extends Activity implements OnClickListener {

	// ��������ַ
	private static final String HOST = "http://192.168.23.10:8080/ImageServer/upServer";
	// ��ʾͼƬ
	private ImageView image;
	// ����but
	private Button take;
	private Button selete;
	// ��¼�ļ���
	private String filename;
	// �ϴ���bitmap
	private Bitmap upbitmap;
	private Button up;
	
	//���߳�ͨ��
	private Handler myHandler;
	private ProgressDialog myDialog;
	//
	File fileCheck;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.shangchuan);
		image = (ImageView) this.findViewById(R.id.shangchuanimageView1);
		take = (Button) this.findViewById(R.id.shangchuantake);
		selete = (Button) this.findViewById(R.id.shangchuanselete);
		up=(Button)this.findViewById(R.id.shangchuanup);
		take.setOnClickListener(this);
		selete.setOnClickListener(this);
		up.setOnClickListener(this);

		myHandler=new MyHandler();
	}

	public void onClick(View v) {
		Intent intent;
		switch (v.getId()) {
		case R.id.shangchuantake:
			intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
			filename = "xiaochun" + System.currentTimeMillis() + ".jpg";
			System.out.println(filename);
			// �������ָ������������պ����Ƭ�洢��·��
			intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(
					Environment.getExternalStorageDirectory(), filename)));
			//
			//File fileCheck = new File(Environment.getExternalStorageDirectory(), filename);
			startActivityForResult(intent, 1);
			break;
		case R.id.shangchuanselete:
			intent = new Intent(Intent.ACTION_PICK, null);
			intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
					"image/*");
			startActivityForResult(intent, 2);
			break;
		case R.id.shangchuanup:
			myDialog = ProgressDialog.show(this, "Loading...", "Please wait...", true, false);
			new Thread(new Runnable() {
				public void run() {
					upload();
					System.out.println("hello success up");
					myHandler.sendMessage(new Message());
				}
			}).start();
			break;
		default:
			break;
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		
		switch (requestCode) {
		case 1:
			//if(fileCheck.exists())
			//{
				//���bitmap,����ü�
				Bitmap bitmap=BitmapFactory.decodeFile(Environment.
						getExternalStorageDirectory().getPath()+"/"+filename);
				//���û�����գ�ֱ�ӷ��أ���������
				if(bitmap!=null)
				{
				float wight=bitmap.getWidth();
				float height=bitmap.getHeight();
//				ZoomBitmap.zoomImage(bitmap, wight/8, height/8);
				image.setImageBitmap(ZoomBitmap.zoomImage(bitmap, wight/8, height/8));
				upbitmap=ZoomBitmap.zoomImage(bitmap, wight/8, height/8);
				}
			//}			
			break;
		case 2:
			if(data!=null){
				image.setImageURI(data.getData());
				System.out.println(getAbsoluteImagePath(data.getData()));
				upbitmap=BitmapFactory.decodeFile(getAbsoluteImagePath(data.getData()));
				//��һ�£���ֹ���Ե�ʱ���ϴ����ļ�̫��
				upbitmap=ZoomBitmap.zoomImage(upbitmap, upbitmap.getWidth()/8, upbitmap.getHeight()/8);
			}
			break;
		default:
			break;
		}
	}

	// ȡ������·��
	protected String getAbsoluteImagePath(Uri uri) {
		// can post image
		String[] proj = { MediaStore.Images.Media.DATA };
		Cursor cursor = managedQuery(uri, proj, // Which columns to return
				null, // WHERE clause; which rows to return (all rows)
				null, // WHERE clause selection arguments (none)
				null); // Order-by clause (ascending by name)

		int column_index = cursor
				.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
		cursor.moveToFirst();
		return cursor.getString(column_index);
	}

	// �ϴ�
	public void upload() {
		ByteArrayOutputStream stream = new ByteArrayOutputStream();
		upbitmap.compress(Bitmap.CompressFormat.JPEG, 60, stream);
		byte[] b = stream.toByteArray();
		// ��ͼƬ�����ַ�����ʽ�洢����
		System.out.println("hello zhengzai shangchuan");
		String file = new String(Base64Coder.encodeLines(b));
		System.out.println("file name is "+file);
		HttpClient client = new DefaultHttpClient();
		// �����ϴ�����
		List<NameValuePair> formparams = new ArrayList<NameValuePair>();
		formparams.add(new BasicNameValuePair("file", file));
		HttpPost post = new HttpPost(HOST);
		UrlEncodedFormEntity entity;
		try {
			entity = new UrlEncodedFormEntity(formparams, "UTF-8");
			post.addHeader("Accept",
					"text/javascript, text/html, application/xml, text/xml");
			post.addHeader("Accept-Charset", "GBK,utf-8;q=0.7,*;q=0.3");
			post.addHeader("Accept-Encoding", "gzip,deflate,sdch");
			post.addHeader("Connection", "Keep-Alive");
			post.addHeader("Cache-Control", "no-cache");
			post.addHeader("Content-Type", "application/x-www-form-urlencoded");
			post.setEntity(entity);
			HttpResponse response = client.execute(post);
			System.out.println(response.getStatusLine().getStatusCode());
			HttpEntity e = response.getEntity();
			System.out.println(EntityUtils.toString(e));
			if (200 == response.getStatusLine().getStatusCode()) {
				System.out.println("�ϴ����");
			} else {
				System.out.println("�ϴ�ʧ��");
			}
			client.getConnectionManager().shutdown();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private class MyHandler extends Handler{
		@Override
		public void handleMessage(Message msg) {
			myDialog.dismiss();
		}
	}

}