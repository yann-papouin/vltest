// GenViewerView.cpp : implementation of the CGenViewerView class
//

#include "stdafx.h"
#include "process.h"

#ifdef _OCX
#include "GenViewer_ocx.h"
#else
#include "GenViewer.h"
#endif

#include "GenViewerDoc.h"
#include "GenViewerView.h"
#include "MainFrm.h"

#include "Renderer/ViewerRenderer.h"

#ifdef OSG
#include "Renderer/OSG/ViewerOsgRenderer.h"
#endif

#ifdef VL
#define CMFCView vlMFC::MDIWindow
#else
#define CMFCView CView
#endif

#ifdef HOOPS
	#include "Renderer/HOOPS/CATOpMouse.h"

	#include "HOpCameraOrbit.h"
	#include "HEventInfo.h"
	#include "HBaseOperator.h"
	#include "HEventManager.h"
	#include "HEventListener.h"
	#include "HOpCameraZoom.h"
	#include "HOpCameraPan.h"
	#include "HOpCameraZoomBox.h"
	#include "HOpCameraManipulate.h"
	#include "HOpSelectArea.h"
	#include "HSelectionSet.h"
	#include "HOpCreateSphere.h"
#elif defined VL
	#include <vlCore/Colors.hpp>
	#include <vlCore/Sphere.hpp>

	#include <vlGraphics/Rendering.hpp>
	#include <vlGraphics/Geometry.hpp>
	#include <vlGraphics/GeometryPrimitives.hpp>
	#include <vlGraphics/Text.hpp>
	#include <vlGraphics/FontManager.hpp>

	#include "renderer/vl/VLBaseView.hpp"
//	#include "Renderer/VL/AxisCameraUpdateCallback.hpp"
#endif
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if defined(HOOPS) && defined(ACIS)
////////////////////////////////////////////////////////////////////////////
// HGenViewerView
HGenViewerView::HGenViewerView(HSolidModel *model,
	const char * 		alias,	
	const char *		driver_type,
	const char *		instance_name,
	void *				window_handle,
	void *				colormap) : HSolidView(model, alias, driver_type, instance_name, window_handle, colormap)
{
}

HGenViewerView::~HGenViewerView()
{
}

// app-specific init function
void HGenViewerView::Init()
{
	// call base's init function first to get the default HOOPS hierarchy for the view
	HSolidView::Init();

	// create our app-specific Selection object and initialize
	m_pSelection = new HSelectionSet(this);
	m_pSelection->Init();

	// TODO: Add your initialization here
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CGenViewerView

IMPLEMENT_DYNCREATE(CGenViewerView, CMFCView)

BEGIN_MESSAGE_MAP(CGenViewerView, CMFCView)
	//{{AFX_MSG_MAP(CPindockView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CMFCView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CMFCView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CMFCView::OnFilePrintPreview)

	ON_COMMAND(ID_VIEW_ISO,&CGenViewerView::OnViewIso)
	ON_COMMAND(ID_VIEW_TOP,&CGenViewerView::OnViewTop)
	ON_COMMAND(ID_VIEW_BOTTOM,&CGenViewerView::OnViewBottom)
	ON_COMMAND(ID_VIEW_FRONT,&CGenViewerView::OnViewFront)
	ON_COMMAND(ID_VIEW_BACK,&CGenViewerView::OnViewBack)
	ON_COMMAND(ID_VIEW_LEFT,&CGenViewerView::OnViewLeft)
	ON_COMMAND(ID_VIEW_RIGHT,&CGenViewerView::OnViewRight)

	ON_COMMAND(ID_VIEW_PERSPECTIVE,&CGenViewerView::OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, &CGenViewerView::OnUpdateViewPerspective)

	ON_COMMAND(ID_ORBIT_OPT,&CGenViewerView::OnOrbitOpt)
	ON_UPDATE_COMMAND_UI(ID_ORBIT_OPT, &CGenViewerView::OnUpdateOrbitOpt)
	ON_COMMAND(ID_PAN_OPT,&CGenViewerView::OnPanOpt)
	ON_UPDATE_COMMAND_UI(ID_PAN_OPT, &CGenViewerView::OnUpdatePanOpt)
	ON_COMMAND(ID_ZOOM_OPT,&CGenViewerView::OnZoomOpt)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OPT, &CGenViewerView::OnUpdateZoomOpt)

	ON_COMMAND(ID_VIEW_RENDER_WIREFRAME,OnViewRenderWireframe)
	ON_COMMAND(ID_VIEW_RENDER_GOURAUD,OnViewRenderGouraud)
	ON_COMMAND(ID_VIEW_RENDER_HIDDENLINE,OnViewRenderHiddenLine)
	ON_COMMAND(ID_VIEW_RENDER_WIREFRAME_WITH_SILHOUETTE,OnViewRenderWireframeWithSilhouette)
	ON_COMMAND(ID_VIEW_RENDER_TRIANGULATION,OnViewRenderTriangulation)
	ON_COMMAND(ID_VIEW_RENDER_GOURAUD_WITH_EDGES,OnViewRenderGouraudWithEdges)
	ON_COMMAND(ID_VIEW_RENDER_PHONG,OnViewRenderPhong)
	ON_COMMAND(ID_VIEW_RENDER_PHONG_WITH_EDGES,OnViewRenderPhongWithEdges)


	ON_COMMAND(ID_ZOOM_TO_EXTENTS, OnZoomToExtents)
	ON_COMMAND(ID_ZOOM_TO_WINDOW, OnZoomToWindow)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_TO_WINDOW, OnUpdateZoomToWindow)
	ON_COMMAND(ID_WINDOW_SELECT, OnWindowSelect)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SELECT, OnUpdateWindowSelect)
	//ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_CREATE_SPHERE, OnCreateSphere)
	ON_UPDATE_COMMAND_UI(ID_CREATE_SPHERE, OnUpdateCreateSphere)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenViewerView construction/destruction

CGenViewerView::CGenViewerView()
{
	m_bFlush=false;
#if defined(HOOPS) 
	m_pDefaultViewOp = NULL;
#endif

	m_logoWidth = 0;
	m_logoHeight = 0;


	m_bPerspective = false;
}

CGenViewerView::~CGenViewerView()
{
#if defined(HOOPS) 
	if (m_pDefaultViewOp != NULL)
	{
		delete m_pDefaultViewOp;
		m_pDefaultViewOp = NULL;
	}

	if (m_pHSolidView != NULL)
	{
		delete m_pHSolidView;
		m_pHSolidView = NULL;
	}
#endif
}

BOOL CGenViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
 	return CMFCView::PreCreateWindow(cs);
}

int CGenViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the renderer
	//Init(); 
	m_pDC = new CClientDC(this);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CGenViewerView message handlers


void CGenViewerView::OnInitialUpdate()
{
	CMFCView::OnInitialUpdate();

	// Only load the logo once
	if (m_logoWidth == 0 || m_logoHeight == 0)
	{
		GetParentFrame()->RecalcLayout();
		HBITMAP hBitmap = (HBITMAP)LoadImage(
			GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDB_LOGO),
			IMAGE_BITMAP,
			0, 0,
			LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

		m_hLogo.Attach(hBitmap);
		BITMAP bmap;
		m_hLogo.GetBitmap(&bmap);
		// Record the logo's size
		m_logoWidth = bmap.bmWidth;
		m_logoHeight = bmap.bmHeight;
	}


	// Initialize the renderer, informing it of the pixel size of the view, the
	// view's handle, and the device context of its client area
	CRect rect;
	GetClientRect(rect);
	// Get Filename from DocumentOpen Dialog
	CGenViewerDoc* pDoc = GetDocument();
	if (NULL!=pDoc)
	{
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if (NULL  != pRenderer)
		{
			pRenderer->Initialize(m_hWnd, m_pDC, rect.right, rect.top);

#ifdef OSG
			// Start the thread to do OSG Rendering
			//mThreadHandle = (HANDLE)_beginthread(&ViewerOsgRenderer::Render, 0, pRenderer); 
#endif	
		}
	}

#ifdef HOOPS
	// TODO: Add your specialized code here and/or call the base class
	m_pHSolidView = new HGenViewerView(GetDocument()->m_pHBaseModel,0,"opengl",0, m_hWnd,NULL);
	m_pHSolidView->Init();
//	LocalSetOperator(new HOpCameraOrbit(m_pHSolidView));
	m_pHSolidView->FitWorld();		// fit the camera to the scene extents
	m_pHSolidView->CameraPositionChanged(true);
    m_pHSolidView->RenderGouraudWithLines();// set the render mode to gouraud with lines

	// 6/23/2011 mwu : 
	m_pHSolidView->SetAxisMode(AxisOn);
	m_pHSolidView->SetFastFitWorld(true);
	m_pHSolidView->SetBackplaneCulling(true);
	m_pHSolidView->SetDisplayListType(DisplayListSegment);
	m_pHSolidView->SetHandedness(HandednessLeft, true);
	m_pHSolidView->SetPolygonHandednessMode( HandednessLeft );
	m_pHSolidView->SetDoubleBuffering(true);
	m_pHSolidView->SetProjMode(ProjOrthographic);

	//11/07/2011 lxu : reset scene bounding
	m_pHSolidView->SetGeometryChanged();
	
	m_pDefaultViewOp = new CATOpMouse(m_pHSolidView);
	m_pHSolidView->SetViewOperator(m_pDefaultViewOp);
	m_pHSolidView->GetEventManager()->RegisterHandler((HMouseListener*)m_pDefaultViewOp, HMouseListener::GetType(), HLISTENER_PRIORITY_NORMAL);
#endif

#ifdef VL
	if (!m_bFlush)//first time
	{
		/* create the applet to be run */
		mVLBaseView = new VLBaseView;
		//	mVLBaseView->setRendering(mRendering.get());

		/* bind the applet so it receives all the GUI events related to the OpenGLContext */
		mVLBaseView->initialize(this->OpenGLContext::framebuffer());
		this->OpenGLContext::addEventListener(mVLBaseView.get());

		/* Initialize the OpenGL context and window properties */	
		CRect r; 	
		GetWindowRect(&r);
		vl::OpenGLContextFormat format;
		format.setDoubleBuffer(true);
		format.setRGBABits( 8,8,8,0 );
		format.setDepthBufferBits(24);
		format.setStencilBufferBits(8);
		format.setFullscreen(false);
		format.setMultisampleSamples(16);
		format.setMultisample(true);
		Win32Context::initWin32GLContext(NULL, "VLView", format, /*these last for are ignored*/0, 0, r.Width(), r.Height());

		// 8/23/2012 mwu : test  
		mVLBaseView->makeBox();
	}

	mVLBaseView->setViewMode(ViewIso,true);
	update();

#endif // VL
}

/////////////////////////////////////////////////////////////////////////////
// CGenViewerView drawing

void CGenViewerView::OnDraw(CDC* pDC)
{
	CGenViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (NULL!=pDoc)
		DrawScene();
	else
		DrawLogo();
}

/////////////////////////////////////////////////////////////////////////////
// CGenViewerView printing

BOOL CGenViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGenViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CGenViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CGenViewerView diagnostics

#ifdef _DEBUG
void CGenViewerView::AssertValid() const
{
	CMFCView::AssertValid();
}

void CGenViewerView::Dump(CDumpContext& dc) const
{
	CMFCView::Dump(dc);
}

CGenViewerDoc* CGenViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGenViewerDoc)));
	return (CGenViewerDoc*)m_pDocument;
}
#endif //_DEBUG

void CGenViewerView::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CMFCView::OnPaint() for painting messages
#ifdef HOOPS
	if(m_pHSolidView)
	{
		HC_Control_Update_By_Key(m_pHSolidView->GetViewKey(),"redraw everything");
		m_pHSolidView->Update();
	}
#endif

	// 6/28/2012 mwu : test 
	CGenViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (NULL!=pDoc)
		DrawScene();
	else
		DrawLogo();

	CMFCView::OnPaint();
}

#ifdef HOOPS
void CGenViewerView::LocalSetOperator(HBaseOperator * NewOperator)
{
	HBaseOperator * op = m_pHSolidView->GetOperator();
	delete op;
	m_pHSolidView->SetOperator(NewOperator);
}

unsigned long CGenViewerView::MapFlags(unsigned long state)
{
	unsigned long flags = 0;
	/*map the qt events state to MVO*/
	if(state & MK_LBUTTON) flags = MVO_LBUTTON;
	if(state & MK_RBUTTON) flags = MVO_RBUTTON;
	if(state & MK_MBUTTON) flags = MVO_MBUTTON;
	if(state & MK_SHIFT) flags = MVO_SHIFT;
	if(state & MK_CONTROL) flags = MVO_CONTROL;
	return flags;
}
#endif

void CGenViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef HOOPS
	if (m_pHSolidView)
	{
		HEventInfo	event(m_pHSolidView);
		event.SetPoint(HE_LButtonDown, point.x, point.y, MapFlags(nFlags));
		HLISTENER_EVENT(HMouseListener, m_pHSolidView->GetEventManager(), OnLButtonDown(event))

			HBaseOperator *op = m_pHSolidView->GetCurrentOperator();

		//this logic needs to be reconsidered for multiple operators
		if (op) 
		{			
			if (op->Capture()) 
				SetCapture();
		}
		else 
			CMFCView::OnLButtonDown(nFlags, point);
	}
#endif
	CMFCView::OnLButtonDown(nFlags,point);
}

void CGenViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
#ifdef HOOPS
	if (m_pHSolidView)
	{
		HEventInfo	event(m_pHSolidView);
		event.SetPoint(HE_LButtonUp, point.x, point.y, MapFlags(nFlags));
		HLISTENER_EVENT(HMouseListener, m_pHSolidView->GetEventManager(), OnLButtonUp(event))

			HBaseOperator *op = m_pHSolidView->GetCurrentOperator();

		if (op) 
		{			
			if (op->Capture()) 
				ReleaseCapture();
		}
		else 
			CMFCView::OnLButtonUp(nFlags, point);
	}
#endif
	CMFCView::OnLButtonUp(nFlags,point);
}

void CGenViewerView::OnMButtonDown(UINT nFlags, CPoint point)
{
#ifdef HOOPS
	if (m_pHSolidView)
	{
		HEventInfo	event(m_pHSolidView);
		event.SetPoint(HE_LButtonDown, point.x, point.y, MapFlags(nFlags));
		HLISTENER_EVENT(HMouseListener, m_pHSolidView->GetEventManager(), OnMButtonDown(event))

			HBaseOperator *op = m_pHSolidView->GetCurrentOperator();

		//this logic needs to be reconsidered for multiple operators
		if (op) 
		{			
			if (op->Capture()) 
				SetCapture();
		}
		else 
			CMFCView::OnMButtonDown(nFlags, point);
	}
#endif
	CMFCView::OnMButtonDown(nFlags,point);
}

void CGenViewerView::OnMButtonUp(UINT nFlags, CPoint point)
{
#ifdef HOOPS
	if (m_pHSolidView)
	{
		HEventInfo	event(m_pHSolidView);
		event.SetPoint(HE_LButtonUp, point.x, point.y, MapFlags(nFlags));
		HLISTENER_EVENT(HMouseListener, m_pHSolidView->GetEventManager(), OnMButtonUp(event))

			HBaseOperator *op = m_pHSolidView->GetCurrentOperator();

		if (op) 
		{			
			if (op->Capture()) 
				ReleaseCapture();
		}
		else 
			CMFCView::OnMButtonUp(nFlags, point);
	}
#endif
	CMFCView::OnMButtonUp(nFlags,point);
}

void CGenViewerView::OnRButtonDown(UINT nFlags, CPoint point)
{
#ifdef HOOPS
	if (m_pHSolidView)
	{
		HEventInfo	event(m_pHSolidView);
		event.SetPoint(HE_LButtonDown, point.x, point.y, MapFlags(nFlags));
		HLISTENER_EVENT(HMouseListener, m_pHSolidView->GetEventManager(), OnRButtonDown(event))

			HBaseOperator *op = m_pHSolidView->GetCurrentOperator();

		//this logic needs to be reconsidered for multiple operators
		if (op) 
		{			
			if (op->Capture()) 
				SetCapture();
		}
		else 
			CMFCView::OnRButtonDown(nFlags, point);
	}
#endif
	CMFCView::OnRButtonDown(nFlags,point);
}


void CGenViewerView::OnRButtonUp(UINT nFlags, CPoint point)
{
#ifdef HOOPS
	if (m_pHSolidView)
	{
		HEventInfo	event(m_pHSolidView);
		event.SetPoint(HE_LButtonUp, point.x, point.y, MapFlags(nFlags));
		HLISTENER_EVENT(HMouseListener, m_pHSolidView->GetEventManager(), OnRButtonUp(event))

		HBaseOperator *op = m_pHSolidView->GetCurrentOperator();

		if (op) 
		{			
			if (op->Capture()) 
				ReleaseCapture();
		}
		else 
			CMFCView::OnRButtonUp(nFlags, point);
	}
#endif
	CMFCView::OnRButtonUp(nFlags,point);
}

void CGenViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
#ifdef HOOPS
	if (m_pHSolidView && m_pHSolidView->GetViewActive())
	{

		HEventInfo	event(m_pHSolidView);

		event.SetPoint(HE_MouseMove, point.x, point.y, MapFlags(nFlags));
		HLISTENER_EVENT(HMouseListener, m_pHSolidView->GetEventManager(), OnMouseMove(event))
	}
	else
		CMFCView::OnMouseMove(nFlags, point);
#elif VL
	CMFCView::OnMouseMove(nFlags,point);

	// 7/9/2012 mwu : draw center of the camera for debug


#endif
}

BOOL CGenViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
#ifdef HOOPS
	RECT windowRect;

	if (m_pHSolidView)
	{
		HEventInfo	event(m_pHSolidView);
		GetWindowRect(&windowRect);    
		event.SetPoint(HE_MouseWheel, pt.x - windowRect.left, pt.y - windowRect.top, MapFlags(nFlags));
		event.SetMouseWheelDelta(zDelta);
		HLISTENER_EVENT(HMouseListener, m_pHSolidView->GetEventManager(), OnMouseWheel(event))
	}
#endif

	CGenViewerDoc* pDoc = GetDocument();
	if (NULL!=pDoc)
	{
		ViewerRenderer* pRenderer = pDoc->GetRenderer();

		if(NULL!=pRenderer)
		{
			float delta = (float)zDelta / (float)WHEEL_DELTA / 10.0f;
			pRenderer->Zoom( delta );
		}

		RedrawWindow();
	}

	CMFCView::OnMouseWheel(nFlags,zDelta,pt);

	return TRUE;
}

void CGenViewerView::OnOrbitOpt()
{
#ifdef HOOPS
	LocalSetOperator(new HOpCameraOrbit(m_pHSolidView));
	((HOpCameraOrbit *)m_pHSolidView->GetOperator())->SetLightFollowsCamera(true);
#endif
}

void CGenViewerView::OnUpdateOrbitOpt(CCmdUI* pCmdUI)
{
#ifdef HOOPS
	HBaseOperator* pBaseOperator = m_pHSolidView->GetCurrentOperator();
	if ( pBaseOperator != NULL && streq(pBaseOperator->GetName(), "HOpCameraOrbit"))
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);
#endif
}

void CGenViewerView::OnZoomOpt() 
{
#ifdef HOOPS
	LocalSetOperator(new HOpCameraZoom(m_pHSolidView));
	((HOpCameraZoom *)m_pHSolidView->GetOperator())->SetLightFollowsCamera(true);
#endif
}

void CGenViewerView::OnUpdateZoomOpt(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	HBaseOperator* pBaseOperator = m_pHSolidView->GetCurrentOperator();
	if (pBaseOperator != NULL && streq(pBaseOperator->GetName(),"HOpCameraZoom"))
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);	
#endif
}

void CGenViewerView::OnPanOpt() 
{
#ifdef HOOPS
	LocalSetOperator(new HOpCameraPan(m_pHSolidView));
#endif
}

void CGenViewerView::OnUpdatePanOpt(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	HBaseOperator* pBaseOperator = m_pHSolidView->GetCurrentOperator();
	if (pBaseOperator != NULL && streq(pBaseOperator->GetName(), "HOpCameraPan"))
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);
#endif
}

void CGenViewerView::OnZoomToExtents() 
{
#ifdef HOOPS
	m_pHSolidView->ZoomToExtents();
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->fitWorld();
	update();
#endif
}

void CGenViewerView::OnZoomToWindow() 
{
#ifdef HOOPS
	LocalSetOperator(new HOpCameraZoomBox(m_pHSolidView));
	((HOpCameraZoomBox *)m_pHSolidView->GetOperator())->SetLightFollowsCamera(true);
#endif
}

void CGenViewerView::OnUpdateZoomToWindow(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	HBaseOperator* pBaseOperator = m_pHSolidView->GetCurrentOperator();
	if (pBaseOperator != NULL && streq(pBaseOperator->GetName(), "HOpCameraZoomBox"))
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);	
#endif
}


void CGenViewerView::OnViewRenderWireframe() 
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->setRenderMode(RenderWireframe);
	update();
#endif
}

void CGenViewerView::OnViewRenderGouraud()
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->setRenderMode(RenderGouraud);
	update();
#endif
}

void CGenViewerView::OnViewRenderHiddenLine()
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->setRenderMode(RenderHiddenLine);
	update();
#endif
}

void CGenViewerView::OnViewRenderWireframeWithSilhouette()
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->setRenderMode(RenderWireframeWithSilhouette);
	update();
#endif
}

void CGenViewerView::OnViewRenderTriangulation()
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->setRenderMode(RenderTriangulation);
	update();
#endif
}

void CGenViewerView::OnViewRenderGouraudWithEdges()
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->setRenderMode(RenderGouraudWithEdges);
	update();
#endif
}

void CGenViewerView::OnViewRenderPhong()
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->setRenderMode(RenderPhong);
	update();
#endif
}

void CGenViewerView::OnViewRenderPhongWithEdges()
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#elif defined VL
	mVLBaseView->setRenderMode(RenderPhongWithEdges);
	update();
#endif
}

void CGenViewerView::OnViewIso() 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetViewMode(HViewIso);
	}
#endif

	CGenViewerDoc* pDoc = GetDocument();
	if (NULL!=pDoc)
	{
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if (NULL!=pRenderer)
		{
			pRenderer->Isometric();
		}
	}

	RedrawWindow();
#ifdef VL
	mVLBaseView->setViewMode(ViewIso);
	update();
#endif
}

void CGenViewerView::OnViewTop() 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetViewMode(HViewTop);
	}
#endif

	CGenViewerDoc* pDoc = GetDocument();
	if (NULL!=pDoc)
	{
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if (NULL!=pRenderer)
		{
			pRenderer->Top();
		}
	}

	RedrawWindow();

#ifdef VL
	mVLBaseView->setViewMode(ViewXY);
	update();
#endif
}

void CGenViewerView::OnViewBottom() 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetViewMode(HViewBottom);
	}
#endif

#ifdef VL
	mVLBaseView->setViewMode(ViewYX);
	update();
#endif
}

void CGenViewerView::OnViewFront() 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetViewMode(HViewFront);
	}
#endif

	CGenViewerDoc* pDoc = GetDocument();
	if (NULL!=pDoc)
	{
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if (NULL!=pRenderer)
		{
			pRenderer->Front();
		}
	}

	RedrawWindow();

#ifdef VL
	mVLBaseView->setViewMode(ViewYZ);
	update();
#endif
}

void CGenViewerView::OnViewBack() 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetViewMode(HViewBack);
	}
#endif

#ifdef VL
	mVLBaseView->setViewMode(ViewZY);
	update();
#endif

}

void CGenViewerView::OnViewLeft() 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetViewMode(HViewLeft);
	}
#endif

#ifdef VL
	mVLBaseView->setViewMode(ViewXZ);
	update();
#endif
}

void CGenViewerView::OnViewRight() 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetViewMode(HViewRight);
	}
#endif

	CGenViewerDoc* pDoc = GetDocument();
	if (NULL!=pDoc)
	{
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if (NULL!=pRenderer)
		{
			pRenderer->Right();
		}
	}

	RedrawWindow();

#ifdef VL
	mVLBaseView->setViewMode(ViewZX);
	update();
#endif
}

void CGenViewerView::OnViewPerspective() 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetProjMode(ProjMode::ProjPerspective);
	}
#elif defined VL

	m_bPerspective = !m_bPerspective;

#endif

}

void CGenViewerView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	if (m_pHSolidView != NULL)
	{
		HBaseView* pView=(HBaseView*)m_pHSolidView;
		pView->SetProjMode(ProjMode::ProjPerspective);
	}
#elif defined VL

	pCmdUI->SetCheck(m_bPerspective);

#endif

}

void CGenViewerView::OnWindowSelect() 
{
#ifdef HOOPS
	LocalSetOperator(new HOpSelectArea(m_pHSolidView));
#endif
}

void CGenViewerView::OnUpdateWindowSelect(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	HBaseOperator* pBaseOperator = m_pHSolidView->GetCurrentOperator();
	if (pBaseOperator != NULL && streq(pBaseOperator->GetName(), "HOpSelectArea"))
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);
#endif
}

void CGenViewerView::OnCreateSphere() 
{
#ifdef HOOPS
	LocalSetOperator(new HOpCreateSphere(m_pHSolidView));		
#endif
}

void CGenViewerView::OnUpdateCreateSphere(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	HBaseOperator* pBaseOperator = m_pHSolidView->GetCurrentOperator();
	if (pBaseOperator != NULL && streq(pBaseOperator->GetName(), "HOpCreateSphere"))
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);
#endif
}

void CGenViewerView::DrawLogo()
{
	if (!m_bFlush && m_logoWidth > 0 && m_logoHeight > 0)
	{
		CRect rc;
		GetClientRect(rc);

		CDC memDC;
		memDC.CreateCompatibleDC(m_pDC);

		CBitmap* pOldBitmap = memDC.SelectObject(&m_hLogo);

		int x = (rc.Width() - m_logoWidth) / 2;
		int y = (rc.Height() - m_logoHeight) / 2;

		m_pDC->BitBlt(0,0, rc.Width(),rc.Height(), &memDC, -x, -y, SRCCOPY);

		memDC.SelectObject(pOldBitmap);
	}
}

void CGenViewerView::DrawScene()
{
	static BOOL bBusy = FALSE;

	if (!bBusy)
	{
		bBusy = TRUE;

		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());

		CGenViewerDoc* pDoc = GetDocument();
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if( pRenderer != NULL )
		{
			pRenderer->Display();			
		}

		bBusy = FALSE;
	}
}

void CGenViewerView::OnSize(UINT nType, int cx, int cy)
{
	// Save old framebuffer size
	if (mVLBaseView != NULL)
	{
		mVLBaseView->setOldBufferFrameSize(this->OpenGLContext::framebuffer()->width(),
			this->OpenGLContext::framebuffer()->height());
	}

	CMFCView::OnSize(nType, cx, cy);

	// Inform the renderer of changes in view size
	if (cy > 0)
	{
		CGenViewerDoc* pDoc = GetDocument();
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if (NULL!=pRenderer)
		{
			pRenderer->SetWindowExtents(cx, cy);
		}

		RedrawWindow();
	}
}

void CGenViewerView::OnDestroy()
{
	// Clean up
	CGenViewerDoc* pDoc = GetDocument();
	ViewerRenderer* pRenderer = pDoc->GetRenderer();
	if (NULL!=pRenderer)
	{
		pRenderer->Terminate();
	}

	if (m_pDC != NULL)
	{
		delete m_pDC;
		m_pDC = NULL;
	}

	WaitForSingleObject(mThreadHandle, 1000);

	CMFCView::OnDestroy();
}


BOOL CGenViewerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	BOOL answer = TRUE;
	CGenViewerDoc* pDoc = GetDocument();
	if (pDoc == NULL)	
	{
		// If no file is yet loaded, paint the background here
		CBrush brNew(RGB(255, 255, 255)); 
		CBrush* pOldBrush = (CBrush*)m_pDC->SelectObject(&brNew);
		CRect rc;
		m_pDC->GetClipBox(rc);
		m_pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
		m_pDC->SelectObject(pOldBrush);
		answer = FALSE;
	}
	return answer;
}

void CGenViewerView::DeleteContents()
{
#ifdef VL
	if (mVLBaseView != NULL)
	{
		mVLBaseView->flush();
		m_bFlush = true;
	}
#endif
}

BOOL CGenViewerView::OnOpenDocument( LPCTSTR lpszPathName )
{
//	 std::string strPathName; 
//#ifdef UNICODE 
//	 std::wstring w;     
//	 w = lpszPathName;    
//	 strPathName = std::string(w.begin(), w.end()); // magic here 
//#else     
//	 strPathName = lpszPathName;
//#endif 

	 std::string strPathName = CT2A( lpszPathName ); 
	 
	mVLBaseView->loadResource(strPathName);
	return TRUE;
}