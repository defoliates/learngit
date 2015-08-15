// ModifyWordNet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShemiXitong.h"
#include "ModifyWordNet.h"
#include "afxdialogex.h"


// CModifyWordNet �Ի���

IMPLEMENT_DYNAMIC(CModifyWordNet, CDialogEx)

CModifyWordNet::CModifyWordNet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyWordNet::IDD, pParent)
	, m_name(_T(""))
	, m_weight(_T(""))
{

}

CModifyWordNet::~CModifyWordNet()
{
}

void CModifyWordNet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_grid);
	DDX_Text(pDX, IDC_SENTWORD, m_name);
	DDX_Text(pDX, IDC_WGT, m_weight);
}


BEGIN_MESSAGE_MAP(CModifyWordNet, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CModifyWordNet::OnNMClickList)
	ON_BN_CLICKED(IDC_BUTADD, &CModifyWordNet::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTUPDATE, &CModifyWordNet::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_BUTDELETE, &CModifyWordNet::OnBnClickedDelete)
END_MESSAGE_MAP()


// CModifyWordNet ��Ϣ�������


void CModifyWordNet::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CString str;
	 int nId;
	 //���ȵõ������λ��
	POSITION pos=m_grid.GetFirstSelectedItemPosition();
	 if(pos==NULL)
	 {
		 AfxMessageBox(_T("������ѡ��һ��"));
	 return;
	 }
	 //�õ��кţ�ͨ��POSITIONת��
	nId=(int)m_grid.GetNextSelectedItem(pos);
	 //�õ����е����ݣ�0��ʾ��һ�У�ͬ��1,2,3...��ʾ�ڶ���������...�У�
	 rowId=_ttoi(m_grid.GetItemText(nId,0));
	 m_name=m_grid.GetItemText(nId,1);
	 m_weight=m_grid.GetItemText(nId,2);

	 str.Format(_T("%d %s %s"),rowId,m_name,m_weight);
	// AfxMessageBox(str);
	 //ˢ�¿ؼ�ֵ
	 UpdateData(FALSE); 
}


void CModifyWordNet::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_name.IsEmpty())
	{
		AfxMessageBox(_T("���дʲ���Ϊ��"));
		return;
	}
	if(m_weight.IsEmpty())
	{
		AfxMessageBox(_T("Ȩֵ����Ϊ��"));
		return;
	}
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	_bstr_t sql;
	sql="select* from dict";
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet(sql);

	try{
		m_pRecordset->AddNew();
		m_pRecordset->put_Collect(_variant_t(_T("name")),_variant_t(m_name));
		m_pRecordset->put_Collect(_variant_t(_T("weight")),_variant_t(m_weight));
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
}


void CModifyWordNet::OnBnClickedUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_name.IsEmpty())
	{
		AfxMessageBox(_T("���дʲ���Ϊ��"));
		return;
	}
	if(m_weight.IsEmpty())
	{
		AfxMessageBox(_T("Ȩֵ����Ϊ��"));
		return;
	}
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	_bstr_t sql;
	sql="select* from dict";
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet(sql);
	//
	int curPos;
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		int pos;
		//pos== m_pRecordset->GetFields()->GetItem((long)0).Value;
		pos=_ttoi((_bstr_t)m_pRecordset->GetCollect(_T("id")));
		if(rowId==pos){
			/*
			CString str;
			str.Format(_T("%d %d"),rowId,m_pRecordset->CursorLocation);
			AfxMessageBox(str);*/
			curPos=m_pRecordset->CursorLocation-2;
			break;
		}
		m_pRecordset->MoveNext();
	}

	try{
		m_pRecordset->MoveFirst();//�ѹ���ƶ�����ʼλ��
		m_pRecordset->Move((long)curPos,vtMissing);//�ƶ���굽Ŀ��λ��
		m_pRecordset->put_Collect(_variant_t(_T("name")),_variant_t(m_name));
		m_pRecordset->put_Collect(_variant_t(_T("weight")),_variant_t(m_weight));
		m_pRecordset->Update();
		m_AdoConn.ExitConnect();
	}
	catch(...)
	{
		AfxMessageBox(_T("����ʧ��"));
		return;
	}
	AfxMessageBox(_T("�޸ĳɹ�"));
	m_grid.DeleteAllItems();//ɾ��ListContrl�ؼ��е�����
	AddToGrid();
}


void CModifyWordNet::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	_bstr_t sql;
	sql="select* from dict";
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet(sql);

	//
	int curPos;
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		int pos;
		//pos== m_pRecordset->GetFields()->GetItem((long)0).Value;
		pos=_ttoi((_bstr_t)m_pRecordset->GetCollect(_T("id")));
		if(rowId==pos){
			m_pRecordset->Delete(adAffectCurrent);
			m_pRecordset->Update();
			m_AdoConn.ExitConnect();
			break;
		}
		m_pRecordset->MoveNext();
	}
	
	AfxMessageBox(_T("ɾ���ɹ�"));
	m_grid.DeleteAllItems();//ɾ��ListContrl�ؼ��е�����
	AddToGrid();
}


BOOL CModifyWordNet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_grid.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
	m_grid.InsertColumn(0,_T(""),LVCFMT_LEFT,0,0);
	m_grid.InsertColumn(1,_T("���д�"),LVCFMT_LEFT,100,1);
	m_grid.InsertColumn(2,_T("Ȩֵ"),LVCFMT_LEFT,100,2);
	
	rowId=0;
	AddToGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CModifyWordNet::AddToGrid()
{
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	CString sql;
	sql.Format(_T("select* from dict"));
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		m_grid.InsertItem(0,_T(""));
		m_grid.SetItemText(0,0,(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("id")));
		m_grid.SetItemText(0,1,(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("name")));
		m_grid.SetItemText(0,2,(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("weight")));
		
		m_pRecordset->MoveNext();
	}
	m_AdoConn.ExitConnect();
}

BOOL CModifyWordNet::DestroyWindow()
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
	/*FILE *fp=fopen("pattern.txt","w+");
	if(fp!=NULL)
	{
		char *temp="\n";
		CString name,weight;
		int i=0;
		while(m_AdoConn.m_pRecordset->adoEOF==0)
		{
			name=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("name"));
			name.Trim();//ȥ���հ��ַ�
			//����ת��
			USES_CONVERSION;
			char* buf = T2A(name);
			//����д��س�
			if (i==0)
				i++;
			else
				fwrite(temp,1,1,fp);;
			fwrite(buf,sizeof(buf),strlen(buf)/sizeof(buf),fp);
			

			m_pRecordset->MoveNext();
		}
		fclose(fp);
	}*/
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
		//weight=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("weight"));
		file.Write(_T("\r\n"),4);	
		m_pRecordset->MoveNext();
	}
	m_AdoConn.ExitConnect();
	file.Flush();
	file.Close();
	//
	return CDialogEx::DestroyWindow();
}
