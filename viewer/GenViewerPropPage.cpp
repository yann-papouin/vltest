// GenViewerPropPage.cpp : Implementation of the CGenViewerPropPage property page class.

#include "stdafx.h"
#include "GenViewer.h"
#include "GenViewerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CGenViewerPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CGenViewerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CGenViewerPropPage, "GenViewer.GenViewerPropPage.1",
	0xcdb516ad, 0xe4e8, 0x443e, 0xa0, 0xc7, 0x7d, 0x84, 0xaf, 0x6b, 0x61, 0x9a)



// CGenViewerPropPage::CGenViewerPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CGenViewerPropPage

BOOL CGenViewerPropPage::CGenViewerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_GenViewer_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CGenViewerPropPage::CGenViewerPropPage - Constructor

CGenViewerPropPage::CGenViewerPropPage() :
	COlePropertyPage(IDD, IDS_GenViewer_PPG_CAPTION)
{
}



// CGenViewerPropPage::DoDataExchange - Moves data between page and properties

void CGenViewerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CGenViewerPropPage message handlers
