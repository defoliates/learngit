package com.wificar.wenjorin;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class Welcome extends Activity
{
  private Button MbuttonControlConfig;
  private Button MbuttonStart;
  private Button MbuttonToConfig;

  public void onCreate(Bundle paramBundle)
  {
    super.onCreate(paramBundle);
    setContentView(R.layout.welcomelayout);
    this.MbuttonStart = ((Button)findViewById(R.id.control));
    this.MbuttonToConfig = ((Button)findViewById(R.id.network));
    this.MbuttonControlConfig = ((Button)findViewById(R.id.controlsetup));
    //��Ӽ���������ת����������ҳ��
    this.MbuttonStart.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View paramView)
      {
        Intent localIntent = new Intent();
        localIntent.setClass(Welcome.this, MainActivity.class);
        Welcome.this.startActivity(localIntent);
        Welcome.this.finish();
      }
    });
    //��Ӽ���������ת����������ҳ��
    this.MbuttonToConfig.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View paramView)
      {
        Intent localIntent = new Intent();
        localIntent.setClass(Welcome.this, Config.class);
        Welcome.this.startActivity(localIntent);
        Welcome.this.finish();
      }
    });
    //��Ӽ���������ת����������ҳ��
    this.MbuttonControlConfig.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View paramView)
      {
        Intent localIntent = new Intent();
        localIntent.setClass(Welcome.this, ControlConfig.class);
        Welcome.this.startActivity(localIntent);
        Welcome.this.finish();
      }
    });
  }
}

