#pragma once
#include "afxcmn.h"
#include <string>
#include <regex>
using namespace std;

// CSearchDlg �Ի���

class CSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSearchDlg();
	void SetSearchValue(CString value);//��������ֵ
	CString GetSearchValue();//��ȡ����ֵ
	void TraversFile(CString csPath);//�����ļ�
	void SearchFile();//��ʼ����
	bool GetEndSearchFlag();//��ȡ����������־
	void SetEndSearchFlag(bool flag);
// �Ի�������
	enum { IDD = IDD_SEARCH };
private:
	CString findStr;
	int count;
	bool stopFlag;//ֹͣ����
	bool searchEndFlag;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CProgressCtrl m_progress;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnMySearchMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndSearchMessage(WPARAM wParam, LPARAM lParam);
};
