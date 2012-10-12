#include <stdafx.h>
#include <vlGraphics/OpenGLContext.hpp>
#include <vlGraphics/Actor.hpp>
#include <vlGraphics/SceneManager.hpp>
#include <vlGraphics/Rendering.hpp>
#include <vlGraphics/RayIntersector.hpp>

#include "Renderer/VL/OrthographicTrackballManipulator.hpp"
#include "Renderer/VL/GeometryPrimitivesEx.hpp"
#include "Renderer/VL/VLBaseView.hpp"

#pragma warning (disable:4244) 

using namespace vl;
//-----------------------------------------------------------------------------
OrthographicTrackballManipulator::OrthographicTrackballManipulator(VLBaseView* pVLBaseView) :
mZoomFactor(50)
{
	m_pVLBaseView = pVLBaseView;
	m_bShift = false;

	setZoomButton(NoButton);
}

void OrthographicTrackballManipulator::resizeEvent( int w, int h )
{
	float nearPlane = camera()->nearPlane();
	float farPlane = camera()->farPlane();

	float scale = 1.0f;
 	//scale = 1.0*mOldSizeX/w;
 	//if (mOldSizeX/mOldSizeY > camera()->aspectRatio())
 	//{
 	//	scale = 1.0f*mOldSizeY/h;
 	//}
	
	float new_w =w/mZoomFactor*scale;
	float new_h = h/mZoomFactor*scale;

	vl::mat4 projMatrix = camera()->projectionMatrix();
	vl::vec3 translation = projMatrix.getT();

	vl::mat4 invMatrix;
//	vl::mat4 orthoMatrix = 	vl::mat4::getOrtho(0,mOldSizeX/mZoomFactor,0,mOldSizeY/mZoomFactor,nearPlane,farPlane);
	vl::mat4 orthoMatrix = 	vl::mat4::getOrtho(0,new_w,0,new_h,nearPlane,farPlane);
	invMatrix = orthoMatrix.getInverse();

	vl::mat4 transf = invMatrix*projMatrix;
	translation = transf.getT();

	// install the orthographic projection
//	camera()->setProjectionMatrix(
//		// compute directly an orthographic projection & center the scene on the screen
//		vl::mat4::getOrtho(0, new_w, 0, new_h, nearPlane, farPlane) *
////		vl::mat4::getTranslation(new_w/2, new_h/2, 0)
//		vl::mat4::getTranslation(translation.x(), translation.y(), 0)
//		,PMT_OrthographicProjection);


	camera()->setProjectionOrtho(-new_w/2.0,new_w/2.0,-new_h/2.0,new_h/2.0, nearPlane, farPlane);

}

void OrthographicTrackballManipulator::mouseWheelEvent( int n )
{
	if (n>0)
	{
		mZoomFactor*=1.1f;
	}
	else
	{
		mZoomFactor*=0.9f;
	}
// 	mZoomFactor += 2 * n;
// 	mZoomFactor = mZoomFactor != 0 ? mZoomFactor : 1;
//	double scale = 1.0* (mZoomFactor+2*n)/mZoomFactor;
	float scale = n>0?1.1f:0.9f;
	vl::mat4 projMatrix = camera()->projectionMatrix();

	if (m_bShift)
	{
		projMatrix *= vl::mat4::getScaling(scale,scale,scale);
		camera()->setProjectionMatrix(projMatrix,PMT_OrthographicProjection);
	}
	else
	{


		//need to consider the best factor for offset
		double dOffsetX = -(_x - camera()->viewport()->width()/2.0 )/800.0*n;
		double dOffsetY = (_y - camera()->viewport()->height()/2.0 ) /800.0*n;

		//const float zoomFactor = mZoomFactor > 0 ? mZoomFactor :
		//	(1 / std::abs(mZoomFactor));

		//dOffsetX = - ( _x - camera()->viewport()->width()/2.0) / zoomFactor ;
		//dOffsetY =  ( _y - camera()->viewport()->height()/2.0) / zoomFactor;

		projMatrix *= vl::mat4::getTranslation(dOffsetX, dOffsetY, 0);
		projMatrix *= vl::mat4::getScaling(scale,scale,scale);

		camera()->setProjectionMatrix(projMatrix,PMT_OrthographicProjection);
	}
}

void OrthographicTrackballManipulator::mouseDownEvent( EMouseButton btn, int x, int y )
{
	TrackballManipulator::mouseDownEvent(btn,x,y);

	if (btn == MiddleButton)
	{
		mProjMatrixBeforePan = camera()->projectionMatrix();
		// convert Y coordinates to the OpenGL conventions
		y = openglContext()->height() - y;

		// compute the ray passing through the selected pixel
		Ray ray = camera()->computeRay(x,y);
		// instance our ray-intersector
		RayIntersector intersector;
		// compute a frustum along the ray to accelerate the intersection test
		intersector.setFrustum( camera()->computeRayFrustum( x,y ) );

		// compute the intersections!
		intersector.intersect(ray, m_pVLBaseView->sceneManager());

		// inspect our intersections, the intersections returned are sorted according to their distance, the first one is the closest.
		if (intersector.intersections().size())
		{
			ref<Effect> fx = new Effect;
			fx->shader()->enable(EN_DEPTH_TEST);
			fx->shader()->enable(EN_LIGHTING);
			fx->shader()->gocLight(0)->setLinearAttenuation(0.025f);
			fx->shader()->gocMaterial()->setDiffuse( green );

			ref<Geometry> intersection_point_geom = makeCross(vec3(0,0,0), 0.5f);

			intersection_point_geom->computeNormals();
			mCrossActor = m_pVLBaseView->sceneManager()->tree()->addActor( intersection_point_geom.get(), fx.get(), new Transform );
			mIntersectionPoint = mCrossActor->transform();

			// highlight the intersection point by moving the green sphere there
			mIntersectionPoint->setLocalMatrix( mat4() );
			mIntersectionPoint->translate( intersector.intersections()[0]->intersectionPoint() );
			mIntersectionPoint->computeWorldMatrix();
			
			mPivot = intersector.intersections()[0]->intersectionPoint();

			//// TODO: Move to the center of the screen
			//const double zoomFactor = m_dZoomFactor > 0 ? m_dZoomFactor :
			//	(1 / std::abs(m_dZoomFactor));

			//vl::vec4 in1(mPivot.x(),mPivot.y(),mPivot.z(),1);
			//vl::vec4 out1;
			//camera()->project(in1,out1);

			//vl::vec4 in2(mStartPivot.x(),mStartPivot.y(),mStartPivot.z(),1);
			//vl::vec4 out2;
			//camera()->project(in2,out2);

			//float tx =out2.x()-out1.x();
			//float ty = out2.y()-out1.y();

			//tx/=zoomFactor;
			//ty/=zoomFactor;

			//camera()->setProjectionMatrix(
			//	// compute directly an orthographic projection & center the scene on the screen
			//	mOldProjMatrix*
			//	vl::mat4::getTranslation(tx, ty, 0),PMT_OrthographicProjection);
		}

		openglContext()->update();
	}
}

void OrthographicTrackballManipulator::mouseUpEvent( EMouseButton btn, int x, int y )
{
	TrackballManipulator::mouseUpEvent(btn,x,y);

	if (btn == MiddleButton)  //Pan and center the window
	{
		m_pVLBaseView->sceneManager()->tree()->eraseActor( mCrossActor);
		openglContext()->update();
	}
}

void OrthographicTrackballManipulator::mouseMoveEvent( int x, int y )
{
	// 10/12/2012 mwu : don't call the mouse move in parent class 
//	TrackballManipulator::mouseMoveEvent(x,y); 

	if (camera() == 0 || mode() == NoMode)
		return;

	_x = x;
	_y = y;

	// set x/y relative to the top/left corner of the viewport
	const vl::Viewport* vp = camera()->viewport();
	x -= vp->x();
	y -= openglContext()->height() - 1 - (vp->y() + vp->height() - 1);

	if (mode() == ZoomMode)
	{
		// Handled by mouse wheel
	}
	else if (mode() == TranslationMode)
	{
 		const float zoomFactor = mZoomFactor > 0 ? mZoomFactor :
 			(1 / std::abs(mZoomFactor));
 
 		float tx =1.0f * (x - mMouseStart.x()) /zoomFactor ;
 		float ty = -1.0f * (y - mMouseStart.y())/zoomFactor;
 		tx *= translationSpeed();
 		ty *= translationSpeed();	
 		
 		camera()->setProjectionMatrix(
 			// compute directly an orthographic projection & pan the scene on the screen
 			mProjMatrixBeforePan*
 			vl::mat4::getTranslation(tx, ty, 0),PMT_OrthographicProjection);
	}
	else
	{
		TrackballManipulator::mouseMoveEvent(x,y);
	}

	// update the view
	openglContext()->update();
}

void OrthographicTrackballManipulator::keyReleaseEvent( unsigned short, EKey key )
{
	if (key == Key_Shift || key == Key_LeftShift || key == Key_RightShift)
		m_bShift = false;
}

void OrthographicTrackballManipulator::keyPressEvent( unsigned short, EKey key)
{
	if (key == Key_Shift || key == Key_LeftShift || key == Key_RightShift)
		m_bShift = true;
}
