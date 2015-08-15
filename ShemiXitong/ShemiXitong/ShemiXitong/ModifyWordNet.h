#pragma once
#include "afxcmn.h"
#include "ADOConn.h"
#include <locale.h>

// CModifyWordNet �Ի���

class CModifyWordNet : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyWordNet)

public:
	CModifyWordNet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyWordNet();
	void AddToGrid();
// �Ի�������
	enum { IDD = IDD_MODIFY };
private:
	int rowId;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_grid;
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_name;
	CString m_weight;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedDelete();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
