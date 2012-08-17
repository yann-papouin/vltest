#include "stdafx.h"
#include "GenViewer.h"
#include "AnnoPaneView.h"

#include "MessageRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnnoPaneView

CAnnoPaneView::CAnnoPaneView()
{
	m_nToolBarID = 0;
}

CAnnoPaneView::~CAnnoPaneView()
{
}


BEGIN_MESSAGE_MAP(CAnnoPaneView, CPaneView)
	//{{AFX_MSG_MAP(CAnnoPaneView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAnnoPaneView message handlers

CWnd* CAnnoPaneView::OnCreateView()
{
	if (!m_wndAnnoTree.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|WM_VSCROLL, CRect(0, 0, 0, 0), this, IDC_ANNO_REPORT))
	{
		TRACE(_T("Failed to create view window\n"));
		return NULL;
	}

	VERIFY(m_ilIcons.Create(16,16, ILC_COLOR24|ILC_MASK, 0, 1));
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_FALGS));
	m_ilIcons.Add(&bmp, RGB(255, 0, 255));

	m_wndAnnoTree.SetImageList(&m_ilIcons);

	m_wndAnnoTree.GetReportHeader()->AllowColumnRemove(FALSE);

	CXTPReportColumn* pColumn = m_wndAnnoTree.AddColumn(new CXTPReportColumn(0, _T("标记实体"), 200));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);

	m_wndAnnoTree.AddColumn(new CXTPReportColumn(1,_T("作者"), _T("作者"), 60,TRUE,XTP_REPORT_NOICON,FALSE));

	pColumn = m_wndAnnoTree.AddColumn(new CXTPReportColumn(2,_T("上次修改"), _T("上次修改"), 80,TRUE,XTP_REPORT_NOICON,FALSE));
	pColumn->SetEditable(FALSE);

	CMessageRecord *pParentRec1 = new CMessageRecord(_T("标注分组1") ,0,TRUE,TRUE,FALSE, FALSE, FALSE); 
	m_wndAnnoTree.AddRecord(pParentRec1);
	pParentRec1->GetChilds()->Add(new CMessageRecord(_T("标注"), 2,TRUE,TRUE,FALSE, FALSE, FALSE)); 

	pParentRec1->SetExpanded(TRUE);

	m_wndAnnoTree.GetPaintManager()->SetColumnStyle(xtpReportColumnResource);
	m_wndAnnoTree.GetPaintManager()->SetGridStyle(FALSE,xtpReportGridNoLines);

	m_wndAnnoTree.AllowEdit(TRUE);
	m_wndAnnoTree.EditOnClick(FALSE);
	m_wndAnnoTree.SetMultipleSelection(FALSE);
	m_wndAnnoTree.SetTreeIndent(10);
	m_wndAnnoTree.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndAnnoTree.EnableToolTips(TRUE);

	m_wndAnnoTree.Populate();

	return &m_wndAnnoTree;
}
