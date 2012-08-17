#pragma once
#include "ViewerDLItem.h"

class ViewerItemPolyline : public ViewerDLItem
{
	std::vector<float> _points;
public:
	ViewerItemPolyline( std::vector<float> const& points);

    virtual void Play(ViewerOpenGLRenderer* pList) const;
	virtual void GetCenter( float * pos ) const;
	virtual bool RayHit( const float * rayOrigin, const float * rayVector, float rayRadius, float * hit, float & param ) const;
};