#include "stdafx.h"
#include "math.h"
#include "float.h"
#include <string.h>
#include "mainfrm.h"
#include <math.h>
#include "ViewerVLRenderer.h"
#include "Math/GeometryMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static int RenderCounter = 0;
static int numberOfErrors = 0;
using namespace std;

ViewerVLRenderer::ViewerVLRenderer()
: ViewerRenderer()
{
// 	mInitialized=false;
// 	mPictureKey = 0;
// 	mLibraryKey= 0;
// 	mDistantLightKey=0;
}

ViewerVLRenderer::~ViewerVLRenderer()
{

}

void ViewerVLRenderer::Initialize(HWND wnd, CClientDC* pDC, int width, int height)
{
	if (pDC)
	{
		ViewerRenderer::Initialize(wnd, pDC, width, height);
	}
}

void ViewerVLRenderer::Terminate()
{

}

void ViewerVLRenderer::SetProjection()
{
	// Determine the vector from camera to target
	float viewVector[3];
	for (int i = 0; i < 3; i++)
		viewVector[i] = _cameraPosition[i] - _cameraTarget[i];

	// Determine the vector from the geometry center to the target
	float geomVector[3];
	for (int i = 0; i < 3; i++)
		geomVector[i] = _geometryCenter[i] - _cameraTarget[i];

	// Determine a radius for the view and, from it, the clipping planes
	float viewRadius = Length(geomVector) + _geometryRadius;
	float nearPlane = Length(viewVector) - viewRadius;
	float farPlane = Length(viewVector) + viewRadius;
	float aspect = 1.0f;
	if (_windowHeight > 0)
	{
		aspect = _windowWidth / _windowHeight;
	}

	float radius = Length(viewVector) * tan(GetFieldOfViewRadians() / 2.0f);
	float fleft   = -radius * aspect;
	float fright  =  radius * aspect;
	float fbottom = -radius;
	float ftop    =  radius;


	// Implement the requested projection

}

void ViewerVLRenderer::Display()
{
	//if (!mInitialized)
	//	return;

	//SetProjection(); // Call glOrtho or glPerspetive

	//// Update the camera light's position
	////// Display all items in the display list

	//printOpenSegments();
	//HC_Update_Display();
}

void ViewerVLRenderer::Make3DRay( int windowX, int windowY, float * rayVector, float * rayOrigin )
{	// Convert the camera vector to a pick ray
	float cameraPosition[ 3 ], cameraTarget[ 3 ], cameraUp[ 3 ];
	GetCamera( cameraPosition, cameraTarget, cameraUp );
	float viewVector[3];
	for (int i = 0; i < 3; i++)
		viewVector[i] = cameraPosition[i] - cameraTarget[i];
	float aspect = 1.0f;
	if ( _windowHeight > 0 )
		aspect = _windowWidth / _windowHeight;
	float cameraHeight = 2 * Length( viewVector ) * tan( GetFieldOfViewRadians() / 2 );
	float cameraWidth = cameraHeight * aspect;
	GetRayVector( windowX, windowY, (int)_windowWidth, (int)_windowHeight, cameraPosition, cameraTarget, cameraUp,
		cameraWidth, cameraHeight, ! GetDisplayOrtho(), rayOrigin, rayVector );
	Normalize( rayVector );
}

bool ViewerVLRenderer::PickPointOnModel( int windowX, int windowY, float * modelX, float * modelY, float * modelZ )
{
	return false;
}

void ViewerVLRenderer::StartRender()
{
	//// Clear all previous display items
	//HC_Open_Segment_By_Key(mLibraryKey); // Open Library Segment
	//{
	//	HC_Flush_Contents(".","segments, geometry");
	//}
	//segmentStack.push(mLibraryKey);
}

void ViewerVLRenderer::EndRender()
{
	//// debug code
	//printOpenSegments();
	//// end debug code
	//HC_Close_Segment(); // Close Library Segment
	//segmentStack.pop();
	//SetDisplayEdges(_displayEdges);
	//SetDisplayFaces(_displayFaces);
	//SetDisplayVertices(_displayVertices);
}

void ViewerVLRenderer::AddPolyline( std::vector<float> const& aEdgePositions, const RGBAColor &iColor )
{

	int floatCount = aEdgePositions.size();
	float* ptList = new float[floatCount];

	for (int i = 0;i<floatCount;i++)
		ptList[i]=aEdgePositions[i];
//	HC_Insert_Polyline(floatCount/3,ptList);

	delete ptList;

}

void ViewerVLRenderer::AddTriangleSet(
	std::vector<float> const& vertexPositionCoords,
	std::vector<float> const& vertexNormalCoords,
	std::vector<int> const& triangles,
	const RGBAColor &iColor)
{

	int floatCount = vertexPositionCoords.size();
	float* ptList = new float[floatCount];

	for (int i = 0;i<floatCount;i++)
		ptList[i]=vertexPositionCoords[i];

	int triCount = triangles.size();
	int newFaceSize = triCount+triCount/3;
	int* faceList = new int[newFaceSize];
	int jj = 0;
	int ii = 0;
	for(int counter = 0;counter<triCount;counter++)
	{
		if (jj == 0)
			faceList[ii++]=3;
		faceList[ii++]=triangles[counter];
		jj++;
		if (jj==3)
			jj=0;
	}

	//HC_KEY newShellKey = HC_Insert_Shell(floatCount/3,ptList,newFaceSize,faceList);

	//HC_Open_Geometry(newShellKey);
	//{
	//	for (int i = 0; i < (int)(vertexNormalCoords.size() / 3); i++)
	//	{
	//		HC_Open_Vertex(i);
	//		HC_Set_Normal(vertexNormalCoords[i * 3 + 0],vertexNormalCoords[i * 3 + 1],vertexNormalCoords[i * 3 + 2]);
	//		HC_Close_Vertex();
	//	}
	//}
	//HC_Close_Geometry();
	_totalTriCount += triCount / 3;

	// HoopsHelper.PopulateNormals(normals, faceShellKey);

	//	delete ptList;
	//	delete faceList;

}

//void
//ViewerVLRenderer::AddLine(float v0[3], float v1[2])
//{
//    float lv0[3];
//    float lv1[3];
//    for (int i = 0; i < 3; i++)
//    {
//       lv0[i] = v0[i];
//       lv1[i] = v1[i];
//    }
//    ViewerDLLineItem* pItem = new ViewerDLLineItem(lv0, lv1);
//    _entries.push_back(pItem);
//}

void ViewerVLRenderer::AddVertex(float v0[3], const RGBAColor &iColor)
{
//	HC_Insert_Marker(v0[0],v0[1],v0[2]);
}

void ViewerVLRenderer::SetWindowExtents(int width, int height)
{
	ViewerRenderer::SetWindowExtents(width, height);
}

void ViewerVLRenderer::OpenNode(int nodeId)
{
// 	HC_KEY idAsKey= (HC_KEY)fabs((float)nodeId);
// 	char idstr[32];
// 	itoa(idAsKey,idstr, 10);
// 	HC_KEY key = HC_KOpen_Segment(idstr);
// 	if(key<0)
// 	{
// 		// new segment
// 		HC_Renumber_Key(key, idAsKey, "local");
// 	}
// 	segmentStack.push(idAsKey);
}
void ViewerVLRenderer::CloseNode()
{
// 	HC_Close_Segment();
// 	segmentStack.pop();
}
void ViewerVLRenderer::SetColor(const RGBAColor &iColor)
{
	double r = (double)iColor.red/255;
	double g = (double)iColor.green/255;
	double b = (double)iColor.blue/255;
// 	HC_Set_Color_By_Value("faces","RGB",r,g,b);
// 	HC_Set_Color_By_Value("lines","RGB",r,g,b);
// 	HC_Set_Color_By_Value("markers","RGB",r,g,b);
}
void ViewerVLRenderer::SetDisplayFaces(bool displayFaces)
{
	_displayFaces = displayFaces;
// 	HC_Open_Segment_By_Key(mPictureKey);
// 	{
// 		if (displayFaces)
// 			HC_Set_Visibility("faces = on");
// 		else
// 			HC_Set_Visibility("faces = off");
// 	}
// 	HC_Close_Segment();
}

void ViewerVLRenderer::SetDisplayEdges(bool displayEdges)
{
	_displayEdges = displayEdges;
// 	HC_Open_Segment_By_Key(mPictureKey);
// 	{
// 		if (displayEdges)
// 			HC_Set_Visibility("lines = on");
// 		else
// 			HC_Set_Visibility("lines = off");
// 	}
// 	HC_Close_Segment();
}

void ViewerVLRenderer::SetDisplayVertices(bool displayVertices)
{
	_displayVertices = displayVertices;
// 	HC_Open_Segment_By_Key(mPictureKey);
// 	{
// 		if (displayVertices)
// 			HC_Set_Visibility("markers = on");
// 		else
// 			HC_Set_Visibility("markers = off");
// 	}
// 	HC_Close_Segment();
}


void ViewerVLRenderer::SaveToFile()
{
//    char pictName[1000];
//     HC_Show_Segment(mPictureKey, pictName);
// 	HC_Write_Metafile(pictName, "c:/temp/scene.hmf", "follow cross-references, use color names, save state");
}
