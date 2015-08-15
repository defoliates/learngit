// FileDetectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SecretInfo.h"
#include "FileDetectDlg.h"
#include "afxdialogex.h"

typedef  void(__stdcall *Dllfun)  (int, char**);
// CFileDetectDlg �Ի���

IMPLEMENT_DYNAMIC(CFileDetectDlg, CDialogEx)

CFileDetectDlg::CFileDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileDetectDlg::IDD, pParent)
{

}

CFileDetectDlg::~CFileDetectDlg()
{
}

void CFileDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_resultList);
}


BEGIN_MESSAGE_MAP(CFileDetectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FILE_ADD, &CFileDetectDlg::OnBnClickedFileAdd)
	ON_BN_CLICKED(IDC_FILE_DELETE, &CFileDetectDlg::OnBnClickedFileDelete)
	ON_BN_CLICKED(IDOK, &CFileDetectDlg::OnBnClickedFileDetect)
END_MESSAGE_MAP()


// CFileDetectDlg ��Ϣ�������

//��Ӵ�����ļ�
void CFileDetectDlg::OnBnClickedFileAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ѡ����Ҫ��ӵ��ļ�
	CString filters = _T("�ļ�(*.txt)|*.txt|�ļ�(*.pdf)|*.pdf|�����ļ�(*.*) |*.*||"); 
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

//ɾ��������ļ�
void CFileDetectDlg::OnBnClickedFileDelete()
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

//����ļ��Ƿ�����
void CFileDetectDlg::OnBnClickedFileDetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	//��ȡ�����
	int result=DetectSecretInfo();	
	
}

//��ʼ���Ի���
BOOL CFileDetectDlg::OnInitDialog()
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
CString CFileDetectDlg::GetExtName(CString fileName)
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
int CFileDetectDlg::DetectSecretInfo()
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
		//AfxMessageBox(m_resultList.GetItemText(i,1));
		result=GetDetectResult(m_resultList.GetItemText(i,1));
		//�����д�����ݿ�
		WriteDetect2DB(m_resultList.GetItemText(i,1));
	}
	return result;
}
//���ؼ����
int CFileDetectDlg::GetDetectResult(CString filename)
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
int CFileDetectDlg::GetExtType(CString fileExtName)
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

void CFileDetectDlg::Pdf2TXT(char* filename)//��PDFתΪTXT
{
	try{
		Dllfun pdfToTxt;
		HINSTANCE hdll;
		hdll = LoadLibrary(_T("DLLXpdf.dll"));// 
		if (hdll == NULL)
		{
			FreeLibrary(hdll);
		}

		pdfToTxt = (Dllfun)GetProcAddress(hdll, "_PdfToTxt@8");//
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
	}catch(...)
	{
		MessageBox(_T("pdf�ļ���ʧ�ܣ�"),_T("PDF�ļ���ȡ"),MB_ICONERROR);
	}
}
void CFileDetectDlg::PIC2TXT(CString filename)
{
	OCRTxt(filename,_T("tmp.txt"));
}
//��wordתΪtxt�ļ�
void CFileDetectDlg::Word2TXT(CString filename)
{
	ReadWord(filename);
}

//��excelתΪtxt�ļ�
void CFileDetectDlg::Excel2TXT(CString filename)
{
	ReadExcel(filename);
}
//��pptתΪtxt�ļ�
void CFileDetectDlg::PPT2TXT(CString filename)
{
	ReadPowerPoint(filename);
}

void CFileDetectDlg::UpdateInfo(CString filename,int level)
{
	//��ȡ��ǰ������Ϣ
	CTime curTime=CTime::GetCurrentTime();
	CString sEndtime=curTime.Format("%Y-%m-%d %H:%M:%S");

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
			m_pRecordset->put_Collect(_variant_t(_T("f_filename")),_variant_t(filename));
			m_pRecordset->put_Collect(_variant_t(_T("f_level")),_variant_t(level));
			m_pRecordset->put_Collect(_variant_t(_T("f_date")),_variant_t(sEndtime));
			m_pRecordset->Update();
			m_AdoConn.ExitConnect();

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
void CFileDetectDlg::Split(CString source, CString divKey, CStringArray& dest)
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

void CFileDetectDlg::ReadWord(CString filename)
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

void CFileDetectDlg::ReadExcel(CString filename)
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

void CFileDetectDlg::ReadPowerPoint(CString filename)
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

void CFileDetectDlg::WriteDetect2DB(CString filename)
{
	//��ȡ�ļ���
	CString filenameTmp=GetFileName(filename);
	//д�����ݿ�
	try{
		//��ȡ�ļ�����
		CFile file(_T("SecretReport.txt"),CFile::modeRead); 
		wchar_t *pBuf; 
		DWORD dwFlielen;
		dwFlielen=file.GetLength();
		pBuf=new wchar_t[dwFlielen+1];
		pBuf[dwFlielen]=0;
		file.Read(pBuf,dwFlielen);
		//
		CString source=pBuf;
		CStringArray dest,subDest;
		Split(source,_T("\r\n"),dest);
		int i,j,count;

		//�������ݿ�
		ADOConn m_AdoConn;
		m_AdoConn.OnInitADOConn();
		//ɾ���Ѵ��ڼ�¼
		CString sql,str;
		sql.Format(_T("delete from t_report where f_filename like '%s%%'"),filenameTmp.Trim());
		m_AdoConn.ExecuteSQL((_bstr_t)sql);
		//����¼�¼
		sql.Format(_T("select* from t_report"));
		_RecordsetPtr  m_pRecordset;
		m_pRecordset=m_AdoConn.GetRecordSet((_bstr_t)sql);

		//�ָ��ַ���
		for(i=0;i<dest.GetSize()-1;i=i+2)
		{
			if(!dest.GetAt(i).IsEmpty()&&(!dest.GetAt(i+1).IsEmpty()))
			{
				//AfxMessageBox(dest.GetAt(i));
				//д�����ݿ�		
				//��ÿ�е����ܴʺͳ��ִ����ֿ�				
				try{
					m_pRecordset->AddNew();
					//str=dest.GetAt(i);
					//AfxMessageBox(str);
					m_pRecordset->put_Collect(_variant_t(_T("f_filename")),_variant_t(filenameTmp));
					if(i==0)
						m_pRecordset->put_Collect(_variant_t(_T("f_word")),_variant_t(_T("")));
					else
						m_pRecordset->put_Collect(_variant_t(_T("f_word")),_variant_t(dest.GetAt(i)));
					//m_pRecordset->put_Collect(_variant_t(_T("f_word")),_variant_t(dest.GetAt(i)));
					count=_ttoi(dest.GetAt(i+1));
					m_pRecordset->put_Collect(_variant_t(_T("f_repeat")),_variant_t(count));
					m_pRecordset->Update();

				}
				catch(...)
				{
					AfxMessageBox(_T("����ʧ��"));
					return;
				}			
			}
		}
		file.Close();
		m_AdoConn.ExitConnect();
		//AfxMessageBox(pBuf);

	}catch(...)
	{
		AfxMessageBox(_T("�����ܼ����д�����ݿ�ʧ�ܣ�"));  
	}
}
//��ȡ�ļ���
CString CFileDetectDlg::GetFileName(CString path)
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