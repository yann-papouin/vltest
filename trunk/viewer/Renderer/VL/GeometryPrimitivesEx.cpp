#include "stdafx.h"
#include <vlGraphics/GeometryPrimitives.hpp>
#include <vlGraphics/Geometry.hpp>
#include <vlGraphics/DoubleVertexRemover.hpp>

using namespace vl;
//-----------------------------------------------------------------------------
ref<Geometry> makeCross( const vec3& origin, real length)
{
	ref<Geometry> geom = new Geometry;
	geom->setObjectName("Cross");

	ref<ArrayFloat3> vert3 = new ArrayFloat3;
	ref<ArrayFloat4> col4 = new ArrayFloat4;
	geom->setVertexArray( vert3.get() );
	geom->setColorArray( col4.get() );
	vert3->resize(6);
	col4->resize(6);

	vert3->at(0)= origin+fvec3(1,0,0)*length;
	vert3->at(1)= origin+fvec3(-1,0,0)*length;
	vert3->at(2)= origin+fvec3(0,1,0)*length;
	vert3->at(3)= origin+fvec3(0,-1,0)*length;
	vert3->at(4)= origin+fvec3(0,0,1)*length;
	vert3->at(5)= origin+fvec3(0,0,-1)*length;

	col4->at(0)  = fvec4(1,0,0,0);
	col4->at(1)  = fvec4(1,0,0,0);
	col4->at(2)  = fvec4(0,1,0,0);
	col4->at(3)  = fvec4(0,1,0,0);
	col4->at(4)  = fvec4(0,0,1,0);
	col4->at(5)  = fvec4(0,0,1,0);

	ref<DrawArrays> xaxis = new DrawArrays( PT_LINES,0,2 );
	ref<DrawArrays> yaxis = new DrawArrays( PT_LINES,2,2 );
	ref<DrawArrays> zaxis = new DrawArrays( PT_LINES,4,2 );

	geom->drawCalls()->push_back(xaxis.get());
	geom->drawCalls()->push_back(yaxis.get());
	geom->drawCalls()->push_back(zaxis.get());

#if defined(VL_OPENGL_ES1) || defined(VL_OPENGL_ES2)
  geom->makeGLESFriendly();
#endif

  return geom;
}

ref<Geometry> makeTriAxis()
{
	ref<Geometry> geom = new Geometry;

	float r = 0.2f;
	//arrow for x axis
//	vl::ref<vl::Geometry> xarrow = vl::makeBox( vl::vec3(0,0,0),  vl::vec3(11,-r,-r), true);
//	vl::ref<vl::Effect> effect1 = new vl::Effect; 
//	effect1->shader()->enable(vl::EN_DEPTH_TEST);
//	effect1->shader()->enable(vl::EN_LIGHTING);
//	effect1->shader()->setRenderState( new Light, 0 );
//	effect1->shader()->gocMaterial()->setDiffuse( vl::red );
//	//mVLBaseView->sceneManager()->tree()->addActor( xarrow.get(), effect1.get(), mCubeTransform.get()  );
//
//	//arrow for y axis
//	vl::ref<vl::Geometry> yarrow = vl::makeBox( vl::vec3(0,0,0),  vl::vec3(-r,11,-r), true);
//	vl::ref<vl::Effect> effect2 = new vl::Effect; 
//	effect2->shader()->enable(vl::EN_DEPTH_TEST);
//	effect2->shader()->enable(vl::EN_LIGHTING);
//	effect2->shader()->setRenderState( new Light, 0 );
//	effect2->shader()->gocMaterial()->setDiffuse( vl::green );
//// 	mVLBaseView->sceneManager()->tree()->addActor( yarrow.get(), effect2.get(), mCubeTransform.get()  );
//
//	//arrow for z axis
//	vl::ref<vl::Geometry> zarrow = vl::makeBox( vl::vec3(0,0,0),  vl::vec3(-r,-r,11), true);
//	vl::ref<vl::Effect> effect3 = new vl::Effect; 
//	effect3->shader()->enable(vl::EN_DEPTH_TEST);
//	effect3->shader()->enable(vl::EN_LIGHTING);
//	effect3->shader()->setRenderState( new Light, 0 );
//	effect3->shader()->gocMaterial()->setDiffuse( vl::blue );
//	mVLBaseView->sceneManager()->tree()->addActor( zarrow.get(), effect3.get(), mCubeTransform.get()  );

	return geom;
}
//-----------------------------------------------------------------------------
