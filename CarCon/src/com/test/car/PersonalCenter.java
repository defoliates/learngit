package com.test.car;



import java.io.File;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class PersonalCenter extends Activity{
	private Button completeBtn,cameraCancel,cameraSelect,camraTake;
	private ImageView changeImg,image;
	private TextView back;
	//
	private Intent intent;
	// ��¼�ļ���
	private String filename;
	// �ϴ���bitmap
	private Bitmap upbitmap;
		
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		MyApplication app=(MyApplication)getApplication();
		app.addActivity(this);
		this.requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.personalcenter);
		this.getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.centertitlebar);
		
		completeBtn=(Button)this.findViewById(R.id.center_complete);
		completeBtn.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				Intent intent=new Intent();
				intent.setClass(PersonalCenter.this, MileShow.class);
				startActivity(intent);						
			}
        });
		//
		changeImg=(ImageView)this.findViewById(R.id.center_change_Img);
		changeImg.setOnClickListener(new OnClickListener(){
			public void onClick(View v) {
				showDialog(0);					
			}
        });
		//
		image=(ImageView)this.findViewById(R.id.center_Img);
		//
		back=(TextView)this.findViewById(R.id.center_back);
		back.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				Intent intent=new Intent();
				intent.setClass(PersonalCenter.this, MileShow.class);
				startActivity(intent);
				finish();
			}
        });
		/*
		LayoutInflater mInflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View view = mInflater.inflate(R.layout.camera, null);
		cameraCancel=(Button)view.findViewById(R.id.photo_cancel);
		cameraCancel.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				dismissDialog(0);					
			}
        });
		*/
	}
	
	/*
	 * ����������dialog
	 */
	protected Dialog onCreateDialog(int id) {
					
		 AlertDialog.Builder builder = new AlertDialog.Builder(this);
		 LayoutInflater inflater = getLayoutInflater();
		
	        View layout = inflater.inflate(R.layout.camera, null);
	       // builder.setTitle("�������");
	        builder.setView(layout);
	        //
	        cameraCancel=(Button)layout.findViewById(R.id.photo_cancel);
			cameraCancel.setOnClickListener(new Button.OnClickListener(){
				public void onClick(View v) {
					dismissDialog(0);					
				}
	        });
			cameraSelect=(Button)layout.findViewById(R.id.photo_select);
			cameraSelect.setOnClickListener(new Button.OnClickListener(){
				public void onClick(View v) {
					dismissDialog(0);
					//ѡ��ͼƬ
					intent = new Intent(Intent.ACTION_PICK, null);
					intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
							"image/*");
					startActivityForResult(intent, 2);
				}
	        });
			//
			camraTake=(Button)layout.findViewById(R.id.photo_take);
			camraTake.setOnClickListener(new Button.OnClickListener(){
				public void onClick(View v) {
					dismissDialog(0);
					//����
					intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
					filename = "xiaochun" + System.currentTimeMillis() + ".jpg";
					System.out.println(filename);
					// �������ָ������������պ����Ƭ�洢��·��
					intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(
							Environment.getExternalStorageDirectory(), filename)));
					//
					//File fileCheck = new File(Environment.getExternalStorageDirectory(), filename);
					startActivityForResult(intent, 1);
				}
	        });
		   /* builder.setItems(R.array.camera, new DialogInterface.OnClickListener() {
		               public void onClick(DialogInterface dialog, int which) {
		               // The 'which' argument contains the index position
		               // of the selected item
		            switch(which)
		            {
		            case 0:
		            	Toast.makeText(PersonalCenter.this, "0" ,Toast.LENGTH_LONG).show();
		            	break;
		            case 1:
		            	Toast.makeText(PersonalCenter.this, "1" ,Toast.LENGTH_LONG).show();
		            	break;
		            case 2:
		            	Toast.makeText(PersonalCenter.this, "2" ,Toast.LENGTH_LONG).show();
		            	break;
		            	}
		           }
		    });
		    */
		    return builder.create();
		    
		/*AlertDialog.Builder builder =new AlertDialog.Builder(this);
        AlertDialog customDialog;
        
        LayoutInflater inflater = getLayoutInflater();
        View layout = inflater.inflate(R.layout.camera, null);
       // builder.setTitle("�������");
        builder.setView(layout);
        builder.setPositiveButton("���", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface arg0, int arg1) {
            	//���ͺ�����ӵ������
            	
            }
        });
        builder.setNegativeButton("ȡ��", new DialogInterface.OnClickListener() {
            
            @Override
            public void onClick(DialogInterface arg0, int arg1) {
                finish();
            }
        });
        customDialog = builder.create();
        
        return customDialog;
        */
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
				image.invalidate();
				//upbitmap=ZoomBitmap.zoomImage(bitmap, wight/8, height/8);
				}
			//}			
			break;
		case 2:
			if(data!=null){
				image.setImageURI(data.getData());
				image.invalidate();
				//System.out.println(getAbsoluteImagePath(data.getData()));
				//upbitmap=BitmapFactory.decodeFile(getAbsoluteImagePath(data.getData()));
				//��һ�£���ֹ���Ե�ʱ���ϴ����ļ�̫��
				//upbitmap=ZoomBitmap.zoomImage(upbitmap, upbitmap.getWidth()/8, upbitmap.getHeight()/8);
			}
			break;
		default:
			break;
		}
	}

}
