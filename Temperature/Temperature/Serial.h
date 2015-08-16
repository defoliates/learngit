#pragma once
#include "afxwin.h"
#include "mscomm.h"
class CShow;
/*
���������໥����ʱ��ע�ⲻҪֱ���໥����ͷ�ļ��������ȡ���о�����ʽ������ᱨ��
//#include "Show.h" //������ķ�ʽ���  
class CShow;//ע�⣺ֻ�Ǹ��߱�����,��Ҫ�����,�������ܽṹ�ȶ�û 
*/
// CSerial �Ի���

class CSerial : public CDialog
{
	DECLARE_DYNAMIC(CSerial)

public:
	CSerial(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSerial();
	BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_SERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comPort;
	CComboBox m_comBaud;
	CComboBox m_comData;
	CComboBox m_comCheck;
	CComboBox m_comStop;
	CButton m_butOpen;
	CString m_revval;
	CString m_sendval;
	CString m_portval;
	CString m_baudval;
	CString m_dataval;
	CString m_stopval;
	CMscomm m_mscomm;
	bool isOpen;
	afx_msg void OnBnClickedButopen();
	afx_msg void OnBnClickedButsend();
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm();
public:
	void SendAlarm(CString str);
	
public:
	CShow* m_showData;
};
