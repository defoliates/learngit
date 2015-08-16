// Show.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Temperature.h"
#include "Show.h"
#include "Serial.h"
#include<stdlib.h>
#include <time.h>
// CShow �Ի���

IMPLEMENT_DYNAMIC(CShow, CDialog)

CShow::CShow(CWnd* pParent /*=NULL*/)
	: CDialog(CShow::IDD, pParent)
	, m_tempval(_T(""))
	, m_tempAlarm(_T(""))
{

}

CShow::~CShow()
{
}

void CShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITTMPERATRUE, m_tempval);
	DDX_Text(pDX, IDC_EDITALARM, m_tempAlarm);
}


BEGIN_MESSAGE_MAP(CShow, CDialog)
	ON_EN_CHANGE(IDC_EDITTMPERATRUE, &CShow::OnEnChangeEdittmperatrue)
	ON_EN_CHANGE(IDC_EDITALARM, &CShow::OnEnChangeEditalarm)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_EDITALARM, &CShow::OnEnKillfocusEditalarm)
	ON_EN_UPDATE(IDC_EDITALARM, &CShow::OnEnUpdateEditalarm)
END_MESSAGE_MAP()


// CShow ��Ϣ�������
BOOL CShow::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetDlgItem(IDC_STATICSHOW)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_ctrlHistogram.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect, this, 1000);
	m_ctrlHistogram.SetRange(0, 100);
	m_ctrlHistogram.SetSpeed(CHistogramCtrl::NORMAL_SPEED);
	//m_ctrlHistogram.SetGridsColor(RGB(0, 0, 0));
	//m_ctrlHistogram.SetBkColor(RGB(255, 255, 255));
	//m_ctrlHistogram.SetPen(2, RGB(255, 0, 0));

	//���ö�ʱ�������������
	//srand(time(0)); /*��������,������α�������*/
	//
	m_tempAlarm.Format(_T("%d"),50);
	//SetTimer(0, 3000, NULL);
	return TRUE;
}
void CShow::OnEnChangeEdittmperatrue()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CShow::OnEnChangeEditalarm()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}

void CShow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//int nRandom = rand()%100;
	//m_ctrlHistogram.SetPos((int)nRandom);
	
	CDialog::OnTimer(nIDEvent);
}

void CShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CShow::SetTemperature(float temperature)
{
	m_ctrlHistogram.SetPos((int)temperature);
	//nRandom=(int)temperature;
	m_tempval.Format(_T("%.1f %cC"),temperature,0xB0);
	/*CString temp=_T("");
    if(temp!=m_tempval)
	{
     temp=m_tempval;
	 MessageBox(temp);
	}
	*/
	UpdateData(FALSE);
}

void CShow::OnEnKillfocusEditalarm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	UpdateData(TRUE);
	if(m_serialAlarm->isOpen)
		m_serialAlarm->SendAlarm(m_tempAlarm);
	*/
}

void CShow::OnEnUpdateEditalarm()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// �������� EM_SETEVENTMASK ��Ϣ���͵��ؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*
	UpdateData(TRUE);
	if(m_serialAlarm->isOpen)
		m_serialAlarm->SendAlarm(m_tempAlarm);
	*/
}
