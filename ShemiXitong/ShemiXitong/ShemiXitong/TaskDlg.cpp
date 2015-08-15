// TaskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShemiXitong.h"
#include "TaskDlg.h"
#include "afxdialogex.h"
//#include <altbase.h>

typedef  void(__stdcall *Dllfun)  (int, char**);
//typedef  void (*pOCRTxt) (CString, CString);
// CTaskDlg �Ի���

IMPLEMENT_DYNAMIC(CTaskDlg, CDialogEx)

CTaskDlg::CTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskDlg::IDD, pParent)
	, m_fileName(_T(""))
	, m_queryStr(_T(""))
{

}

CTaskDlg::~CTaskDlg()
{
}

void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCHLIST, m_searchList);
	DDX_Control(pDX, IDC_RESULTLIST, m_resultList);
	DDX_Text(pDX, IDC_SEARCHSPECIFIC, m_fileName);
	DDX_Text(pDX, IDC_KEYWORD, m_queryStr);
}


BEGIN_MESSAGE_MAP(CTaskDlg, CDialogEx)
	ON_BN_CLICKED(IDC_DETECTBUT, &CTaskDlg::OnBnClickedDetect)
	ON_NOTIFY(NM_CLICK, IDC_SEARCHLIST, &CTaskDlg::OnNMClickSearchlist)
	ON_NOTIFY(NM_CLICK, IDC_RESULTLIST, &CTaskDlg::OnNMClickResultlist)
	ON_BN_CLICKED(IDC_ADDBUT, &CTaskDlg::OnBnClickedAddInfo)
	ON_BN_CLICKED(IDC_DELETEBUT, &CTaskDlg::OnBnClickedDeleteInfo)
	ON_BN_CLICKED(IDC_SEARCHBUT, &CTaskDlg::OnBnClickedSearch)
END_MESSAGE_MAP()


// CTaskDlg ��Ϣ�������


BOOL CTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����ListCtrl�ؼ�
	m_searchList.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
	m_searchList.InsertColumn(0,_T("���"),LVCFMT_LEFT,50,0);
	m_searchList.InsertColumn(1,_T("�ļ���"),LVCFMT_LEFT,100,0);

	m_resultList.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);
	m_resultList.InsertColumn(0,_T("���"),LVCFMT_LEFT,50,0);
	m_resultList.InsertColumn(1,_T("�ļ���"),LVCFMT_LEFT,100,0);
	//��ʼ��
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CTaskDlg::OnBnClickedDetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	//��ȡ�����
	int result=DetectSecretInfo();		
	//AfxMessageBox(_T("�����ɣ�"));
}

CString CTaskDlg::GetExtName(CString fileName)//��ȡ��׺��
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
int CTaskDlg::DetectSecretInfo()
{
	//������Ϣ
	UpdateData(TRUE);
	//����������
	int len=m_resultList.GetItemCount();
	int i;

	//�ж��Ƿ��Ѿ�ѡ�����ļ�
	if(m_fileName=="" && len==0)
	{
		AfxMessageBox(_T("��ѡ���ļ���"));
		return -1;
	}
	//���ļ����м��
	int result=-1;
	//ģ������
	for(i=0;i<len;i++)
	{
		//AfxMessageBox(m_resultList.GetItemText(i,1));
		result=GetDetectResult(m_resultList.GetItemText(i,1));
	}
	//��ȷ����
	if(!(m_fileName==""))
	{
		result=GetDetectResult(m_fileName);
	}

	return result;
}
//���ؼ����
int CTaskDlg::GetDetectResult(CString filename)
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
		//str.Format(_T("ƥ��ɹ����ַ�������%d"),result);
		//AfxMessageBox(str);
		break;
	case PDFEXTNAME://��ΪPDF��ʽ�ļ�
		//��pdfתΪtxt�ļ�
		Pdf2TXT(buf);
		result = last_search("tmp.txt","pattern.txt");
		//str.Format(_T("ƥ��ɹ����ַ�������%d"),result);
		//AfxMessageBox(str);
		break;
	case WORDEXTNAME://��ΪWORD��ʽ�ļ�
		//��wordתΪtxt�ļ�
		Word2TXT(filename);
		result = last_search("tmp.txt","pattern.txt");
		break;
	case EXCELEXTNAME://��ΪEXCEL��ʽ�ļ�
		//��wordתΪtxt�ļ�
		Excel2TXT(filename);
		result = last_search("tmp.txt","pattern.txt");
		break;
	case PPTEXTNAME://��ΪPPT��ʽ�ļ�
		//��wordתΪtxt�ļ�
		PPT2TXT(filename);
		result = last_search("tmp.txt","pattern.txt");
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
//�����ļ���ʽ����
int CTaskDlg::GetExtType(CString fileExtName)
{
	if((fileExtName=="txt")||(fileExtName=="TXT"))
	{
		return TXTEXTNAME;
	}
	else if((fileExtName=="pdf")||(fileExtName=="PDF"))
	{
		return PDFEXTNAME;
	}
	else if((fileExtName=="doc")||(fileExtName=="DOC")||(fileExtName=="docx")||(fileExtName=="DOCX"))
	{
		return WORDEXTNAME;
	}
	else if((fileExtName=="xls")||(fileExtName=="XLS")||(fileExtName=="xlsx")||(fileExtName=="XLSX"))
	{
		return EXCELEXTNAME;
	}
	else if((fileExtName=="ppt")||(fileExtName=="PPT")||(fileExtName=="pptx")||(fileExtName=="PPTX"))
	{
		return PPTEXTNAME;
	}
	else if((fileExtName=="png")||(fileExtName=="PNG")||(fileExtName=="JPEG")||(fileExtName=="jpeg")
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

void CTaskDlg::Pdf2TXT(char* filename)//��PDFתΪTXT
{
	Dllfun pdfToTxt;
	HINSTANCE hdll;
	hdll = LoadLibrary(_T("DLLXpdf.dll"));// D:\\Develop\\Projects\\VS2013\\xpdf - 3.02\\xpdf\\Debug\\DLLXpdf.dll");
	if (hdll == NULL)
	{
		FreeLibrary(hdll);
	}

	pdfToTxt = (Dllfun)GetProcAddress(hdll, "_PdfToTxt@8");// MAKEINTRESOURCE(1));// "PdfToTxt");
	if (pdfToTxt == NULL)
	{
		FreeLibrary(hdll);
	}
	else
	{
		int argsCount = 5;
		// 5��������ǰ�������̶������ĸ���pdf�ļ�������·���������������ļ���·��
		//USES_CONVERSION;
		//char* buf = T2A(m_fileName);

		char* temp[5]={"a", 
			"-enc", "UTF-8", 
			filename,
			//"������ѧϰ���ض���ʶ���������ʻ����.pdf", 
			"tmp.txt" };
		char** args=temp; 

		pdfToTxt(argsCount, args);
	}
}
void CTaskDlg::PIC2TXT(CString filename)
{
	OCRTxt(filename,_T("tmp.txt"));
}
//��wordתΪtxt�ļ�
void CTaskDlg::Word2TXT(CString filename)
{
	ReadWord(filename);
}

//��excelתΪtxt�ļ�
void CTaskDlg::Excel2TXT(CString filename)
{
	ReadExcel(filename);
}
//��pptתΪtxt�ļ�
void CTaskDlg::PPT2TXT(CString filename)
{
	ReadPowerPoint(filename);
}

void CTaskDlg::UpdateInfo(CString filename,int level)
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

void CTaskDlg::OnNMClickSearchlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

}


void CTaskDlg::OnNMClickResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CTaskDlg::OnBnClickedAddInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str,m_name;
	int nId,rowId;
	 //���ȵõ������λ��
	POSITION pos=m_searchList.GetFirstSelectedItemPosition();
	 if(pos==NULL)
	 {
		 AfxMessageBox(_T("������ѡ��һ��"));
		return;
	 }
	 else
	 {
		 while (pos)
		{
			 //�õ��кţ�ͨ��POSITIONת��
			 nId=(int)m_searchList.GetNextSelectedItem(pos);
			 //�õ����е����ݣ�0��ʾ��һ�У�ͬ��1,2,3...��ʾ�ڶ���������...�У�

			 //��ѡ�е���ӵ������
			 m_resultList.InsertItem(0,_T(""));
			 m_resultList.SetItemText(0,0,m_searchList.GetItemText(nId,0));
			 m_resultList.SetItemText(0,1,m_searchList.GetItemText(nId,1));
		 }
	 }
	 //AfxMessageBox(str);
	 //ˢ�¿ؼ�ֵ
	 UpdateData(FALSE); 
}


void CTaskDlg::OnBnClickedDeleteInfo()
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


void CTaskDlg::OnBnClickedSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	//�ж������ַ��Ƿ�Ϊ�գ���Ϊ�գ���ֱ�ӷ���
	if(m_queryStr.IsEmpty())
	{
		MessageBox(_T("��������ݲ���Ϊ�գ�"),_T("��ѯ"), MB_ICONERROR); 
		return ;
	}

	//�������ƽ���
	CSearchDlg dlg;
	dlg.SetSearchValue(m_queryStr);
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		//�������ҵĽ��
		CFile pfile;
		if(!pfile.Open(_T("queryfile.txt"),CFile::modeRead))
			return;
		TCHAR* pBuf;
		int dwFileLen;
		dwFileLen = pfile.GetLength();
		pBuf=new TCHAR[dwFileLen+1];
		memset(pBuf,0,dwFileLen+1);
		pfile.Read(pBuf,dwFileLen);
		pBuf[dwFileLen]='\0';
		CString str(pBuf);
		CStringArray arr;
		Split(str,_T("\r\n"),arr);
		int nSize = arr.GetSize(); 
		//���������list�ؼ���
		CString code;
		for (int i = 0; i < nSize-1; i++)
		{
			//str+=arr.GetAt(i);
			code.Format(_T("%d"),i+1);
			m_searchList.InsertItem(0,_T(""));//������
			m_searchList.SetItemText(0,0,code);
			m_searchList.SetItemText(0,1,arr.GetAt(i));
		}

		//AfxMessageBox(str);
		pfile.Close();
		delete [] pBuf;
		AfxMessageBox(_T("������ϣ�"));
	}
}
//�ָ��ַ���
void CTaskDlg::Split(CString source, CString divKey, CStringArray& dest)
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

void CTaskDlg::ReadWord(CString filename)
{
	try{
		//��ָ��·���ļ�
		CString FileName=filename;
		if (!wordApp.CreateDispatch(_T("Word.Application"),NULL)) {
			AfxMessageBox(_T("Word failed to start!"));
			return ;
		}
		wordApp.put_Visible(FALSE);
		docs = wordApp.get_Documents();
		COleVariant vTrue((short)TRUE);    
		COleVariant vFalse((short)FALSE);
		COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		//COleVariant vFileName(FileName + ".docx");
		COleVariant vFileName(FileName);
		doc = docs.Open(vFileName,vTrue,vTrue,vTrue,vOptional,vOptional,vOptional,vOptional,vOptional,vOptional,vOptional,vOptional,vOptional,vOptional,vOptional,vOptional);
		wordRange = doc.Range(vOptional,vOptional);

		//����һ�����±��ļ�
		CFile   file;
		file.Open(_T("tmp.txt"),CFile::modeCreate|CFile::modeWrite);
		WORD unicode = 0xFEFF;
		file.Write(&unicode,2);	
		//AfxMessageBox(wordRange.get_Text());
		//����ȡ��word����д���ļ�
		CString str=wordRange.get_Text();
		file.Write(str,wcslen(str)*sizeof(wchar_t));
		//�ر��ĵ�
		file.Flush();
		file.Close();
	
		//�ͷ���Դ
		doc.ReleaseDispatch();

		docs.ReleaseDispatch();
		wordApp.ReleaseDispatch();
	}catch(...)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�"));
	}
}

void CTaskDlg::ReadExcel(CString filename)
{
	LPDISPATCH lpDisp = NULL;
	//
	 COleVariant vResult;
	COleVariant
        covTrue((short)TRUE),
        covFalse((short)FALSE),
        vtMissing((long)DISP_E_PARAMNOTFOUND, VT_ERROR);    
 
	//��ָ��·���ļ�
	CString FileName=filename;
	if (!excelApp.CreateDispatch(_T("Excel.Application"),NULL)) {
		AfxMessageBox(_T("Excel failed to start!"));
		return ;
	}
	//����excel���ɼ�
	excelApp.put_Visible(FALSE);
	
	//����һ���ı��ļ�
	CFile   file;
	file.Open(_T("tmp.txt"),CFile::modeCreate|CFile::modeWrite);
	WORD unicode = 0xFEFF;
	file.Write(&unicode,2);	

	try
    {
		
        //��һ��������
		books.AttachDispatch(excelApp.get_Workbooks());
		
        lpDisp = books.Open(FileName, 
            vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
            vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
            vtMissing, vtMissing, vtMissing, vtMissing);

        book.AttachDispatch(lpDisp);

		//�õ�Worksheets  
		sheets.AttachDispatch(book.get_Worksheets());

		 //����е�Ԫ�������ڱ༭״̬�У��˲������ܷ��أ���һֱ�ȴ�  
		lpDisp=book.get_ActiveSheet();  
		sheet.AttachDispatch(lpDisp); 
		
		//�õ��������е�Sheet������
		// sheets.AttachDispatch(book.get_Sheets());
		 
		 //��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
		 CRange0 usedRange;
		 usedRange.AttachDispatch(sheet.get_UsedRange());
		 excelRange.AttachDispatch(usedRange.get_Rows());
		 long iRowNum=excelRange.get_Count();                   //�Ѿ�ʹ�õ�����
 
		 excelRange.AttachDispatch(usedRange.get_Columns());
		 long iColNum=excelRange.get_Count();                   //�Ѿ�ʹ�õ�����
  
		 long iStartRow=usedRange.get_Row();               //��ʹ���������ʼ�У���1��ʼ
		 long iStartCol=usedRange.get_Column();            //��ʹ���������ʼ�У���1��ʼ
		//
		 long i,j;
		 for(i=iStartRow;i<=iRowNum;i++)
			 for(j=iStartCol;j<=iColNum;j++)
			{
				 //��ȡ��һ����Ԫ���ֵ
				 excelRange.AttachDispatch(sheet.get_Cells()); 
				 excelRange.AttachDispatch(excelRange.get_Item(COleVariant((long)i),COleVariant((long)j)).pdispVal );
				 COleVariant vResult =excelRange.get_Value2();
				 CString str;
				 if(vResult.vt == VT_BSTR)       //�ַ���
				 {
				  str=vResult.bstrVal;
				 }
				 else if (vResult.vt==VT_R8)     //8�ֽڵ����� 
				 {
				  str.Format(_T("%f"),vResult.dblVal);
				 }
				 else 
					 str="";
				 //AfxMessageBox(str);
				file.Write(str,wcslen(str)*sizeof(wchar_t));
			 }
	}
    catch(...)
    {
       AfxMessageBox(_T("���ļ�ʧ�ܣ�"));
    }

	
		//�ر��ĵ�
	file.Flush();
	file.Close();
    /*�ͷ���Դ*/
    sheet.ReleaseDispatch();
    sheets.ReleaseDispatch();
    book.ReleaseDispatch();
    books.ReleaseDispatch();
    excelApp.Quit();
    excelApp.ReleaseDispatch();

	//AfxMessageBox(aRange.get_Text());
}

void CTaskDlg::ReadPowerPoint(CString filename)
{
	CString strText;
	//
	if (!pptApp.CreateDispatch(_T("Powerpoint.Application"),NULL)) {
		AfxMessageBox(_T("ppt failed to start!"));
		return ;
	}
	//���ò��ɼ�
	//pptApp.put_Visible(FALSE);
	pptApp.put_Visible((long)TRUE);
	// �����PowerPoint���ڷ��ˣ����԰Ѵ����Ƶ���������ȥ�磺
	//pptApp.put_Top(2000);
	//
	//CString filename=filename;
	try
	{
		//���ļ�
		presentations = pptApp.get_Presentations();
        presentation = presentations.Open(filename,0,0,1);

		// ��ȡ�õ�Ƭ��Ŀ
		int i,j;
		slides=presentation.get_Slides();
		//
		//����һ���ı��ļ��������洢��ȡ�Ľ��
		CFile   file;
		file.Open(_T("tmp.txt"),CFile::modeCreate|CFile::modeWrite);
		WORD unicode = 0xFEFF;
		file.Write(&unicode,2);	
		//��ȡÿһҳ�ϵ��ı�
		for(i = 1; i <=slides.get_Count(); i++) {
			slide = slides.Range(COleVariant((long)(i)));
			shapes=slide.get_Shapes();
			for(j = 0; j < shapes.get_Count(); j ++) {
				shape=shapes.Item(COleVariant((long)(j + 1)));
				if(shape.get_HasTextFrame()) {
					textFrame = shape.get_TextFrame();
					textRange = textFrame.get_TextRange();
					CString txt = textRange.get_Text();
					if(txt.GetLength() > 0) {
						//strText += txt;
						//д���ı�
						file.Write(txt,wcslen(txt)*sizeof(wchar_t));
					}
				}
			}
		}

		//�ر��ĵ�
		file.Flush();
		file.Close();
		//AfxMessageBox(strText);
		//�ͷ���Դ
		presentation.Close();
		pptApp.Quit();

	}catch(...)
	{
		AfxMessageBox(_T("ppt�ļ���ʧ��!"));
	}
}