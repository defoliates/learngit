// WordNetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShemiXitong.h"
#include "WordNetDlg.h"
#include "afxdialogex.h"


// CWordNetDlg �Ի���

IMPLEMENT_DYNAMIC(CWordNetDlg, CDialogEx)

CWordNetDlg::CWordNetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWordNetDlg::IDD, pParent)
	, m_username(_T(""))
	, m_psw(_T(""))
{

}

CWordNetDlg::~CWordNetDlg()
{
}

void CWordNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, m_username);
	DDX_Text(pDX, IDC_PASSWORD, m_psw);
}


BEGIN_MESSAGE_MAP(CWordNetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWordNetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWordNetDlg ��Ϣ�������


void CWordNetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	//���±�����Ϣ
	UpdateData(TRUE);

	bool flag=false;
	//�ж��Ƿ�Ϊ��
	if(m_username.IsEmpty()||m_psw.IsEmpty())
	{
		AfxMessageBox(_T("�û����������벻��Ϊ�գ�"));
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
			//EndDialog(0);

			//�������ƽ���
			CModifyWordNet dlg;
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
		AfxMessageBox(_T("�û����������벻��ȷ��"));
	}
}

//���������ļ�
void CWordNetDlg::ProducePwdFile()
{
	CFile   file;  
	 CString str1,str2,str3,test;
	 str1=_T("admin");
	 str2=_T("root");
	 str3=_T("123456");

	 file.Open(_T("pwd.txt"),CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
	 md5.update(str1.GetBuffer(),str1.GetLength());
	test=md5.toString().c_str();
	file.Write(test.GetBuffer(test.GetLength()),test.GetLength()*sizeof(TCHAR)); 
	 file.Write(_T("\r\n"),2);
	 md5.reset();
	 //
	  md5.update(str2.GetBuffer(),str2.GetLength());
	test=md5.toString().c_str();
	file.Write(test.GetBuffer(test.GetLength()),test.GetLength()*sizeof(TCHAR)); 
	 file.Write(_T("\r\n"),2);
	 md5.reset();
	 //
	  md5.update(str3.GetBuffer(),str3.GetLength());
	test=md5.toString().c_str();
	file.Write(test.GetBuffer(test.GetLength()),test.GetLength()*sizeof(TCHAR)); 
	 file.Write(_T("\r\n"),2);
	 md5.reset();
	 //
	 file.Flush();
	 file.Close();
}