// MyAppDoc.cpp : implementation of the CMyAppDoc class
//

#include "stdafx.h"
#include "MyApp.h"

#include "MyAppDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyAppDoc

IMPLEMENT_DYNCREATE(CMyAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyAppDoc, CDocument)
END_MESSAGE_MAP()


// CMyAppDoc construction/destruction

CMyAppDoc::CMyAppDoc()
{
	// TODO: add one-time construction code here

}

CMyAppDoc::~CMyAppDoc()
{
}

BOOL CMyAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMyAppDoc serialization

void CMyAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMyAppDoc diagnostics

#ifdef _DEBUG
void CMyAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMyAppDoc commands
