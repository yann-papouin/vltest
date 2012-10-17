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

			ref<Geometry> intersection_point_geom = makeCross(vec3(0,0,0), 20.0f/mZoomFactor);

			intersection_point_geom->computeNormals();
			mCrossActor = m_pVLBaseView->sceneManager()->tree()->addActor( intersection_point_geom.get(), fx.get(), new Transform );
			mIntersectionPointTransf = mCrossActor->transform();
			// highlight the intersection point by moving the green sphere there
			mIntersectionPointTransf->setLocalMatrix( mat4() );
			mIntersectionPoint = intersector.intersections()[0]->intersectionPoint();
			mIntersectionPointTransf->translate( mIntersectionPoint );
			mIntersectionPointTransf->computeWorldMatrix();
		}

		openglContext()->update();
	}
}

void OrthographicTrackballManipulator::mouseMoveEvent( int x, int y )
{
	// 10/12/2012 mwu : don't call the mouse move in parent class 
//	TrackballManipulator::mouseMoveEvent(x,y); 

	if (camera() == 0 || mode() == NoMode)
		return;

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

void OrthographicTrackballManipulator::mouseUpEvent( EMouseButton btn, int x, int y )
{
	TrackballManipulator::mouseUpEvent(btn,x,y);

	if (btn == MiddleButton)  //Pan and center the window
	{
		if (x== mMouseStart.x() && y == mMouseStart.y() )//single click
		{
			mPivot = mIntersectionPoint;

			//Move to the center of the screen	
			const float zoomFactor = mZoomFactor > 0 ? mZoomFactor :
				(1 / std::abs(mZoomFactor));

			int centerX = camera()->viewport()->width()/2.0;
			int centerY = camera()->viewport()->height()/2.0;

			double tx = -(x- centerX ) /zoomFactor;
			double ty = (y - centerY ) /zoomFactor;

			camera()->setProjectionMatrix(
				// compute directly an orthographic projection & pan the scene on the screen
				mProjMatrixBeforePan*
				vl::mat4::getTranslation(tx, ty, 0),PMT_OrthographicProjection);
		}

		m_pVLBaseView->sceneManager()->tree()->eraseActor( mCrossActor);
		openglContext()->update();
	}
}

void OrthographicTrackballManipulator::mouseWheelEvent( int n )
{
	float scaleRelativeFactor = n>0?1.1f:0.9f;
	mZoomFactor*=scaleRelativeFactor;

	vl::mat4 projMatrix = camera()->projectionMatrix();
	camera()->setProjectionMatrix(projMatrix.scale(scaleRelativeFactor,scaleRelativeFactor,scaleRelativeFactor),PMT_OrthographicProjection);

	if (!m_bShift) //if shift key is not pushed, need to translate the camera
	{
		int centerX = camera()->viewport()->width()/2.0;
		int centerY = camera()->viewport()->height()/2.0;

		// 10/17/2012 modified by mwu : need to consider the best factor for offset
		double dOffsetX = -(m_pVLBaseView->mWheelX- centerX ) /8000.0*n;
		double dOffsetY = (m_pVLBaseView->mWheelY - centerY ) /8000.0*n;

		projMatrix = camera()->projectionMatrix();
		camera()->setProjectionMatrix(projMatrix.translate(dOffsetX,dOffsetY,0),PMT_OrthographicProjection);
	}
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
