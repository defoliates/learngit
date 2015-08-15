
// SecretInfoView.h : CSecretInfoView ��Ľӿ�
//

#pragma once


class CSecretInfoView : public CView
{
protected: // �������л�����
	CSecretInfoView();
	DECLARE_DYNCREATE(CSecretInfoView)

// ����
public:
	CSecretInfoDoc* GetDocument() const;

// ����
public:
	CBrush m_brushBackground;
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CSecretInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // SecretInfoView.cpp �еĵ��԰汾
inline CSecretInfoDoc* CSecretInfoView::GetDocument() const
   { return reinterpret_cast<CSecretInfoDoc*>(m_pDocument); }
#endif

