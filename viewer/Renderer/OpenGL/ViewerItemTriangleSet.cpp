#include "stdafx.h"
#include "ViewerItemTriangleSet.h"
#include "../../math/GeometryMath.h"


ViewerItemTriangleSet::ViewerItemTriangleSet( 
					std::vector<float> const& iVertexPositionCoords,
					std::vector<float> const& iVertexNormalCoords,
					std::vector<int> const& iTriangles
				)
				: _vertices( iVertexPositionCoords), _normals(iVertexNormalCoords), _triangles(iTriangles)
{
}

ViewerItemTriangleSet::~ViewerItemTriangleSet( )
{
}

void DrawTrianglesOpenGL
		(
			int iNbVertices,
			float const* iaVertexPositionCoords,
			float const* iaVertexNormalCoords,
			int iNbTriangles,
			int const* iaTriangles
		)
{
		glEnableClientState( GL_VERTEX_ARRAY);
		glVertexPointer( 3, GL_FLOAT, 0, iaVertexPositionCoords );
 
		if( iaVertexNormalCoords != NULL )
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer( GL_FLOAT, 0, iaVertexNormalCoords );
		}
 
		glDrawElements( GL_TRIANGLES, 3*iNbTriangles, GL_UNSIGNED_INT, iaTriangles);
 
		if( iaVertexNormalCoords != NULL )
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
}

void ViewerItemTriangleSet::Play(ViewerOpenGLRenderer* pList) const 
{
	if( !pList || !pList->GetDisplayFaces() )
	{
		return;
	}

	glColor4f( _color[0], _color[1], _color[2], _color[3] );
	const int nbVertices = int(_vertices.size())/3;
	const int nbTriangles = int(_triangles.size())/3;
	float const* normalsPtr = &(_normals[0]);
	if( _normals.size() == 0 )
	{
		normalsPtr = NULL;
	}
	DrawTrianglesOpenGL( nbVertices, &(_vertices[0]), normalsPtr, nbTriangles, (&_triangles[0]) );
}



void ViewerItemTriangleSet::GetCenter( float * pos ) const
{
	::GetCenter( pos, _vertices );
}

bool ViewerItemTriangleSet::RayHit( const float * rayOrigin, const float * rayVector, float rayRadius, float * hit, float & param ) const
{
	bool retval = false;
	const int nbTriangles = int(_triangles.size())/3;
	for( int ii=0; ii<nbTriangles; ++ii )
	{
		// The following code initializes points[0],points[1],points[2]
		// so that they point to the first coordinate of the 0th,1st, 2nd
		// vertex in the triangle (respectively).
		float* points[3];
		for(int pointIdx=0; pointIdx<3; ++pointIdx)
		{
			const int thisPointIdx = _triangles[ 3*ii + pointIdx];
			points[pointIdx] = (float*)&(_vertices[ thisPointIdx]);
		}

		bool coincident;
		bool thisTriangleHits = RayHitsTriangle( 
									points[0],points[1],points[2],
									rayOrigin,
									rayVector,
									hit, 
									param,
									coincident );
		if( thisTriangleHits )
		{
			retval = true;
			break;
		}
	}
	return retval;
}
