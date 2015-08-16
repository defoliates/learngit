package com.hurry.client;

import java.util.ArrayList;
import java.util.List;

import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.ChatManager;
import org.jivesoftware.smack.ChatManagerListener;
import org.jivesoftware.smack.MessageListener;
import org.jivesoftware.smack.XMPPException;
import org.jivesoftware.smack.packet.Message;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.hurry.client.util.MyApplication;
import com.hurry.client.util.TimeRender;
import com.hurry.client.util.XmppTool;

public class HurryTalkOne extends Activity{
	private final String FRIEND_ID="friend_id";
    private final String FRIEND_MESSAGE="friend_message";
    private final String BROADCAST_FRIEND_INFO="action.hurry.friendListInfo";
    
	private MyAdapter adapter;
	private List<Msg> listMsg = new ArrayList<Msg>();
	private String pUSERID;
	private String pCOUNTERPART;
	private EditText msgText;
	private ProgressBar pb;
	
	private MediaPlayer mediaPlayer;
	private MyApplication app;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.hurry_talkone);
		this.getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.title_bar);
		
		//��ȡApplication����,��ӻ���б���
		app=(MyApplication)getApplication();
		app.addActivity(this);
		
		//��ȡIntent���������û���
		this.pUSERID = getIntent().getStringExtra("USERID");
		this.pCOUNTERPART=getIntent().getStringExtra("COUNTERPART");
		//Toast.makeText(HurryTalkOne.this,pUSERID+pCOUNTERPART, Toast.LENGTH_SHORT).show();
		//��ʼ��listview
		ListView listview = (ListView) findViewById(R.id.client_listview);
		listview.setTranscriptMode(ListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
		this.adapter = new MyAdapter(this);
		listview.setAdapter(adapter);
		
		//��ȡ�ı���Ϣ
		this.msgText = (EditText) findViewById(R.id.client_text);
		this.pb = (ProgressBar) findViewById(R.id.client_pb);
		
		//ע��һ���㲥
		IntentFilter intentFilter = new IntentFilter();  
	      intentFilter.addAction(BROADCAST_FRIEND_INFO);  
	      registerReceiver(mRefreshBroadcastReceiver, intentFilter);
	    //������Ϣ��water-pc������water����ȡ�Լ��ķ��������ͺ��ѣ�
		final Chat newchat = XmppTool.getConnection().getChatManager().createChat(pCOUNTERPART+"@hnu-pc", null);
		/*
		//��Ϣ����
		ChatManager cm = XmppTool.getConnection().getChatManager();
		//������Ϣ��water-pc������water����ȡ�Լ��ķ��������ͺ��ѣ�
		final Chat newchat = cm.createChat(pCOUNTERPART+"@hnu-pc", null);
		
		cm.addChatListener(new ChatManagerListener() {
			@Override
			public void chatCreated(Chat chat, boolean able) 
			{
				chat.addMessageListener(new MessageListener() {
					@Override
					public void processMessage(Chat chat2, Message message)
					{
						Log.v("--tags--", "--tags-form--"+message.getFrom());
						Log.v("--tags--", "--tags-message--"+message.getBody());
						//�յ�����hnu-pc������hnu����Ϣ����ȡ�Լ��ķ��������ͺ��ѣ�
						if(message.getFrom().contains(pUSERID+"@hnu-pc"))
						{
							//��ȡ�û�����Ϣ��ʱ�䡢IN
							String[] args = new String[] { pUSERID, message.getBody(), TimeRender.getDate(), "IN" };
							
							//��handler��ȡ������ʾ��Ϣ
							android.os.Message msg = handler.obtainMessage();
							msg.what = 1;
							msg.obj = args;
							msg.sendToTarget();
						}
						else
						{
							//message.getFrom().cantatins(��ȡ�б��ϵ��û����飬������Ϣ);
							//��ȡ�û�����Ϣ��ʱ�䡢IN
							String[] args = new String[] { message.getFrom(), message.getBody(), TimeRender.getDate(), "IN" };
							
							//��handler��ȡ������ʾ��Ϣ
							android.os.Message msg = handler.obtainMessage();
							msg.what = 1;
							msg.obj = args;
							msg.sendToTarget();
						}
						
					}
				});
			}
		});
		*/
		//������Ϣ
		Button btsend = (Button) findViewById(R.id.client_btsend);
		btsend.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				//��ȡtext�ı�
				String msg = msgText.getText().toString();
				
				if(msg.length() > 0){
					//������Ϣ
					listMsg.add(new Msg(pUSERID, msg, TimeRender.getDate(), "OUT"));
					//ˢ��������
					adapter.notifyDataSetChanged();
					
					try {
						//������Ϣ��xiaowang
						newchat.sendMessage(msg);
					} 
					catch (XMPPException e)
					{
						e.printStackTrace();
					}
				}
				else
				{
					Toast.makeText(HurryTalkOne.this, "��������Ϣ", Toast.LENGTH_SHORT).show();
				}
				//���text
				msgText.setText("");
			}
		});
		//
		Button voice = (Button) findViewById(R.id.client_btattach);
		voice.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				//��ȡtext�ı�
				//mediaPlayer = MediaPlayer.create(HurryTalkOne.this,R.raw.chord);
				//mediaPlayer.start();
				Toast.makeText(HurryTalkOne.this, "Ŀǰ��û����¼���", Toast.LENGTH_SHORT).show();
			}
		});
		
	}
	
	private Handler handler = new Handler() {
		public void handleMessage(android.os.Message msg) 
		{
							
			switch (msg.what) {
			case 1:
				//��ȡ��Ϣ����ʾ
				String[] args = (String[]) msg.obj;
				listMsg.add(new Msg(args[0], args[1], args[2], args[3]));
				//ˢ��������
				adapter.notifyDataSetChanged();
				break;			
			case 2:
				//����������
				if(pb.getVisibility()==View.GONE){
					pb.setMax(100);
					pb.setProgress(1);
					pb.setVisibility(View.VISIBLE);
				}
				break;
			case 3:
				pb.setProgress(msg.arg1);
				break;
			case 4:
				pb.setVisibility(View.GONE);
				break;
			default:
				break;
			}
		};
	};
	 /*
	  * ������Ϣ��ʽ
	  */
	public class Msg {
		String userid;
		String msg;
		String date;
		String from;

		public Msg(String userid, String msg, String date, String from) {
			this.userid = userid;
			this.msg = msg;
			this.date = date;
			this.from = from;
		}
	}
	
	/*
	 * ���嵥�˶Ի�listview��������
	 */
	class MyAdapter extends BaseAdapter {

		private Context cxt;
		private LayoutInflater inflater;

		public MyAdapter(HurryTalkOne client) {
			this.cxt = client;
		}

		@Override
		public int getCount() {
			return listMsg.size();
		}

		@Override
		public Object getItem(int position) {
			return listMsg.get(position);
		}

		@Override
		public long getItemId(int position) {
			return position;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) 
		{
			//��ʾ��Ϣ�Ĳ��֣����ݡ��������û���ʱ��
			this.inflater = (LayoutInflater) this.cxt.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			
			//IN,OUT��ͼƬ
			if(listMsg.get(position).from.equals("IN"))
			{
				convertView = this.inflater.inflate(R.layout.one_chat_in, null);
			}
			else
			{
				convertView = this.inflater.inflate(R.layout.one_chat_out, null);
			}
			
			TextView useridView = (TextView) convertView.findViewById(R.id.client_row_userid);
			TextView dateView = (TextView) convertView.findViewById(R.id.client_row_date);
			TextView msgView = (TextView) convertView.findViewById(R.id.client_row_msg);
			
			useridView.setText(listMsg.get(position).userid);
			dateView.setText(listMsg.get(position).date);
			msgView.setText(listMsg.get(position).msg);
			
			return convertView;
		}
	}
	
	// �����Ƿ����ݿ����ݷ����仯��ˢ��ҳ��  
  private BroadcastReceiver mRefreshBroadcastReceiver = new BroadcastReceiver() {  
  
      @Override  
      public void onReceive(Context context, Intent intent) {  
          String action = intent.getAction();
          String userName=intent.getStringExtra(FRIEND_ID);
          String[] userId=userName.split("@");
          
          if (action.equals(BROADCAST_FRIEND_INFO))  
          {  
        	  //updateList(intent.getStringExtra(FRIEND_ID),intent.getStringExtra(FRIEND_MESSAGE));
        	  	Log.v("--tags--", "--tags-form--"+intent.getStringExtra(FRIEND_ID));
				Log.v("--tags--", "--tags-message--"+intent.getStringExtra(FRIEND_MESSAGE));
				//�յ�����hnu-pc������hnu����Ϣ����ȡ�Լ��ķ��������ͺ��ѣ�
				if(intent.getStringExtra(FRIEND_ID).contains(pUSERID+"@hnu-pc"))
				{
					//��ȡ�û�����Ϣ��ʱ�䡢IN
					String[] args = new String[] { pUSERID, intent.getStringExtra(FRIEND_MESSAGE), TimeRender.getDate(), "IN" };
					
					//��handler��ȡ������ʾ��Ϣ
					android.os.Message msg = handler.obtainMessage();
					msg.what = 1;
					msg.obj = args;
					msg.sendToTarget();
				}
				else
				{
					//message.getFrom().cantatins(��ȡ�б��ϵ��û����飬������Ϣ);
					//��ȡ�û�����Ϣ��ʱ�䡢IN
					String[] args = new String[] {userId[0], intent.getStringExtra(FRIEND_MESSAGE), TimeRender.getDate(), "IN" };
					
					//��handler��ȡ������ʾ��Ϣ
					android.os.Message msg = handler.obtainMessage();
					msg.what = 1;
					msg.obj = args;
					msg.sendToTarget();
				}
        	  //Toast.makeText(HurryTalkOne.this, intent.getStringExtra(FRIEND_ID)+","+intent.getStringExtra(FRIEND_MESSAGE) ,Toast.LENGTH_LONG).show();
          }  
      }  
  }; 
  
  @Override
 	protected void onDestroy() {
 	        super.onDestroy();
 	        unregisterReceiver(mRefreshBroadcastReceiver);
 	    }
}
