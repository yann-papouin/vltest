#include "stdafx.h"
#include "GenViewer.h"
#include "ViewPaneView.h"
#include "MessageRecord.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconsView

CViewPaneView::CViewPaneView()
{
}

CViewPaneView::~CViewPaneView()
{
}


BEGIN_MESSAGE_MAP(CViewPaneView, CPaneView)
	//{{AFX_MSG_MAP(CIconsView)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_VIEW_REPORT , OnReportItemClick)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIconsView message handlers

CWnd* CViewPaneView::OnCreateView()
{
	if (!m_wndViewTree.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|WM_VSCROLL, CRect(0, 0, 0, 0), this, IDC_VIEW_REPORT))
	{
		TRACE(_T("Failed to create view window\n"));
		return NULL;
	}

	VERIFY(m_ilIcons.Create(16,16, ILC_COLOR24|ILC_MASK, 0, 1));
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_VIEW));
	m_ilIcons.Add(&bmp, RGB(255, 0, 255));

	m_wndViewTree.SetImageList(&m_ilIcons);

	m_wndViewTree.GetReportHeader()->AllowColumnRemove(FALSE);

	CXTPReportColumn* pColumn = m_wndViewTree.AddColumn(new CXTPReportColumn(0, _T("视图"), 200));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);

	pColumn->SetIconID(2);

	CMessageRecord *pParentRec1 = new CMessageRecord(_T("相机视图"), 1,FALSE); 
	m_wndViewTree.AddRecord(pParentRec1);
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("等轴测"), 0,FALSE)); 
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("顶部"), 1,FALSE)); 
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("底部"), 2,FALSE)); 
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("前"), 3,FALSE)); 
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("后"), 4,FALSE)); 
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("左"), 5,FALSE)); 
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("右"), 6,FALSE)); 
	pParentRec1->SetExpanded(TRUE);

	CMessageRecord *pParentRec2 = new CMessageRecord(_T("用户视图"),  1,FALSE); 
	m_wndViewTree.AddRecord(pParentRec2);


	m_wndViewTree.GetPaintManager()->SetColumnStyle(xtpReportColumnResource);
	m_wndViewTree.GetPaintManager()->SetGridStyle(FALSE,xtpReportGridNoLines);

	m_wndViewTree.AllowEdit(TRUE);
	m_wndViewTree.EditOnClick(FALSE);
	m_wndViewTree.SetMultipleSelection(FALSE);
	m_wndViewTree.SetTreeIndent(5);
	m_wndViewTree.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndViewTree.EnableToolTips(TRUE);

	m_wndViewTree.Populate();


	return &m_wndViewTree;
}

// Handles clicking ReportControl row.
// If "Checked" item is clicked, change "Checked" attribute for current record
void CViewPaneView::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
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

			if (strItemText.CompareNoCase(_T("等轴测")) == 0)
			{
				pWnd->SendMessage(WM_COMMAND, ID_VIEW_ISO, 0); 
			}
			else if (strItemText.CompareNoCase(_T("顶部")) == 0)
			{
				pWnd->SendMessage(WM_COMMAND, ID_VIEW_TOP, 0); 
			}
			else if (strItemText.CompareNoCase(_T("底部")) == 0)
			{
				pWnd->SendMessage(WM_COMMAND, ID_VIEW_BOTTOM, 0); 
			}
			else if (strItemText.CompareNoCase(_T("前")) == 0)
			{
				pWnd->SendMessage(WM_COMMAND, ID_VIEW_FRONT, 0); 
			}
			else if (strItemText.CompareNoCase(_T("后")) == 0)
			{
				pWnd->SendMessage(WM_COMMAND, ID_VIEW_BACK, 0); 
			}
			else if (strItemText.CompareNoCase(_T("左")) == 0)
			{
				pWnd->SendMessage(WM_COMMAND, ID_VIEW_LEFT, 0); 
			}
			else if (strItemText.CompareNoCase(_T("右")) == 0)
			{
				pWnd->SendMessage(WM_COMMAND, ID_VIEW_RIGHT, 0); 
			}
		}
	}
}

