// LookUpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecretInfo.h"
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
	DDX_Control(pDX, IDC_DATE_START, m_dateStart);
	DDX_Control(pDX, IDC_DATE_STOP, m_dateStop);
	DDX_Control(pDX, IDC_LIST1, m_queryResult);
}


BEGIN_MESSAGE_MAP(CLookUpDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLookUpDlg::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CLookUpDlg::OnNMRClickList1)
	ON_COMMAND(ID_LOOLUP_DELETE, &CLookUpDlg::OnMenuItemDelete)
	ON_COMMAND(ID_LOOKUP_EMPTY, &CLookUpDlg::OnMenuItemEmpty)
	ON_COMMAND(ID_LOOKUP_EXPORT, &CLookUpDlg::OnMenuItemExport)
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
	//��ʼ��LISTCTRL
	m_queryResult.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
	m_queryResult.InsertColumn(0,_T("���"),LVCFMT_LEFT,50,0);
	m_queryResult.InsertColumn(1,_T("�ļ���"),LVCFMT_LEFT,200,0);
	m_queryResult.InsertColumn(2,_T("���ܵȼ�"),LVCFMT_LEFT,100,0);
	m_queryResult.InsertColumn(3,_T("�������"),LVCFMT_LEFT,200,0);
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
}

//��ѯ
void CLookUpDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
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


void CLookUpDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CMenu   menu;   //��������Ҫ�õ���cmenu����
	 menu.LoadMenu(IDR_MENU); //װ���Զ�����Ҽ��˵� 
	 CMenu   *pContextMenu=menu.GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵� 
	// pContextMenu->EnableMenuItem(ID_32771,MF_BYCOMMAND|MF_ENABLED); //����˵���ʹ��
	 
	 CPoint point1;//����һ������ȷ�����λ�õ�λ��  
	 GetCursorPos(&point1);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  

	 pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point1.x,point1.y,this); //��ָ��λ����ʾ�����˵�
	 pContextMenu->Detach();
}

//�����˵�ɾ���¼�
void CLookUpDlg::OnMenuItemDelete()
{
	// TODO: �ڴ���������������
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
				 for(int i=0; i<m_queryResult.GetItemCount(); i++)
				  {
					   if( m_queryResult.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
					   {
							UpdateDectectInfo(m_queryResult.GetItemText(i,1));
							//AfxMessageBox(m_queryResult.GetItemText(i,1));
					   }
				  }
				 //������Ϣ
				AfxMessageBox(_T("ɾ���ɹ�"));
				UpdateGridInfo();
			
			 }
			 else
			{
				return;
			}
		 
	 }
}


void CLookUpDlg::OnMenuItemEmpty()
{
	// TODO: �ڴ���������������
	try{
		//������ݿ�����
		ADOConn m_AdoConn;
		m_AdoConn.OnInitADOConn();
		CString sql;
		sql.Format(_T("delete from t_info"));
		//ִ��SQL����
		if(m_AdoConn.ExecuteSQL((_bstr_t)sql))
			MessageBox(_T("��������ļ��ɹ���"),_T("���������Ϣ"),MB_ICONWARNING);
		else
			MessageBox(_T("��������ļ�ʧ�ܣ�"),_T("���������Ϣ"),MB_ICONWARNING);
		m_AdoConn.ExitConnect();

		//����Listctrl��ʾ
		UpdateGridInfo();
	}catch(...)
	{
		MessageBox(_T("��������ļ��쳣��"),_T("���������Ϣ"),MB_ICONWARNING);
	}
}


void CLookUpDlg::Write2Excel()
{
	  //�����Ի����������ñ���·��
	  CFileDialog dlg(FALSE,_T("�ļ�(*.xls)"),NULL,OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, 
	_T("�ļ�(*.xls)|*.xls|�ļ�(*.xlsx)|*.xlsx||"),NULL);

	if (dlg.DoModal() == IDOK)
	{
		//��ȡ·��
		CString strFileName=dlg.GetPathName();

	  //�������
	  CApplication0 objApp;
		CWorkbooks objBooks;
		CWorkbook objBook;
		CWorksheets objSheets;
		CWorksheet objSheet;
		CRange0 objRange,usedRange;

		LPDISPATCH lpDisp;
		COleVariant vResult;
		COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);

		try{
			 if(!objApp.CreateDispatch(_T("Excel.Application"),NULL))
			 {
					   AfxMessageBox(_T("����Excel����ʧ��!"));
					   return;
			 }
			//���ò��ɼ�
			 objApp.put_Visible(FALSE);
			 //����excel��������
			 objBooks.AttachDispatch(objApp.get_Workbooks(),TRUE);
			 objBook=objBooks.Add(covOptional);      
			 objSheets.AttachDispatch(objBook.get_Sheets(),TRUE);//����Sheetҳ��
 
			 //�ѵ�һ��Sheetҳ������ָı�ΪTestSheet
			 objSheet.AttachDispatch(objSheets.get_Item(COleVariant((long)1)),TRUE);
			 objSheet.put_Name(_T("���ܼ�ⱨ��"));
 
			 ///////�ϲ���һ�е�Ԫ��A1��D1//////
			 //����Ҫ�ϲ��ĵ�Ԫ��
			 objRange.AttachDispatch(objSheet.get_Range(COleVariant(_T("A1")),COleVariant(_T("B1"))),TRUE);
			 objRange.Merge(COleVariant((long)0));
 
			 ////////���ñ������////////
 
			 objRange.AttachDispatch(objSheet.get_Cells(),TRUE);//�������е�Ԫ��
			//�����_variant_t����ʹ��COleVariant����
			 //���ȵõ������λ��
			int nId,index=2;
			POSITION pos=m_queryResult.GetFirstSelectedItemPosition();
			 if(pos==NULL)
			 {
				 AfxMessageBox(_T("������ѡ��һ��"));
				return;
			 }
			 else
			 {
				 if(MessageBox(_T("�Ƿ񵼳���ѡ���"),_T("��������¼") ,MB_ICONEXCLAMATION|MB_OKCANCEL)==IDOK)
					{
						//ɾ��ѡ����Ϣ
						 while (pos)
						{
							 //�õ��кţ�ͨ��POSITIONת��
							 nId=(int)m_queryResult.GetNextSelectedItem(pos);
							 ADOConn m_AdoConn;
							m_AdoConn.OnInitADOConn();
							CString sql;
							sql.Format(_T("select* from t_report where f_filename like '%s%%'"),m_queryResult.GetItemText(nId,1).Trim().Trim());
							_RecordsetPtr  m_pRecordset;
							m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);

							//���Ҹü�¼����λ��
							//���ñ���
							 objRange.put_Item(COleVariant((long)1),COleVariant((long)1),COleVariant(m_queryResult.GetItemText(nId,1)));
							 objRange.put_Item(_variant_t((long)index),_variant_t((long)1),_variant_t(_T("���ܴ�")));
							 objRange.put_Item(_variant_t((long)index++),_variant_t((long)2),_variant_t(_T("���ִ���")));
							 //���ñ������
							CString file,level;
							bool flag=true;
							while(m_AdoConn.m_pRecordset->adoEOF==0)
							{
								file=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("f_word"));
								level=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("f_repeat"));
								//���ƥ��ɹ�
								//AfxMessageBox(file);
								if(flag)
								{
									flag=false;
									m_pRecordset->MoveNext();
									continue;
								}
								//
								if(!file.IsEmpty())
									objRange.put_Item(_variant_t((long)index),_variant_t((long)1),_variant_t(file));
								if(!level.IsEmpty())
									objRange.put_Item(_variant_t((long)index),_variant_t((long)2),_variant_t(level));
								index++;
								
								m_pRecordset->MoveNext();
							}			 
						 }
					 }
					 else
					{
						return;
					}
		 
			 }
			 objRange.AttachDispatch(objSheet.get_UsedRange());//������ʹ�õĵ�Ԫ��
 
			 objRange.put_WrapText(_variant_t((long)1));//���õ�Ԫ���ڵ��ı�Ϊ�Զ�����
        
			objRange.put_HorizontalAlignment(_variant_t((long)-4108));
			objRange.put_VerticalAlignment(_variant_t((long)-4108));
 
	//      //////////////Ϊ������ñ߿�/////////////
		   CRange0 UnitRge;
		   CString CellName;  
		   for(int i=1;i<index;i++)
		   {
					 for(int j=1;j<=EXPORT_MAX_LENGTH;j++)
					 {    
						 CellName.Format(_T("%c%d"),j+64,i);//��Ԫ�������
						 UnitRge.AttachDispatch(objRange.get_Range(_variant_t(CellName),_variant_t(CellName)));//���ص�Ԫ��
 
					 //LineStyle=����Weight=�߿�ColorIndex=�ߵ���ɫ(-4105Ϊ�Զ�)
					UnitRge.BorderAround(_variant_t((long)1),_variant_t((long)2),_variant_t((long)-4105),vtMissing,vtMissing);//���ñ߿�
					 }
		   }
			//������
			objBook.SaveAs(COleVariant(strFileName),covOptional,covOptional,
			covOptional,covOptional,covOptional,0,
			covOptional,covOptional,covOptional,covOptional,covOptional); 
			objBook.Close (covOptional,COleVariant(strFileName),covOptional);

			AfxMessageBox(_T("������Ϣ�ɹ���"));
			 //�ͷŶ����൱��Ҫ����
			 objRange.ReleaseDispatch();
			 objSheet.ReleaseDispatch();
			 objSheets.ReleaseDispatch();
			 objBook.ReleaseDispatch();
			 objBooks.ReleaseDispatch();
			 //m_ExlAppһ��Ҫ�ͷţ������������󻹻���һ��Excel����פ�����ڴ��У����ҳ����ظ����е�ʱ������
			 objApp.ReleaseDispatch();
			 //�˳�����	
		}catch(...)
		{
			MessageBox(_T("������ⱨ���쳣��"),_T("����������Ϣ"),MB_ICONERROR);
		}
		}
}

void CLookUpDlg::OnMenuItemExport()
{
	// TODO: �ڴ���������������
	Write2Excel();
}

void CLookUpDlg::ExportDetectInfo(CString filename)
{
	
}

//��ȡ�ļ���
CString CLookUpDlg::GetFileName(CString path)
{
	CString str;
	 int pos=-1;

	 for(int i=path.GetLength();i>0;i--)
	 {
	  pos=path.Find(_T("\\"),i);
	  if(pos>0)
	   break;
	 }
	 str=path.Right(pos-1);//��ȡ·����

	 return str;
}