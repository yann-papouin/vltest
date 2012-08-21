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
	#include "vlCore/Colors.hpp"
	#include "vlCore/Sphere.hpp"

	#include "vlGraphics/Rendering.hpp"
	#include <vlGraphics/Geometry.hpp>
	#include "vlGraphics/GeometryPrimitives.hpp"
	#include <vlGraphics/EdgeRenderer.hpp>
	#include <vlGraphics/Text.hpp>
	#include <vlGraphics/FontManager.hpp>

	#include "renderer/vl/VLBaseView.hpp"
	#include "Renderer/VL/AxisCameraUpdateCallback.hpp"
#include "Renderer/VL/AxisRenderer.hpp"
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
	ON_COMMAND(ID_TOOLS_RENDERMODE_GOURAUDWITHLINES, OnToolsRendermodeGouraudWithLines)
	ON_COMMAND(ID_TOOLS_RENDERMODE_HIDDENLINE, OnToolsRendermodeHiddenline)
	ON_COMMAND(ID_TOOLS_RENDERMODE_WIREFRAME, OnToolsRendermodeWireframe)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RENDERMODE_WIREFRAME, OnUpdateToolsRendermodeWireframe)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RENDERMODE_GOURAUDWITHLINES, OnUpdateToolsRendermodeGouraudWithLines)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RENDERMODE_HIDDENLINE, OnUpdateToolsRendermodeHiddenline)
	ON_COMMAND(ID_ZOOM_TO_EXTENTS, OnZoomToExtents)
	ON_COMMAND(ID_ZOOM_TO_WINDOW, OnZoomToWindow)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_TO_WINDOW, OnUpdateZoomToWindow)
	ON_COMMAND(ID_WINDOW_SELECT, OnWindowSelect)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SELECT, OnUpdateWindowSelect)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
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

	m_pDC = NULL;
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

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CGenViewerView message handlers


void CGenViewerView::OnInitialUpdate()
{
	CMFCView::OnInitialUpdate();
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

	Init();

	// Get Filename from DocumentOpen Dialog
	CGenViewerDoc* pDoc = GetDocument();
	if (NULL!=pDoc)
	{
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if (NULL  != pRenderer)
		{
//				pRenderer->Initialize(this->GetSafeHwnd(), m_pDC, rect.right, rect.top);
	//		pRenderer->OpenOsgFile(pDoc->GetFileName().GetString());
#ifdef OSG
		// Start the thread to do OSG Rendering
		//mThreadHandle = (HANDLE)_beginthread(&ViewerOsgRenderer::Render, 0, pRenderer); 
#endif	
		}
	}
}

void CGenViewerView::Init()
{
	m_pDC = new CClientDC(this);

	// Initialize the renderer, informing it of the pixel size of the view, the
	// view's handle, and the device context of its client area
	CRect rect;
	GetClientRect(rect);
	CGenViewerDoc* pDoc = GetDocument();
	if (NULL!=pDoc)
	{
		ViewerRenderer* pRenderer = pDoc->GetRenderer();
		if (NULL  != pRenderer)
		{
			pRenderer->Initialize(m_hWnd, m_pDC, rect.right, rect.top);
		}
	}


#ifdef VL

	vl::OpenGLContextFormat format;
	format.setDoubleBuffer(true);
	format.setRGBABits( 8,8,8,0 );
	format.setDepthBufferBits(24);
	format.setStencilBufferBits(8);
	format.setFullscreen(false);
	format.setMultisampleSamples(16);
	format.setMultisample(true);

	/* create a new vl::Rendering for this window */
	mRendering = new vl::Rendering;
	mRendering->renderer()->setFramebuffer( this->OpenGLContext::framebuffer() );

	OpenGLContext::framebuffer()->setWidth(rect.Width());
	OpenGLContext::framebuffer()->setHeight(rect.Height());


	/*edge renderer*/
	//ref< EdgeRenderer > mEdgeRenderer = new EdgeRenderer;
	//mEdgeRenderer->setFramebuffer( this->OpenGLContext::framebuffer());

	//mEdgeRenderer->setShowHiddenLines(true);
	//mEdgeRenderer->setShowCreases(true);
	//mEdgeRenderer->setCreaseAngle(35.0f);

	//// style options
	//mEdgeRenderer->setLineWidth(2.0f);
	//mEdgeRenderer->setSmoothLines(true);
	//mEdgeRenderer->setDefaultLineColor(black);

	//mRendering->renderers().push_back( mEdgeRenderer.get() );

	/*axis renderer*/
	ref< AxisRenderer > mAxisRenderer = new AxisRenderer;
	mAxisRenderer->setFramebuffer( this->OpenGLContext::framebuffer());
	mRendering->renderers().push_back( mAxisRenderer.get() );

	/* black background */
	mRendering->camera()->viewport()->setClearColor( vl::gray );

	/* create the applet to be run */
	mVLBaseView = new VLBaseView;
	mVLBaseView->setRendering(mRendering.get());
	
	mVLBaseView->initialize();

	/* bind the applet so it receives all the GUI events related to the OpenGLContext */
	this->OpenGLContext::addEventListener(mVLBaseView.get());



	//vl::ref<vl::Rendering> mAppletRendering = new vl::Rendering;
	//mAppletRendering->renderer()->setFramebuffer( this->OpenGLContext::framebuffer() );
	//mAppletRendering->camera()->viewport()->setClearColor(vl::white);
	//mAppletRendering->camera()->viewport()->setWidth(100);
	//mAppletRendering->camera()->viewport()->setHeight(100);


	//mAxisApplet= new AxisApplet;
	//mAxisApplet->setRendering(mAppletRendering.get());
	//mAxisApplet->initialize();
	///* bind the applet so it receives all the GUI events related to the OpenGLContext */
	//this->OpenGLContext::addEventListener(mAxisApplet.get());

	/* Initialize the OpenGL context and window properties */	
	CRect r; 	
	GetWindowRect(&r);
	Win32Context::initWin32GLContext(NULL, "Visualization Library MFC MDI- Rotating Cube", format, /*these last for are ignored*/0, 0, r.Width(), r.Height());

	MakeCube();

	mVLBaseView->SetViewMode(ViewIso,true);
	update();

#endif // VL
}

void CGenViewerView::MakeCube()
{
#ifdef VL
	// allocate the Transform 
	vl::ref<vl::Transform> mCubeTransform = new vl::Transform;
	// bind the Transform with the transform tree of the rendring pipeline 
	mVLBaseView->rendering()->as<vl::Rendering>()->transform()->addChild( mCubeTransform.get() );

	// create the cube's Geometry and compute its normals to support lighting 
	vl::ref<vl::Geometry> cube = vl::makeBox( vl::vec3(0,0,0),  vl::vec3(10,10,10), true);
	cube->computeNormals();

	// setup the effect to be used to render the cube 
	vl::ref<vl::Effect> effect = new vl::Effect;
	// enable depth test and lighting 
	effect->shader()->enable(vl::EN_DEPTH_TEST);
	// add a Light to the scene, since no Transform is associated to the Light it will follow the camera 

	vl::ref<vl::Light> light = new vl::Light;
	// since the fourth component is 0 OpenGL considers this a direction
	light->setPosition(vl::fvec4(0, 0, 1,0)); 

	effect->shader()->setRenderState( light.get(), 0 );
	// enable the standard OpenGL lighting 
	effect->shader()->enable(vl::EN_LIGHTING);
	// set the front and back material color of the cube 
	// "gocMaterial" stands for "get-or-create Material"
	effect->shader()->gocMaterial()->setDiffuse( vl::green );

	/* wireframe shader */
#if defined(VL_OPENGL)
	effect->lod(0)->push_back( new Shader );
	effect->shader(0,1)->setRenderState( new Light, 0 );
	effect->shader(0,1)->enable(EN_LIGHTING);
//	effect->shader(0,1)->gocLightModel()->setTwoSide(true);
	effect->shader(0,1)->gocMaterial()->setDiffuse(vl::blue);
	effect->shader(0,1)->gocPolygonMode()->set(vl::PM_LINE, vl::PM_LINE);
	effect->shader(0,1)->gocPolygonOffset()->set(-0.5f, 0);
	effect->shader(0,1)->enable(vl::EN_POLYGON_OFFSET_LINE);
	effect->shader(0,1)->enable(EN_DEPTH_TEST);
#endif

	// install our scene manager, we use the SceneManagerActorTree which is the most generic
	// add the cube to the scene using the previously defined effect and transform 
	mVLBaseView->sceneManager()->tree()->addActor( cube.get(), effect.get(), mCubeTransform.get()  );

	//float r = 0.2f;
	////arrow for x axis
	//vl::ref<vl::Geometry> xarrow = vl::makeBox( vl::vec3(0,0,0),  vl::vec3(11,-r,-r), true);
	//vl::ref<vl::Effect> effect1 = new vl::Effect; 
	//effect1->shader()->enable(vl::EN_DEPTH_TEST);
	//effect1->shader()->enable(vl::EN_LIGHTING);
	//effect1->shader()->setRenderState( new Light, 0 );
	//effect1->shader()->gocMaterial()->setDiffuse( vl::red );
	//mVLBaseView->sceneManager()->tree()->addActor( xarrow.get(), effect1.get(), mCubeTransform.get()  );

	////arrow for y axis
	//vl::ref<vl::Geometry> yarrow = vl::makeBox( vl::vec3(0,0,0),  vl::vec3(-r,11,-r), true);
	//vl::ref<vl::Effect> effect2 = new vl::Effect; 
	//effect2->shader()->enable(vl::EN_DEPTH_TEST);
	//effect2->shader()->enable(vl::EN_LIGHTING);
	//effect2->shader()->setRenderState( new Light, 0 );
	//effect2->shader()->gocMaterial()->setDiffuse( vl::green );
	//mVLBaseView->sceneManager()->tree()->addActor( yarrow.get(), effect2.get(), mCubeTransform.get()  );

	////arrow for z axis
	//vl::ref<vl::Geometry> zarrow = vl::makeBox( vl::vec3(0,0,0),  vl::vec3(-r,-r,11), true);
	//vl::ref<vl::Effect> effect3 = new vl::Effect; 
	//effect3->shader()->enable(vl::EN_DEPTH_TEST);
	//effect3->shader()->enable(vl::EN_LIGHTING);
	//effect3->shader()->setRenderState( new Light, 0 );
	//effect3->shader()->gocMaterial()->setDiffuse( vl::blue );
	//vl::ref<vl::Actor> zarrow_actor = new vl::Actor(zarrow.get(), effect3.get(), mCubeTransform.get());


	//vl::ref<AxisCameraUpdateCallback> axisCameraUpdateCallback = new AxisCameraUpdateCallback(zarrow_actor.get());
	//zarrow_actor->actorEventCallbacks()->push_back(axisCameraUpdateCallback.get());

	//mVLBaseView->sceneManager()->tree()->addActor( zarrow_actor.get());

//
//	  vl::ref<vl::Effect> name_effect = new vl::Effect;
//      name_effect->shader()->setRenderState( light.get(), 0 );
//      name_effect->shader()->enable(vl::EN_LIGHTING);
//      name_effect->shader()->enable(vl::EN_DEPTH_TEST);
//      name_effect->shader()->enable(vl::EN_CULL_FACE);
//      name_effect->shader()->disable(vl::EN_LIGHTING);
//      name_effect->shader()->enable(vl::EN_BLEND);
//      ///* to avoid clipping artefacts due to partial character overlapping we either disable depth
//      //   testing, set depth-write mask to false or enable an appropriate alpha testing. */
//      // name_effect->shader()->disable(vl::EN_DEPTH_TEST);
//      name_effect->shader()->enable(vl::EN_DEPTH_TEST);
//
//	vl::ref<vl::Text> text = new vl::Text;
//
////	vl::ref<vl::Font> font = vl::defFontManager()->acquireFont("/font/bitstream-vera/Vera.ttf", 10);
//	vl::ref<vl::Font> font = vl::defFontManager()->acquireFont("C:/Windows/Fonts/arial.ttf", 20);
//
//	text->setFont(font.get());
//	text->setMode( vl::Text2D );
////	text->setMode( vl::Text3D );
//	text->setText( L"The Viewer System");
//	text->setColor(vl::red);
//	text->setAlignment(vl::AlignTop | vl::AlignHCenter );
//	text->setViewportAlignment(vl::AlignTop | vl::AlignHCenter );
////	text->translate(0,-20,0);
//	mVLBaseView->sceneManager()->tree()->addActor( text.get(), name_effect.get() );


#endif
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
	mVLBaseView->FitWorld();
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

void CGenViewerView::OnToolsRendermodeGouraudWithLines() 
{
#ifdef HOOPS
	m_pHSolidView->RenderGouraudWithLines();
	m_pHSolidView->Update();
#endif
}

void CGenViewerView::OnUpdateToolsRendermodeGouraudWithLines(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	if (m_pHSolidView->GetRenderMode() == HRenderGouraudWithLines)
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);
#endif
}


void CGenViewerView::OnToolsRendermodeHiddenline() 
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderHiddenLine, true);
	m_pHSolidView->Update();
#endif
}

void CGenViewerView::OnUpdateToolsRendermodeHiddenline(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	if (m_pHSolidView->GetRenderMode() == HRenderHiddenLine || m_pHSolidView->GetRenderMode() == HRenderHiddenLineHOOPS)
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);
#endif
}

void CGenViewerView::OnToolsRendermodeWireframe() 
{
#ifdef HOOPS
	m_pHSolidView->SetRenderMode(HRenderWireframe, true);
	m_pHSolidView->Update();
#endif
}

void CGenViewerView::OnUpdateToolsRendermodeWireframe(CCmdUI* pCmdUI) 
{
#ifdef HOOPS
	if (m_pHSolidView->GetRenderMode() == HRenderWireframe)
		pCmdUI->SetCheck (1);
	else
		pCmdUI->SetCheck (0);
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
	mVLBaseView->SetViewMode(ViewIso);
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
	mVLBaseView->SetViewMode(ViewXY);
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
	mVLBaseView->SetViewMode(ViewYX);
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
	mVLBaseView->SetViewMode(ViewYZ);
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
	mVLBaseView->SetViewMode(ViewZY);
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
	mVLBaseView->SetViewMode(ViewXZ);
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
	mVLBaseView->SetViewMode(ViewZX);
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

void  CGenViewerView::OnFileOpen()
{
	CString filter = _T("");
	CString def_ext = _T(".hsf");
	def_ext = _T(".sldasm");

#ifdef INTEROP
	/*filter  = _T("Catia V5 Files (*.CATPart;*.CATProduct;*.CATShape;*.cgr)|*.CATPart;*.CATProduct;*.CATShape;*.cgr|");*/
	filter += _T("Solidworks Assembly Files (*.sldasm)|*.sldasm|");
	filter += _T("Solidworks Part Files (*.sldprt)|*.sldprt|");
	filter += _T("ACIS Files (*.sat;*.asat)|*.sat;*.asat|");
	filter += _T("Catia V4 Files (*.model;*.session;*.exp)|*.model;*.session;*.exp|");
	filter += _T("ProE Files (*.prt;*.prt.*;*.xpr;*.xpr.*;*.asm;*.asm.*;*.xas;*.xas.*)|*.prt;*.prt.*;*.xpr;*.xpr.*;*.asm;*.asm.*;*.xas;*.xas.*|");
	filter += _T("IGES Files (*.igs;*.iges)|*.igs;*.iges|");
	filter += _T("STEP Files (*.stp;*.step)|*.stp;*.step|");
	filter += _T("VDA Files(*.vda)|*.VDA|");
	filter += _T("UG Files (*.prt)|*.prt|");
	filter += _T("Inventor Files (*.ipt;*.iam)|*.ipt;*.iam|");
	filter += _T("Parasolid Files (*.x_t*;*.xmt_*)|*.x_t*;*.xmt_*|");/**/
	filter += _T("All Files (*.*)|*.*|");/**/
#ifdef ACIS
	filter += _T("HOOPS Files (*.hsf)|*.hsf|");
//	def_ext = _T(".sat;.asat");
//	def_ext = _T(".sldasm");
	//filter += _T("AcisHOOPS Stream Files (*.asf)|*.asf|");
#endif // ACIS
#endif // INTEROP

	CFileDialog file_dlg(TRUE, def_ext, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter , NULL);

	if (file_dlg.DoModal() == IDOK)
	{
		CString pathname;
		pathname = file_dlg.GetPathName();
#if defined(InterOp) && defined(HOOPS)
		if (m_pHSolidView)
		{
			if(m_bFlush)
			{
				m_pHSolidView->FlushProgressBar();
				m_pHSolidView->Flush();
				m_pHSolidView->GetModel()->Flush();
				m_bFlush=false;
			}
			char* c_pathname=pathname.GetBuffer(0);
			put_Filename(c_pathname);
		}
#endif
	}
}

void  CGenViewerView::put_Filename(char* c_pathname)
{
	if (c_pathname)
	{
		TCHAR error_msg[256] = {_T("")};
		TCHAR error_fmt[] = {_T("An error occured reading %s\nThe error code was %s.")};
#if defined(HOOPS) && defined(ACIS)
		HFileInputResult result = m_pHSolidView->GetModel()->Read(c_pathname, m_pHSolidView);
		switch(result)
		{
		case InputOK:
			{
				m_pHSolidView->FitWorld();
				m_pHSolidView->GetModel()->CleanIncludes();
				m_pHSolidView->Update();
				m_bFlush=true;
			}
			break;
		case InputVersionMismatch:
			wsprintf(error_msg, error_fmt, c_pathname, _T("InputVersionMismatch"));
			break;
		case InputFail:
			wsprintf(error_msg, error_fmt, c_pathname, _T("InputFail"));
			break;
		case InputNotHandled:
			wsprintf(error_msg, error_fmt, c_pathname, _T("InputNotHandled"));
			break;
		case InputBadFileName:
			wsprintf(error_msg, error_fmt, c_pathname, _T("InputBadFileName"));
			break;
		case InputBadOptions:
			wsprintf(error_msg, error_fmt, c_pathname, _T("InputBadOptions"));
			break;
		default:
			wsprintf(error_msg, error_fmt, c_pathname, _T("Unknown error"));
			break;
		}
		if(result != InputOK)
		{
			::MessageBox(NULL, error_msg, _T("ERROR!"), MB_OK);
		}
#endif
	}
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
	if (m_logoWidth > 0 && m_logoHeight > 0)
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



	//glColor4f(1.f,.0f,.0f,1.0f);
	//m_Xtext.text_out("x",axislength+0.01,0);
	//glColor4f(0.f,1.0f,.0f,1.0f);
	//m_Ytext.text_out("y",0,axislength+0.01);
	//glColor4f(0.f,0.0f,1.0f,1.0f);
	//m_Ztext.text_out("z",0,0,axislength+0.01);
}

void CGenViewerView::OnSize(UINT nType, int cx, int cy)
{
	if (mVLBaseView)
	{
		mVLBaseView->SetOldBufferFrameSize(this->OpenGLContext::framebuffer()->width(),
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

	if (m_pDC)
		delete m_pDC;

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
