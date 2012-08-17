#include "stdafx.h"
#include "math.h"
#include "float.h"
#include "math/GeometryMath.h"
#include "ViewerRenderer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

ViewerRenderer::ViewerRenderer() :
	_displayFaces(true),
	_displayEdges(false),
	_displayVertices(false),
	_displayOrtho(true)
{
	_cameraPosition[0] = 0;
	_cameraPosition[1] = 0;
	_cameraPosition[2] = 50;
	_cameraTarget[0] = 0;
	_cameraTarget[1] = 0;
	_cameraTarget[2] = 0;
	_cameraUp[0] = 0;
	_cameraUp[1] = 1;
	_cameraUp[2] = 0;
	SetFieldOfView(45);
	_totalTriCount = 0;
}

ViewerRenderer::~ViewerRenderer()
{
}

void
ViewerRenderer::Initialize(HWND wnd, CClientDC* pDC, int width, int height)
{
    SetWindowExtents(width, height);
}

void
ViewerRenderer::Terminate()
{
}

void
ViewerRenderer::SetDisplayFaces(bool displayFaces)
{
    _displayFaces = displayFaces;
}

void
ViewerRenderer::SetDisplayEdges(bool displayEdges)
{
    _displayEdges = displayEdges;
}

void
ViewerRenderer::SetDisplayVertices(bool displayVertices)
{
    _displayVertices = displayVertices;
}

bool
ViewerRenderer::GetDisplayFaces() const
{
    return _displayFaces;
}

bool
ViewerRenderer::GetDisplayEdges() const
{
    return _displayEdges;
}

bool
ViewerRenderer::GetDisplayVertices() const
{
    return _displayVertices;
}

void
ViewerRenderer::SetGeometryExtents(float center[3], float radius)
{
    for (int i = 0; i < 3; i++)
    {
       _geometryCenter[i] = center[i];
    }
    _geometryRadius = radius;
}

void
ViewerRenderer::SetWindowExtents(int width, int height)
{
    _windowWidth = (float)width;
    _windowHeight = (float)height;
}

const float*
ViewerRenderer::GetGeometryCenter() const
{
    return &_geometryCenter[0];
}

const float
ViewerRenderer::GetGeometryRadius() const
{
    return _geometryRadius;
}

void
ViewerRenderer::SetDisplayOrtho(bool displayOrtho)
{
	_displayOrtho = displayOrtho;
}

bool
ViewerRenderer::GetDisplayOrtho() const
{
	return _displayOrtho;
}

bool IsANumber( float x )
{
	return ( x == x );
}

bool IsFinite( float x ) 
{
	return ( x <= FLT_MAX && x >= -FLT_MAX );
}  

void
ViewerRenderer::SetCamera(float* cameraPosition, float* cameraTarget, float* cameraUp)
{
	Normalize( cameraUp );
	bool good_numbers = true;
	for ( int i = 0; i < 3 && good_numbers; i++ )
	{
		if ( ! IsANumber( cameraPosition[ i ] ) || ! IsFinite( cameraPosition[ i ] ) )
			good_numbers = false;
		if ( ! IsANumber( cameraTarget[ i ] ) || ! IsFinite( cameraTarget[ i ] ) )
			good_numbers = false;
		if ( ! IsANumber( cameraUp[ i ] ) || ! IsFinite( cameraUp[ i ] ) )
			good_numbers = false;
	}
	if ( ! good_numbers )
		return;
	float viewVector[ 3 ];
	Normalize( Subtract( cameraTarget, cameraPosition, viewVector ) );
	if ( Length( viewVector ) < 0.0001 )
		return; // Can't have cameraPosition == cameraTarget
	float dot = DotProduct( viewVector, cameraUp );
	if ( dot < -0.999f || 0.999f < dot )
		return; // Can't have view vector parallel to up vector
	if ( dot < -.1 || 1 < dot )
	{	// Change the up vector to form a 90 degree angle with the view vector...
		float cameraRight[ 3 ];
		CrossProduct( viewVector, cameraUp, cameraRight );
		CrossProduct( cameraRight, viewVector, cameraUp );
		dot = DotProduct( viewVector, cameraUp );
		if ( dot < -0.999f || 0.999f < dot )
			return; // Can't have view vector parallel to up vector
	}
	for (int i = 0; i < 3; i++)
	{
		_cameraPosition[i] = cameraPosition[i];
		_cameraTarget[i]   = cameraTarget[i];
		_cameraUp[i]       = cameraUp[i];
	}
}

void 
ViewerRenderer::Zoom(float delta)
{
	float cameraPosition[3], cameraTarget[3], cameraUp[3];
	GetCamera(cameraPosition, cameraTarget, cameraUp);

	for (int i = 0; i < 3; i++)
	{
		cameraPosition[i] = cameraTarget[i] +
			(delta + 1.f) * (cameraPosition[i] - cameraTarget[i]);
	}
	
	SetCamera(cameraPosition, cameraTarget, cameraUp);
}

void
ViewerRenderer::GetCamera(float* cameraPosition, float* cameraTarget, float* cameraUp)
{
	for (int i = 0; i < 3; i++)
	{
		cameraPosition[i] = _cameraPosition[i];
		cameraTarget[i]   = _cameraTarget[i];
		cameraUp[i]       = _cameraUp[i];
	}
}

void
ViewerRenderer::SetFieldOfView(float degrees)
{
	_FOV = degrees * 3.1415926f / 180.0f;
}

float
ViewerRenderer::GetFieldOfView()
{
	return _FOV * 180.0f / 3.1415926f;
}

float
ViewerRenderer::GetFieldOfViewRadians()
{
	return _FOV;
}

void
ViewerRenderer::ZoomAll()
{
	float geomRadius = GetGeometryRadius();
	float eye[3], target[3], up[3];
	GetCamera(eye, target, up);

	float oldView[3];
	for (int i = 0; i < 3; i++)
		oldView[i] = eye[i] - target[i];

	Normalize(oldView);

	const float* geomCenter = GetGeometryCenter();
	target[0] = geomCenter[0];
	target[1] = geomCenter[1];
	target[2] = geomCenter[2];

	float distEyeToTarget = geomRadius / (tan(GetFieldOfViewRadians() / 2.0f));
	for (int i = 0; i < 3; i++)
		oldView[i] *= distEyeToTarget;

	for (int i = 0; i < 3; i++)
		eye[i] = oldView[i] + target[i];

	SetCamera(eye, target, up);
}

void
ViewerRenderer::SetStandardCameraPositionAndUp(float* cameraPosition, float* cameraTarget, float* cameraUp)
{
	GetCamera( cameraPosition, cameraTarget, cameraUp );
	float dist[3];
	float distance = 0;

	for (int i = 0; i < 3; i++)
	{
		dist[i] = cameraTarget[i] - cameraPosition[i];
		dist[i] *= dist[i];
		distance += dist[i];
		cameraPosition[i] = cameraTarget[i];
	}
	distance = sqrt( distance );
	cameraPosition[2] += distance;
	
	cameraUp[0] = 0;
	cameraUp[1] = 1;
	cameraUp[2] = 0;
}

void
ViewerRenderer::Top()
{
	float cameraPosition[3], cameraTarget[3], cameraUp[3], axis[3];
	SetStandardCameraPositionAndUp(cameraPosition, cameraTarget, cameraUp);

	axis[0] = 0;
	axis[1] = 0;
	axis[2] = -1;
	RotatePoint(cameraPosition, 90, cameraTarget, axis, cameraPosition);
	cameraUp[0] = -1;
	cameraUp[1] = 0;
	cameraUp[2] = 0;
	
	SetCamera(cameraPosition, cameraTarget, cameraUp);
}

void
ViewerRenderer::Front()
{
	float cameraPosition[3], cameraTarget[3], cameraUp[3], axis[3];
	SetStandardCameraPositionAndUp(cameraPosition, cameraTarget, cameraUp);

	axis[0] = -1;
	axis[1] = 0;
	axis[2] = 0;
	RotatePoint(cameraPosition, 90, cameraTarget, axis, cameraPosition);
	axis[0] = 0;
	axis[1] = 0;
	axis[2] = -1;
	RotatePoint(cameraPosition, 90, cameraTarget, axis, cameraPosition);
	cameraUp[0] = 0;
	cameraUp[1] = 0;
	cameraUp[2] = 1;

	SetCamera(cameraPosition, cameraTarget, cameraUp);
}

void
ViewerRenderer::Right()
{
	float cameraPosition[3], cameraTarget[3], cameraUp[3], axis[3];
	SetStandardCameraPositionAndUp(cameraPosition, cameraTarget, cameraUp);

	axis[0] = 1;
	axis[1] = 0;
	axis[2] = 0;
	RotatePoint(cameraPosition, 90, cameraTarget, axis, cameraPosition);
	axis[0] = 0;
	axis[1] = 0;
	axis[2] = 1;
	RotatePoint(cameraPosition, 180, cameraTarget, axis, cameraPosition);
	cameraUp[0] = 0;
	cameraUp[1] = 0;
	cameraUp[2] = 1;

	SetCamera(cameraPosition, cameraTarget, cameraUp);
}

void
ViewerRenderer::Isometric()
{
	Top();
	float cameraPosition[3], cameraTarget[3], cameraUp[3], axis[3], origin[3];
	GetCamera(cameraPosition, cameraTarget, cameraUp);

	axis[0] = -1.0f / (float)sqrt(2.0);
	axis[1] = 1.0f / (float)sqrt(2.0);
	axis[2] = 0;
	RotatePoint(cameraPosition, 45, cameraTarget, axis, cameraPosition);
	cameraUp[0] = -1;
	cameraUp[1] = -1;
	cameraUp[2] = 0;
	origin[0] = 0;
	origin[1] = 0;
	origin[2] = 0;
	RotatePoint(cameraUp, 45, origin, axis, cameraUp);
	
	SetCamera(cameraPosition, cameraTarget, cameraUp);
}
