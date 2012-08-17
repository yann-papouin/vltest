#pragma once
#include "renderer/ViewerRenderer.h"
#include "SelectionManipulator.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

//#include "SceneFunctions.h"

//#include "SimpleScene.h"

////////////////////////////////////////////
// Scenix implementation of Renderer
////////////////////////////////////////////
class ViewerOsgRenderer : public ViewerRenderer
{
public:
    ViewerOsgRenderer();
    ~ViewerOsgRenderer();

	// Called when the view is created
	//    wnd:		handle to the view window
	//    pDC:		device context for the client area of the view
	//    width:	current view width (pixels)
	//    height:	current view height (pixels)
    virtual void Initialize(HWND wnd, CClientDC* pDC, int width, int height);
	void InitManipulators(void);
	void InitCameraConfig(void);

	// Called when the view is destroyed
    virtual void Terminate();

    // Rendering a new scene
    virtual void StartRender();
	// Finish the scene
    virtual void EndRender();
    // Display to the view
    static void Render(void* ptr);
	virtual void Display(){};
	virtual void OpenNode(int nodeId);
	virtual void CloseNode();

	void ReadNodeFile(std::string modelName);

	// Add geometric items to the display list
	virtual void AddTriangleSet(
				std::vector<float> const& iVertexPositionCoords,
				std::vector<float> const& iVertexNormalCoords,
				std::vector<int> const& iTriangles,
				const RGBAColor &iColor
				);
	virtual void AddTriangleSet(
				float* iVertexPositionCoords,
				float* iVertexNormalCoords,
				int* iTriangles,
				int positionCount,
				int normalCount,
				int triangleCount,
				const RGBAColor &iColor
				) {};

	virtual void AddPolyline( std::vector<float> const& aEdgePositions, const RGBAColor &iColor);
	virtual void AddPolyline( float* aEdgePositions, int positionCount, const RGBAColor &iColor) {};

	// Add a single point to represent a vertex
	//    v0:		vertex position
    virtual void AddVertex( float v0[3], const RGBAColor &iColor);


    // Record the view extents in pixels
	//    width:	window width (pixels)
	//    height:	window height (pixels)
    virtual void SetWindowExtents(int width, int height);

	// Pick a point on model using the display screen
	virtual bool PickPointOnModel( int windowX, int windowY, float * modelX, float * modelY, float * modelZ );

	// Using an X, Y position on the screen, make a 3D ray
	void Make3DRay( int windowX, int windowY, float * rayVector, float * rayOrigin );

	virtual void SetCamera(float* cameraPosition, float* cameraTarget, float* cameraUp);

	virtual void ViewerOsgRenderer::SetDisplayOrtho();

	osgViewer::Viewer* GetViewer(){return mViewer;};

private:
	CClientDC* m_pDC;

	HWND m_hWnd;

	bool mDone;
	std::string mModelName;
	osgViewer::Viewer* mViewer;

	osg::ref_ptr<osg::Group> mRoot;
	osg::ref_ptr<osg::Node> mModel;
	osg::ref_ptr<osgGA::TrackballManipulator> trackball;
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;
	osg::ref_ptr<SelectionManipulator> selectionOperator;


	void initializeRenderContext();
};

