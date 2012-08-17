
#ifndef VIEWERTESSELLATE_H
#define VIEWERTESSELLATE_H

#include "CATDataType.h"
#include "Hpt.h"
#include "PerformanceMetrics.h"

class CATFace;
class CATEdge;
class CATVertex;
class CATCGMTessBody;
class ViewerRenderer;

class ViewerTessellate
{
public:
	virtual HRESULT RenderFace(CATFace *iFace, CATCGMTessBody *iTessBody, const RGBAColor &iColor);
	virtual HRESULT RenderEdge(CATEdge *iEdge, CATCGMTessBody *iTessBody, const RGBAColor &iColor);
	virtual HRESULT RenderVertex(CATVertex *iVertex, const RGBAColor &iColor);  

	ViewerTessellate(ViewerRenderer* ipRenderer);
	~ViewerTessellate();

	ViewerRenderer* _renderer;
	bool _useArrays;
};

#endif
