/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "GenViewer.h"
#include "MessageRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(CMessageRecord,               CXTPReportRecord,             VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CMessageRecordItemCheck,      CXTPReportRecordItemText,     VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CMessageRecordItemIconCheckText,       CXTPReportRecordItemText,         VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemCheck

CMessageRecordItemCheck::CMessageRecordItemCheck(BOOL bCheck)
{
	HasCheckbox(TRUE);
	SetChecked(bCheck);
}

int CMessageRecordItemCheck::GetGroupCaptionID(CXTPReportColumn* /*pColumn*/)
{
	return IsChecked()? IDS_GROUP_CHECKED_TRUE: IDS_GROUP_CHECKED_FALSE;
}

int CMessageRecordItemCheck::Compare(CXTPReportColumn* /*pColumn*/, CXTPReportRecordItem* pItem)
{
	return int(IsChecked()) - int(pItem->IsChecked());
}




///**
//* CMessageRecordItemIconText
//*/
//
//CMessageRecordItemIconText::CMessageRecordItemIconText(CString szText, unsigned int nIconID)
//{
//	SetIconIndex(nIconID);
//	SetValue(szText);
//}


///**
//* CMessageRecordItemIconCheckText
//*/

CMessageRecordItemIconCheckText::CMessageRecordItemIconCheckText(CString szText,unsigned int nIconID, BOOL bHasCheckBox, BOOL bCheck)
: m_bRead(bCheck)
{
	SetValue(szText);

	HasCheckbox(bHasCheckBox);
	SetChecked(bCheck);

	SetIconIndex(nIconID);
}

void CMessageRecordItemIconCheckText::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItem::DoPropExchange(pPX);

	PX_Bool(pPX, _T("Read"), m_bRead);
}



//////////////////////////////////////////////////////////////////////
// CMessageRecord class

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessageRecord::CMessageRecord()
{
	m_pItemName = NULL;

	CreateItems();
}

CMessageRecord::CMessageRecord(
			   CString strName, unsigned int nIconID, BOOL bHasCheckBox,BOOL bCheck,BOOL bCuttingPlane, BOOL bManipulate, BOOL bTransparent
			   )
{

	m_pItemName = new CMessageRecordItemIconCheckText(strName,nIconID,bHasCheckBox,bCheck);
	AddItem(m_pItemName);

	CMessageRecordItemCheck *pItemCheckCuttingPlane = new CMessageRecordItemCheck(bCuttingPlane);
	pItemCheckCuttingPlane->SetCanEditCheckboxItem(TRUE);
	AddItem(pItemCheckCuttingPlane);

	CMessageRecordItemCheck *pItemCheckManipulate = new CMessageRecordItemCheck(bManipulate);
	pItemCheckManipulate->SetCanEditCheckboxItem(TRUE);
	AddItem(pItemCheckManipulate);

	CMessageRecordItemCheck *pItemCheckTransparent = new CMessageRecordItemCheck(bTransparent);
	pItemCheckTransparent->SetCanEditCheckboxItem(TRUE);
	AddItem(pItemCheckTransparent);

}

void CMessageRecord::CreateItems()
{
	AddItem(new CMessageRecordItemIconCheckText(_T("")));
	AddItem(new CMessageRecordItemCheck(TRUE));
	AddItem(new CMessageRecordItemCheck(TRUE));
	AddItem(new CMessageRecordItemCheck(TRUE));

	SetPreviewItem(new CXTPReportRecordItemPreview(_T("")));
}

CMessageRecord::~CMessageRecord()
{

}

void CMessageRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	CXTPReportRecord::GetItemMetrics(pDrawArgs, pItemMetrics);
}
