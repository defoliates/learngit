#pragma once
#include "afxcmn.h"


// CImageDetectDlg �Ի���

class CImageDetectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDetectDlg)

public:
	CImageDetectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageDetectDlg();
	CString GetExtName(CString fileName);//��ȡ��׺��
	int DetectSecretInfo();//����������
	int GetExtType(CString fileExtName);//��ȡ�ļ�����
	void PIC2TXT(CString filename);//��ͼƬʶ��ΪTXT
	void UpdateInfo(CString filename,int level);//���¼����Ϣ
	int GetDetectResult(CString filename);
	void Split(CString source, CString divKey, CStringArray& dest);
// �Ի�������
	enum { IDD = IDD_IMAGE };
private:
	int count;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_resultList;
	afx_msg void OnBnClickedImageAdd();
	afx_msg void OnBnClickedImageDelete();
	afx_msg void OnBnClickedDetect();
	virtual BOOL OnInitDialog();
};
