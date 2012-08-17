#ifndef VIEWERHOOPSRENDERER_H
#define VIEWERHOOPSRENDERER_H

#include <hc.h>
#include "Renderer/ViewerRenderer.h"
#include <vector>
#include <stack>

class ViewerDLItem;

////////////////////////////////////////////
// Hoops implementation of Renderer
////////////////////////////////////////////
class ViewerHoopsRenderer : public ViewerRenderer
{
public:

    ViewerHoopsRenderer();
    ~ViewerHoopsRenderer();

	// Called when the view is created
	//    wnd:		handle to the view window
	//    pDC:		device context for the client area of the view
	//    width:	current view width (pixels)
	//    height:	current view height (pixels)
    virtual void Initialize(HWND wnd, CClientDC* pDC, int width, int height);
	// Called when the view is destroyed
    virtual void Terminate();

    // Rendering a new scene
    virtual void StartRender();
	// Finish the scene
    virtual void EndRender();

	virtual void OpenNode(int nodeId);
	virtual void CloseNode();
	virtual void DeleteNode(int nodeId) {};

	virtual void SetColor(const RGBAColor &iColor);
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

    // Display to the view
    virtual void Display();

    // Record the view extents in pixels
	//    width:	window width (pixels)
	//    height:	window height (pixels)
    virtual void SetWindowExtents(int width, int height);

	// Pick a point on model using the display screen
	virtual bool PickPointOnModel( int windowX, int windowY, float * modelX, float * modelY, float * modelZ );

	// Using an X, Y position on the screen, make a 3D ray
	void Make3DRay( int windowX, int windowY, float * rayVector, float * rayOrigin );

	virtual void SetDisplayFaces(bool displayFaces);
    virtual void SetDisplayEdges(bool displayEdges);
    virtual void SetDisplayVertices(bool displayVertices);
	
	virtual void SaveToFile();


private:
  
	HC_KEY mPictureKey;
	HC_KEY mLibraryKey;
	HC_KEY mDistantLightKey;

	std::stack<HC_KEY> segmentStack;
	bool mInitialized;

	char* mPictureSegmentName;
	char* mLibrarySegmentName;
	void SetProjection();
};

#endif
