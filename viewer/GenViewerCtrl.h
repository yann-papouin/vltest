#pragma once

#include "ActivDoc.h"
#include "HDB.h"
// GenViewerCtrl.h : Declaration of the CGenViewerCtrl ActiveX Control class.


// CGenViewerCtrl : See GenViewerCtrl.cpp for implementation.

class CGenViewerCtrl : public CActiveXDocControl
{
	DECLARE_DYNCREATE(CGenViewerCtrl)

// Constructor
public:
	CGenViewerCtrl();

// Overrides
public:
//	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CGenViewerCtrl();

	DECLARE_OLECREATE_EX(CGenViewerCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CGenViewerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CGenViewerCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CGenViewerCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidDocumentName = 1
	};

protected:
	void OnDocumentNameChanged(void);
	CString m_documentName;
	HDB* m_pHDB;
private:
	char m_AppDirectory[MVO_BUFFER_SIZE];
};

