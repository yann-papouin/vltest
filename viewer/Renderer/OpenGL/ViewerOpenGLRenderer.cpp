#include "stdafx.h"
#include "math.h"
#include "float.h"
#include "mainfrm.h"
#include "ViewerOpenGLRenderer.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "../../math/GeometryMath.h"
#include "ViewerItemTriangleSet.h"
#include "ViewerItemPolyline.h"
#include "ViewerDLVertexItem.h"
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

ViewerOpenGLRenderer::ViewerOpenGLRenderer(): ViewerRenderer()
{
	m_FaceListId = 0;
	m_EdgeListId = 0;
	m_VertexListId = 0;
	m_RetainMode = false;
}

ViewerOpenGLRenderer::~ViewerOpenGLRenderer()
{
	ClearLists();
    ClearEntries();
}

void ViewerOpenGLRenderer::Initialize(HWND wnd, CClientDC* pDC, int width, int height)
{
	if (pDC)
	{
		ViewerRenderer::Initialize(wnd, pDC, width, height);

		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
			1,                              // version number
			PFD_DRAW_TO_WINDOW |            // draw to window rather than bitmap
			  PFD_SUPPORT_OPENGL |          // support OpenGL
			  PFD_DOUBLEBUFFER,             // use double buffering
			PFD_TYPE_RGBA,                  // RGBA type
			24,                             // 24-bit color depth
			0, 0, 0, 0, 0, 0,               // color bits ignored
			0,                              // no alpha buffer
			0,                              // shift bit ignored
			0,                              // no accumulation buffer
			0, 0, 0, 0,                     // accum bits ignored
			32,                             // 32-bit z-buffer
			0,                              // no stencil buffer
			0,                              // no auxiliary buffer
			PFD_MAIN_PLANE,                 // main layer
			0,                              // reserved
			0, 0, 0                         // layer masks ignored
		};

		// Request the above pixel format
		int pixelFormat = ChoosePixelFormat(pDC->GetSafeHdc(), &pfd);
		ASSERT(pixelFormat != 0);
		// If everything is OK, select that pixel format
		BOOL okay = SetPixelFormat(pDC->GetSafeHdc(), pixelFormat, &pfd);
		ASSERT(okay);

		// Create a new OpenGL rendering context and make it current
		HGLRC hrc = wglCreateContext(pDC->GetSafeHdc());
		wglMakeCurrent(pDC->GetSafeHdc(), hrc);

		// Accept the default depth-buffer clearing value
		glClearDepth(1.0f);

		// Enable the depth buffer
		glEnable(GL_DEPTH_TEST);

		// Setup the camera light
		glEnable(GL_LIGHTING); 
		GLfloat cameraSpecularColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };
		GLfloat cameraDiffuseColor[] = { 0.6f, 0.6f, 0.6f, 1.0 };
		glLightfv( GL_LIGHT0, GL_SPECULAR, cameraSpecularColor );
		glLightfv( GL_LIGHT0, GL_DIFFUSE,  cameraDiffuseColor );
		glEnable( GL_LIGHT0 );
		glLightModelf( GL_LIGHT_MODEL_TWO_SIDE, 1.0f );

		// Define the model shininess and the reflected color
		GLfloat matAmbientColor[] = { 0.234375f, 0.234375f, 0.234375f, 1.0};
		GLfloat matSpecularColor[] = { 0.566681f, 0.580872f, 0.580874f, 1.0};
		GLfloat matDiffuseColor[] = { 0.673937f, 0.457340f, 0.436528f, 1.0 };
		GLfloat matShininess[] = { 0.168182f * 128 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientColor);
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularColor);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseColor);
		glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

		// Enable color tracking
		glEnable(GL_COLOR_MATERIAL);

		// Enable normal normalization
		glEnable(GL_NORMALIZE);

		// Specify a smooth shading model
		glShadeModel(GL_SMOOTH);

		// Specify the background color
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		// setup polygon offset so that edges show clearly.
		glEnable(GL_POLYGON_OFFSET_FILL );
		glPolygonOffset(1.0,1.0);
	}
}

void ViewerOpenGLRenderer::Terminate()
{
	// Mark the rendering context as no longer current, then delete it
    HGLRC hrc = ::wglGetCurrentContext();
    ::wglMakeCurrent(NULL, NULL);
    if (hrc)
        ::wglDeleteContext(hrc);
}

void ViewerOpenGLRenderer::SetProjection()
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
    GLfloat nearPlane = Length(viewVector) - viewRadius;
    GLfloat farPlane = Length(viewVector) + viewRadius;
    float aspect = 1.0f;
    if (_windowHeight > 0)
    {
        aspect = _windowWidth / _windowHeight;
    }

	// Implement the requested projection
	if ( GetDisplayOrtho() )
	{
		GLfloat radius = Length(viewVector) * tan(GetFieldOfViewRadians() / 2.0f);
		GLfloat fleft   = -radius * aspect;
		GLfloat fright  =  radius * aspect;
		GLfloat fbottom = -radius;
		GLfloat ftop    =  radius;
		glOrtho(fleft, fright, fbottom, ftop, nearPlane, farPlane);
	}
	else
	{
		float limit = _geometryRadius / 500.0f;
		if (nearPlane < limit)
			nearPlane = limit;
		gluPerspective( GetFieldOfView(), aspect, nearPlane, farPlane );
	}
}

void ViewerOpenGLRenderer::Display()
{
	// Define the viewport size
	glViewport(0, 0, (GLsizei)_windowWidth, (GLsizei)_windowHeight);

	// Implement the requested projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	SetProjection(); // Call glOrtho or glPerspetive

	// View the model
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//For trasparent color alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
	glLoadIdentity();

	// Direct the view
	gluLookAt(_cameraPosition[0], _cameraPosition[1], _cameraPosition[2],
		_cameraTarget[0], _cameraTarget[1], _cameraTarget[2],
		_cameraUp[0], _cameraUp[1], _cameraUp[2]);

	// Update the camera light's position
	GLfloat lightPosition[4];
	for ( int i = 0; i < 3; i++ )
		lightPosition[i] = ( _cameraPosition[i] - _cameraTarget[i] ) * 1000 + _cameraPosition[i];
	lightPosition[3] = 0;
	glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );

	// Display all items in the display list
	if (_displayFaces)
	{
		if (m_RetainMode)
		{
			glCallList(m_FaceListId);
		}
		else
		{
			size_t size = _face_entries.size();
			for (size_t i = 0; i < size; ++i)
			{
				_face_entries[i]->Play(this);
			}
		}
	}
	if (_displayEdges)
	{
		if (m_RetainMode)
		{
			glCallList(m_EdgeListId);
		}
		else
		{
			size_t size = _edge_entries.size();
			for (size_t i = 0; i < size; ++i)
			{
				_edge_entries[i]->Play(this);
			}
		}
	}

	if (_displayVertices)
	{
		if (m_RetainMode)
		{
			glCallList(m_VertexListId);
		}
		else
		{
			size_t size = _vertex_entries.size();
			for (size_t i = 0; i < size; ++i)
			{
				_vertex_entries[i]->Play(this);
			}
		}
	}


	// Finish up
    glPopMatrix();
	glFlush();
    glFinish();
    SwapBuffers(wglGetCurrentDC());
}

void ViewerOpenGLRenderer::Make3DRay( int windowX, int windowY, float * rayVector, float * rayOrigin )
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

bool ViewerOpenGLRenderer::PickPointOnModel( int windowX, int windowY, float * modelX, float * modelY, float * modelZ )
{
#define BUFSIZE 1000
	float cameraPosition[ 3 ], cameraTarget[ 3 ], cameraUp[ 3 ];
	GetCamera( cameraPosition, cameraTarget, cameraUp );

	// Convert the camera vector to a pick ray
	float rayVector[ 3 ], rayOrigin[ 3 ];
	Make3DRay( windowX, windowY, rayVector, rayOrigin );

	// Have OpenGL pick an entity...
	GLuint selectBuf[ BUFSIZE ];
	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );
	glSelectBuffer( BUFSIZE, selectBuf );
	glRenderMode(GL_SELECT );
	glInitNames();
	glPushName(0);
	glMatrixMode( GL_PROJECTION );
    glPushMatrix();
	glLoadIdentity();
	GLdouble rayRadius2D = 4.0;
	gluPickMatrix( (GLdouble)( windowX - rayRadius2D / 2 ), (GLdouble)( viewport[3] - windowY - rayRadius2D / 2 ), rayRadius2D, rayRadius2D, viewport );

	SetProjection(); // Calls glOrtho or glPerspective

	// Direct the view...
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		cameraTarget[0], cameraTarget[1], cameraTarget[2],
		cameraUp[0], cameraUp[1], cameraUp[2]);

	// Display all items in the display list (essentially "picks" an entity)
	
	if (m_RetainMode)
	{
		// TODO: retain mode implementation
	}
	else
	{
		size_t face_size = _face_entries.size();
		for (size_t i = 0; i < face_size; ++i)
		{
			glLoadName( (GLuint)i );
			_face_entries[i]->Play(this);
		}
		
		size_t edge_size = _edge_entries.size();
		for (size_t i = 0; i < edge_size; ++i)
		{
			glLoadName( (GLuint)i );
			_edge_entries[i]->Play(this);
		}
		
		size_t vertex_size = _vertex_entries.size();
		for (size_t i = 0; i < vertex_size; ++i)
		{
			glLoadName( (GLuint)i );
			_vertex_entries[i]->Play(this);
		}
	}
	

	// Terminate pick mode...
	glPopMatrix();
	glFlush();
	GLint hitCount = glRenderMode( GL_RENDER );

	float rayRadius3D;
	{	// Calculate reyRadius3D...
		float viewVector[3];
		for (int jj = 0; jj < 3; jj++)
			viewVector[ jj ] = cameraPosition[ jj ] - cameraTarget[ jj ];
		float aspect = 1.0f;
		if ( _windowHeight > 0 )
			aspect = _windowWidth / _windowHeight;
		float cameraHeight = 2 * Length( viewVector ) * tan( GetFieldOfViewRadians() / 2 );
		float cameraWidth = cameraHeight * aspect;
		rayRadius3D = (float)( rayRadius2D * cameraHeight / _windowHeight ); // Assumes height and width are nearly equal
	}

	// Process hit list...
	GLuint names, *ptr;
	ptr = selectBuf;
	float depth = FLT_MAX;
	for ( GLint i = 0; i < hitCount; i++ )
	{
		names = *ptr; // list of all the OpenGL names picked.
		ptr++;
		float z1 = (float)*ptr/0x7fffffff; // closest pixel of this OpenGL entity picked.
		ptr++;
		float z2 = (float)*ptr/0x7fffffff; // farthest pixel of this OpenGL entity picked.
		ptr++;
		int openGLName; // Get the deepest name (the name put on the stack last) ...
		for ( GLuint j = 0; j < names; j++ )
		{
			openGLName = *ptr;
			ptr++;
		}
		float hit[ 3 ], param;
		// We have a whole GL primitive. Could be big. Refine the hit to a specific point on that primitive...
		bool result = _face_entries[ openGLName ]->RayHit( rayOrigin, rayVector, rayRadius3D, hit, param );
		if (!result)
			result = _edge_entries[ openGLName ]->RayHit( rayOrigin, rayVector, rayRadius3D, hit, param );
		if (!result)
			result = _vertex_entries[ openGLName ]->RayHit( rayOrigin, rayVector, rayRadius3D, hit, param );
		if ( result )
		{
			float temp[ 3 ];
			float dist = Length( Subtract( hit, cameraPosition, temp ) );
			if ( dist < depth )
			{	// Closest entry so far. Set return values...
				depth = dist;
				*modelX = hit[ 0 ];
				*modelY = hit[ 1 ];
				*modelZ = hit[ 2 ];
			}
		}
	}
	if ( depth < FLT_MAX )
		return true;
	return false;
}
void ViewerOpenGLRenderer::ClearLists()
{
	if (m_FaceListId>0)
	{
		glDeleteLists(m_FaceListId, 1);
	}
	m_FaceListId = 0;
	if (m_EdgeListId>0)
	{
		glDeleteLists(m_EdgeListId, 1);
	}
	m_EdgeListId = 0;
	if (m_VertexListId>0)
	{
		glDeleteLists(m_VertexListId, 1);
	}
	m_VertexListId=0;
}
void ViewerOpenGLRenderer::ClearEntries()
{
	// Clear all previous display items
    size_t face_size = _face_entries.size();
    for (size_t i = 0; i < face_size; ++i)
    {
        ViewerDLItem* pItem = _face_entries[i];
        delete pItem;
    }
    _face_entries.clear();
	  
	size_t edge_size = _edge_entries.size();
    for (size_t i = 0; i < edge_size; ++i)
    {
        ViewerDLItem* pItem = _edge_entries[i];
        delete pItem;
    }
    _edge_entries.clear();
	  
	size_t vertex_size = _vertex_entries.size();
    for (size_t i = 0; i < vertex_size; ++i)
    {
        ViewerDLItem* pItem = _vertex_entries[i];
        delete pItem;
    }
    _vertex_entries.clear();
}
void ViewerOpenGLRenderer::StartRender()
{
	ClearLists();
	ClearEntries();
}

void ViewerOpenGLRenderer::EndRender()
{
	// convert items into displaylists
	if (m_RetainMode)
	{
		m_FaceListId = glGenLists(1);
		glNewList(m_FaceListId, GL_COMPILE);
		{
			bool prevval = _displayFaces;
			_displayFaces = true;
			size_t face_size = _face_entries.size();
			for (size_t i = 0; i < face_size; ++i)
			{
				_face_entries[i]->Play(this);
			}
			_displayFaces = prevval;
		}
		glEndList();
		m_EdgeListId = glGenLists(1);
		glNewList(m_EdgeListId, GL_COMPILE);
		{
			bool prevval = _displayEdges;
			_displayEdges = true;
			size_t edge_size = _edge_entries.size();
			for (size_t i = 0; i < edge_size; ++i)
			{
				_edge_entries[i]->Play(this);
			}
			_displayEdges = prevval;
		}
		glEndList();
		
		m_VertexListId = glGenLists(1);
		glNewList(m_VertexListId, GL_COMPILE);
		{
			bool prevval = _displayVertices;
			_displayVertices = true;
			size_t vertex_size = _vertex_entries.size();
			for (size_t i = 0; i < vertex_size; ++i)
			{
				_vertex_entries[i]->Play(this);
			}
			_displayVertices = prevval;

		}
		glEndList();
		ClearEntries();
	}

	SetDisplayEdges(_displayEdges);
	SetDisplayFaces(_displayFaces);
	SetDisplayVertices(_displayVertices);
}

void ViewerOpenGLRenderer::AddPolyline( std::vector<float> const& aEdgePositions, const RGBAColor &iColor )
{
	ViewerDLItem* pItem = new ViewerItemPolyline( aEdgePositions );
	pItem->SetColor(iColor);
	_edge_entries.push_back( pItem );
}

void ViewerOpenGLRenderer::AddTriangleSet(
				std::vector<float> const& vertexPositionCoords,
				std::vector<float> const& vertexNormalCoords,
				std::vector<int> const& triangles,
				const RGBAColor &iColor)
{
	ViewerDLItem* pItem = new ViewerItemTriangleSet(
								vertexPositionCoords,
								vertexNormalCoords,
								triangles
								);
	pItem->SetColor(iColor);
	_face_entries.push_back( pItem );
}


void ViewerOpenGLRenderer::AddVertex(float v0[3], const RGBAColor &iColor)
{
    float lv0[3];
    for (int i = 0; i < 3; i++)
    {
       lv0[i] = v0[i];
    }
    ViewerDLVertexItem* pItem = new ViewerDLVertexItem(lv0);
	pItem->SetColor(iColor);
    _vertex_entries.push_back(pItem);
}

void ViewerOpenGLRenderer::SetWindowExtents(int width, int height)
{
    ViewerRenderer::SetWindowExtents(width, height);

    glViewport(0, 0, width, height);
}






