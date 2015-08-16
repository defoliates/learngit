package com.hurry.client;

import org.jivesoftware.smack.XMPPException;
import org.jivesoftware.smack.packet.Presence;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.os.Handler;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.hurry.client.util.MyApplication;
import com.hurry.client.util.SharedPreferencesHelper;
import com.hurry.client.util.XmppTool;


public class HurryLogin extends Activity implements OnClickListener{
	//����UI����
	private EditText useridText, pwdText;
	private CheckBox saveInfo;
	private LinearLayout layout1, layout2;
	//����ÿ�ε�¼����Ϣ
	private SharedPreferencesHelper sp;
	public final static String USER_ACCOUNT="account";
	public final static String USER_PSD="password";
	//����ע���ַ�
	private String s="No access? Sign Up";
	private TextView mClickableText;
	//
	private MyApplication app;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.hurry_login);
		this.getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.title_bar);
		//��ȡApplication����,��ӻ���б���
		app=(MyApplication)getApplication();
		app.addActivity(this);
		
		//����Ƿ������Ѿ�����
		checkNetwork();
		//��ȡ�û�������
		this.useridText = (EditText) findViewById(R.id.login_edit_account);
		this.pwdText = (EditText) findViewById(R.id.login_edit_pwd);
		saveInfo=(CheckBox)findViewById(R.id.login_cb_savepwd);
		//��ȡ�����������û���
		sp=new SharedPreferencesHelper(this,"userInfo");
		useridText.setText(sp.getValue(USER_ACCOUNT));
		pwdText.setText(sp.getValue(USER_PSD));				
		
		//���ڵ�¼
		this.layout1 = (LinearLayout) findViewById(R.id.login_layout1);
		//��¼����
		this.layout2 = (LinearLayout) findViewById(R.id.login_layout2);
		//Ϊ��½��ť��Ӽ����¼�
		Button btLogin = (Button) findViewById(R.id.login_btn_login);
		btLogin.setOnClickListener(this);
		//Ϊ��ע�ᡱ�ı���Ӽ����¼�
		mClickableText=(TextView)this.findViewById(R.id.login_signUp);
		mClickableText.setText(getClickableSpan());
		mClickableText.setMovementMethod(LinkMovementMethod.getInstance());
	}
	
	@Override
	public void onClick(View v) {
		//����ID�������ύ����ȡ��
		switch (v.getId()) {
		case R.id.login_btn_login:
			//ȡ��������û�������
			final String USERID = this.useridText.getText().toString();
			final String PWD = this.pwdText.getText().toString();
			//���浱ǰ��¼���û���������
			if(saveInfo.isChecked())
			{
				sp.putValue(USER_ACCOUNT, USERID);
				sp.putValue(USER_PSD, PWD);
				Log.i("login", "sharepreference����ɹ�");
			}
			
			Thread t=new Thread(new Runnable() {				
				public void run() {
					//sendEmptyMessage:����һ����Ϣ
					handler.sendEmptyMessage(1);
					try {
						//����
						XmppTool.getConnection().login(USERID, PWD);
						Log.i("XMPPClient", "Logged in as " + XmppTool.getConnection().getUser());
						
						//״̬
						Presence presence = new Presence(Presence.Type.available);
						XmppTool.getConnection().sendPacket(presence);
						//���浱ǰ�û���IDֵ��application
						app.setUserID(USERID);
						//��ת����ҳ��
						Intent intent = new Intent();
						intent.setClass(HurryLogin.this, HurryMainActivity.class);
						intent.putExtra("USERID", USERID);						
						HurryLogin.this.startActivity(intent);
						HurryLogin.this.finish();
					}
					catch (XMPPException e) 
					{
						XmppTool.closeConnection();
						
						handler.sendEmptyMessage(2);
						app.exit();
					}					
				}
			});
			t.start();
			break;
		}
	}
	
	private Handler handler = new Handler(){
		public void handleMessage(android.os.Message msg)
		{
			
			if(msg.what==1)
			{
				layout1.setVisibility(View.VISIBLE);
				layout2.setVisibility(View.GONE);
			}
			else if(msg.what==2)
			{
				layout1.setVisibility(View.GONE);
				layout2.setVisibility(View.VISIBLE);
				Toast.makeText(HurryLogin.this, "��¼ʧ�ܣ�",Toast.LENGTH_SHORT).show();
			}
		};
	};
	//����ע���ַ������ת�¼�
	 private SpannableString getClickableSpan() {
		  View.OnClickListener l = new View.OnClickListener() {
		   //���¶����Լ��Ķ���
		   public void onClick(View v) {
		    //
		    Intent intent=new Intent();
			intent.setClass(HurryLogin.this, HurrySignUp.class);
			startActivity(intent);
		   }
		  };
	  // s=s.replaceAll("Sign Up", "<a  style=\"color:#ff0000;\"  href='Sign Up'>"  
		//	    + "Sign Up" + "</a>");
	  SpannableString spanableInfo = new SpannableString(s);
	  int start = 11;
	  int end = spanableInfo.length();
	  spanableInfo.setSpan(new Clickable(l), start, end,
	    Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
	  return spanableInfo;
	 }
	 //��������
	 class Clickable extends ClickableSpan implements OnClickListener {
		 private final View.OnClickListener mListener;

		 public Clickable(View.OnClickListener l) {
		  mListener = l;
		 }
		 public void onClick(View v) {
			  mListener.onClick(v);
			 }
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
						XmppTool.closeConnection();
						//System.exit(0);
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
		
		/** �ж��Ƿ������� */
		private void checkNetwork() {
			boolean flag = false;
			ConnectivityManager cwjManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
			if (cwjManager.getActiveNetworkInfo() != null)
				flag = cwjManager.getActiveNetworkInfo().isAvailable();
			if (!flag) {
				Builder b = new AlertDialog.Builder(this).setTitle("û�п��õ�����").setMessage("�뿪��GPRS��WIFI��������");
				b.setPositiveButton("ȷ��", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int whichButton) {
						Intent intent=null;
		                //�ж��ֻ�ϵͳ�İ汾  ��API����10 ����3.0�����ϰ汾 
		                if(android.os.Build.VERSION.SDK_INT>10){
		                    intent = new Intent(android.provider.Settings.ACTION_WIRELESS_SETTINGS);
		                }else{
		                    intent = new Intent();
		                    ComponentName component = new ComponentName("com.hurry.client","com.hurry.client.WirelessSettings");
		                    intent.setComponent(component);
		                    intent.setAction("android.intent.action.VIEW");
		                }
		                startActivity(intent);
		            }
				}).setNeutralButton("ȡ��", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int whichButton) {
						dialog.cancel();
					}
				}).create();
				b.show();
			}

			//return flag;
		}
	 /*
	 protected void onDestroy() {  
	        super.onDestroy();  
	        XmppTool.closeConnection();
	        app.exit();
	    }  
	 */
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.hurry_login, menu);
		return true;
	}
	 @Override
	    public boolean onMenuItemSelected(int featureId, MenuItem item) {
	        switch (item.getItemId()) {
	        case com.hurry.client.R.id.about:
	            Toast.makeText(getApplicationContext(), "������һ���ͼ��λ�ֻ�����app���������£������ţ����ӣ����Ž�����������ʹ�ã����پ�׼��λ�Է�λ�ã����Է���ɣ���ʹʹ��˫��������Ϊ�ȴ������ĺͽ��ǡ�", Toast.LENGTH_SHORT).show();
	            break;  
	    }
	        return false;
	 }
}
