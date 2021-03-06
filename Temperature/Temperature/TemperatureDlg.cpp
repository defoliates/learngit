// TemperatureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Temperature.h"
#include "TemperatureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTemperatureDlg 对话框




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


// CTemperatureDlg 消息处理程序

BOOL CTemperatureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_tab.InsertItem(0,_T("串口设置"));  //添加串口设置选项卡
	m_tab.InsertItem(1,_T("数据显示"));    //添加数据显示选项卡
	//
	//关联对话框,并且将IDC_TABTEST控件设为父窗口
	m_serial.Create(IDD_SERIAL,GetDlgItem(IDC_TAB));
	m_show.Create(IDD_SHOW,GetDlgItem(IDC_TAB));
	m_serial.m_showData=&m_show;//------------
	m_show.m_serialAlarm=&m_serial;
	//获得IDC_TABTEST客户区大小
	 CRect rs,rc;
	 m_tab.GetClientRect(&rs);

	 //调整子对话框在父窗口中的位置
	 rs.top+=20;
	 //rs.bottom-=15;
	 rs.left-=20;
	 rs.right-=2;
	 
	 //设置子对话框尺寸并移动到指定位置
	 m_serial.MoveWindow(&rs);
	 m_show.MoveWindow(&rs);
 
	 //分别设置隐藏和显示
	 m_serial.ShowWindow(true);
	 m_show.ShowWindow(false);
 
	 //设置默认的选项卡
	 m_tab.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTemperatureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTemperatureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTemperatureDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
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