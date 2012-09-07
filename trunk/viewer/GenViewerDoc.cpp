// pindockDoc.cpp : implementation of the CGenViewerDoc class
//

#include "stdafx.h"

#ifdef _OCX
#include "GenViewer_ocx.h"
#else
#include "GenViewer.h"
#endif

#include "MainFrm.h"
#include "GenViewerDoc.h"
#include "GenViewerView.h"

#include "Renderer/ViewerRenderer.h"
#ifdef HOOPS
//#include "Renderer/HOOPS/ViewerHoopsRenderer.h"
#elif VL
//#include "Renderer/VL/ViewerVLRenderer.h"
#elif OSG
#include "Renderer/OSG/ViewerOsgRenderer.h"
#else
#include "Renderer/OpenGL/ViewerOpenGLRenderer.h"
#endif 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenViewerDoc

IMPLEMENT_DYNCREATE(CGenViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGenViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CGenViewerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenViewerDoc construction/destruction

CGenViewerDoc::CGenViewerDoc()
{
	// TODO: add one-time construction code here

	m_renderer = NULL;

#ifdef HOOPS
//	m_renderer = new ViewerHoopsRenderer();
#elif OSG
	m_renderer = new ViewerOsgRenderer();
#elif VL
//	m_renderer = new ViewerVLRenderer();
#endif

}

CGenViewerDoc::~CGenViewerDoc()
{
#if defined(HOOPS) && defined(ACIS) 
	if( m_pHBaseModel != NULL )
	{
		delete m_pHBaseModel;
		m_pHBaseModel = NULL;
	}
#endif

	if (NULL != m_renderer)
	{
		delete m_renderer;
		m_renderer = NULL;
	}
}

BOOL CGenViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

#if defined(HOOPS) && defined(ACIS) 
	// (SDI documents will reuse this document)
	m_pHBaseModel = new HSolidModel();
	m_pHBaseModel->Init();
#endif
	return TRUE;
}

BOOL CGenViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
//	m_csFileName = lpszPathName;

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// 8/23/2012 mwu :  
	CMainFrame *pMainFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	if(pMainFrame != NULL)
	{
		CGenViewerView *pView = (CGenViewerView*)pMainFrame->GetActiveView();
		if (pView != NULL)
		{
			pView->OnOpenDocument(lpszPathName);
			//pView->SetModify(FALSE);
			//ASSERT(!pView->GetModify());
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGenViewerDoc serialization

void CGenViewerDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CGenViewerDoc diagnostics

#ifdef _DEBUG
void CGenViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGenViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

void CGenViewerDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	CDocument::DeleteContents();

	CMainFrame *pMainFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	if(pMainFrame != NULL)
	{
		CGenViewerView *pView = (CGenViewerView*)pMainFrame->GetActiveView();
		if (pView != NULL)
		{
			pView->DeleteContents();
		}
	}
}
