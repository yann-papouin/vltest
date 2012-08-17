#include "stdafx.h"
#include "ViewerDLVertexItem.h"
#include "../../math/GeometryMath.h"
ViewerDLVertexItem::ViewerDLVertexItem(float v0[3])
{
    for (int i = 0; i < 3; i++)
    {
       _v0[i] = v0[i];
    }
}

void
ViewerDLVertexItem::Play(ViewerOpenGLRenderer* pList) const
{
    if (pList && pList->GetDisplayVertices())
    {
		glColor4f( _color[0], _color[1], _color[2], _color[3] );
		glPointSize(6.0);
        glBegin(GL_POINTS);
        glVertex3f(_v0[0], _v0[1], _v0[2]);
        glEnd();
    }
}

void ViewerDLVertexItem::GetCenter( float * pos ) const
{
	for ( int i = 0; i < 3; i++ )
		pos[i] = _v0[i];
}

static float nearZero = 1e-9f;

bool ViewerDLVertexItem::RayHit( const float * rayOrigin, const float * rayVector, float rayRadius, float * hit, float & param ) const
{
	float distance = Length( rayVector );
	if ( distance > nearZero )
	{
		float ray[ 3 ];
		for ( int i = 0; i < 3; i++ )
			ray[ i ] = rayVector[ i ];
		Normalize( ray );
		float temp[ 3 ];
		float dot = DotProduct( Subtract( _v0, rayOrigin, temp ), ray );
		float pointOnRay[ 3 ];
		for ( int i = 0; i < 3; i++ )
			pointOnRay[ i ] = rayOrigin[ i ] + dot * ray[ i ];
		distance = Length( Subtract( _v0, pointOnRay, temp ) );
	}
	if ( distance < rayRadius )
	{
		param = 0;
		for ( int i = 0; i < 3; i++ )
			hit[ i ] = _v0[ i ];
		return true;
	}
	return false;
}