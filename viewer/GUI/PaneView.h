#if !defined(AFX_PANEVIEW_H__31D830D9_23DE_4FA5_9213_3FC8E271E0CD__INCLUDED_)
#define AFX_PANEVIEW_H__31D830D9_23DE_4FA5_9213_3FC8E271E0CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPaneView window


class CPaneView : public CWnd
{
// Construction
public:
	CPaneView();

// Attributes
public:
	CWnd* m_pView;

	CListCtrl m_wndView;
	int m_nToolBarID;


	CXTPToolBar m_wndToolBar;

// Operations
public:

	virtual CWnd* CPaneView::OnCreateView();


	CWnd* CreatePane(CWnd* pParentWnd);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaneView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaneView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaneView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//===========================================================================
// Summary:
//     This Class represents an item button and inherits basic functionality
//     from CXTPReportRecordItemControl class.
//     You create a button simply by calling a constructor with one
//     parameter - text string.
// See Also: CXTPReportRecordItemControl
//===========================================================================
class CXTPReportRecordItemCheck : public CXTPReportRecordItemControl
{
	DECLARE_SERIAL(CXTPReportRecordItemCheck)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an item button.
	// Parameters:
	//     szCaption - The button caption.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemCheck(LPCTSTR szCaption = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report item control
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnLButtonDown function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnLButtonUp function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that mouse cursor has entered the item controls's area.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseEnter(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that mouse cursor has left the item controls's area.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseLeave(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

protected:
	int m_nSavedState; // Button state when the mouse cursor leaves the button area.

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANEVIEW_H__31D830D9_23DE_4FA5_9213_3FC8E271E0CD__INCLUDED_)
