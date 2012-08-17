// GenViewer_ocx.cpp : Implementation of CGenViewerApp and DLL registration.

#include "stdafx.h"
#include "GenViewer_ocx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGenViewerApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xEE38FA44, 0xA470, 0x42BB, { 0x88, 0x38, 0xE4, 0xF5, 0xBC, 0x4, 0x4C, 0x58 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

// CGenViewerApp::InitInstance - DLL initialization

BOOL CGenViewerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}

// CGenViewerApp::ExitInstance - DLL termination

int CGenViewerApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
