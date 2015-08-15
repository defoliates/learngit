package com.test.pptcontrol;
/**
 * ����һ�����ݿ�����ɾ���ġ����������
 */

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;

import android.os.Bundle;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;


public class SQLSong extends Activity{
	//DBAdapter db;
	
	private Button SQLadd;
	private Button SQLmodify;
	private Button SQLdelete;
	private EditText singer;
	private EditText name;
	private EditText id;
	
	private Toast mToast;
	private MyApplication app;
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.song);	
		//���Ĵ�����ʽ
		getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.songbar);
		app=(MyApplication)getApplication();
		//
		mToast = Toast.makeText(this, "", Toast.LENGTH_LONG);	
		// ����������ť����ӡ����ĺ�ɾ��
		 SQLadd=(Button)findViewById(R.id.sql_add);
		 SQLmodify=(Button)findViewById(R.id.sql_modify);
		 SQLdelete=(Button)findViewById(R.id.sql_delete);
		 //��������EditText
		 singer=(EditText)findViewById(R.id.sql_singer);
		 name=(EditText)findViewById(R.id.sql_name);
		 id=(EditText)findViewById(R.id.sql_id);
		 //Ϊ��Ӱ�ť��Ӽ����¼�
		 SQLadd.setOnClickListener(new Button.OnClickListener(){

				@Override
				public void onClick(View v) {
					 String singerstr=singer.getText().toString(); 
					 String namestr=name.getText().toString(); 
					 //showTip(""+singerstr+","+namestr);
					 //String idstr=id.getText().toString();
					 if(app.insertSong(singerstr,namestr))
					 {
						 Toast.makeText(SQLSong.this, "��ӳɹ���" ,Toast.LENGTH_LONG).show();
						 //showTip("��ӳɹ���");
					 }
					 else
					 {
						 showTip("���ʧ�ܣ�");
					 }
					// �㲥֪ͨ
			         Intent intent = new Intent();
			         intent.setAction("action.refreshList");
			         sendBroadcast(intent);
				}
	        	
	        });
		//Ϊ���İ�ť��Ӽ����¼�
		 SQLmodify.setOnClickListener(new Button.OnClickListener(){

				@Override
				public void onClick(View v) {
					 String singerstr=singer.getText().toString(); 
					 String namestr=name.getText().toString(); 
					 String idstr=id.getText().toString();
					 if(idstr.equals(""))
					 {
						 Toast.makeText(SQLSong.this, "�б�λ��ֵ����Ϊ�գ����������룡" ,Toast.LENGTH_LONG).show();
						 //showTip("�б�λ��ֵ����Ϊ�գ����������룡");
					 }
					 else
					 {
						 idstr=""+app.getIndex(Integer.parseInt(idstr));
						 if(app.updataSong(idstr, singerstr, namestr))
						 {
							 Toast.makeText(SQLSong.this, "�޸ĳɹ���" ,Toast.LENGTH_LONG).show();
							 //showTip("�޸ĳɹ���");
						 }
						 else
						 {
							 showTip("�޸�ʧ�ܣ�");
						 }
					 }
					// �㲥֪ͨ
			         Intent intent = new Intent();
			         intent.setAction("action.refreshList");
			         sendBroadcast(intent);
				}
	        	
	        });
		//Ϊɾ����ť��Ӽ����¼�
		 SQLdelete.setOnClickListener(new Button.OnClickListener(){

				@Override
				public void onClick(View v) {
					 //String singerstr=singer.getText().toString(); 
					 //String namestr=name.getText().toString(); 
					 String idstr=id.getText().toString();
					 if(idstr.equals(""))
					 {
						 //showTip("�б�λ��ֵ����Ϊ�գ����������룡");
						 Toast.makeText(SQLSong.this, "�б�λ��ֵ����Ϊ�գ����������룡" ,Toast.LENGTH_LONG).show();
					 }
					 else
					 {
						 idstr=""+app.getIndex(Integer.parseInt(idstr));
						 if(app.deleteSong(idstr))
						 {
							 //showTip("ɾ���ɹ���");
							 Toast.makeText(SQLSong.this, "ɾ���ɹ���" ,Toast.LENGTH_LONG).show();
						 }
						 else
						 {
							 showTip("ɾ��ʧ�ܣ�");
						 }
					 }
					// �㲥֪ͨ
			         Intent intent = new Intent();
			         intent.setAction("action.refreshList");
			         sendBroadcast(intent);
				}
	        	
	        });
	}
	//������Ϣ
	private void showTip(String str)
	{
		if(TextUtils.isEmpty(str))
		{
			mToast.setText(str);
			mToast.show();
		}
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event) 
    {	
		if ( event.getKeyCode() == KeyEvent.KEYCODE_BACK){
			
		}
		return super.onKeyDown(keyCode, event);
	}
}
