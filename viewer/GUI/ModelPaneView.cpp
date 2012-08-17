#include "stdafx.h"
#include "GenViewer.h"
#include "ModelPaneView.h"
#include "MainFrm.h"
#include "MessageRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelPaneView

CModelPaneView::CModelPaneView()
{
//	m_nToolBarID = IDR_EXPLORER_BAR;
	m_nToolBarID = 0;
}

CModelPaneView::~CModelPaneView()
{
}


BEGIN_MESSAGE_MAP(CModelPaneView, CPaneView)
	//{{AFX_MSG_MAP(CModelPaneView)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_MODEL_REPORT , OnReportItemClick)
	ON_NOTIFY(NM_RCLICK, IDC_MODEL_REPORT, OnReportItemRClick)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModelPaneView message handlers

CWnd* CModelPaneView::OnCreateView()
{

	if (!m_wndModelTree.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|WM_VSCROLL, CRect(0, 0, 0, 0), this, IDC_MODEL_REPORT))
	{
		TRACE(_T("Failed to create view window\n"));
		return NULL;
	}

	VERIFY(m_ilIcons.Create(16,16, ILC_COLOR24|ILC_MASK, 0, 1));
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_FALGS));
	m_ilIcons.Add(&bmp, RGB(255, 0, 255));

	m_wndModelTree.SetImageList(&m_ilIcons);

	m_wndModelTree.GetReportHeader()->AllowColumnRemove(FALSE);

	CXTPReportColumn* pColumn = m_wndModelTree.AddColumn(new CXTPReportColumn(0, _T("模型树"), 200));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);

	m_wndModelTree.AddColumn(new CXTPReportColumn(1,_T("截面"), _T("截面"), 25,FALSE,5,FALSE));

	pColumn = m_wndModelTree.AddColumn(new CXTPReportColumn(2,_T("操纵"), _T("操纵"), 25,FALSE,6,FALSE));
	pColumn->SetEditable(FALSE);

	pColumn = m_wndModelTree.AddColumn(new CXTPReportColumn(3,_T("透明度"), _T("透明度"), 25,FALSE,7,FALSE));
	pColumn->SetEditable(FALSE);

	CMessageRecord *pParentRec1 = new CMessageRecord(_T("装配1") ,0,TRUE,TRUE,FALSE, FALSE, FALSE); 
	m_wndModelTree.AddRecord(pParentRec1);
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("零件"), 2,TRUE,TRUE,FALSE, FALSE, FALSE)); 
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("零件"),2,TRUE,TRUE,FALSE, FALSE, FALSE)); 
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("零件"), 2,TRUE,TRUE,FALSE, FALSE, FALSE)); 

	pParentRec1->SetExpanded(TRUE);

	CMessageRecord *pParentRec2 = new CMessageRecord(_T("子装配1"),1, TRUE,TRUE,FALSE, FALSE, FALSE); 
	pParentRec1->GetChilds()->Add(pParentRec2);
	pParentRec2->GetChilds()->Add(new CMessageRecord(_T("零件4"), 2, TRUE,TRUE,FALSE, FALSE, FALSE));
	pParentRec2->SetExpanded(TRUE);


	m_wndModelTree.GetPaintManager()->SetColumnStyle(xtpReportColumnResource);
	m_wndModelTree.GetPaintManager()->SetGridStyle(FALSE,xtpReportGridNoLines);

	m_wndModelTree.AllowEdit(TRUE);
	m_wndModelTree.EditOnClick(FALSE);
	m_wndModelTree.SetMultipleSelection(FALSE);
	m_wndModelTree.SetTreeIndent(10);
	m_wndModelTree.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndModelTree.EnableToolTips(TRUE);
	//m_wndModelTree.EnableTrackingToolTips(TRUE);

	m_wndModelTree.Populate();

	return &m_wndModelTree;
}

// Handles clicking ReportControl row.
// If "Checked" item is clicked, change "Checked" attribute for current record
void CModelPaneView::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	TRACE(_T("Click on row %d, col %d\n"),
		pItemNotify->pRow->GetIndex(), pItemNotify->pColumn->GetItemIndex());

	//MD - another way to fix wrong reaction on column checkbox click - comment line GetReportCtrl().Populate();
	//	instead of 	adding condition if (!bCheckBoxClicked)in function CXTPReportRecordItem::OnClick
	//if (pItemNotify->pColumn->GetItemIndex() == COLUMN_CHECK)
	//{
	//	//		GetReportCtrl().Populate();
	//	//Here we can have more reasonable reaction on Checked / Unchecked field state changed
	//}


	CWnd* pWnd = ((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	if (pWnd != NULL)
	{
		CString strItemText;
		CMessageRecordItemIconCheckText* pRecordItem = dynamic_cast<CMessageRecordItemIconCheckText*>(pItemNotify->pItem);

		if (pRecordItem != NULL)
		{
			strItemText = pRecordItem->GetValue();
		}
	}
}




// When user does a right click on row with a group name, generate popup menu
// Procedure contains handlers of pressing all buttons in popup menu
void CModelPaneView::OnReportItemRClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow)
		return;

	//if (pItemNotify->pRow->IsGroupRow())
	//{
	//	CMenu menu;
	//	VERIFY(menu.LoadMenu(IDR_MENU_CONTEXT_GROUP));

	//	// track menu
	//	int nMenuResult = CXTPCommandBars::TrackPopupMenu(menu.GetSubMenu(0), TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN |TPM_RIGHTBUTTON, pItemNotify->pt.x, pItemNotify->pt.y, this, NULL);

	//	// general items processing
	//	switch (nMenuResult)
	//	{
	//	case ID_POPUP_COLLAPSEALLGROUPS:
	//		pItemNotify->pRow->GetControl()->CollapseAll();
	//		break;
	//	case ID_POPUP_EXPANDALLGROUPS:
	//		pItemNotify->pRow->GetControl()->ExpandAll();
	//		break;
	//	}
	//} 
	//else
	//{
	//}
}