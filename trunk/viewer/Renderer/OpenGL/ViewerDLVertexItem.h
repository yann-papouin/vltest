#pragma once
#include "ViewerDLItem.h"
class ViewerDLVertexItem : public ViewerDLItem
{
public:
    ViewerDLVertexItem(float v0[3]);

    float _v0[3];

    virtual void Play(ViewerOpenGLRenderer* pList) const;
	virtual void GetCenter( float * pos ) const;
	virtual bool RayHit( const float * rayOrigin, const float * rayVector, float rayRadius, float * hit, float & param ) const;
};