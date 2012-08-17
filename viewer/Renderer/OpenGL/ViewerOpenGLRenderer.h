#ifndef VIEWEROPENGLRENDERER_H
#define VIEWEROPENGLRENDERER_H

#include "../ViewerRenderer.h"
#include "gl/gl.h"
#include "gl/glu.h"
class ViewerDLItem;
////////////////////////////////////////////
// OpenGL implementation of Renderer
////////////////////////////////////////////
class ViewerOpenGLRenderer : public ViewerRenderer
{
public:
    ViewerOpenGLRenderer();
    ~ViewerOpenGLRenderer();

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

	virtual void OpenNode(int nodeId) {};
	virtual void CloseNode() {};
	virtual void DeleteNode(int nodeId) {};
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

private:

	void ClearEntries();
	void ClearLists();

	bool m_RetainMode;
	GLuint m_FaceListId;
	GLuint m_EdgeListId;
	GLuint m_VertexListId;
    // List of items to display
    std::vector<ViewerDLItem*> _face_entries;
	std::vector<ViewerDLItem*> _edge_entries;
	std::vector<ViewerDLItem*> _vertex_entries;
	void SetProjection();
};

#endif
