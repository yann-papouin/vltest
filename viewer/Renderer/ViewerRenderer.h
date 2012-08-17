#ifndef VIEWERRENDERER_H
#define VIEWERRENDERER_H

////////////////////////////////////////////
// Abstract base class for rendering
////////////////////////////////////////////
#include "stdafx.h"
#include <vector>
#include "Util/PerformanceMetrics.h"

#define OPEN_NODE( nodeId )\
{\
  PerformanceMetrics::Start( GRAPH_BUILDING_TIME );\
  m_renderer->OpenNode( nodeId );\
  PerformanceMetrics::Stop( GRAPH_BUILDING_TIME );\
}

#define CLOSE_NODE()\
{\
  PerformanceMetrics::Start( GRAPH_BUILDING_TIME );\
  m_renderer->CloseNode();\
  PerformanceMetrics::Stop( GRAPH_BUILDING_TIME );\
}
#define SET_COLOR( colorId )\
{\
  PerformanceMetrics::Start( RENDERING_TIME );\
  m_renderer->SetColor( colorId);\
  PerformanceMetrics::Stop( RENDERING_TIME );\
}


struct RGBAColor
{
	//Default color to opaque black 
	RGBAColor() 
	{
		red   = 0;
		green = 0;
		blue  = 0;
		alpha = 255;
	};

	//Here we omitted the check if inputs are valid
	RGBAColor(long iRed, long iGreen, long iBlue, long iAlpha)
	{
		red   = iRed;
		green = iGreen;
		blue  = iBlue;
		alpha = iAlpha;
	};

	//copy construcator
	RGBAColor(const RGBAColor &iColor)
	{
		*this = iColor;
	};

	void operator=(const RGBAColor& iSrcColor)
	{
		red   = iSrcColor.red;
		green = iSrcColor.green;
		blue  = iSrcColor.blue;
		alpha = iSrcColor.alpha;
	}

	int ToInt32() const
	{
		// Making ARGB
		int a = ((alpha & 0xff) / 2) << 24;
		int r = (red & 0xff) << 16;
		int g = (green & 0xff) << 8;
		int b = (blue & 0xff);
		return (a | r | g | b);
	}

	long red;
	long green;
	long blue;
	long alpha;
};


class ViewerRenderer
{
public:
#ifdef _USE_SCENIX_RENDERER
	static const unsigned long EntityTypeFace = 123456787;	
	static const unsigned long EntityTypeEdge = 123456788;	
	static const unsigned long EntityTypeVertex = 123456789;
#else
	static const unsigned long EntityTypeFace = 3;	
	static const unsigned long EntityTypeEdge = 2;	
	static const unsigned long EntityTypeVertex = 1;

#endif
	static const int RED_RENDERER = 1;
	static const int HOOPS_RENDERER = 2;
	static const int SCENIX_RENDERER = 3;

public:
    ViewerRenderer();
    virtual ~ViewerRenderer();

	// Called when the view is created
	//    wnd:		handle to the view window
	//    pDC:		device context for the client area of the view
	//    width:	current view width (pixels)
	//    height:	current view height (pixels)
    virtual void Initialize(HWND wnd, CClientDC* pDC, int width, int height);
	// Called when the view is destroyed
    virtual void Terminate();

    // Rendering a new scene
    virtual void StartRender() = 0;
	// Finish the scene
    virtual void EndRender() = 0;

	//virtual void CreateNode(int nodeId);
	virtual void OpenNode(int nodeId) = 0;
	virtual void CloseNode() = 0;
	virtual void DeleteNode(int nodeId) {};

	// Set color on current node
	virtual void SetColor(const RGBAColor &iColor) {};

	// Add geometric items to the scene

	// Add a set of triangles to the scene.
	virtual void AddTriangleSet(
				std::vector<float> const& iVertexPositionCoords,
				std::vector<float> const& iVertexNormalCoords,
				std::vector<int> const& iTriangles,
				const RGBAColor &iColor
				)=0;
	virtual void AddTriangleSet(
				float* iVertexPositionCoords,
				float* iVertexNormalCoords,
				int* iTriangles,
				int positionCount,
				int normalCount,
				int triangleCount,
				const RGBAColor &iColor
				)=0;

	virtual void AddPolyline( std::vector<float> const& aEdgePositions, const RGBAColor &iColor)=0;
	virtual void AddPolyline( float* aEdgePositions, int positionCount, const RGBAColor &iColor)=0;

	// Add a single point to represent a vertex
	//    v0:		vertex position
    virtual void AddVertex(float v0[3], const RGBAColor &iColor) = 0;

	// Estimate the center and radius of the geometry to be rendered
    const float* GetGeometryCenter() const;
    const float GetGeometryRadius() const;
    void SetGeometryExtents(float center[3], float radius );

    // Display to the view
	virtual void Display()=0;

    // Record the view extents in pixels
	//    width:	window width (pixels)
	//    height:	window height (pixels)
    virtual void SetWindowExtents(int width, int height);

	// Pick a point on model using the display screen
	virtual bool PickPointOnModel( int windowX, int windowY, float * modelX, float * modelY, float * modelZ ) = 0;

	// Toggle the display of faces, edges, and vertices
    virtual void SetDisplayFaces(bool displayFaces);
    virtual bool GetDisplayFaces() const;
    virtual void SetDisplayEdges(bool displayEdges);
    virtual bool GetDisplayEdges() const;
    virtual void SetDisplayVertices(bool displayVertices);
    virtual bool GetDisplayVertices() const;
	// Toggle the projection method between orthographic and perspective
    virtual void SetDisplayOrtho(bool displayOrtho);
    virtual bool GetDisplayOrtho() const;

    // Specify camera settings
	//    cameraPosition:	position from which the scene is viewed
	//    cameraTarget:		position to which the view is directed
	//    cameraUp:			the "up" position relative to the view
    virtual void  SetCamera(float* cameraPosition, float* cameraTarget, float* cameraUp);
    virtual void  GetCamera(float* cameraPosition, float* cameraTarget, float* cameraUp);
	//    degrees:			field of view expressed in degrees
    virtual void  SetFieldOfView(float degrees);
    virtual float GetFieldOfView();
	virtual float GetFieldOfViewRadians();

	// Standard camera settings
	virtual void  Zoom(float delta);
	virtual void  ZoomAll();
	virtual void  Top();
	virtual void  Front();
	virtual void  Right();
	virtual void  Isometric();

	// Returns the reference orientation from which top, front, right, and isometric views are created
	void SetStandardCameraPositionAndUp(float * cameraPosition, float * cameraTarget, float * cameraUp);

	virtual void SaveToFile() {};

	virtual void SetTestMode(CString mode){};
	virtual int GetTotalTriangleCount() { return _totalTriCount; };

protected:
	// Booleans to toggle display of geometry
    bool _displayFaces;
    bool _displayEdges;
    bool _displayVertices;
	// Boolean to set projection to orthographic (true) or perspective (false)
	bool _displayOrtho;

	// Approximate center of the geometry being displayed
    float _geometryCenter[3];
	// Approximate radius of the geometry being displayed
    float _geometryRadius;

	// Width of the view in pixels
    float _windowWidth;
	// Height of the view in pixels
    float _windowHeight;

	// Position of the camera
	float _cameraPosition[3];
	// Target position
	float _cameraTarget[3];
	// Direction to use as "up"
	float _cameraUp[3];
	// Field of view, in radians
	float _FOV; 

	int _totalTriCount;
};

#endif
