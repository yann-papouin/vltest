#include "stdafx.h"
#include "ViewerDLItem.h"
void ViewerDLItem::SetColor(const RGBAColor &iColor)
{
	_color[0] = (float)iColor.red / 255.0f;
	_color[1] = (float)iColor.green / 255.0f;
	_color[2] = (float)iColor.blue / 255.0f;
	_color[3] = 1;//(float)iColor.alpha / 255.0f;
}

void GetCenter( float* pos, std::vector<float> const& points )
{
	pos[0] = pos[1] = pos[2] = 0.0;

	const int nbVertices = int(points.size())/3;
	for( int ii=0; ii<nbVertices; ++ii )
	{
		int x_index = 3*ii;
		int y_index = 3*ii + 1;
		int z_index = 3*ii + 2;

		pos[0] += points[x_index];
		pos[1] += points[y_index];
		pos[2] += points[z_index];
	}

	pos[0] /= nbVertices;
	pos[1] /= nbVertices;
	pos[2] /= nbVertices;
}
