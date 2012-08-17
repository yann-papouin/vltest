// GenViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GenViewer.h"

#include "MainFrm.h"
#include "GenViewerDoc.h"
#include "GenViewerView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if defined(INTEROP) || defined(ACIS)
#include "license.hxx"
#include "spa_unlock_result.hxx"
#include <direct.h>

#if defined(ACIS)
#include "entity_mgr_factory.hxx"
#include "asm_api.hxx"
#endif // Acis
#endif

#ifdef VL
#include <vlCore/VisualizationLibrary.hpp>
#include "Renderer/vl/IOManager.hpp"
#endif

#define UtC(x,y)     (wcstombs(y, x, wcslen(x)), y)

#if defined(ACIS)
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

/////////////////////////////////////////////////////////////////////////////
// CGenViewerApp

BEGIN_MESSAGE_MAP(CGenViewerApp, CWinApp)
	//{{AFX_MSG_MAP(CGenViewerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)	

	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CGenViewerApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenViewerApp construction

CGenViewerApp::CGenViewerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGenViewerSdiApp object

CGenViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGenViewerApp initialization

BOOL CGenViewerApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
#ifdef HOOPS
	m_pHDB=new HDB();
	m_pHDB->Init();
	m_pHDB->SetIsolatedDrivers(false);

	TCHAR cur_dir[MVO_BUFFER_SIZE];
	GetCurrentDirectory(MVO_BUFFER_SIZE, cur_dir);
	memset(m_AppDirectory,0,MVO_BUFFER_SIZE);	
	//UtC(cur_dir, m_AppDirectory);
#endif

#ifdef VL
//	vl::showWin32Console();

	/* init Visualization Library */
	vl::VisualizationLibrary::init();
#endif

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

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGenViewerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CGenViewerView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Don't display a new MDI child window during startup
// 	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
// 		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	AfxInitRichEdit();

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CGenViewerApp::ExitInstance()
{
#ifdef HOOPS
	if( m_pHDB != NULL ) 
	{
		delete m_pHDB;
		m_pHDB = NULL;
	}
#endif

#ifdef ACIS
	outcome o = api_terminate_hoops_acis_bridge();
	check_outcome(o);
	o = api_stop_modeller();
	check_outcome(o);
	terminate_base();		
#endif // ACIS

#ifdef VL
	  vl::VisualizationLibrary::shutdown();
#endif // VL

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CGenViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CGenViewerApp message handlers

void CGenViewerApp::CreateAllFileTypesString(CString * filters, const char * file_types) 
{
	int stringLength = (int)strlen(file_types);
	char extension[4096];

	*filters += CString(_T("All Supported Files |"));

	//strings have the following form ";jpeg:tiff;hsf;hmf;"

	int lastOccurence = stringLength-2;

	for (int i = stringLength-2; i > 0; i--)
	{
		if (file_types[i] == ';')
		{
			int offset = lastOccurence - i;
			memcpy(extension, &file_types[i+1], offset);

			extension[offset] = '\0';
			*filters += CString(_T("*."));
			*filters += CString(extension);
			if (i != 1)
				*filters += CString(_T(";"));

			lastOccurence = i-1;
		}
	}
}

void CGenViewerApp::OnFileOpen()
{
	// TODO: Add your command handler code here

	CString filter = _T("");
	CString def_ext = _T(".hsf");

#ifdef INTEROP
	filter  = _T("Catia V5 Files (*.CATPart;*.CATProduct;*.CATShape;*.cgr)|*.CATPart;*.CATProduct;*.CATShape;*.cgr|");
	filter += _T("Catia V4 Files (*.model;*.session;*.exp)|*.model;*.session;*.exp|");
	filter += _T("ProE Files (*.prt;*.prt.*;*.xpr;*.xpr.*;*.asm;*.asm.*;*.xas;*.xas.*)|*.prt;*.prt.*;*.xpr;*.xpr.*;*.asm;*.asm.*;*.xas;*.xas.*|");
	filter += _T("IGES Files (*.igs;*.iges)|*.igs;*.iges|");
	filter += _T("STEP Files (*.stp;*.step)|*.stp;*.step|");
	filter += _T("VDA Files(*.vda)|*.VDA|");
	filter += _T("UG Files (*.prt)|*.prt|");
	filter += _T("Inventor Files (*.ipt;*.iam)|*.ipt;*.iam|");
	filter += _T("Solidworks Files (*.sldprt;*.sldasm)|*.sldprt;*.sldasm|");
	filter += _T("Parasolid Files (*.x_t*;*.xmt_*)|*.x_t*;*.xmt_*|");
	filter += _T("Acis Files (*.sat)|*.sat|");
#ifdef ACIS
	filter += _T("AcisHOOPS Stream Files (*.asf)|*.asf|");
#endif // ACIS
#ifdef PARASOLID
	filter += _T("ParaHOOPS Stream Files (*.psf)|*.psf|");
#endif // PARASOLID

	def_ext = _T(".CATPart");
#elif PARASOLID
	filter = _T("Parasolid Part Files (*.x_t*;*.xmt_*)|*.x_t*;*.xmt_*|ParaHOOPS Stream Files (*.psf)|*.psf|");
	def_ext = _T(".x_t");
#elif ACIS
	filter = _T("Acis Files (*.sat;*.asat)|*.sat;*.asat|AcisHOOPS Stream Files (*.asf)|*.asf|");
	def_ext = _T(".sat");
#elif GRANITE
	filter = _T("ProE Files (*.prt;*.prt.*)|*.prt;*.prt.*|ProE Assemblies (*.asm;*.asm.*)|*.asm;*.asm.*|GraniteHOOPS Stream Files (*.gsf)|*.gsf|Parasolid Files (*.x_t;*.xmt_txt;x_b)|*.x_t;*.xmt_txt;*.xmb|IGES Files (*.igs)|*.igs|Acis Files (*.sat)|*.sat|STEP Files (*.stp;*.step)|*.stp;*.step|VDA Files (*.vda)|*.vda|Granite Neutral Files (*.g)|*.g|");
	def_ext = _T(".prt");
#endif 

	CreateAllFileTypesString(&filter,IOManager::Instance()->GetInputHandlerTypes());
	filter += _T("|");

	// this adds file types that have HIO handlers and appear in ProcessFilters()
	ProcessFilters(&filter, IOManager::Instance()->GetInputHandlerTypes());

	filter += _T("|");

	TCHAR cur_dir[4096];
	GetCurrentDirectory(4096, cur_dir);
	CFileDialog my_dlg(TRUE, def_ext, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter , NULL);
	my_dlg.m_ofn.lpstrInitialDir = cur_dir;

	if (my_dlg.DoModal() == IDOK) {
		CString pathname;
		pathname = my_dlg.GetPathName();

		OpenDocumentFile(pathname);
		int end = pathname.ReverseFind('\\');
		pathname.Truncate(end);
		SetCurrentDirectory(pathname);
	}

}

void CGenViewerApp::ProcessFilters( CString * filters, const char * file_types )
{
	if(strstr(file_types, ";hsf;"))
		*filters += CString(_T("HOOPS Stream File (*.hsf)|*.hsf|"));

	if(strstr(file_types, ";hmf;"))
		*filters += CString(_T("HOOPS Metafile (*.hmf)|*.hmf|"));

	if(strstr(file_types, ";dwf;"))
		*filters += CString(_T("Autodesk DWF/DWFx (*.dwf; *.dwfx)|*.dwf; *.dwfx|"));

	if(strstr(file_types, ";u3d;"))
		*filters += CString(_T("Universal 3D (*.u3d)|*.u3d|"));

	if(strstr(file_types, ";dwg;"))
		*filters += CString(_T("AutoCAD DWG (*.dwg)|*.dwg|"));

	if(strstr(file_types, ";jt;"))
		*filters += CString(_T("UGS JT (*.jt)|*.jt|"));

	if(strstr(file_types, ";dxf;"))
		*filters += CString(_T("AutoCAD DXF (*.dxf)|*.dxf|"));

	if(strstr(file_types, ";html;"))
		*filters += CString(_T("HOOPS 3D Stream Control (*.html)|*.html|"));

	if(strstr(file_types, ";stl;"))
		*filters += CString(_T("Stereolithography Files(*.stl)|*.stl|"));

	if(strstr(file_types, ";obj;"))
		*filters += CString(_T("Alias Wavefront (*.obj)|*.obj|"));

	if(strstr(file_types, ";ooc;"))
		*filters += CString(_T("HOOPS Out of Core (*.ooc)|*.ooc|"));

	if(strstr(file_types, ";ply;"))
		*filters += CString(_T("PLY (*.ply)|*.ply|"));

	if(strstr(file_types, ";wrl;"))
		*filters += CString(_T("VRML (*.wrl)|*.wrl|"));

	if(strstr(file_types, ";iv;"))
		*filters += CString(_T("Open Inventor (*.iv)|*.iv|"));

	if(strstr(file_types, ";pdf;"))
		*filters += CString(_T("PDF (*.pdf)|*.pdf|"));

	if(strstr(file_types, ";ps;"))
		*filters += CString(_T("Postscript (*.ps)|*.ps|"));

	if(strstr(file_types, ";hp;"))
		*filters += CString(_T("HPGL2 (*.hp)|*.hp|"));

	if(strstr(file_types, ";cgm;"))
		*filters += CString(_T("CGM (*.cgm)|*.cgm|"));

	if(strstr(file_types, ";wmf;"))
		*filters += CString(_T("WMF Windows Metafile (*.wmf)|*.wmf|"));

	if(strstr(file_types, ";emf;"))
		*filters += CString(_T("EMF Enhanced Metafile (*.emf)|*.emf|"));

	if(strstr(file_types, ";jpg;"))
		*filters += CString(_T("JPEG (*.jpg; *.jpeg)|*.jpg; *.jpeg|"));

	if(strstr(file_types, ";bmp;"))
		*filters += CString(_T("BMP (*.bmp)|*.bmp|"));

	if(strstr(file_types, ";png;"))
		*filters += CString(_T("PNG (*.png)|*.png|"));

	if(strstr(file_types, ";tif;"))
		*filters += CString(_T("TIFF (*.tif; *.tiff)|*.tif; *.tiff|"));

	if(strstr(file_types, ";gif;"))
		*filters += CString(_T("GIF (*.gif)|*.gif|"));

	if(strstr(file_types, ";dgn;"))
		*filters += CString(_T("MicroStation DGN (*.dgn)|*.dgn|"));

	if(strstr(file_types, ";skp;"))
		*filters += CString(_T("Google SketchUp (*.skp)|*.skp|"));

	if(strstr(file_types, ";step;"))
		*filters += CString(_T("STEP (*.step)|*.step|"));

	if(strstr(file_types, ";iges;"))
		*filters += CString(_T("IGES (*.iges)|*.iges|"));

	if(strstr(file_types, ";x_t;"))
		*filters += CString(_T("Parasolid (*.x_t)|*.x_t|"));
}