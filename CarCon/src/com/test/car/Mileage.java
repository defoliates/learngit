package com.test.car;


import android.app.Activity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ProgressBar;

public class Mileage extends Activity implements OnTouchListener,OnGestureListener{
	private ImageButton setup;
	private Button order;
	private RoundProgressBar mileBar;
	private int progress=0;
	private GestureDetector gd;
	final static int FLING_MIN_DISTANCE = 100, FLING_MIN_VELOCITY = 200; 
	
	private SharedPreferencesHelper sp;
	private String currentMile;
	final static String TOTAL_MILE="totalMile";
	private MyApplication app;
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		app=(MyApplication)getApplication();
		app.addActivity(this);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.mileage);
		
		LinearLayout viewSnsLayout=(LinearLayout)findViewById(R.id.mileview);
		//��֤�ܹ����������¼�
		viewSnsLayout.setOnTouchListener(this);
		viewSnsLayout.setLongClickable(true);
		//����һ������ʶ�������
		gd = new GestureDetector((OnGestureListener)this);
		
		setup=(ImageButton)this.findViewById(R.id.mileage_setup);
		order=(Button)this.findViewById(R.id.orderBtn);
		mileBar=(RoundProgressBar)this.findViewById(R.id.mileBar);
		//����ProgressBar��ǰֵ
		sp=new SharedPreferencesHelper(this,"mile");
		//��ʼ�������Ϊ4000KM
		//sp.putValue(TOTAL_MILE,"4000");
		currentMile=new String("");
		currentMile=sp.getValue(TOTAL_MILE);
		if(currentMile.equals(""))
			currentMile="4000";
		//mileBar.setProgress(4000);
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				while(progress < Integer.parseInt(currentMile)){//
					progress += 5;
					//System.out.println(progress);					
					mileBar.setProgress(progress);							
					try {
						Thread.sleep(5);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
				
			}
		}).start();
		//
		order.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				Intent intent=new Intent();
				intent.setClass(Mileage.this, MapInfo.class);
				startActivity(intent);					
			}
        });
		//
		setup.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				Intent intent=new Intent();
				intent.setClass(Mileage.this, DataCorrect.class);
				startActivity(intent);					
			}
        });
		//
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
            Intent intent = new Intent(Mileage.this,DateInfo.class);  
            startActivity(intent);  
            overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
             //Toast.makeText(this, "��������", Toast.LENGTH_SHORT).show();   
  
        }  
        else
        if (e2.getX()-e1.getX() > FLING_MIN_DISTANCE && Math.abs(velocityX) >FLING_MIN_VELOCITY) {  
              
            //�л�Activity  
            Intent intent = new Intent(Mileage.this, Manager.class);  
            startActivity(intent);  
            //Toast.makeText(this, "��������", Toast.LENGTH_SHORT).show();  
        }  
        return false;  
    }
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
					app.exit();
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
