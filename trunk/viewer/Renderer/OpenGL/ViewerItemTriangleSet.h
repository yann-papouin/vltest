#pragma once
#include "ViewerDLItem.h"
class ViewerItemTriangleSet: public ViewerDLItem
{
	std::vector<float> _vertices;
	std::vector<float> _normals;
	std::vector<int> _triangles;
public:
	ViewerItemTriangleSet( 
					std::vector<float> const& iVertexPositionCoords,
					std::vector<float> const& iVertexNormalCoords,
					std::vector<int> const& iTriangles
					);
	~ViewerItemTriangleSet( );

    virtual void Play(ViewerOpenGLRenderer* pList) const ;
	virtual void GetCenter( float * pos ) const;
	virtual bool RayHit( const float * rayOrigin, const float * rayVector, float rayRadius, float * hit, float & param ) const;
};
