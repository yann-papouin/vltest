/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_)
#define AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModelPaneView;

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying checkboxes.
class CMessageRecordItemCheck : public CXTPReportRecordItemText
{
	DECLARE_SERIAL(CMessageRecordItemCheck)
public:
	// Constructs record item with the initial checkbox value.
	CMessageRecordItemCheck(BOOL bCheck = FALSE);

	// Provides custom group captions depending on checkbox value.
	// Returns caption string ID to be read from application resources.
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);

	// Provides custom records comparison by this item based on checkbox value, 
	// instead of based on captions.
	virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};

////////////////////////////////////////////////////////////////////////////
//// Customized record item, used for displaying read/unread icons.
//class CMessageRecordItemIconText : public CXTPReportRecordItemText
//{
//	DECLARE_SERIAL(CMessageRecordItemIconText)
//public:
//	// Constructs record item with the initial read/unread value.
//	CMessageRecordItemIconText(CString szText = _T("ss"),unsigned int nIconID = 0);
//};


//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying read/unread icons.
class CMessageRecordItemIconCheckText : public CXTPReportRecordItemText
{
	DECLARE_SERIAL(CMessageRecordItemIconCheckText)
public:
	// Constructs record item with the initial read/unread value.
	CMessageRecordItemIconCheckText(CString szText = _T("ss"),unsigned int nIconID = 0, BOOL bHasCheckBox = TRUE, BOOL bRead = FALSE);

	virtual void DoPropExchange(CXTPPropExchange* pPX);
public:
	BOOL m_bRead;	// TRUE for read, FALSE for unread.
};

//////////////////////////////////////////////////////////////////////////
// This class is your main custom Record class which you'll manipulate with.
// It contains any kind of specific methods like different types of constructors,
// any additional custom data as class members, any data manipulation methods.
class CMessageRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CMessageRecord)
public:
	
	// Construct record object using empty values on each field
	CMessageRecord();
	
	// Construct record object from detailed values on each field
	CMessageRecord( 	 CString strName, unsigned int nIconID = 0, BOOL bHasCheckBox = TRUE,BOOL bCheck = TRUE,BOOL bCuttingPlane = FALSE, BOOL bManipulate = FALSE, BOOL bTransparent = FALSE );

	// Clean up internal objects
	virtual ~CMessageRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

protected:
	CMessageRecordItemIconCheckText *m_pItemName;     // name
};

#endif // !defined(AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_)
