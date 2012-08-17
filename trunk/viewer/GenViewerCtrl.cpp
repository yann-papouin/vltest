// GenViewerCtrl.cpp : Implementation of the CGenViewerCtrl ActiveX Control class.

#include "stdafx.h"
#include "GenViewer_ocx.h"
#include "GenViewerCtrl.h"
#include "GenViewerPropPage.h"
#include "MainFrm.h"
#include "GenViewerDoc.h"
#include "GenViewerView.h"

#if defined(INTEROP) || defined(ACIS)
#include "HSInclude.h"
#include "license.hxx"
#include "spa_unlock_result.hxx"
#include <direct.h>

#if defined(ACIS)
#include "entity_mgr_factory.hxx"
#include "asm_api.hxx"
#endif // Acis

#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UtC(x,y)     (wcstombs(y, x, wcslen(x)), y)

#if defined(INTEROP) || defined(ACIS)
void unlock_spatial_products_3353();
void unlock_spatial_products_4158();

void CHECK_OUTCOME( const outcome& result )
{
	if( !result.ok() ) {

		char default_msg[MVO_BUFFER_SIZE];
		const char* error_string = find_err_mess( result.error_number() );
		if( (error_string == NULL ) || (*error_string == '\0')) {
			sprintf(default_msg,"%d",result.error_number());
			error_string = default_msg;
		}

		MessageBox(NULL, H_TEXT(error_string), _T("ACIS Error"),
			MB_OK|MB_ICONSTOP|MB_APPLMODAL);
	}
}
#endif

IMPLEMENT_DYNCREATE(CGenViewerCtrl, CActiveXDocControl)



// Message map

BEGIN_MESSAGE_MAP(CGenViewerCtrl, CActiveXDocControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CGenViewerCtrl, CActiveXDocControl)
	DISP_FUNCTION_ID(CGenViewerCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(CGenViewerCtrl, "DocumentName", dispidDocumentName, m_documentName, OnDocumentNameChanged, VT_BSTR)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CGenViewerCtrl, CActiveXDocControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CGenViewerCtrl, 1)
	PROPPAGEID(CGenViewerPropPage::guid)
END_PROPPAGEIDS(CGenViewerCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CGenViewerCtrl, "GenViewer.GenViewerCtrl.1",
	0xaf0099dc, 0x3ac8, 0x4e61, 0xb0, 0x4, 0x1d, 0x6c, 0xa5, 0x40, 0x78, 0x79)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CGenViewerCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DGenViewer =
		{ 0x26B6621A, 0x6C6B, 0x499E, { 0xAE, 0x28, 0x48, 0xB1, 0x9E, 0x1E, 0x68, 0x26 } };
const IID BASED_CODE IID_DGenViewerEvents =
		{ 0x2F253F55, 0xD831, 0x43E5, { 0xA1, 0x92, 0xE2, 0xD4, 0x74, 0x47, 0x4D, 0x67 } };



// Control type information

static const DWORD BASED_CODE _dwGenViewerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CGenViewerCtrl, IDS_GenViewer, _dwGenViewerOleMisc)



// CGenViewerCtrl::CGenViewerCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CGenViewerCtrl

BOOL CGenViewerCtrl::CGenViewerCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_GenViewer,
			IDB_GenViewer,
			afxRegApartmentThreading,
			_dwGenViewerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CGenViewerCtrl::CGenViewerCtrl - Constructor

CGenViewerCtrl::CGenViewerCtrl()
{
	InitializeIIDs(&IID_DGenViewer, &IID_DGenViewerEvents);
	// TODO: Initialize your control's instance data here.

	SetInitialSize(200, 200);

#ifdef HOOPS
	m_pHDB=new HDB();
	m_pHDB->Init();
	m_pHDB->SetIsolatedDrivers(false);
#endif
	TCHAR cur_dir[MVO_BUFFER_SIZE];
	GetCurrentDirectory(MVO_BUFFER_SIZE, cur_dir);
	memset(m_AppDirectory,0,MVO_BUFFER_SIZE);	
	//UtC(cur_dir, m_AppDirectory);

#if defined(INTEROP) || defined(ACIS)
#ifdef ACIS
	//Base configuration must happen before unlocking ACIS.
	//   If not using initialize_base, then unlock must 
	//   happen after the initialize modeller 
	base_configuration base_config;
	logical ok = initialize_base( &base_config);
	unlock_spatial_products_4158();
#endif // ACIS


#ifdef INTEROP
	char *path;
	path = getenv("PATH");
	if (path == NULL) {
		wchar_t * error_msg = L"Unable to add CATIA V5 DLLs to the path.  CATIA V5 translation will not work.";
		AfxMessageBox(LPCTSTR(error_msg), MB_ICONSTOP);
	}
	else {
		_putenv(H_FORMAT_TEXT("PATH=%s\\lib3dx\\intel_a\\code\\bin;%s", m_AppDirectory, path));
	}
#endif // INTEROP
#endif // defined(INTEROP) || defined(ACIS)


#ifdef ACIS
	outcome o; 
	o = api_start_modeller(0);
	check_outcome(o);

	//Issue #10605: Default Entity Manager Factory no longer registered by default		
	if (get_major_version() >= 20) 
	{
		entity_mgr_factory* my_manager = new default_entity_mgr_factory();
		{
			outcome o = asmi_set_entity_mgr_factory(my_manager);
			check_outcome(o);
		}
	}

	o = api_initialize_hoops_acis_bridge();
	check_outcome(o);
	ha_rendering_options &roptions=HA_Get_Rendering_Options();
	{
		roptions.SetMergeFacesMode(TRUE);
		roptions.SetGeomPattern ("?Include Library/ACIS model geometry");
		roptions.SetPattern("entity");
	}	
#endif // ACIS

	AfxInitRichEdit();

	AddDocTemplate(new CActiveXDocTemplate(
		RUNTIME_CLASS(CGenViewerDoc),   //改为你的文档类
		RUNTIME_CLASS(CMainFrame),  //改为你的框架类
		RUNTIME_CLASS(CGenViewerView))); //改为你的视图类

}


// CGenViewerCtrl::~CGenViewerCtrl - Destructor

CGenViewerCtrl::~CGenViewerCtrl()
{
	// TODO: Cleanup your control's instance data here.
	if( m_pHDB != NULL ) 
	{
		delete m_pHDB;
		m_pHDB = NULL;
	}

#ifdef ACIS
	outcome o = api_terminate_hoops_acis_bridge();
	check_outcome(o);
	o = api_stop_modeller();
	check_outcome(o);
	terminate_base();		
#endif // ACIS
}



// CGenViewerCtrl::OnDraw - Drawing function

// void CGenViewerCtrl::OnDraw(
// 			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
// {
// 	if (!pdc)
// 		return;
// 
// 	// TODO: Replace the following code with your own drawing code.
// 	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
// 	pdc->Ellipse(rcBounds);
// }



// CGenViewerCtrl::DoPropExchange - Persistence support

void CGenViewerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CGenViewerCtrl::OnResetState - Reset control to default state

void CGenViewerCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CGenViewerCtrl::AboutBox - Display an "About" box to the user

void CGenViewerCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_GenViewer);
	dlgAbout.DoModal();
}


void CGenViewerCtrl::OnDocumentNameChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	GetDocTemplate()->OpenDocumentFile(m_documentName);
	SetModifiedFlag();
}
// CGenViewerCtrl message handlers
