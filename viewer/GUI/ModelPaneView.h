#if !defined(AFX_MODELPANEVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_)
#define AFX_MODELPANEVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneView.h"

/////////////////////////////////////////////////////////////////////////////
// CModelPaneView window

class CModelPaneView : public CPaneView
{
// Construction
public:
	CModelPaneView();

// Attributes
public:
//	CTreeCtrl   m_wndModelTree;
	CImageList m_ilIcons;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModelTree;

// Operations
public:
	CWnd* OnCreateView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelPaneView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModelPaneView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CModelPaneView)
	//}}AFX_MSG
	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnReportItemRClick(NMHDR * pNotifyStruct, LRESULT * result);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORERVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_)
