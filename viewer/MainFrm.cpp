// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#ifdef _OCX
#include "GenViewer_ocx.h"
#else
#include "GenViewer.h"
#endif

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CComandBarsPaintManager : public CXTPOffice2003Theme
{
public:

	void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
	{
		if (DYNAMIC_DOWNCAST(CXTPToolBar, pBar) != 0 && pBar->GetCommandBars() == 0)
			pDC->FillSolidRect(CXTPClientRect(pBar), m_clrDockBar.clrLight);
		else
			CXTPOffice2003Theme::FillCommandBarEntry(pDC, pBar);
	}

	void RoundRect(CDC* pDC, CRect rc, COLORREF clr)
	{
		CXTPBrushDC brush(*pDC, clr);
		CXTPPenDC pen(*pDC, GetXtremeColor(XPCOLOR_MENUBAR_BORDER));
		pDC->RoundRect(rc.left, rc.top, rc.right, rc.bottom, 6, 6);

	}

	void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition)
	{
		if (bPopuped || !bEnabled || m_bLunaTheme)
		{
			CXTPOffice2003Theme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
			return;
		}
		if (bChecked && !bSelected && !bPressed) RoundRect(pDC, rc, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		else if (bChecked && bSelected && !bPressed) RoundRect(pDC, rc, m_clrDockBar.clrLight);
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed))  RoundRect(pDC, rc, m_clrDockBar.clrLight);
		else if (bSelected || bPressed)  RoundRect(pDC, rc, m_clrDockBar.clrDark);
	}
};



/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CXTPFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CXTPFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDR_PANE_ANNO, OnPaneAnno)
	ON_UPDATE_COMMAND_UI(IDR_PANE_ANNO, OnUpdatePaneAnno)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND_RANGE(IDR_PANE_MODEL,IDR_PANE_VIEW, OnShowView)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static unsigned int indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pPaneModel = NULL;
	m_pPaneView = NULL;
	m_pPaneAnno = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(unsigned int)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!InitCommandBars())
		return -1;

	XTPPaintManager()->SetCustomTheme(new CComandBarsPaintManager());

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagAddMDISysPopup);

	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	CXTPToolBar* pViewBar = (CXTPToolBar*)pCommandBars->Add(_T("View Bar"), xtpBarTop);
	if (!pViewBar ||
		!pViewBar->LoadToolBar(IDR_VIEW_BAR))
	{
		TRACE0("Failed to create view toolbar\n");
		return -1;
	}
	DockRightOf(pViewBar, pStandardBar);

	CXTPToolBar* pViewRenderBar = (CXTPToolBar*)pCommandBars->Add(_T("View Render Bar"), xtpBarTop);
	if (!pViewRenderBar ||
		!pViewRenderBar->LoadToolBar(IDR_VIEW_RENDER_BAR))
	{
		TRACE0("Failed to create view render toolbar\n");
		return -1;
	}
	DockRightOf(pViewRenderBar, pViewBar);//You need to clear the registry HKEY_CURRENT_USER\Software\Local AppWizard-Generated Applications when adding a new toolbar 

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

	XTPImageManager()->SetIcons(IDR_MAINFRAME);

	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetCustomTheme(new CDockingPanePaintManager());

	// Create docking panes.
	m_pPaneModel = m_paneManager.CreatePane(
		IDR_PANE_MODEL, CRect(0, 0,250, 120), xtpPaneDockLeft);

	m_pPaneView = m_paneManager.CreatePane(
		IDR_PANE_VIEW, CRect(0, 0,200, 120), xtpPaneDockBottom, m_pPaneModel);
	m_paneManager.AttachPane(m_pPaneView, m_pPaneModel);

	m_pPaneModel->Select();

	m_pPaneAnno = m_paneManager.CreatePane(
		IDR_PANE_ANNO, CRect(0, 0,200, 120), xtpPaneDockBottom, NULL);
	m_pPaneAnno->Close();


	m_paneManager.SetThemedFloatingFrames(TRUE);


	CXTPImageManager* pImageManager = pCommandBars->GetImageManager();

	pImageManager->InternalAddRef();
	m_paneManager.SetImageManager(pImageManager);

	m_paneManager.SetAlphaDockingContext(TRUE);
	m_paneManager.SetShowDockingContextStickers(TRUE);
	m_paneManager.SetShowContentsWhileDragging(TRUE);
	m_paneManager.SetDefaultPaneOptions(xtpPaneHasMenuButton);

	// Load the previous state for docking panes.
	//CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	//if (layoutNormal.Load(_T("NormalLayout")))
	//{
	//	m_paneManager.SetLayout(&layoutNormal);
	//}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.lpszClass = _T("XTPMainFrame");
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
		CS_DBLCLKS, AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CXTPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTPFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnPaneAnno() 
{
	if (m_pPaneAnno->IsClosed())
	{
		m_paneManager.ShowPane(m_pPaneAnno);
	}
	else
	{
		m_paneManager.ClosePane(m_pPaneAnno);
	}
}

void CMainFrame::OnUpdatePaneAnno(CCmdUI* pCmdUI) 
{
	if (m_pPaneAnno != NULL)
	{
		pCmdUI->SetCheck(!m_pPaneAnno->IsClosed());
	}
}

void CMainFrame::RefreshPanes()
{
}

void CMainFrame::OnShowView(unsigned int nID)
{
	m_paneManager.ShowPane(nID);
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_MODEL:
				pPane->Attach(m_wndModelPaneView.CreatePane(this));
				break;
			case IDR_PANE_VIEW:
				pPane->Attach(m_wndViewPaneView.CreatePane(this));
				break;
			case IDR_PANE_ANNO:
				pPane->Attach(m_wndAnnoPaneView.CreatePane(this));
				break;
			}
		}
		return 1;
	}
	return 0;
}

void CMainFrame::OnClose()
{
	//CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	//m_paneManager.GetLayout(&layoutNormal);
	//layoutNormal.Save(_T("NormalLayout"));

	//// Save the current state for toolbars and menus.
	//SaveCommandBars(_T("CommandBars"));
	CFrameWnd::OnClose();
}
