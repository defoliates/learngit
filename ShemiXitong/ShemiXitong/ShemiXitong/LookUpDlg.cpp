// LookUpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShemiXitong.h"
#include "LookUpDlg.h"
#include "afxdialogex.h"


// CLookUpDlg �Ի���

IMPLEMENT_DYNAMIC(CLookUpDlg, CDialogEx)

CLookUpDlg::CLookUpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLookUpDlg::IDD, pParent)
{

}

CLookUpDlg::~CLookUpDlg()
{
}

void CLookUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, m_secretLevel);
	DDX_Control(pDX, IDC_LIST, m_queryResult);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dateStop);
}


BEGIN_MESSAGE_MAP(CLookUpDlg, CDialogEx)
	ON_BN_CLICKED(IDC_QUERY, &CLookUpDlg::OnBnClickedQuery)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CLookUpDlg::OnNMRClickList)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_32771, &CLookUpDlg::OnMenuItemDelete)
	//ON_COMMAND(ID_MENUITEM, &CLookUpDlg::OnMenuItemTest)
END_MESSAGE_MAP()


// CLookUpDlg ��Ϣ�������


BOOL CLookUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//Ϊ���ڲ�����Combox��ֵ
	m_secretLevel.InsertString(0,_T("Level-0"));
	m_secretLevel.InsertString(1,_T("Level-1"));
	m_secretLevel.InsertString(2,_T("Level-2"));
	m_secretLevel.InsertString(3,_T("Level-3"));
	m_secretLevel.InsertString(4,_T("Level-4"));
	m_secretLevel.InsertString(5,_T("Level-5"));
	m_secretLevel.InsertString(6,_T("ALL"));
	m_secretLevel.SetCurSel(6);
	//
	m_queryResult.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
	m_queryResult.InsertColumn(0,_T("���"),LVCFMT_LEFT,50,0);
	m_queryResult.InsertColumn(1,_T("�ļ���"),LVCFMT_LEFT,100,0);
	m_queryResult.InsertColumn(2,_T("���ܵȼ�"),LVCFMT_LEFT,100,0);
	m_queryResult.InsertColumn(3,_T("�������"),LVCFMT_LEFT,100,0);
	//	
	UpdateGridInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLookUpDlg::AddToGrid(CString sql)
{
	//�������ݿ�
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);
	//ɾ��ListContrl�ؼ��е�����
	m_queryResult.DeleteAllItems();

	//���°�listcontrol�ؼ��е�����
	int count=1;
	CString serialNum,strLevel;
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		serialNum.Format(_T("%d"),count++);
		int level=atoi((char*)(_bstr_t)m_pRecordset->GetCollect(_T("f_level")));
		strLevel.Format(_T("Level-%d"),level);
		//������Ϣ
		m_queryResult.InsertItem(0,_T(""));
		m_queryResult.SetItemText(0,0,serialNum);
		m_queryResult.SetItemText(0,1,(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("f_filename")));
		m_queryResult.SetItemText(0,2,strLevel);
		m_queryResult.SetItemText(0,3,(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("f_date")));

		m_pRecordset->MoveNext();
	}
	m_AdoConn.ExitConnect();
}
//����list�ؼ���ʾ����
void CLookUpDlg::UpdateGridInfo()
{
	CString sql;
	sql.Format(_T("select* from t_info"));
	AddToGrid(sql);
}

void CLookUpDlg::OnBnClickedQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//��ȡ���õ�ʱ�䷶Χ
	CTime timeStart,timeStop;
	m_dateStart.GetTime(timeStart);
	m_dateStop.GetTime(timeStop);
	CString strTimeFrom=timeStart.Format("%Y-%m-%d");
	CString strTimeTo=timeStop.Format("%Y-%m-%d");
	strTimeFrom.Format(_T("' %s '"),strTimeFrom);//ע��ʱ��ǰ��Ҫ�ӵ�����
	strTimeTo.Format(_T("' %s '"),strTimeTo);//ע��ʱ��ǰ��Ҫ�ӵ�����
	//AfxMessageBox(strTimeFrom);
	//��ȡ���õ����ܵȼ�
	short level=(short)m_secretLevel.GetCurSel();
	CString strLevel=_T("");
	if(level<=5)
		strLevel.Format(_T("and f_level=%d"),level);

	CString sql;
	//�������ڣ����ú����sql���
	if(timeStart>timeStop)
	{
		AfxMessageBox(_T("���õ��������󣬿�ʼʱ�䲻�ܱȽ���ʱ���!"));
		return ;
	}
	else if(timeStart==timeStop)
	{
		sql.Format(_T("select* from t_info where f_date >= %s %s"),strTimeTo,strLevel);
	}
	else
	{
		sql.Format(_T("select* from t_info where f_date >= %s and f_date <= %s %s"),strTimeFrom,strTimeTo,strLevel);
	}
	//
	AddToGrid(sql);
}


void CLookUpDlg::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	 CMenu   menu;   //��������Ҫ�õ���cmenu����
	 menu.LoadMenu(IDR_MENU); //װ���Զ�����Ҽ��˵� 
	 CMenu   *pContextMenu=menu.GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵� 
	 pContextMenu->EnableMenuItem(ID_32771,MF_BYCOMMAND|MF_ENABLED); //����˵���ʹ��
	 
	 CPoint point1;//����һ������ȷ�����λ�õ�λ��  
	 GetCursorPos(&point1);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
	// ScreenToClient(&point1);
	 pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point1.x,point1.y,this); //��ָ��λ����ʾ�����˵�
	 pContextMenu->Detach();
	// menu.DestroyMenu();
}


void CLookUpDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

}


void CLookUpDlg::OnMenuItemDelete()
{
	// TODO: �ڴ���������������
	//AfxMessageBox(_T("ɾ��"));

	 //���ȵõ������λ��
	int nId;
	POSITION pos=m_queryResult.GetFirstSelectedItemPosition();
	 if(pos==NULL)
	 {
		 AfxMessageBox(_T("������ѡ��һ��"));
		return;
	 }
	 else
	 {
		 if(MessageBox(_T("�Ƿ�ɾ����ѡ���"),_T("ɾ������¼") ,MB_ICONEXCLAMATION|MB_OKCANCEL)==IDOK)
			{
				//AfxMessageBox(_T("a"));
				//ɾ��ѡ����Ϣ
				 while (pos)
				{
					 //�õ��кţ�ͨ��POSITIONת��
					 nId=(int)m_queryResult.GetNextSelectedItem(pos);
					 UpdateDectectInfo(m_queryResult.GetItemText(nId,1));
					 //AfxMessageBox(m_queryResult.GetItemText(nId,1));
			 
				 }
			 }
			 else
			{
				//AfxMessageBox(_T("b"));
				return;
			}
		 
	 }
}

void CLookUpDlg::UpdateDectectInfo(CString filename)
{
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	CString sql;
	sql.Format(_T("select* from t_info"));
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);
	//���Ҹü�¼����λ�ã�ɾ����
	//
	int curPos;
	CString file;
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		file=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("f_filename"));
		//���ƥ��ɹ�
		if(file.Trim()==filename.Trim())
		{
			m_pRecordset->Delete(adAffectCurrent);
			m_pRecordset->Update();
			m_AdoConn.ExitConnect();
			break;
		}

		m_pRecordset->MoveNext();
	}

	//������Ϣ
	AfxMessageBox(_T("ɾ���ɹ�"));
	UpdateGridInfo();
}

BOOL CLookUpDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	int menuID = LOWORD(wParam);   

	return CDialogEx::OnCommand(wParam, lParam);
}


BOOL CLookUpDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	/*
	if(WM_RBUTTONUP ==pMsg->message) 
    {
       AfxMessageBox(_T("hahha"));
     }
	*/
	return CDialogEx::PreTranslateMessage(pMsg);
}
