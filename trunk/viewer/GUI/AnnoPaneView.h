#if !defined(AFX_ANNOPANEVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_)
#define AFX_ANNOPANEVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneView.h"

/////////////////////////////////////////////////////////////////////////////
// CAnnoPaneView window

class CAnnoPaneView : public CPaneView
{
// Construction
public:
	CAnnoPaneView();

// Attributes
public:
	CImageList m_ilIcons;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndAnnoTree;

// Operations
public:
	CWnd* OnCreateView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnnoPaneView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnnoPaneView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAnnoPaneView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORERVIEW_H__38375D16_655F_4C83_BDEC_60CC1986A706__INCLUDED_)
