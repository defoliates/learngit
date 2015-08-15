// SearchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShemiXitong.h"
#include "SearchDlg.h"
#include "afxdialogex.h"

UINT ThreadProc(LPVOID pParam);
// CSearchDlg �Ի���

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogEx)

CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchDlg::IDD, pParent)
{

}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSearchDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_MESSAGE(WM_MY_SEARCH, OnMySearchMessage)
	ON_MESSAGE(WM_END_SEARCH, OnEndSearchMessage)
END_MESSAGE_MAP()


// CSearchDlg ��Ϣ�������


void CSearchDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	stopFlag=true;

	//CDialogEx::OnOK();
}


BOOL CSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//
	stopFlag=false;
	count=0;
	searchEndFlag=false;

	//��ʼ��������
	m_progress.SetRange(0,PROGRESS_MAX_LENGTH);
	m_progress.SetStep(1);
	m_progress.SetPos(0);
	m_progress.StepIt();

	//AfxMessageBox(findStr);
	PostMessage(WM_MY_SEARCH);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//���ò��ҹؼ���
void CSearchDlg::SetSearchValue(CString value)
{
	//findStr=value;
	//���ó�������ʽ
	findStr.Format(_T(".*%s.*"),value);
}
//
CString CSearchDlg::GetSearchValue()
{
	return findStr;
}

//
bool CSearchDlg::GetEndSearchFlag()
{
	return searchEndFlag;
}
//
void CSearchDlg::SetEndSearchFlag(bool flag)
{
	searchEndFlag=flag;
}
//�ݹ��������
void CSearchDlg::TraversFile(CString csPath)
{
	try{
		 CString csPrePath = csPath;
		CString csNextPath = csPath;
		CFileFind ff;
		csPath += _T("*.*");//������һ��ȫ����Ŀ¼
		int nResult = ff.FindFile(csPath);
		while(nResult &&(!stopFlag))
		{
			nResult = ff.FindNextFileW();
			if(ff.IsDirectory() && !ff.IsDots())
			{
			   // wcout << (LPCTSTR)ff.GetFilePath() << endl;		

				csNextPath += ff.GetFileName();
				csNextPath += _T("\\");
				TraversFile(csNextPath);
			}
			else if(!ff.IsDirectory() && !ff.IsDots())
			{
				CString str=(LPCTSTR)ff.GetFileName();
				std::string strStl,strTmp;
				//���ļ��������ַ�ת��
				USES_CONVERSION;
				strStl= T2A(findStr);
				strTmp=T2A(str);
				//���ƥ�䲻�ɹ����������һ��ѭ��,����д���ļ�
				if (!std::regex_match (strTmp, std::regex(strStl)))  
					 continue;
				CFile   file;
				file.Open(_T("queryfile.txt"),CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
				file.SeekToEnd();
				CString writeInfo;
				writeInfo.Format(_T("%s\%s"),csPrePath,str);
				file.Write(writeInfo,wcslen(writeInfo)*sizeof(wchar_t));
				file.Write(_T("\r\n"),4);
				//�ر��ĵ�
				file.Flush();
				file.Close();
			}
			csNextPath = csPrePath;
		}
	}catch(...)
	{
		AfxMessageBox(_T("�����ļ��쳣��"));
	}
}

//�����ļ�
void CSearchDlg::SearchFile()
{
	//
	CFile   file;
	file.Open(_T("queryfile.txt"),CFile::modeCreate|CFile::modeWrite);
	WORD unicode = 0xFEFF;
	file.Write(&unicode,2);	
	//�ر��ĵ�
	file.Flush();
	file.Close();

	 //��Ӷ�ʱ����ÿ��0.5s���
	SetTimer(TIME_COUNT,500,NULL);//��һ��������ʾ��ʱ����ID��
    //��ʼ����
	int hThread;
	DWORD ThreadID;
	//����createthread�����߳�
	hThread=(int)CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,(void*)this,0,&ThreadID);

	//while(!searchEndFlag);
	
}

void CSearchDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case TIME_COUNT:
		//���½�����״̬
		if(count<PROGRESS_MAX_LENGTH-1){
			count++;
			m_progress.SetPos(count);
			m_progress.StepIt();
		}
		/*else
		{
			m_progress.SetPos(PROGRESS_MAX_LENGTH);
			m_progress.StepIt();
		}*/
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

UINT ThreadProc(LPVOID pParam)
{
	CSearchDlg *dlg=(CSearchDlg*)pParam;
	CString queryStr=dlg->GetSearchValue();
	try{

		//CString cs = _T("F:\\");
		//��ȡ���е��߼����̷���
		DWORD diskSign=GetLogicalDrives();

		int i;
		CString str;	
		//����ÿ�������ļ�
		for(i=2;i<25;i++)
			if((diskSign>>i)&0x1==0x1)
			{
				str.Format(_T("%c:\\"),i+0x41);
				//AfxMessageBox(str);
				 //��ʼ����
				dlg->TraversFile(str);
			}
		//��ʼ����
		//dlg->TraversFile(cs);

		dlg->SendMessage(WM_END_SEARCH,0x0,0x0);

	}catch(...)
	{

		AfxMessageBox(_T("��ȡ�����ļ�ʧ�ܣ�"));

	}
	//
	dlg->SendMessage(WM_END_SEARCH,0x0,0x0);

	return 0;
}

LRESULT  CSearchDlg::OnMySearchMessage(WPARAM wParam, LPARAM lParam)
{
	SearchFile();

	return 0;
}

LRESULT  CSearchDlg::OnEndSearchMessage(WPARAM wParam, LPARAM lParam)
{
	//
	try{
		m_progress.SetPos(PROGRESS_MAX_LENGTH);
		m_progress.StepIt();
		//�رն�ʱ��
		KillTimer(TIME_COUNT);
		//Sleep(2000);
		//�˳���ǰ�Ի���
		CDialogEx::OnOK();
	}catch(...)
	{
		AfxMessageBox(_T("����ֹͣ�쳣��"));
	}
	return 0;
}

