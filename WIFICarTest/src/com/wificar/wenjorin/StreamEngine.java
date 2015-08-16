package com.wificar.wenjorin;

import java.io.PrintWriter;
import java.net.Socket;

import android.util.Log;

public class StreamEngine
{
  public String HexString;

  private static String StrToBinstr(String paramString)
  {
	  //�����Զ����Ƶ���ʽ����
    char[] arrayOfChar = paramString.toCharArray();
    String str = "";
    for (int i = 0; ; i++)
    {
      if (i >= arrayOfChar.length)
        return str;
      str = str + Integer.toBinaryString(arrayOfChar[i]) + " ";
    }
  }

  public void sendMsg(Socket paramSocket, String paramString, int paramInt)
  {
    if (paramInt == 0)
    	{    	
    	HexString = StrToBinstr(paramString);
	        try
	        {
	          PrintWriter localPrintWriter1 = new PrintWriter(paramSocket.getOutputStream());
	          localPrintWriter1.println(this.HexString);
	          localPrintWriter1.flush();
	          Log.v("��ʾ", "���ͳɹ���" + paramString);
	        }
	        catch (Exception e)
	        {
	        	Log.v("��ʾ", "����ʧ�ܣ�");
	        }
    	}
    else
    {
      try
      {
        PrintWriter localPrintWriter2 = new PrintWriter(paramSocket.getOutputStream());
        localPrintWriter2.println(paramString);
        localPrintWriter2.flush();
        Log.v("��ʾ", "���ͳɹ���" + paramString);       
      }
      catch (Exception localException2)
      {
        Log.v("��ʾ", "����ʧ�ܣ�");       
      }
      
    }
  }
}
