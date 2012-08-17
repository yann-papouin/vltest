#include "stdafx.h"
#include "ViewerItemPolyline.h"
#include "../../math/GeometryMath.h"

ViewerItemPolyline::ViewerItemPolyline( std::vector<float> const& points): _points(points)
{
}
void DrawPolylineOpenGL( int nbCoords, float const* iaCoords )
{
	glEnableClientState( GL_VERTEX_ARRAY);
	glVertexPointer( 3, GL_FLOAT, 0, iaCoords );
	
	int nbVertices = nbCoords/3;
	glDrawArrays( GL_LINE_STRIP,0,nbVertices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void ViewerItemPolyline::Play(ViewerOpenGLRenderer* pList) const
{
	if( !pList || !pList->GetDisplayEdges() )
	{
		return;
	}

	glColor4f( _color[0], _color[1], _color[2], _color[3] );
	glDisable(GL_LIGHTING);
	DrawPolylineOpenGL( int(_points.size()), &(_points[0]) );
	glEnable(GL_LIGHTING);
}

void ViewerItemPolyline::GetCenter( float * pos ) const
{
	::GetCenter( pos, _points );
}

bool ViewerItemPolyline::RayHit( const float * rayOrigin, const float * rayVector, float rayRadius, float * hit, float & param ) const
{
	const int nbVertices = int(_points.size())/3;
	const int loopEnd = nbVertices - 1;

	bool result = false;
	for( int ii=0; ii<loopEnd; ++ii )
	{
		float* v0 = (float*)&(_points[ 3*ii ]);
		float* v1 = (float*)&(_points[ 3*(ii + 1) ]);
		result = RayHitsLineSegment( v0, v1, rayRadius, rayOrigin, rayVector, hit, param );
		if( result )
		{
			break;
		}
	}
	return result;
}
