// ImageDetectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecretInfo.h"
#include "ImageDetectDlg.h"
#include "afxdialogex.h"
#include "AC_BH.h"
#include "ADOConn.h"
#include "OCR.h"

#pragma comment(lib,"OCR.lib")
// CImageDetectDlg �Ի���

IMPLEMENT_DYNAMIC(CImageDetectDlg, CDialogEx)

CImageDetectDlg::CImageDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageDetectDlg::IDD, pParent)
{

}

CImageDetectDlg::~CImageDetectDlg()
{
}

void CImageDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMAGE, m_resultList);
}


BEGIN_MESSAGE_MAP(CImageDetectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_IMAGE_ADD, &CImageDetectDlg::OnBnClickedImageAdd)
	ON_BN_CLICKED(IDC_IMAGE_DELETE, &CImageDetectDlg::OnBnClickedImageDelete)
	ON_BN_CLICKED(IDOK, &CImageDetectDlg::OnBnClickedDetect)
END_MESSAGE_MAP()


// CImageDetectDlg ��Ϣ�������


void CImageDetectDlg::OnBnClickedImageAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ѡ����Ҫ��ӵ��ļ�
	CString filters = _T("λͼ�ļ�(*.bmp)|*.bmp|PNG�ļ�(*.png)|*.png|�����ļ�(*.*) |*.*||"); 
	try{
		CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, filters);
		DWORD MAXFILE = 4000;
		dlg.m_ofn.nMaxFile = MAXFILE;
		TCHAR* pc = new TCHAR[MAXFILE];
		dlg.m_ofn.lpstrFile = pc;
		dlg.m_ofn.lpstrFile[0] = NULL;
		int iReturn = dlg.DoModal();
		//���ļ���ӵ��б�
		if(iReturn == IDOK)
		{
			int nCount = 0;
			CString strName;
			POSITION pos = dlg.GetStartPosition();
			CString strCode;
			while (pos != NULL)
			{
				strName=dlg.GetNextPathName(pos);
				 //��ѡ�е���ӵ������
				 m_resultList.InsertItem(0,_T(""));
				 strCode.Format(_T("%d"),count++);
				 m_resultList.SetItemText(0,0,strCode);
				 m_resultList.SetItemText(0,1,strName);
			}
			
		}
	
		delete []pc;
	}catch(...)
	{
		AfxMessageBox(_T("�����ļ��쳣��"));
	}
}


void CImageDetectDlg::OnBnClickedImageDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem;
	//���ȵõ������λ��
	POSITION pos=m_resultList.GetFirstSelectedItemPosition();
	 if(pos==NULL)
	 {
		 AfxMessageBox(_T("������ѡ��һ��"));
		return;
	 }
	 else
	 {
		while (pos)
		{
			nItem = m_resultList.GetNextSelectedItem(pos);
			m_resultList.DeleteItem(nItem);
		}
	 }

	  //ˢ�¿ؼ�ֵ
	 UpdateData(FALSE); 
}


void CImageDetectDlg::OnBnClickedDetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	//��ȡ�����
	int result=DetectSecretInfo();	

}


BOOL CImageDetectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����ListCtrl�ؼ�
	m_resultList.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
	m_resultList.InsertColumn(0,_T("���"),LVCFMT_LEFT,50,0);
	m_resultList.InsertColumn(1,_T("�ļ���"),LVCFMT_LEFT,500,0);
	//
	count=1;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
//��ȡ��׺��
CString CImageDetectDlg::GetExtName(CString fileName)
{
	int pos=fileName.Find(_T(".")); //��ȡ . ��λ��
	if(pos==-1){ //���û���ҵ���ֱ�ӷ��ظ��ַ���
		return fileName; 
		}
	else{
		return GetExtName(fileName.Mid(pos+1)); //�ҵ��˵Ļ�������������ֱ����ײ�
	}
}
//����������
int CImageDetectDlg::DetectSecretInfo()
{
	//������Ϣ
	UpdateData(TRUE);
	//����������
	int len=m_resultList.GetItemCount();
	int i;

	//�ж��Ƿ��Ѿ�ѡ�����ļ�
	if(len==0)
	{
		AfxMessageBox(_T("��ѡ���ļ���"));
		return -1;
	}
	//���ļ����м��
	int result=-1;
	//ģ������
	for(i=0;i<len;i++)
	{
		result=GetDetectResult(m_resultList.GetItemText(i,1));
	}
	return result;
}
//���ؼ����
int CImageDetectDlg::GetDetectResult(CString filename)
{
	CString str;
	//��ȡ��׺��
	CString fileExtName=GetExtName(filename);
	int extType=GetExtType(fileExtName);

	//���ļ��������ַ�ת��
	USES_CONVERSION;
	char* buf = T2A(filename);

	//ƥ��ɹ����ַ�����
	int result=-1;
	//���ڲ�ͬ��׺����ʽ�ļ��ֱ���д���
	switch(extType)
	{
	case TXTEXTNAME://��ΪTXT��ʽ�ļ�		
		result = last_search(buf,"pattern.txt");
		break;
	case PICTUREEXTNAME://��Ϊjpg,bmp,png,tifͼƬ��ʽ
		PIC2TXT(filename);
		result = last_search("tmp.txt","pattern.txt");
		break;
	case ERROREXTNAME:
		AfxMessageBox(_T("������ļ���ʽ����ʶ��"));
		break;
	default:
		AfxMessageBox(_T("������ļ���ʽ����ʶ��"));
	}

	//�жϼ�����
	if(result==-1)
	{
		//AfxMessageBox(_T("���ļ����ִ���"));
		return result;
	}

	//
	int level=result/SECRETLEVEL;
	//�������ݿ���Ϣ
	CString fileName=filename.Mid(filename.ReverseFind('\\')+1);
	//AfxMessageBox(fileName);
	if(level>=5)
		level=5;
	UpdateInfo(fileName,level);
	//�ж����ܵȼ�
	switch(level)
	{
	case 0:
		str.Format(_T("%sδ���ܣ�"),fileName);
		AfxMessageBox(str);
		break;
	case 1:
		str.Format(_T("%s���ܵȼ�Ϊ1��"),fileName);
		AfxMessageBox(str);
		//AfxMessageBox(_T("���ܵȼ�Ϊ1��"));
		break;
	case 2:
		str.Format(_T("%s���ܵȼ�Ϊ2��"),fileName);
		AfxMessageBox(str);
		//AfxMessageBox(_T("���ܵȼ�Ϊ2��"));
		break;
	case 3:
		str.Format(_T("%s���ܵȼ�Ϊ3��"),fileName);
		AfxMessageBox(str);
		//AfxMessageBox(_T("���ܵȼ�Ϊ3��"));
		break;
	case 4:
		str.Format(_T("%s���ܵȼ�Ϊ4��"),fileName);
		AfxMessageBox(str);
		//AfxMessageBox(_T("���ܵȼ�Ϊ4��"));
		break;
	default:
		str.Format(_T("%s���ܵȼ�Ϊ5��"),fileName);
		AfxMessageBox(str);
		//AfxMessageBox(_T("���ܵȼ�Ϊ5��"));
	}

	return result;
}

void CImageDetectDlg::PIC2TXT(CString filename)
{
	OCRTxt(filename,_T("tmp.txt"));
}

void CImageDetectDlg::UpdateInfo(CString filename,int level)
{
	//�������ݿ�
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	CString sql;
	sql.Format(_T("select* from t_info"));
	_RecordsetPtr  m_pRecordset;
	m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);
	//�жϸ��ı��Ƿ��Ѿ�������
	CString file;
	while(m_AdoConn.m_pRecordset->adoEOF==0)
	{
		file=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_T("f_filename"));
		if(file.Trim()==filename)
		{
			AfxMessageBox(_T("���ļ��Ѿ��������ܼ���ˣ�"));
			return;
		}
		m_pRecordset->MoveNext();
	}
	//�����µ�����
	if(filename.GetLength()>80)
	{
		AfxMessageBox(_T("���ļ����������޷��������ݿ⣡"));
		return;
	}

	CTime curTime=CTime::GetCurrentTime();
	CString sEndtime=curTime.Format("%Y-%m-%d %H:%M:%S");
	try{
		m_pRecordset->AddNew();
		m_pRecordset->put_Collect(_variant_t(_T("f_filename")),_variant_t(filename));
		m_pRecordset->put_Collect(_variant_t(_T("f_level")),_variant_t(level));
		m_pRecordset->put_Collect(_variant_t(_T("f_date")),_variant_t(sEndtime));
		m_pRecordset->Update();
		m_AdoConn.ExitConnect();
	}
	catch(...)
	{
		AfxMessageBox(_T("����ʧ��"));
		return;
	}
}


//�ָ��ַ���
void CImageDetectDlg::Split(CString source, CString divKey, CStringArray& dest)
{
	dest.RemoveAll();
     int pos = 0;
     int pre_pos = 0;
     while( -1 != pos ){
         pre_pos = pos;
         pos = source.Find(divKey,(pos+1));
         dest.Add(source.Mid(pre_pos,(pos-pre_pos)));
     }
}

//�����ļ���ʽ����
int CImageDetectDlg::GetExtType(CString fileExtName)
{
	 if((fileExtName=="png")||(fileExtName=="PNG")||(fileExtName=="JPEG")||(fileExtName=="jpeg")
		||(fileExtName=="jpg")||(fileExtName=="JPG")||(fileExtName=="bmp")||(fileExtName=="BMP")
		||(fileExtName=="tif")||(fileExtName=="TIF"))
	{
		return PICTUREEXTNAME;
	}
	else
	{
		return ERROREXTNAME;
	}
}