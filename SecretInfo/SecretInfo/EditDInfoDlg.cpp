// EditDInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecretInfo.h"
#include "EditDInfoDlg.h"
#include "afxdialogex.h"


// CEditDInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CEditDInfoDlg, CDialogEx)

CEditDInfoDlg::CEditDInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDInfoDlg::IDD, pParent)
	, m_senseWord(_T(""))
{

}

CEditDInfoDlg::~CEditDInfoDlg()
{
}

void CEditDInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SENSEWORD, m_senseWord);
	DDV_MaxChars(pDX, m_senseWord, 255);
}


BEGIN_MESSAGE_MAP(CEditDInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditDInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditDInfoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEditDInfoDlg ��Ϣ�������


void CEditDInfoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CEditDInfoDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
//��ȡ���ܴ�
CString CEditDInfoDlg::GetSenseWord()
{
	//UpdateData(TRUE);
	return m_senseWord;
}