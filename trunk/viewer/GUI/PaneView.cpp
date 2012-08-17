#include "stdafx.h"
#include "GenViewer.h"
#include "PaneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneView

CPaneView::CPaneView()
{
	m_pView = 0;

	m_nToolBarID = 0;
}

CPaneView::~CPaneView()
{
}


BEGIN_MESSAGE_MAP(CPaneView, CWnd)
	//{{AFX_MSG_MAP(CPaneView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaneView message handlers

int CPaneView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pView = OnCreateView();

	if (m_nToolBarID != 0)
	{
		VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE | WS_CHILD | 
			CBRS_TOOLTIPS | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, this));
		VERIFY(m_wndToolBar.LoadToolBar(m_nToolBarID));
		m_wndToolBar.SetButtonSize(CSize(23, 23));
	}

	return 0;
}


CWnd* CPaneView::OnCreateView()
{
	if (!m_wndView.Create(WS_CHILD|WS_VISIBLE|LVS_ICON, CXTPEmptyRect(), this, 0))
		return NULL;

	return &m_wndView;
}

void CPaneView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CSize sz(0);

	if (m_wndToolBar.GetSafeHwnd())
	{

		sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, 0, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
	}

	if (m_pView && m_pView->GetSafeHwnd())
	{
		m_pView->MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}

}


CWnd* CPaneView::CreatePane(CWnd* pParentWnd)
{
	if (GetSafeHwnd() == 0)
	{
		VERIFY(Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), pParentWnd, 0));
	}
	return this;
}


IMPLEMENT_SERIAL(CXTPReportRecordItemCheck, CXTPReportRecordItemControl, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemButton

CXTPReportRecordItemCheck::CXTPReportRecordItemCheck(LPCTSTR szCaption)
: CXTPReportRecordItemControl(szCaption)
{
	m_nState = m_nSavedState = PBS_NORMAL;
}

void CXTPReportRecordItemCheck::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItemControl::DoPropExchange(pPX);
}

void CXTPReportRecordItemCheck::OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	UNREFERENCED_PARAMETER(pClickArgs);

	if (GetEnable())
		m_nState = PBS_PRESSED;
}

void CXTPReportRecordItemCheck::OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	//UNREFERENCED_PARAMETER(pClickArgs);
	//>>attempt for 23588 issue case
	if (pClickArgs
		&& pClickArgs->pControl
		&& pClickArgs->pControl->IsEditMode())
		return;
	//>>attempt for 23588 issue case

	m_nState = GetEnable() ? PBS_NORMAL : PBS_DISABLED;
	if (m_rcControl.PtInRect(pClickArgs->ptClient)
		&& pClickArgs->pControl
		&& m_rcControl.PtInRect(pClickArgs->pControl->m_mouseDownState.ptMouse))
	{
		XTP_NM_REPORTITEMCONTROL nm;
		::ZeroMemory(&nm, sizeof(nm));
		nm.pItem = pClickArgs->pItem;
		nm.pRow = pClickArgs->pRow;
		nm.pColumn = pClickArgs->pColumn;
		nm.pt = pClickArgs->ptClient;
		nm.pItemControl = this;
		pClickArgs->pControl->SendNotifyMessage(XTP_NM_REPORT_ITEMBUTTONCLICK, (NMHDR*)&nm);
	}
}

void CXTPReportRecordItemCheck::OnMouseEnter(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	if (m_nSavedState == PBS_PRESSED && (nFlags & MK_LBUTTON))
		m_nState = m_nSavedState;
}

void CXTPReportRecordItemCheck::OnMouseLeave(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);
	m_nSavedState = m_nState;
	m_nState = GetEnable() ? PBS_NORMAL : PBS_DISABLED;
}

void CXTPReportRecordItemCheck::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rcControl.PtInRect(point) && m_nSavedState == PBS_PRESSED && (nFlags & MK_LBUTTON))
		m_nState = m_nSavedState;
}


