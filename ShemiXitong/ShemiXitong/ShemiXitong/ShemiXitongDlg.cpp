
// ShemiXitongDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShemiXitong.h"
#include "ShemiXitongDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CShemiXitongDlg �Ի���




CShemiXitongDlg::CShemiXitongDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShemiXitongDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShemiXitongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CShemiXitongDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CShemiXitongDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CShemiXitongDlg ��Ϣ�������

BOOL CShemiXitongDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_tab.InsertItem(0,_T("����"));
	m_tab.InsertItem(1,_T("�鿴"));
	m_tab.InsertItem(2,_T("���ܴʿ�"));
	m_tab.InsertItem(3,_T("����"));
	//�����Ի���,���ҽ�IDC_TAB�ؼ���Ϊ������
	m_taskDlg.Create(IDD_TASK,GetDlgItem(IDC_TAB));
	m_lookUpDlg.Create(IDD_LOOKUP,GetDlgItem(IDC_TAB));
	m_wordNetDlg.Create(IDD_WORDNET,GetDlgItem(IDC_TAB));
	m_helpDlg.Create(IDD_HELP,GetDlgItem(IDC_TAB));
		//���IDC_TAB�ͻ�����С
	 CRect rs,rc;
	 m_tab.GetClientRect(&rs);

	 //�����ӶԻ����ڸ������е�λ��
	 rs.top+=25;
	 rs.bottom+=40;
	 rs.left-=10;
	// rs.right-=2;
	 
	 //�����ӶԻ���ߴ粢�ƶ���ָ��λ��
	 m_lookUpDlg.MoveWindow(&rs);
	 m_taskDlg.MoveWindow(&rs);
	 m_wordNetDlg.MoveWindow(&rs);
	 m_helpDlg.MoveWindow(&rs);
	 
	 //�ֱ��������غ���ʾ
	 m_taskDlg.ShowWindow(true);
	 m_lookUpDlg.ShowWindow(false);
	 m_wordNetDlg.ShowWindow(false);
	 m_helpDlg.ShowWindow(false);

	 //����Ĭ�ϵ�ѡ�
	 m_tab.SetCurSel(0);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CShemiXitongDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CShemiXitongDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CShemiXitongDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CShemiXitongDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

		//��ȡ��ǰѡ�����
	 int CurSel = m_tab.GetCurSel();
	 //����ѡ��л�״̬
	 switch(CurSel)
	 {
		 case 0:
			 m_taskDlg.ShowWindow(true);
			 m_lookUpDlg.ShowWindow(false);
			 m_wordNetDlg.ShowWindow(false);
			 m_helpDlg.ShowWindow(false);

			break;
		 case 1:
			 m_taskDlg.ShowWindow(false);
			 m_lookUpDlg.ShowWindow(true);
			 //����������ʾ
			 m_lookUpDlg.UpdateGridInfo();

			 m_wordNetDlg.ShowWindow(false);
			 m_helpDlg.ShowWindow(false);
			  break;
		 case 2:
			  m_taskDlg.ShowWindow(false);
			 m_lookUpDlg.ShowWindow(false);
			 //����������ʾ
			 m_wordNetDlg.ShowWindow(true);
			 m_helpDlg.ShowWindow(false);
			 break;
		 case 3:
			  m_taskDlg.ShowWindow(false);
			 m_lookUpDlg.ShowWindow(false);
			 m_wordNetDlg.ShowWindow(false);
			 m_helpDlg.ShowWindow(true);
			  break;
		 default:    
					;
		}  
}
