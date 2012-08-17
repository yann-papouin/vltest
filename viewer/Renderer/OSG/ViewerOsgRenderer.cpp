#include "stdafx.h"
#include "process.h"
#include "ViewerOsgRenderer.h"
#include "math/GeometryMath.h"

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgGA/FlightManipulator>
#include <osgDB/FileNameUtils>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace osg;

ViewerOsgRenderer::ViewerOsgRenderer() : ViewerRenderer()
{
	
}

ViewerOsgRenderer::~ViewerOsgRenderer()
{
	//mViewer->setDone(true);
	//Sleep(1000);
	//mViewer->stopThreading();
}

void 
ViewerOsgRenderer::Initialize(HWND hWnd, CClientDC* pDC, int width, int height)
{
	if (NULL != pDC)
	{
		ViewerRenderer::Initialize(hWnd, pDC, width, height);
		m_hWnd = hWnd;
		m_pDC = pDC;
		HDC tempHandle = m_pDC->GetSafeHdc();
	}

	// Init different parts of OSG	
	
	InitManipulators();
	InitCameraConfig();
}

void ViewerOsgRenderer::InitManipulators(void)
{
	// Create a trackball manipulator
	trackball = new osgGA::TrackballManipulator();
	selectionOperator = new SelectionManipulator();
	//selectionOperator->setVerticalAxisFixed(true);

	// Create a Manipulator Switcher
	keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

	//keyswitchManipulator->addMatrixManipulator( '1', "Trackball", trackball.get());
	keyswitchManipulator->addMatrixManipulator( '1', "SelectionManipulator",
		selectionOperator.get());

	// Init the switcher to the first manipulator (in this case the only manipulator)
	keyswitchManipulator->selectMatrixManipulator(0);  // Zero based index Value
}
void ViewerOsgRenderer::InitCameraConfig(void)
{
	// Local Variable to hold window size data
	RECT rect;

	// Create the viewer for this window
	mViewer = new osgViewer::Viewer();

	// Add a Stats Handler to the viewer
	mViewer->addEventHandler(new osgViewer::StatsHandler);

	// Get the current window size
	::GetWindowRect(m_hWnd, &rect);

	// Init the GraphicsContext Traits
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

	// Init the Windata Variable that holds the handle for the Window to display OSG in.
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);

	// Setup the traits parameters
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;

	// Create the Graphics Context
	osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	// Init a new Camera (Master for this View)
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;

	// Assign Graphics Context to the Camera
	camera->setGraphicsContext(gc);

	// Set the viewport for the Camera
	camera->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));

	// Set projection matrix and camera attribtues
	camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);

	// Add the Camera to the Viewer
	//mViewer->addSlave(camera.get());
	mViewer->setCamera(camera.get());

	// Add the Camera Manipulator to the Viewer
	mViewer->setCameraManipulator(keyswitchManipulator.get());

	// Set the Scene Data
	mViewer->setSceneData(mRoot.get());

	// Realize the Viewer
	mViewer->realize();
}

void 
ViewerOsgRenderer::ReadNodeFile(std::string modelName)
{

	mModelName = modelName;

	mRoot  = new osg::Group;

	// Load the Model from the model name
	mModel = osgDB::readNodeFile(modelName);
	if (NULL!=mModel)	{		// Optimize the model
		osgUtil::Optimizer optimizer;
		optimizer.optimize(mModel.get());
		optimizer.reset();

		// Add the model to the scene
		mRoot->addChild(mModel.get());
	}}

void
ViewerOsgRenderer::Terminate()
{

}

void 
ViewerOsgRenderer::SetDisplayOrtho()
{
	
}

void 
ViewerOsgRenderer::initializeRenderContext()
{

}

void 
ViewerOsgRenderer::StartRender()
{
	mViewer->requestRedraw();
}

void 
ViewerOsgRenderer::Render(void* ptr)
{
	ViewerOsgRenderer* osg = (ViewerOsgRenderer*)ptr;

	osgViewer::Viewer* viewer = osg->GetViewer();

	while(!viewer->done())
	{
		osg->StartRender();
		viewer->frame();
		osg->EndRender();
		//Sleep(10);         // Use this command if you need to allow other processes to have cpu time
	}

	_endthread();
}

void
ViewerOsgRenderer::OpenNode(int nodeId)
{

}

void
ViewerOsgRenderer::CloseNode() 
{

}

void 
ViewerOsgRenderer::EndRender()
{
	
}

void 
ViewerOsgRenderer::Make3DRay( int windowX, int windowY, float * rayVector, float * rayOrigin )
{	

}

bool 
ViewerOsgRenderer::PickPointOnModel( int windowX, int windowY, float * modelX, float * modelY, float * modelZ )
{
	return false;
}

void 
ViewerOsgRenderer::AddPolyline( std::vector<float> const& aEdgePositions, const RGBAColor &iColor )
{

}

void 
ViewerOsgRenderer::AddTriangleSet(
				std::vector<float> const& vertexPositionCoords,
				std::vector<float> const& vertexNormalCoords,
				std::vector<int> const& triangles,
				const RGBAColor &iColor)
{

}

void 
ViewerOsgRenderer::AddVertex(float v0[3], const RGBAColor &iColor)
{
    
}

void 
ViewerOsgRenderer::SetWindowExtents(int width, int height)
{
  // ViewerRenderer::SetWindowExtents(width, height);
  // if (g_renderTargetGL != NULL)
		//g_renderTargetGL->setSize(width,height);
}


void 
ViewerOsgRenderer::SetCamera(float* cameraPosition, float* cameraTarget, float* cameraUp)
{
	ViewerRenderer::SetCamera(cameraPosition,cameraTarget,cameraUp);

	//CameraSharedPtr camera = getViewStateCamera();
	//if (camera != NULL)
	//{
	//	CameraWriteLock cameraLock(camera);
	//	osg::Vec3f cameraDirection(cameraTarget[0]-cameraPosition[0],cameraTarget[1]-cameraPosition[1],cameraTarget[2]-cameraPosition[2]);
	//	cameraDirection.normalize();
	//	osg::Vec3f cameraUpVector(cameraUp[0],cameraUp[1],cameraUp[2]);
	//	cameraUpVector.normalize();

	//	cameraLock->setPosition(Vec3f(cameraPosition[0],cameraPosition[1],cameraPosition[2]));
	//	cameraLock->setDirection(cameraDirection);
	//	cameraLock->setUpVector(cameraUpVector);
	//}
}

static void 
GetCenter( float* pos, std::vector<float> const& points )
{
	
}
