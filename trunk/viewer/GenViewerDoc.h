// pindockDoc.h : interface of the CGenViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINDOCKDOC_H__B775765E_9BE3_417D_84B7_22592711D9C9__INCLUDED_)
#define AFX_PINDOCKDOC_H__B775765E_9BE3_417D_84B7_22592711D9C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(HOOPS) && defined(ACIS) 
	#include "Renderer/HOOPS/HSolidModel.h"
#endif

class ViewerRenderer;

class CGenViewerDoc : public CDocument
{
protected: // create from serialization only
	CGenViewerDoc();
	DECLARE_DYNCREATE(CGenViewerDoc)

// Attributes
public:
	// Rendering
public:
	ViewerRenderer*	GetRenderer() const
	{
		return m_renderer;
	}
 //   CString GetFileName() const { return m_csFileName; }

protected:
	ViewerRenderer*	m_renderer;
//	CString m_csFileName;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGenViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGenViewerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
#if defined(HOOPS) && defined(ACIS) 
	HSolidModel* m_pHBaseModel;
#endif

	virtual void DeleteContents();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINDOCKDOC_H__B775765E_9BE3_417D_84B7_22592711D9C9__INCLUDED_)
