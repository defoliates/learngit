#pragma once
#include "afxcmn.h"
#include "AC_BH.h"
#include "ADOConn.h"
#include "OCR.h"
//
#include "CApplication.h"
#include "CDocument0.h"
#include "CDocuments.h"
#include "CRange.h"
//excel
#include "CApplication0.h"
#include "CRange0.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
//ppt
#include "CApplication1.h"
#include "CPresentation.h"
#include "CPresentations.h"
#include "CSlide.h"
#include "CSlides.h"
#include "CShape.h"
#include "CShapes.h"
#include "CTextFrame.h"
#include "CTextRange.h"

#define _UNICODE
#pragma comment(lib,"OCR.lib")
// CFileDetectDlg �Ի���

class CFileDetectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDetectDlg)

public:
	CFileDetectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileDetectDlg();
	//
	CString GetExtName(CString fileName);//��ȡ��׺��
	int DetectSecretInfo();//����������
	int GetExtType(CString fileExtName);//��ȡ�ļ�����
	void Pdf2TXT(char* filename);//��PDFתΪTXT
	void Word2TXT(CString filename);//��wordתΪTXT
	void PIC2TXT(CString filename);//��ͼƬʶ��ΪTXT
	void Excel2TXT(CString filename);//��excelתΪTXT
	void PPT2TXT(CString filename);//��pptתΪTXT
	void UpdateInfo(CString filename,int level);//���¼����Ϣ
	int GetDetectResult(CString filename);
	void Split(CString source, CString divKey, CStringArray& dest);
	void ReadWord(CString filename);//��ȡword�е�����
	void ReadExcel(CString filename);//��ȡexcel�е�����
	void ReadPowerPoint(CString filename);//��ȡPPT�е�����
	void WriteDetect2DB(CString filename);//�������д�����ݿ�
	CString GetFileName(CString path);//��ȡ�ļ���
// �Ի�������
	enum { IDD = IDD_FILE };
private:
	int count;
	//word
	CApplication  wordApp;  
     CDocuments  docs;  
     CDocument0  doc;  
     CRange  wordRange;  
	 //excel
	CApplication0 excelApp;
    CWorkbooks books;
    CWorkbook book;
    CWorksheets sheets;
    CWorksheet sheet;
    CRange0 excelRange;
	//ppt
	CApplication1  pptApp; 
	CPresentations presentations;
	CPresentation presentation;
	CSlides        slides; 
	CSlide        slide;
	CShapes shapes;
	CShape shape;
	CTextFrame textFrame;
	CTextRange textRange;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFileAdd();
	afx_msg void OnBnClickedFileDelete();
	afx_msg void OnBnClickedFileDetect();
	CListCtrl m_resultList;
	virtual BOOL OnInitDialog();
};
