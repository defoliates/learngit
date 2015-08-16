#pragma once

#include "HistogramCtrl.h"
class CSerial;
// CShow �Ի���

class CShow : public CDialog
{
	DECLARE_DYNAMIC(CShow)

public:
	CShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShow();

// �Ի�������
	enum { IDD = IDD_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdittmperatrue();
	CString m_tempval;
	CString m_tempAlarm;
	afx_msg void OnEnChangeEditalarm();
private:
	CHistogramCtrl	m_ctrlHistogram;
	//int nRandom;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	BOOL OnInitDialog();
	void SetTemperature(float temperature);
public:
	CSerial *m_serialAlarm;
	afx_msg void OnEnKillfocusEditalarm();
	afx_msg void OnEnUpdateEditalarm();
};
