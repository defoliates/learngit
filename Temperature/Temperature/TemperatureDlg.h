// TemperatureDlg.h : ͷ�ļ�
//

#pragma once
#include "mscomm.h"
#include "afxcmn.h"
#include "Serial.h"
#include "Show.h"

// CTemperatureDlg �Ի���
class CTemperatureDlg : public CDialog
{
// ����
public:
	CTemperatureDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEMPERATURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//CMscomm m_mscomm;
	CTabCtrl m_tab;
	CShow m_show;
	CSerial m_serial;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	//CShow GetShowDlg();
};
