// MyAppView.h : interface of the CMyAppView class
//


#pragma once


class CMyAppView : public CView
{
protected: // create from serialization only
	CMyAppView();
	DECLARE_DYNCREATE(CMyAppView)

// Attributes
public:
	CMyAppDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMyAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MyAppView.cpp
inline CMyAppDoc* CMyAppView::GetDocument() const
   { return reinterpret_cast<CMyAppDoc*>(m_pDocument); }
#endif

