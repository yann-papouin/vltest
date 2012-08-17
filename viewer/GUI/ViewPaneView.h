#if !defined(AFX_VIEWPANEVIEW_H__2F11BA39_DA1B_407C_B4A5_09487EE6249F__INCLUDED_)
#define AFX_VIEWPANEVIEW_H__2F11BA39_DA1B_407C_B4A5_09487EE6249F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneView.h"
/////////////////////////////////////////////////////////////////////////////
// CIconsView window

class CViewPaneView : public CPaneView
{
// Construction
public:
	CViewPaneView();

// Attributes
public:
	//CListCtrl m_wndIcons;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndViewTree;
	CImageList m_ilIcons;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconsView)
	//}}AFX_VIRTUAL
	CWnd* OnCreateView();

// Implementation
public:
	virtual ~CViewPaneView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconsView)
	//}}AFX_MSG
	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONSVIEW_H__2F11BA39_DA1B_407C_B4A5_09487EE6249F__INCLUDED_)
