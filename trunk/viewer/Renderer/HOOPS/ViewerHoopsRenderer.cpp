#include "stdafx.h"
#include "math.h"
#include "float.h"
#include <string.h>
#include "mainfrm.h"
#include <math.h>
#include "ViewerHoopsRenderer.h"
#include "CGMGeometryMath.h"
#include "Debug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static int RenderCounter = 0;
static int numberOfErrors = 0;
using namespace std;
static void hoops_error_handler(int category, int specific, int severity, int msgc,
								char** msgv, int stackc, char** stackv)
{
	numberOfErrors++;
	if (specific == 305) // Specified window coordinates not in the range of -1 to +1
		return;
	string totalMessage = "";
	string message = "";
	for (int i = 0; i < msgc; i++)
	{
		message.clear();
		message.append(string(msgv[i]));
		if (i > 0)
			totalMessage.append(string(" \n"));
		totalMessage.append(message);
	}
	if (msgc > 0)
		totalMessage.append(string(" \n"));
	for (int i = 0; i < stackc; i++)
	{
		message.clear();
		message.append(string(stackv[i]));
		if (i > 0)
			totalMessage.append(string(" \n"));
		totalMessage.append(message);
	}

	//StringBuilder sss = new StringBuilder(1024);
	//HCS.Show_Pathname_Expansion(".", sss);

	//UNDO
	dbg << "*** HOOPS error : "<<totalMessage << endl;
}


static void printOpenSegments()
{
	int count = 0;
	HC_Begin_Open_Segment_Search();
	HC_Show_Open_Segment_Count(&count);
	if (count > 0)
	{
		printf("Opened segments ( %d )", + count);
		for (int i = 0; i < count; i++)
		{
			char* tempSegment = new char[1024];
			HC_Find_Open_Segment(tempSegment);
			printf("Segment Path is %s\n",tempSegment);
		}
		HC_End_Open_Segment_Search();
	}
}

ViewerHoopsRenderer::ViewerHoopsRenderer()
: ViewerRenderer()
{
	mInitialized=false;
	mPictureKey = 0;
	mLibraryKey= 0;
	mDistantLightKey=0;
}

ViewerHoopsRenderer::~ViewerHoopsRenderer()
{
	if(mPictureSegmentName)
		HC_UnDefine_Alias(mPictureSegmentName);
	if (mPictureKey)
	{
		HC_Open_Segment_By_Key(mPictureKey);
		{
			HC_Flush_Contents(".","segments,geometry");
		}
		HC_Close_Segment();
		HC_Delete_By_Key(mPictureKey);
	}
	mPictureKey=0;

	if(mLibraryKey)
	{
		HC_Open_Segment_By_Key(mLibraryKey);
		{
			HC_Flush_Contents(".","segments,geometry");
		}
		HC_Close_Segment();
		HC_Delete_By_Key(mLibraryKey);
	}
	mLibraryKey = 0;
	
	mInitialized = false;

	if (mPictureSegmentName)
		delete mPictureSegmentName;
	mPictureSegmentName = 0;
	if (mLibrarySegmentName)
		delete mLibrarySegmentName;
	mLibrarySegmentName = 0;

	HC_UnDefine_Error_Handler((void (HC_CDECL*)(HC_ANY_ARGS))hoops_error_handler);
}

void ViewerHoopsRenderer::Initialize(HWND wnd, CClientDC* pDC, int width, int height)
{
	if (pDC)
	{
		ViewerRenderer::Initialize(wnd, pDC, width, height);
		char* completeLicense = new char[1024];
		sprintf(completeLicense,"license=(customer = spatial corp., product = (im, kanji, classic), key = 125A9FEC-EC764B7-5B58077F-3F5C7F)");
		HC_Define_System_Options(completeLicense);

		//static PIXELFORMATDESCRIPTOR pfd =
		//{
		//	sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		//	1,                              // version number
		//	PFD_DRAW_TO_WINDOW |            // draw to window rather than bitmap
		//	  PFD_SUPPORT_OPENGL |          // support OpenGL
		//	  PFD_DOUBLEBUFFER,             // use double buffering
		//	PFD_TYPE_RGBA,                  // RGBA type
		//	24,                             // 24-bit color depth
		//	0, 0, 0, 0, 0, 0,               // color bits ignored
		//	0,                              // no alpha buffer
		//	0,                              // shift bit ignored
		//	0,                              // no accumulation buffer
		//	0, 0, 0, 0,                     // accum bits ignored
		//	32,                             // 32-bit z-buffer
		//	0,                              // no stencil buffer
		//	0,                              // no auxiliary buffer
		//	PFD_MAIN_PLANE,                 // main layer
		//	0,                              // reserved
		//	0, 0, 0                         // layer masks ignored
		//};


		char* tempStr = new char[16];
		//Create Library Segment First
		mLibrarySegmentName = new char[128];
		itoa(RenderCounter,tempStr,10);
		sprintf(mLibrarySegmentName,"?include library/%d",RenderCounter);

		mLibraryKey = HC_Create_Segment(mLibrarySegmentName);


		// Create Picture Segment Next
		mPictureSegmentName = new char[128];
		char* fullPath = new char[256];
		sprintf(mPictureSegmentName,"%s%d","?picture",(int)wnd);
		sprintf(fullPath,"?driver/OpenGl/window%d",RenderCounter);

		HC_Define_Alias(mPictureSegmentName, fullPath);
		mPictureKey=HC_Create_Segment(mPictureSegmentName);
		HC_Open_Segment_By_Key(mPictureKey);
		{

			char* driverOptionWindowHandle = new char[128];
			sprintf(driverOptionWindowHandle,"use window ID = %d",wnd);
			HC_Set_Driver_Options(driverOptionWindowHandle);
			HC_Set_Driver_Options("double-buffering");
			HC_Set_Visibility("edges = off, markers = off");
			HC_Set_Selectability("faces = on, lines = on, edges = off");
			HC_Set_Handedness("right");
			HC_Set_Marker_Size(0.25);
			HC_Set_Marker_Symbol("[*]");
			// --- Rendering options
			//HC_Set_Rendering_Options("anti-alias, display lists = segment, lighting = gouraud, level of detail, hsra = hzb");
			HC_Set_Rendering_Options("no anti-alias, display lists = segment, lighting = gouraud, level of detail, hsra = hzb"); // Bashi
			HC_Set_Heuristics("backplane cull, polygon handedness = left");
			HC_Set_Heuristics("visual selection = on");

			static RGBAColor backgroundColor(255,255,255,255);
			static RGBAColor defaultFaceColor(172, 117, 111, 255);
			static RGBAColor defaultEdgeColor(0, 0, 0, 255);
			static RGBAColor defaultVertexColor(0, 0, 0, 255);



			HC_Set_Color_By_Value("window", "RGB", backgroundColor.red / 255.0, backgroundColor.green / 255.0, backgroundColor.blue / 255.0);
			HC_Set_Color_By_Value("lines", "RGB", defaultEdgeColor.red / 255.0, defaultEdgeColor.green / 255.0, defaultEdgeColor.blue / 255.0);
			HC_Set_Color_By_Value("edges", "RGB", defaultEdgeColor.red / 255.0, defaultEdgeColor.green / 255.0, defaultEdgeColor.blue / 255.0);
			HC_Set_Color_By_Value("faces", "RGB", defaultFaceColor.red / 255.0, defaultFaceColor.green / 255.0, defaultFaceColor.blue / 255.0);
			HC_Set_Color_By_Value("markers", "RGB", defaultVertexColor.red / 255.0, defaultVertexColor.green / 255.0, defaultVertexColor.blue / 255.0);



			// --- Light 
			mDistantLightKey = HC_Insert_Distant_Light(_cameraPosition[0],_cameraPosition[1],_cameraPosition[2]);
			//mAmbientLightKey = HC_Insert_Local_Light(_cameraPosition[0],_cameraPosition[1],_cameraPosition[2]);

			// Setup the camera light
			//glEnable(GL_LIGHTING); 
			//GLfloat cameraSpecularColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };
			//GLfloat cameraDiffuseColor[] = { 0.6f, 0.6f, 0.6f, 1.0 };
			//glLightfv( GL_LIGHT0, GL_SPECULAR, cameraSpecularColor );
			//glLightfv( GL_LIGHT0, GL_DIFFUSE,  cameraDiffuseColor );
			//glEnable( GL_LIGHT0 );
			//glLightModelf( GL_LIGHT_MODEL_TWO_SIDE, 1.0f );

			//// Define the model shininess and the reflected color
			//GLfloat matAmbientColor[] = { 0.234375f, 0.234375f, 0.234375f, 1.0};
			//GLfloat matSpecularColor[] = { 0.566681f, 0.580872f, 0.580874f, 1.0};
			//GLfloat matDiffuseColor[] = { 0.673937f, 0.457340f, 0.436528f, 1.0 };
			//GLfloat matShininess[] = { 0.168182f * 128 };
			//glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientColor);
			//glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularColor);
			//glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseColor);
			//glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
			//


			HC_Define_Error_Handler((void (HC_CDECL*)(HC_ANY_ARGS))hoops_error_handler);




			//Now do the include
			HC_Include_Segment_By_Key(mLibraryKey);
			mInitialized=true;
		}
		HC_Close_Segment();
		RenderCounter++;
	}
}

void ViewerHoopsRenderer::Terminate()
{
	if(mPictureSegmentName)
		HC_UnDefine_Alias(mPictureSegmentName);
	if (mPictureKey)
	{
		HC_Open_Segment_By_Key(mPictureKey);
		{
			HC_Flush_Contents(".","segments,geometry");
		}
		HC_Close_Segment();
		HC_Delete_By_Key(mPictureKey);
	}
	mPictureKey=0;

	if(mLibraryKey)
	{
		HC_Open_Segment_By_Key(mLibraryKey);
		{
			HC_Flush_Contents(".","segments,geometry");
		}
		HC_Close_Segment();
		HC_Delete_By_Key(mLibraryKey);
	}
	mLibraryKey = 0;
	
	mInitialized = false;

	if (mPictureSegmentName)
		delete mPictureSegmentName;
	mPictureSegmentName = 0;
	if (mLibrarySegmentName)
		delete mLibrarySegmentName;
	mLibrarySegmentName = 0;

}

void ViewerHoopsRenderer::SetProjection()
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
	HC_Open_Segment_By_Key(mPictureKey);
	{
		if ( GetDisplayOrtho() )
		{

			//glOrtho(fleft, fright, fbottom, ftop, nearPlane, farPlane);
			HC_Set_Camera(_cameraPosition,_cameraTarget,_cameraUp,fright-fleft,ftop-fbottom,"orthographic");
		}
		else
		{
			float limit = _geometryRadius / 500.0f;
			if (nearPlane < limit)
				nearPlane = limit;
			//gluPerspective( GetFieldOfView(), aspect, nearPlane, farPlane );
			HC_Set_Camera_Near_Limit(nearPlane);
			HC_Set_Camera(_cameraPosition,_cameraTarget,_cameraUp,fleft-fright,fbottom-ftop,"perspective");
		}
		HC_Move_Distant_Light(mDistantLightKey, _cameraPosition[0] -_cameraTarget[0], 
			_cameraPosition[1] -_cameraTarget[1], _cameraPosition[2] -_cameraTarget[2]);

	}
	HC_Close_Segment();
}

void ViewerHoopsRenderer::Display()
{
	if (!mInitialized)
		return;

	SetProjection(); // Call glOrtho or glPerspetive

	// Update the camera light's position
	//// Display all items in the display list

	printOpenSegments();
	HC_Update_Display();
}

void ViewerHoopsRenderer::Make3DRay( int windowX, int windowY, float * rayVector, float * rayOrigin )
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

bool ViewerHoopsRenderer::PickPointOnModel( int windowX, int windowY, float * modelX, float * modelY, float * modelZ )
{
	return false;
}

void ViewerHoopsRenderer::StartRender()
{
	// Clear all previous display items
	HC_Open_Segment_By_Key(mLibraryKey); // Open Library Segment
	{
		HC_Flush_Contents(".","segments, geometry");
	}
	segmentStack.push(mLibraryKey);
}

void ViewerHoopsRenderer::EndRender()
{
	// debug code
	printOpenSegments();
	// end debug code
	HC_Close_Segment(); // Close Library Segment
	segmentStack.pop();
	SetDisplayEdges(_displayEdges);
	SetDisplayFaces(_displayFaces);
	SetDisplayVertices(_displayVertices);
}

//void ViewerHoopsRenderer::OpenNode(int nodeId)
//{
//	int anyOpen = HC_Show_Existence("self");
//	if (anyOpen > 0)
//	{
//		int count = HC_Show_Existence_By_Key(nodeId, "subsegment");
//		if (count > 0)
//		{
//			HC_Open_Segment_By_Key((long)nodeId);
//			return;
//		}
//	}
//	long key = HC_Open_Segment("");
//	HC_Renumber_Key(key, nodeId, ".");
//}
//void ViewerHoopsRenderer::CloseNode()
//{
//	HC_Close_Segment();
//}
//void ViewerHoopsRenderer::DeleteNode(int nodeId)
//{
//	HC_Delete_By_Key(nodeId);
//}

void ViewerHoopsRenderer::AddPolyline( std::vector<float> const& aEdgePositions, const RGBAColor &iColor )
{

	int floatCount = aEdgePositions.size();
	float* ptList = new float[floatCount];

	for (int i = 0;i<floatCount;i++)
		ptList[i]=aEdgePositions[i];
	HC_Insert_Polyline(floatCount/3,ptList);

	delete ptList;

}

void ViewerHoopsRenderer::AddTriangleSet(
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

	HC_KEY newShellKey = HC_Insert_Shell(floatCount/3,ptList,newFaceSize,faceList);

	HC_Open_Geometry(newShellKey);
	{
		for (int i = 0; i < (int)(vertexNormalCoords.size() / 3); i++)
		{
			HC_Open_Vertex(i);
			HC_Set_Normal(vertexNormalCoords[i * 3 + 0],vertexNormalCoords[i * 3 + 1],vertexNormalCoords[i * 3 + 2]);
			HC_Close_Vertex();
		}
	}
	HC_Close_Geometry();
	_totalTriCount += triCount / 3;

	// HoopsHelper.PopulateNormals(normals, faceShellKey);

	//	delete ptList;
	//	delete faceList;

}

//void
//ViewerHoopsRenderer::AddLine(float v0[3], float v1[2])
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

void ViewerHoopsRenderer::AddVertex(float v0[3], const RGBAColor &iColor)
{
	HC_Insert_Marker(v0[0],v0[1],v0[2]);
}

void ViewerHoopsRenderer::SetWindowExtents(int width, int height)
{
	ViewerRenderer::SetWindowExtents(width, height);
}

void ViewerHoopsRenderer::OpenNode(int nodeId)
{
	HC_KEY idAsKey= (HC_KEY)fabs((float)nodeId);
	char idstr[32];
	itoa(idAsKey,idstr, 10);
	HC_KEY key = HC_KOpen_Segment(idstr);
	if(key<0)
	{
		// new segment
		HC_Renumber_Key(key, idAsKey, "local");
	}
	segmentStack.push(idAsKey);
}
void ViewerHoopsRenderer::CloseNode()
{
	HC_Close_Segment();
	segmentStack.pop();
}
void ViewerHoopsRenderer::SetColor(const RGBAColor &iColor)
{
	double r = (double)iColor.red/255;
	double g = (double)iColor.green/255;
	double b = (double)iColor.blue/255;
	HC_Set_Color_By_Value("faces","RGB",r,g,b);
	HC_Set_Color_By_Value("lines","RGB",r,g,b);
	HC_Set_Color_By_Value("markers","RGB",r,g,b);
}
void ViewerHoopsRenderer::SetDisplayFaces(bool displayFaces)
{
	_displayFaces = displayFaces;
	HC_Open_Segment_By_Key(mPictureKey);
	{
		if (displayFaces)
			HC_Set_Visibility("faces = on");
		else
			HC_Set_Visibility("faces = off");
	}
	HC_Close_Segment();
}

void ViewerHoopsRenderer::SetDisplayEdges(bool displayEdges)
{
	_displayEdges = displayEdges;
	HC_Open_Segment_By_Key(mPictureKey);
	{
		if (displayEdges)
			HC_Set_Visibility("lines = on");
		else
			HC_Set_Visibility("lines = off");
	}
	HC_Close_Segment();
}

void ViewerHoopsRenderer::SetDisplayVertices(bool displayVertices)
{
	_displayVertices = displayVertices;
	HC_Open_Segment_By_Key(mPictureKey);
	{
		if (displayVertices)
			HC_Set_Visibility("markers = on");
		else
			HC_Set_Visibility("markers = off");
	}
	HC_Close_Segment();
}


void ViewerHoopsRenderer::SaveToFile()
{
    char pictName[1000];
    HC_Show_Segment(mPictureKey, pictName);
	HC_Write_Metafile(pictName, "c:/temp/scene.hmf", "follow cross-references, use color names, save state");
}
