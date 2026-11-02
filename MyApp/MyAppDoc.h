// MyAppDoc.h : interface of the CMyAppDoc class
//


#pragma once

class CMyAppDoc : public CDocument
{
protected: // create from serialization only
	CMyAppDoc();
	DECLARE_DYNCREATE(CMyAppDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMyAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


