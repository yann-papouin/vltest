#pragma once

// GenViewerPropPage.h : Declaration of the CGenViewerPropPage property page class.


// CGenViewerPropPage : See GenViewerPropPage.cpp for implementation.

class CGenViewerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CGenViewerPropPage)
	DECLARE_OLECREATE_EX(CGenViewerPropPage)

// Constructor
public:
	CGenViewerPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_GenViewer };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

