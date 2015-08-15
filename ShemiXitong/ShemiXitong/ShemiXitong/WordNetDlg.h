#pragma once
#include "ModifyWordNet.h"
#include "MD5.h"
#include "ADOConn.h"

// CWordNetDlg �Ի���

class CWordNetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWordNetDlg)

public:
	CWordNetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWordNetDlg();
	void ProducePwdFile();//�������ĺ���
// �Ի�������
	enum { IDD = IDD_WORDNET };
private:
	MD5 md5;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_username;
	CString m_psw;
	afx_msg void OnBnClickedOk();
};
