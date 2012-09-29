// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__61653CAB_65AC_43E0_A857_0A9507861E6C__INCLUDED_)
#define AFX_MAINFRM_H__61653CAB_65AC_43E0_A857_0A9507861E6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GUI/DockingPaneManager.h"
#include "GUI/ModelPaneView.h"
#include "GUI/ViewPaneView.h"
#include "GUI/AnnoPaneView.h"

class CMainFrame : public CXTPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CImageList m_ImgList;

	CDockingPaneManager m_paneManager;

	CModelPaneView m_wndModelPaneView;
	CViewPaneView m_wndViewPaneView;
	CAnnoPaneView m_wndAnnoPaneView;

	CXTPDockingPane* m_pPaneModel;
	CXTPDockingPane* m_pPaneView;
	CXTPDockingPane* m_pPaneAnno;

	LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	void RefreshPanes();

	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CXTPToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowView(unsigned int nID);
	afx_msg void OnPaneAnno();
	afx_msg void OnUpdatePaneAnno(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

AFX_INLINE void RefreshPanes()
{
	((CMainFrame*)AfxGetMainWnd())->RefreshPanes();
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__61653CAB_65AC_43E0_A857_0A9507861E6C__INCLUDED_)
