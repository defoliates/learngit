// WordNetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecretInfo.h"
#include "WordNetDlg.h"
#include "afxdialogex.h"


// CWordNetDlg �Ի���

IMPLEMENT_DYNAMIC(CWordNetDlg, CDialogEx)

CWordNetDlg::CWordNetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWordNetDlg::IDD, pParent)
{

}

CWordNetDlg::~CWordNetDlg()
{
}

void CWordNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORDNET_LIST, m_grid);
}


BEGIN_MESSAGE_MAP(CWordNetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTADD, &CWordNetDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTUPDATE, &CWordNetDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_BUTDELETE, &CWordNetDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CWordNetDlg ��Ϣ�������

//������ܴ�
void CWordNetDlg::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����������ܴʶԻ���
	CEditDInfoDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		//��ȡ��ӵ����ܴʣ����������ݿ�
		try{
			UpdateData(TRUE);
			//��ȡ���ܴʣ����ж��Ƿ�Ϊ��
			CString m_name=dlg.GetSenseWord();	
			if(m_name.IsEmpty())
			{
				MessageBox(_T("���ܴʲ���Ϊ�գ�"),_T("�༭���ܴ�"),MB_ICONERROR);
				return;
			}
			
			//�������ݿ⣬��������ʾ
			ADOConn m_AdoConn;
			m_AdoConn.OnInitADOConn();
			_bstr_t sql;
			sql="select* from dict";
			_RecordsetPtr  m_pRecordset;
			m_pRecordset=m_AdoConn.GetRecordSet(sql);

			try{
				//�ж��Ƿ��Ѿ����ڣ������ڣ��򲻲��룬�������
				while(m_AdoConn.m_pRecordset->adoEOF==0)
				{
					CString strName=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("name"));
					if(strName.Trim()==m_name.Trim()){
						MessageBox(_T("���ܴ��Ѵ��ڣ�"),_T("�༭���ܴ�"),MB_ICONWARNING);
						m_AdoConn.ExitConnect();

						return;
					}
					m_pRecordset->MoveNext();
				}
				//����
				m_pRecordset->AddNew();
				m_pRecordset->put_Collect(_variant_t(_T("name")),_variant_t(m_name));
				m_pRecordset->Update();
				m_AdoConn.ExitConnect();
			}
			catch(...)
			{
				AfxMessageBox(_T("����ʧ��"));
				return;
			}
			AfxMessageBox(_T("��ӳɹ�"));

			m_grid.DeleteAllItems();//ɾ��ListContrl�ؼ��е�����
			AddToGrid();

		}catch(...)
		{
			MessageBox(_T("������ܴ�ʧ�ܣ�"),_T("�༭���ܴ�"),MB_ICONERROR);
		}
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
}

//�޸����ܴ�
void CWordNetDlg::OnBnClickedUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ж��Ƿ�����ѡ��
	POSITION pos=m_grid.GetFirstSelectedItemPosition();
	 if(pos==NULL)
	 {
		 MessageBox(_T("������ѡ��һ�"),_T("�༭���ܴ�"),MB_ICONERROR);
		 return;
	 }

	 //�õ��кţ�ͨ��POSITIONת��
	int nId=(int)m_grid.GetNextSelectedItem(pos);
	 //�õ����е����ݣ�0��ʾ��һ�У�ͬ��1,2,3...��ʾ�ڶ���������...�У�
	 CString name=m_grid.GetItemText(nId,1);

	//�����޸����ܴʶԻ���
	CEditDInfoDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		//��ȡ�޸ĵ����ܴʣ����������ݿ�
		try{
			UpdateData(TRUE);
			//��ȡ���ܴʣ����ж��Ƿ�Ϊ��
			CString m_name=dlg.GetSenseWord();	
			if(m_name.IsEmpty())
				MessageBox(_T("���ܴʲ���Ϊ�գ�"),_T("�༭���ܴ�"),MB_ICONERROR);
			
			//�������ݿ⣬��������ʾ
			ADOConn m_AdoConn;
			m_AdoConn.OnInitADOConn();
			_bstr_t sql;
			sql="select* from dict";
			_RecordsetPtr  m_pRecordset;
			m_pRecordset=m_AdoConn.GetRecordSet(sql);

			try{
				while(m_AdoConn.m_pRecordset->adoEOF==0)
				{
					CString strName=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("name"));
					if(strName.Trim()==name.Trim()){
						m_pRecordset->put_Collect(_variant_t(_T("name")),_variant_t(m_name));
						break;
					}
					m_pRecordset->MoveNext();
				}
				//
				m_pRecordset->Update();
				m_AdoConn.ExitConnect();
			}
			catch(...)
			{
				AfxMessageBox(_T("����ʧ��"));
				return;
			}
			AfxMessageBox(_T("�޸ĳɹ�"));
			//ɾ��ListContrl�ؼ��е�����
			m_grid.DeleteAllItems();
			AddToGrid();

		}catch(...)
		{
			MessageBox(_T("�޸����ܴ�ʧ�ܣ�"),_T("�༭���ܴ�"),MB_ICONERROR);
		}
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
}

//ɾ�����ܴ�
void CWordNetDlg::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ж��Ƿ�����ѡ��
	POSITION pos=m_grid.GetFirstSelectedItemPosition();
	 if(pos==NULL)
	 {
		 MessageBox(_T("������ѡ��һ�"),_T("�༭���ܴ�"),MB_ICONERROR);
		 return;
	 }

	 //�õ��кţ�ͨ��POSITIONת��
	int nId=(int)m_grid.GetNextSelectedItem(pos);
	 //�õ����е����ݣ�0��ʾ��һ�У�ͬ��1,2,3...��ʾ�ڶ���������...�У�
	 CString name=m_grid.GetItemText(nId,1);

	 try{
		  //ɾ�����ݿ��¼����������ʾ
			ADOConn m_AdoConn;
			m_AdoConn.OnInitADOConn();
			_bstr_t sql;
			sql="select* from dict";
			_RecordsetPtr  m_pRecordset;
			m_pRecordset=m_AdoConn.GetRecordSet(sql);
			//��������
			while(m_AdoConn.m_pRecordset->adoEOF==0)
			{
				CString strName=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("name"));
				if(strName.Trim()==name.Trim()){
					m_pRecordset->Delete(adAffectCurrent);
					break;
				}
				m_pRecordset->MoveNext();
			}
			//
			m_pRecordset->Update();
			m_AdoConn.ExitConnect();
		}
		catch(...)
		{
			MessageBox(_T("ɾ�����ܴ�ʧ�ܣ�"),_T("�༭���ܴ�"),MB_ICONERROR);
			return;
		}
		AfxMessageBox(_T("ɾ���ɹ�"));
		//ɾ��ListContrl�ؼ��е�����
		m_grid.DeleteAllItems();
		AddToGrid();
}

//�����ݰ󶨵�ListCtrl
void CWordNetDlg::AddToGrid()
{
	//�������ݿ�
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	CString sql;
	sql.Format(_T("select* from dict"));
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);
	//������
	int count=1;
	CString strCode;
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		m_grid.InsertItem(0,_T(""));
		strCode.Format(_T("%d"),count++);
		m_grid.SetItemText(0,0,strCode);
		m_grid.SetItemText(0,1,(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("name")));
		
		m_pRecordset->MoveNext();
	}
	m_AdoConn.ExitConnect();
}

//��ʼ���Ի���
BOOL CWordNetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��ListCtrl�ؼ�
	m_grid.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
	m_grid.InsertColumn(0,_T("���"),LVCFMT_LEFT,50,0);
	m_grid.InsertColumn(1,_T("���д�"),LVCFMT_LEFT,500,1);
	//������
	AddToGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//���ٶԻ��򣬽����ܴ�д���ı��ļ�
BOOL CWordNetDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	//�������ݿ�
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	CString sql;
	sql.Format(_T("select* from dict"));
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);

	//�򿪴ʿ��ļ�	
	CFile   file;
	file.Open(_T("pattern.txt"),CFile::modeCreate|CFile::modeWrite);
	WORD unicode = 0xFEFF;
	file.Write(&unicode,2);
	
	//���´ʿ��ļ�
	CString name,weight;
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		name=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("name"));
		//AfxMessageBox(name);
		name=name.Trim();
		file.Write(name,wcslen(name)*sizeof(wchar_t));
		file.Write(_T("\r\n"),4);	
		m_pRecordset->MoveNext();
	}
	m_AdoConn.ExitConnect();
	file.Flush();
	file.Close();
	return CDialogEx::DestroyWindow();
}
