#pragma once


// CEditDInfoDlg �Ի���

class CEditDInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDInfoDlg)

public:
	CEditDInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditDInfoDlg();
	CString GetSenseWord();
// �Ի�������
	enum { IDD = IDD_EDITINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_senseWord;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
