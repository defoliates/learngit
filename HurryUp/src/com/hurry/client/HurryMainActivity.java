package com.hurry.client;

import org.jivesoftware.smack.PacketListener;
import org.jivesoftware.smack.Roster;
import org.jivesoftware.smack.RosterEntry;
import org.jivesoftware.smack.packet.Message;
import org.jivesoftware.smack.packet.Packet;
import org.jivesoftware.smack.packet.Presence;
import org.jivesoftware.smack.sasl.SASLMechanism.AuthMechanism;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;

import com.hurry.client.util.MyApplication;
import com.hurry.client.util.XmppTool;

public class HurryMainActivity extends Activity{
	 private final String BROADCAST_ADD_FRIEND="action.hurry.addFriend";
	 private final String BROADCAST_MINUS_FRIEND="action.hurry.minusFriend";
	 
	private MyApplication app;
	//ΪUI��ӱ���
	private Button positionBtn;
	private TextView requestInfo;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.hurry_main);
		this.getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.title_bar);
		
		//��ȡApplication����,��ӻ���б���
		app=(MyApplication)getApplication();
		app.addActivity(this);
		//����service�����к�̨��Ϣ����
		Intent startIntent = new Intent(HurryMainActivity.this, MyService.class);  
        startService(startIntent);  

		//Ϊ�û������ʾ��Ϣ�Ӽ����¼�
		requestInfo=(TextView)this.findViewById(R.id.main_info);
		requestInfo.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				 //
			    Intent intent=new Intent();
				intent.setClass(HurryMainActivity.this, HurryPositionShare.class);
				startActivity(intent);
			}
        });
		//Ϊλ�ð�ť��Ӽ�����
		positionBtn=(Button)findViewById(R.id.main_position);
		positionBtn.setOnClickListener(new Button.OnClickListener(){
			public void onClick(View v) {
				 //
			    Intent intent=new Intent();
				intent.setClass(HurryMainActivity.this, HurryMapPosition.class);
				startActivity(intent);
			}
        });
		
		//ע��һ���㲥
		IntentFilter intentFilter = new IntentFilter();  
	      intentFilter.addAction(BROADCAST_ADD_FRIEND);
	      intentFilter.addAction(BROADCAST_MINUS_FRIEND);
	      registerReceiver(mRefreshBroadcastReceiver, intentFilter); 
	}
	/*
	String tag="";
	PacketListener myListener = new PacketListener() {      
        public void processPacket(Packet packet) {  
         if(packet instanceof Message) {
          Message message = (Message) packet;
          String type = message.getType().toString();
          if(type.equals("chat")) {
           //������Ϣ
	           if(message.getBody()!=null)
	           	{
	            //������Ϣ���ͻ������ݿ�
	               }
             }  
          }
  //       }
        else if(packet instanceof Presence) 
        {
	          Presence presence = (Presence) packet;
	          String type = presence.getType().toString();
	          boolean b= this.roster.contains(packet.getFrom());
	          Log.i(tag, b+"");
	 
	          Roster roster = connection.getRoster();
	      	  RosterEntry rosterEntry = roster.getEntry(presence.getFrom());         
	      	  String itemType="";
	       
		       if(rosterEntry!=null)
		       {
			        Log.e(tag, "PacketListener Presence: rosterEntry.getName()= "+rosterEntry.getName()+
			          " - rosterEntry.getUser()="+rosterEntry.getUser()+
			          " - rosterEntry.getStatus()="+rosterEntry.getStatus()+
			          "  -rosterEntry.getType()= "+rosterEntry.getType());
			        itemType=rosterEntry.getType().toString();
		       }
		       Log.e(tag, "itemType=="+itemType);
		       
	          if(type.equals("subscribe"))
		          {//���յ��Է��ļӺ��ѵ�����
		           //��Ӻ�������
		           FriendRelationParam param = new FriendRelationParam();
		           TalkCallback callback = new TalkCallback();
		           callback.from = getUId(presence.getFrom());
		           callback.to = getUId(presence.getTo());
		           try {
						   param.callbackStr = callback.toJsonObject().toString();
						  } catch (Exception e) {
						   e.printStackTrace();
						  }
		           param.userId = Long.parseLong(callback.to);
		           param.friendUid = Long.parseLong(callback.from);
		           
		           startFriendRelation(param);
	           
		           if("none".equals(itemType)){//�յ��Է��ļӺ��ѵ�����
		            Log.i(tag, "type="+type+"  ���յ��Է��ļӺ��ѵ�����");
		           	}
	           
		           if("to".equals(itemType)){//�ҼӶԷ����Ѻ�,�Է�ͬ�⣬���ҷ��صĽ�������
		            Log.i(tag, "type="+type+"  �ҼӶԷ����Ѻ�,�Է�ͬ�⣬���ҷ��صĽ�������");
		           	}                    
		          }
	          	else if("subscribed".equals(type))
	          	{//�Է�ͬ�����Ϊ����          	           
		           if("both".equals(itemType)){//˫����Ϊ���ѹ�ϵ����
		            Log.i(tag, "type="+type+"  ˫����Ϊ���ѹ�ϵ����!");
		           }
		           Log.e("TalkService"+Thread.currentThread().getName(), presence.getFrom()+"ͬ������["+packet.getTo()+"]�ļӺ������� ");
		          }
	          	else if(type.equals("unsubscribe")) {//�Է������˰���ɾ����||���߾ܾ�    //�ܾ���ʱ��ֻ������A
	           
		           if("none".equals(itemType)){//�ܾ�
		            Log.i(tag, "type="+type+"  �ұ��ܾ�!!!");
		           }
		           if("to".equals(itemType)){//���յ����Է�ɾ��
		            Log.i(tag, "type="+type+"  ���յ����Է�ɾ��");
		           }             
		          }else if(type.equals("unsubscribed")) {//�Է�����ɾ����   //ɾ����ʱ�� ����A,ͬʱ����һ����
		           
		           if("none".equals(itemType)){// �ұ�ɾ��  ,˫����ϵ���**************************
		            Log.i(tag, "type="+type+"  �ұ�ɾ����!!!");
		           }
		          }else if(type.equals("available")) {//�Է�������������
		//           presence = new Presence(Presence.Type.available);
		//           presence.setTo(presence.getFrom());
		//           connection.sendPacket(presence);
		          }
         		}
        else if(packet instanceof AuthMechanism) {
          
          
         }
         
        }      
    }; 
    */
	
	//�Ͽ����������ӣ���������activity
	protected void onDestroy() {  
        super.onDestroy();  
        XmppTool.closeConnection();
        //
        unregisterReceiver(mRefreshBroadcastReceiver);
        //�ر�service
        Intent stopIntent = new Intent(this, MyService.class);  
        stopService(stopIntent); 
        app.exit();
    }  
	
	// �����Ƿ����ݿ����ݷ����仯��ˢ��ҳ��  
	  private BroadcastReceiver mRefreshBroadcastReceiver = new BroadcastReceiver() {  
	  
	      @Override  
	      public void onReceive(Context context, Intent intent) {  
	          String action = intent.getAction();
	          //���µ�ǰҳ����ʾ����������
	        if(action.equals(BROADCAST_ADD_FRIEND)||action.equals(BROADCAST_MINUS_FRIEND))
	          {
	        	  //updateFriendApplication(intent.getStringExtra(FRIEND_ID),"yes");
	        	if(app.currentApplicationNum!=0)
	        		requestInfo.setText("Ŀǰ��"+app.currentApplicationNum+"������λ�ù���");
	        	else
	        		requestInfo.setText("Ŀǰû��������λ�ù���");
	          }
	      }  
	  };  
}
