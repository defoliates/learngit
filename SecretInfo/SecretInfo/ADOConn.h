#ifndef _ADOCONN_H
#define _ADOCONN_H
#if !defined(AFX_ADOCONN_H__256F7B31_7156_4B81_B295_09375C61D2C2__INCLUDED_)
#define AFX_ADOCONN_H__256F7B31_7156_4B81_B295_09375C61D2C2__INCLUDED_

#import "C:\Program Files\Common Files\System\ado\msado15.dll"no_namespace\
    rename("EOF","adoEOF")rename("BOF","adoBOF")

#if _MSC_VER > 1000
#endif

#pragma once
/*
  ����һ�����ݿ�������
*/
class ADOConn
{
public:
	ADOConn(void);
	~ADOConn(void);
	void OnInitADOConn();//��ʼ������
	void ExitConnect();//�Ͽ�����

	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);//��ȡ��¼��
	BOOL ExecuteSQL(_bstr_t bstrSQL);//ִ��SQL����
 public:
	_ConnectionPtr m_pConnection;//����Դ���ӱ���
	_RecordsetPtr  m_pRecordset;//���ݼ�����
};

#endif

#endif