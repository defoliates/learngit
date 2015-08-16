// TemperatureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Temperature.h"
#include "TemperatureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTemperatureDlg �Ի���




CTemperatureDlg::CTemperatureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTemperatureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTemperatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_MSCOMM, m_mscomm);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CTemperatureDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CTemperatureDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CTemperatureDlg ��Ϣ�������

BOOL CTemperatureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	m_tab.InsertItem(0,_T("��������"));  //��Ӵ�������ѡ�
	m_tab.InsertItem(1,_T("������ʾ"));    //���������ʾѡ�
	//
	//�����Ի���,���ҽ�IDC_TABTEST�ؼ���Ϊ������
	m_serial.Create(IDD_SERIAL,GetDlgItem(IDC_TAB));
	m_show.Create(IDD_SHOW,GetDlgItem(IDC_TAB));
	m_serial.m_showData=&m_show;//------------
	m_show.m_serialAlarm=&m_serial;
	//���IDC_TABTEST�ͻ�����С
	 CRect rs,rc;
	 m_tab.GetClientRect(&rs);

	 //�����ӶԻ����ڸ������е�λ��
	 rs.top+=20;
	 //rs.bottom-=15;
	 rs.left-=20;
	 rs.right-=2;
	 
	 //�����ӶԻ���ߴ粢�ƶ���ָ��λ��
	 m_serial.MoveWindow(&rs);
	 m_show.MoveWindow(&rs);
 
	 //�ֱ��������غ���ʾ
	 m_serial.ShowWindow(true);
	 m_show.ShowWindow(false);
 
	 //����Ĭ�ϵ�ѡ�
	 m_tab.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTemperatureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTemperatureDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTemperatureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTemperatureDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 int CurSel = m_tab.GetCurSel();

	 switch(CurSel)
	 {
		 case 0:
			 m_serial.ShowWindow(true);
			 m_show.ShowWindow(false);
		 break;
		 case 1:
			 m_serial.ShowWindow(false);
			 m_show.ShowWindow(true);
		 default:    
					;
		}    

	*pResult = 0;
}
/*
CShow CTemperatureDlg::GetShowDlg()
{
	return m_show;
}
*/