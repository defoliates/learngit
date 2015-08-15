
// ShemiXitongDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "HelpDlg.h"
#include "LookUpDlg.h"
#include "TaskDlg.h"
#include "WordNetDlg.h"

// CShemiXitongDlg �Ի���
class CShemiXitongDlg : public CDialogEx
{
// ����
public:
	CShemiXitongDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SHEMIXITONG_DIALOG };

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
	CTabCtrl m_tab;
	CHelpDlg m_helpDlg;
	CLookUpDlg m_lookUpDlg;
	CTaskDlg  m_taskDlg;
	CWordNetDlg  m_wordNetDlg;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};
