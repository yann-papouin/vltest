#pragma once
#include "ViewerOpenGLRenderer.h"
#include "gl/gl.h"
#include "gl/glu.h"
class ViewerDLItem
{
protected:
	GLfloat _color[4];
public:
	ViewerDLItem(){_color[0] = 0;_color[1]=0;_color[2]=0;}
	virtual ~ViewerDLItem(){}
    virtual void Play(ViewerOpenGLRenderer* pList) const = 0;
	virtual void GetCenter( float * pos ) const = 0;
	virtual bool RayHit( const float * rayOrigin, const float * rayVector, float rayRadius, float * hit, float & param ) const = 0;
	virtual void SetColor(const RGBAColor &iColor);
};
void GetCenter( float* pos, std::vector<float> const& points );



