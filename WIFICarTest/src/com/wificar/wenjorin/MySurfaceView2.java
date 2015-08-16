package com.wificar.wenjorin;

import java.net.Socket;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MySurfaceView2 extends SurfaceView implements SurfaceHolder.Callback
{
String Controlsite = "";
StreamEngine IOEngine;
private int RockerCircleR = 80;
private int RockerCircleX = 150;
private int RockerCircleY = 350;
private float SmallRockerCircleR = 50.0F;
private float SmallRockerCircleX = 150.0F;
private float SmallRockerCircleY = 350.0F;
String Videosite = "";
private Canvas canvas;
int cmdflag;
String down;
public boolean flag;
String left;
private Paint p;
private Paint paint;
int port;
String right;
String scrolldown;
String scrollup;
private SurfaceHolder sfh;
Socket socket2;
String stop;
String up;

public MySurfaceView2(Context paramContext, AttributeSet paramAttributeSet)
{
  super(paramContext, paramAttributeSet);
  setZOrderOnTop(true);
  getHolder().setFormat(-3);
  this.sfh = getHolder();
  this.sfh.addCallback(this);
  this.paint = new Paint();
  this.paint.setAntiAlias(true);
  setFocusable(true);
  setFocusableInTouchMode(true);
  setZOrderOnTop(true);
  this.getHolder().setFormat(PixelFormat.TRANSPARENT);
}
//�õ�����֮��Ļ���
public double getRad(float px1, float py1, float px2, float py2)
{
  float x = px2-px1;
  float y = py1-py2;
  float rad = (float)Math.acos(x / (float)Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2)));
  if (py2<py1)
    rad = -rad;
  return rad;
}
//��ȡԲ���˶������� 
public void getXY(float centerX, float centerY, float R, double rad)
{
  this.SmallRockerCircleX = (centerX+(float)(R* Math.cos(rad)));
  this.SmallRockerCircleY = (centerY+ (float)(R* Math.sin(rad)));
}

public boolean onTouchEvent(MotionEvent event)
{
  if ((event.getAction() == MotionEvent.ACTION_DOWN) || (event.getAction() == MotionEvent.ACTION_MOVE))
    {
	  //�����������ڻ��Χ��
	  if (Math.sqrt(Math.pow(this.RockerCircleX-(int)event.getX(), 2) + Math.pow(this.RockerCircleY - (int)event.getY(), 2)) >= this.RockerCircleR)
        {
		//�õ�ҡ���봥�������γɵĽǶ�
	      double tempRad = getRad(this.RockerCircleX, this.RockerCircleY, event.getX(),event.getY());
	    //��֤�ڲ�СԲ�˶��ĳ�������
	      getXY(this.RockerCircleX, this.RockerCircleY, this.RockerCircleR, tempRad);
	    }
	  else {//���С�����ĵ�С�ڻ�����������û��������ƶ�����   
          SmallRockerCircleX = (int) event.getX();   
          SmallRockerCircleY = (int) event.getY();   
	  		} 	   
      }   
  	else if (event.getAction() == MotionEvent.ACTION_UP) {   
      //���ͷŰ���ʱҡ��Ҫ�ָ�ҡ�˵�λ��Ϊ��ʼλ��   
      SmallRockerCircleX = 150;   
      SmallRockerCircleY = 350;  
      //���嶯��ֹͣ
      this.cmdflag = 0;      
    }
  
  if ((this.SmallRockerCircleY < this.RockerCircleY) && (this.SmallRockerCircleX < 175.0F) && (this.SmallRockerCircleX > 125.0F))
  {
	  //�������µĶ���
    this.cmdflag = 1;
    }
  
  if ((this.SmallRockerCircleY > this.RockerCircleY) && (this.SmallRockerCircleX < 175.0F) && (this.SmallRockerCircleX > 125.0F))
  {
	  //�������ϵĶ���
    this.cmdflag = 2;   
  }
  
  if ((this.SmallRockerCircleX < this.RockerCircleX) && (this.SmallRockerCircleY < 375.0F) && (this.SmallRockerCircleY > 325.0F))
  {
	  //��������Ķ���
    this.cmdflag = 3;    
  }
  
  if ((this.SmallRockerCircleX > this.RockerCircleX) && (this.SmallRockerCircleY < 375.0F) && (this.SmallRockerCircleY > 325.0F))
  {
	  //�������ҵĶ���
    this.cmdflag = 4;   
  }
  
  switch (this.cmdflag)
  {
  	  case 0:
  		  this.IOEngine.sendMsg(this.socket2, this.stop, 0);
  		  Log.v("��ʾ", "ͣ");
  		  break;
	  case 1:
		  this.IOEngine.sendMsg(this.socket2, this.down, 0);
	      Log.v("��ʾ", "��" + this.down);
	      break;
	  case 2:
		  this.IOEngine.sendMsg(this.socket2, this.up, 0);
	      Log.v("��ʾ", "ǰ" + this.up);
	      Log.v("��ʾ", "���Ƶ�ַ" + this.Controlsite);
	      Log.v("��ʾ", "�˿�" + this.port);
	      break;
	  case 3:
		  this.IOEngine.sendMsg(this.socket2, this.left, 0);
	      Log.v("��ʾ", "��" + this.left);
	      break;
	  case 4:	
		  this.IOEngine.sendMsg(this.socket2, this.right, 0);
	      Log.v("��ʾ", "��" + this.right);
	      break;
	  default:
		  this.IOEngine.sendMsg(this.socket2, this.stop, 0);
  }
 
  return true; 
}

public void setDown(String paramString)
{
  this.down = paramString;
}

public void setLeft(String paramString)
{
  this.left = paramString;
}

public void setPort(int paramInt)
{
  this.port = paramInt;
}

public void setRight(String paramString)
{
  this.right = paramString;
}

public void setSocket(Socket paramSocket)
{
  this.socket2 = paramSocket;
}

public void setStop(String paramString)
{
  this.Controlsite = paramString;
}

public void setUp(String paramString)
{
  this.up = paramString;
}

public void setVideosite(String paramString)
{
  this.Videosite = paramString;
}

public void surfaceChanged(SurfaceHolder paramSurfaceHolder, int paramInt1, int paramInt2, int paramInt3)
{
  this.flag = true;
  new DrawControl().start();
  Log.v("Himi", "surfaceChanged");
}

public void surfaceCreated(SurfaceHolder paramSurfaceHolder)
{
  this.IOEngine = new StreamEngine();
}

public void surfaceDestroyed(SurfaceHolder paramSurfaceHolder)
{
	flag=false;
	Log.v("Himi", "surfaceDestroyed");
}

class DrawControl extends Thread
{
  public DrawControl()
  {
  }

  public void draw() {   
      try {   
          canvas = sfh.lockCanvas();   
          canvas.drawColor(Color.BLACK);   
          //����͸����   
          paint.setColor(Color.BLACK);   
          //����ҡ�˱���   
          canvas.drawCircle(RockerCircleX, RockerCircleY, RockerCircleR, paint);   
          paint.setColor(Color.GREEN);   
          //����ҡ��   
          canvas.drawCircle(SmallRockerCircleX, SmallRockerCircleY,SmallRockerCircleR, paint);   
      } catch (Exception e) {   
          // TODO: handle exception   
      } finally {   
          try {   
              if (canvas != null)   
            	  sfh.unlockCanvasAndPost(canvas);   
          } catch (Exception e2) {   
          }   
      }   
  }  
  
  public void run()
  {
	  while (flag) {   
          draw();   
          try {   
              Thread.sleep(50);   
          } catch (Exception ex) {   
          }   
	  }
  }
}
}
