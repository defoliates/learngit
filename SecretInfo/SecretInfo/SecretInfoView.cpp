
// SecretInfoView.cpp : CSecretInfoView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "SecretInfo.h"
#endif

#include "SecretInfoDoc.h"
#include "SecretInfoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSecretInfoView

IMPLEMENT_DYNCREATE(CSecretInfoView, CView)

BEGIN_MESSAGE_MAP(CSecretInfoView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSecretInfoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CSecretInfoView ����/����

CSecretInfoView::CSecretInfoView()
{
	// TODO: �ڴ˴���ӹ������
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP); ///����λͼ
   m_brushBackground.CreatePatternBrush(&bmp); ///����λͼ��ˢ
}

CSecretInfoView::~CSecretInfoView()
{
}

BOOL CSecretInfoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSecretInfoView ����

void CSecretInfoView::OnDraw(CDC* pDC)
{
	CSecretInfoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	/*CRect rect;
	GetClientRect(rect);///ȡ�ÿͻ�����
   pDC->FillRect(rect,&m_brushBackground); ///�ñ�����ˢ�������
   */
	 CRect rctClient;
	 GetClientRect(rctClient);
	 CDC dcMem;
	 dcMem.CreateCompatibleDC(pDC);
	 CBitmap m_bmp;
	 m_bmp.LoadBitmap(IDB_BACKGROUND);
	 BITMAP bitmap;
	 m_bmp.GetBitmap(&bitmap);
	 CBitmap   *pbmpOld=dcMem.SelectObject(&m_bmp); 
	 pDC->StretchBlt(0,0,rctClient.Width(),rctClient.Height(),&dcMem,0,0, 
	  bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
}


// CSecretInfoView ��ӡ


void CSecretInfoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSecretInfoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSecretInfoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSecretInfoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CSecretInfoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSecretInfoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSecretInfoView ���

#ifdef _DEBUG
void CSecretInfoView::AssertValid() const
{
	CView::AssertValid();
}

void CSecretInfoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSecretInfoDoc* CSecretInfoView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSecretInfoDoc)));
	return (CSecretInfoDoc*)m_pDocument;
}
#endif //_DEBUG


// CSecretInfoView ��Ϣ�������


BOOL CSecretInfoView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}
