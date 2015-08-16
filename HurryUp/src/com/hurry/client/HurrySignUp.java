package com.hurry.client;

import org.jivesoftware.smack.PacketCollector;
import org.jivesoftware.smack.SmackConfiguration;
import org.jivesoftware.smack.filter.AndFilter;
import org.jivesoftware.smack.filter.PacketFilter;
import org.jivesoftware.smack.filter.PacketIDFilter;
import org.jivesoftware.smack.filter.PacketTypeFilter;
import org.jivesoftware.smack.packet.IQ;
import org.jivesoftware.smack.packet.Registration;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.hurry.client.util.MyApplication;
import com.hurry.client.util.XmppTool;


public class HurrySignUp extends Activity{
	private MyApplication app;
	private Handler mHandler;
	//�û�UI����
	private EditText account;
	private EditText psd;
	private EditText psdcomfirm;
	private Button signUpBtn;
	//
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.hurry_signup);
		this.getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.title_bar);
		
		//��ȡApplication����,��ӻ���б���
		app=(MyApplication)getApplication();
		app.addActivity(this);
		//
		new CustomThread().start();
		
		account=(EditText)findViewById(R.id.signAccount);
		psd=(EditText)findViewById(R.id.signPsd);
		psdcomfirm=(EditText)findViewById(R.id.signComfirPsd);
		signUpBtn=(Button)findViewById(R.id.signUpbtn);
		//
		signUpBtn.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				mHandler.obtainMessage(0, "��ע������").sendToTarget();
				/*
				final String accountStr=account.getText().toString();
				final String psdStr=psd.getText().toString();
				String psdcomStr=psdcomfirm.getText().toString();
				if(accountStr.equals("")||psdStr.equals("")||psdcomStr.equals("")||(!psdcomStr.equals(psdStr)))
				{
					Toast.makeText(HurrySignUp.this,"ע���������Ϣ����������ע�ᣡ", Toast.LENGTH_LONG).show(); 
				}
				else
					{
					Thread t=new Thread(new Runnable() {				
						public void run() {
							try {
								//
								String info=regist(accountStr,psdStr);
								if(info.equals("0"))
								{
									Toast.makeText(HurrySignUp.this,"������û����Ӧ��ע��ʧ�ܣ�", Toast.LENGTH_LONG).show(); 
								}
								else if(info.equals("1"))
								{
									Toast.makeText(HurrySignUp.this,"ע��ɹ���", Toast.LENGTH_LONG).show(); 
								}
								else if(info.equals("2"))
								{
									Toast.makeText(HurrySignUp.this,"����˺��Ѵ��ڣ���ֱ�ӵ�¼������ע�ᣡ", Toast.LENGTH_LONG).show(); 
								}
								else
								{
									Toast.makeText(HurrySignUp.this,"ע��ʧ�ܣ�������ע�ᣡ", Toast.LENGTH_LONG).show(); 
								}
								
							}
							catch (Exception e) 
							{
								XmppTool.closeConnection();
								
								e.printStackTrace();
							}	
						}
					});
					t.start();
					
				}*/
			}
        });
		
	}
	//ע�ắ����return 0��������û�з��ؽ�� 1��ע��ɹ� 2������˺��Ѿ�����3��ע��ʧ��
	public String regist(String account,String password) {  
	    
	    Registration reg = new Registration();  
	    reg.setType(IQ.Type.SET);  
	    reg.setTo(XmppTool.getConnection().getServiceName());  
	    reg.setUsername(account);// ע������createAccountע��ʱ��������username������jid���ǡ�@��ǰ��Ĳ��֡�  
	    reg.setPassword(password);  
	    reg.addAttribute("android", "geolo_createUser_android");// ���addAttribute����Ϊ�գ������������������־��android�ֻ������İɣ���������  
	    PacketFilter filter = new AndFilter(new PacketIDFilter(  
	            reg.getPacketID()), new PacketTypeFilter(IQ.class));  
	    PacketCollector collector = XmppTool.getConnection()  
	            .createPacketCollector(filter);  
	    XmppTool.getConnection().sendPacket(reg);  
	    IQ result = (IQ) collector.nextResult(SmackConfiguration  
	            .getPacketReplyTimeout());  
	    // ֹͣ��ѯ��� 
	    collector.cancel();// ֹͣ����results���Ƿ�ɹ��Ľ����  
	    if (result == null) {  
	        Log.e("RegistActivity", "No response from server.");  
	        return "0";  
	    } else if (result.getType() == IQ.Type.RESULT) {  
	        return "1";  
	    } else { // if (result.getType() == IQ.Type.ERROR)  
	        if (result.getError().toString().equalsIgnoreCase("conflict(409)")) {  
	            Log.e("RegistActivity", "IQ.Type.ERROR: "  
	                    + result.getError().toString());  
	            return "2";  
	        } else {  
	            Log.e("RegistActivity", "IQ.Type.ERROR: "  
	                    + result.getError().toString());  
	            return "3";  
	        }  
	    }  
	}
	//
	 class CustomThread extends Thread {
		 //
         String str="";
        @Override
        public void run() {
            //������Ϣѭ���Ĳ���
            Looper.prepare();//1����ʼ��Looper
            mHandler = new Handler(){//2����handler��CustomThreadʵ����Looper����
                public void handleMessage (Message msg) {//3�����崦����Ϣ�ķ���
                    Log.d("Test", "CustomThread receive msg:" + (String) msg.obj);                	                    
                    //��ȡ�û�ע����Ϣ
					String accountStr=account.getText().toString();
					String psdStr=psd.getText().toString();
					String psdcomStr=psdcomfirm.getText().toString();
					if(accountStr.equals("")||psdStr.equals("")||psdcomStr.equals("")||(!psdcomStr.equals(psdStr)))
					{
						//Toast.makeText(HurrySignUp.this,"ע���������Ϣ����������ע�ᣡ", Toast.LENGTH_LONG).show();
						str="ע���������Ϣ����������ע�ᣡ";
					}
					else
					{          					
						//
						String info=regist(accountStr,psdStr);
						if(info.equals("0"))
						{
							//Toast.makeText(HurrySignUp.this,"������û����Ӧ��ע��ʧ�ܣ�", Toast.LENGTH_LONG).show();
							str="������û����Ӧ��ע��ʧ�ܣ�";
						}
						else if(info.equals("1"))
						{
							//Toast.makeText(HurrySignUp.this,"ע��ɹ���", Toast.LENGTH_LONG).show();
							str="ע��ɹ���";
						}
						else if(info.equals("2"))
						{
							//Toast.makeText(HurrySignUp.this,"����˺��Ѵ��ڣ���ֱ�ӵ�¼������ע�ᣡ", Toast.LENGTH_LONG).show();
							str="����˺��Ѵ��ڣ���ֱ�ӵ�¼������ע�ᣡ";
						}
						else
						{
							//Toast.makeText(HurrySignUp.this,"ע��ʧ�ܣ�������ע�ᣡ", Toast.LENGTH_LONG).show();
							str="ע��ʧ�ܣ�������ע�ᣡ";
						}						
					}
					//
					HurrySignUp.this.runOnUiThread(new Runnable()    
			        {    
			            public void run()    
			            {    
			            	Toast.makeText(HurrySignUp.this,str, Toast.LENGTH_LONG).show();
			            }    
			    
			        }); 
                }

            };

            Looper.loop();//4��������Ϣѭ��

        }

    }
}
