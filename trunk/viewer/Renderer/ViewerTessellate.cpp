#include "stdafx.h"

#include "CGMContainer.h"
#include "ViewerRenderer.h"
#ifdef HOOPSRENDERER
#include "ViewerHoopsRenderer.h"
#elif _USE_SCENIX_RENDERER
#include "ViewerScenixRenderer.h"
#elif _USE_RED_RENDERER
#include "ViewerRedRenderer.h"
#else
#include "ViewerOpenGLRenderer.h"
#endif
#include "CGMViewerdoc.h"
#include "ViewerTessellate.h"

#include "CATUnicodeString.h"
#include "CATErrorMacros.h"
#include "CATGeoFactory.h"
#include "CATGeometry.h"
#include "CATBody.h"
#include "CATFace.h"
#include "CATEdge.h"
#include "CATVertex.h"
#include "CATMathBox.h"
#include "CATMathPoint.h"
#include "CATMathVector.h"
#include "CATPoint.h"
#include "CGMGeometryMath.h"

#include "CATCGMTessBody.h"
#include "CATCGMTessBodyFaceSerializer.h"
#include "CATCGMTessBodyEdgeSerializer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static void
PointDoubleToFloat(double iPt[3], float oPt[3]);

ViewerTessellate::ViewerTessellate(ViewerRenderer* pRenderer)
	: _renderer(pRenderer)
{
	_useArrays = false;
#ifdef _USE_RED_RENDERER
	ViewerRedRenderer* r = dynamic_cast<ViewerRedRenderer*>(_renderer);
	_useArrays = r != NULL;
#endif
}

ViewerTessellate::~ViewerTessellate()
{
}


HRESULT ViewerTessellate::RenderFace(CATFace *iFace, CATCGMTessBody *iTessBody, const RGBAColor &iColor)
{
	if (!iFace || !iTessBody)
		return E_FAIL;

	CATCGMTessBodyFaceSerializer* pSerializer = CATCGMCreateTessBodyFaceSerializer( *iTessBody, *iFace );
	const int nbVertices = pSerializer->GetNbVertices();
	const int nbTriangles = pSerializer->GetNbTriangles();

	if( nbTriangles == 0 )
	{
		return S_OK;
	}

	const int sizeNormalBuffer = pSerializer->GetSizeOfVertexNormalBuffer();
	bool normalsPresent = ( 0 != sizeNormalBuffer );

	if (_useArrays)
	{
		int* aTriangles = new int[ 3*nbTriangles ];
		float* aPositions = new float[ 3*nbVertices ];
		float* aNormals;
		if( normalsPresent )
		{
			aNormals = new float[ sizeNormalBuffer ];
		}
		pSerializer->SerializeTriangles( &(aTriangles[0]) );
		pSerializer->SerializeVertexPositionLayer( &(aPositions[0]) );
		pSerializer->SerializeVertexNormalLayer( &(aNormals[0]) );

		START_TIMER( RENDERING_TIME );
		START_TIMER( DEBUG_TIME_1 );

		_renderer->AddTriangleSet( aPositions, aNormals, aTriangles, nbVertices, sizeNormalBuffer / 3,	nbTriangles, iColor);

		STOP_TIMER( DEBUG_TIME_1 );
		STOP_TIMER( RENDERING_TIME );

		delete [] aTriangles;
		delete [] aPositions;
		delete [] aNormals;
	}
	else
	{
		std::vector<float> aPositions;
		std::vector<float> aNormals;
		std::vector<int> aTriangles;

		aTriangles.resize( 3*nbTriangles );
		aPositions.resize( 3*nbVertices );
		if( normalsPresent )
		{
			aNormals.resize( sizeNormalBuffer );
		}
		pSerializer->SerializeTriangles( &(aTriangles[0]) );
		pSerializer->SerializeVertexPositionLayer( &(aPositions[0]) );
		pSerializer->SerializeVertexNormalLayer( &(aNormals[0]) );

		START_TIMER( RENDERING_TIME );
		START_TIMER( DEBUG_TIME_1 );

		_renderer->AddTriangleSet( aPositions, aNormals, aTriangles, iColor);

		STOP_TIMER( DEBUG_TIME_1 );
		STOP_TIMER( RENDERING_TIME );
	}

	delete pSerializer;
	return S_OK;
}

HRESULT ViewerTessellate::RenderEdge(CATEdge *iEdge, CATCGMTessBody *iTessBody, const RGBAColor &iColor)
{
	if (!iEdge && !iTessBody)
	{
		return E_FAIL;
	}

	CATCGMTessBodyEdgeSerializer* pSerializer = CATCGMCreateTessBodyEdgeSerializer( *iTessBody, *iEdge );
	const int nPositions = pSerializer->GetSizeOfVertexPositionBuffer();

	if (_useArrays)
	{
		float* aEdgePositions = new float[ 3*nPositions ];
		pSerializer->SerializeVertexPositionLayer( &(aEdgePositions[0]) );

		START_TIMER( RENDERING_TIME );
		START_TIMER( DEBUG_TIME_2 );

		_renderer->AddPolyline( aEdgePositions, nPositions, iColor );

		STOP_TIMER( DEBUG_TIME_2 );
		STOP_TIMER( RENDERING_TIME );

		delete [] aEdgePositions;
	}
	else
	{
		std::vector<float> aEdgePositions;
		aEdgePositions.resize( nPositions );
		pSerializer->SerializeVertexPositionLayer( &(aEdgePositions[0]) );

		START_TIMER( RENDERING_TIME );
		START_TIMER( DEBUG_TIME_2 );

		_renderer->AddPolyline( aEdgePositions, iColor );

		STOP_TIMER( DEBUG_TIME_2 );
		STOP_TIMER( RENDERING_TIME );
	}

	delete pSerializer;
	return S_OK;
}

HRESULT ViewerTessellate::RenderVertex(CATVertex *iVertex, const RGBAColor &iColor)
{
	HRESULT hr = S_OK;
	if (iVertex)
	{
		CATPoint *vrtPt = iVertex->GetPoint();
		if (vrtPt)
        {
			CATMathPoint vrtPos = vrtPt->GetMathPoint();
			double tmp[3];
			vrtPos.GetCoord (tmp);
			float pt[3]; // The vertex position for rendering
			PointDoubleToFloat(tmp, pt);

			START_TIMER( RENDERING_TIME );
			START_TIMER( DEBUG_TIME_3 );

			_renderer->AddVertex(pt, iColor);
			
			STOP_TIMER( DEBUG_TIME_3 );
			STOP_TIMER( RENDERING_TIME );
		}
		else
			hr = E_FAIL;
    }
	else
		hr = E_FAIL;

   return hr;
}

void PointDoubleToFloat(double iPt[3], float oPt[3])
{
	oPt[0] = (float)iPt[0];
	oPt[1] = (float)iPt[1];
	oPt[2] = (float)iPt[2];
}
