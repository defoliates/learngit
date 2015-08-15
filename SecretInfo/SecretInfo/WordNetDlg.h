#pragma once
#include "afxcmn.h"
#include "ADOConn.h"
#include "EditDInfoDlg.h"
// CWordNetDlg �Ի���

class CWordNetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWordNetDlg)

public:
	CWordNetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWordNetDlg();
	void AddToGrid();//�����ݰ󶨵�ListCtrl
// �Ի�������
	enum { IDD = IDD_WORDNET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedDelete();
	CListCtrl m_grid;
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
