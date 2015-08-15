// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecretInfo.h"
#include "Login.h"
#include "afxdialogex.h"


// CLogin �Ի���

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
	, m_username(_T(""))
	, m_psw(_T(""))
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, m_username);
	DDV_MaxChars(pDX, m_username, 255);
	DDX_Text(pDX, IDC_PWD, m_psw);
	DDV_MaxChars(pDX, m_psw, 255);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedLogin)
END_MESSAGE_MAP()


// CLogin ��Ϣ�������


void CLogin::OnBnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	bool flag=false;
	//�ж��Ƿ�Ϊ��
	if(m_username.IsEmpty()||m_psw.IsEmpty())
	{
		MessageBox(_T("�û����������벻��Ϊ�գ�"),_T("��½"),MB_ICONWARNING);
		return;
	}
	
	//���ַ�תΪMD5���ܵ���ʽ
	m_psw=m_psw.Trim();
	md5.update(m_psw.GetBuffer(),m_psw.GetLength());
	m_psw=md5.toString().c_str();
	md5.reset();
	//ProducePwdFile();

	//��֤�û����������Ƿ���ȷ
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	CString sql;
	sql.Format(_T("select* from t_user"));
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);
	//ȥ���ַ��Ŀհ��ַ�
	m_username=m_username.TrimLeft().TrimRight();
	m_psw=m_psw.TrimLeft().TrimRight();
	//AfxMessageBox(m_username+m_psw);
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		CString name,pwd;
		name=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("f_username"));
		pwd=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("f_pwd"));
		name=name.TrimLeft().TrimRight();
		pwd=pwd.TrimLeft().TrimRight();
		//AfxMessageBox(name+pwd);
		if((m_username.Trim()==name.Trim())&&(m_psw.Trim()==pwd.Trim()))
		{
			flag=true;

			break;
		}		
		m_pRecordset->MoveNext();
	}
	m_AdoConn.ExitConnect();

	//����û�����������ȷ
	if(flag)
	{
			//�رյ�ǰ�Ի���
			EndDialog(0);

			//�������ƽ���
			CWordNetDlg dlg;
			INT_PTR nResponse = dlg.DoModal();
			if (nResponse == IDOK)
			{
				// TODO: �ڴ˷��ô����ʱ��
				//  ��ȷ�������رնԻ���Ĵ���
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO: �ڴ˷��ô����ʱ��
				//  ��ȡ�������رնԻ���Ĵ���
			}
		
	}
	else
	{
		//��ʾ��������
		MessageBox(_T("�û����������벻��ȷ��"),_T("��½"),MB_ICONERROR);
	}
}


BOOL CLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//���ûس���Ϊ��½��ݼ�
	if(VK_RETURN == pMsg->wParam)
	{
		OnBnClickedLogin();

		return TRUE;	
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
