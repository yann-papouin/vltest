#include <stdafx.h>
#include <vlGraphics/OpenGLContext.hpp>
#include <vlGraphics/Actor.hpp>
#include <vlGraphics/SceneManager.hpp>
#include <vlGraphics/Rendering.hpp>
#include <vlGraphics/RayIntersector.hpp>

#include "Renderer/VL/ViewManipulator.hpp"
#include "Renderer/VL/GeometryPrimitivesEx.hpp"
#include "Renderer/VL/VLBaseView.hpp"

#pragma warning (disable:4244) 

using namespace vl;

#pragma region PerspectiveTrackballManipulator

PerspectiveTrackballManipulator::PerspectiveTrackballManipulator( VLBaseView* pVLBaseView )
{
	VL_DEBUG_SET_OBJECT_NAME()
		m_pVLBaseView = pVLBaseView;
}
//-----------------------------------------------------------------------------
void PerspectiveTrackballManipulator::mouseDownEvent(EMouseButton btn, int x, int y)
{
	if ( camera() == NULL )
		return;

	// if already busy ignore the event
	if (mode() != NoMode)
		return;

	// enter new mode
	if (btn == rotationButton())
		mMode = RotationMode;
	else
		if (btn == translationButton())
			mMode = TranslationMode;
		else
			if (btn == zoomButton())
				mMode = ZoomMode;

	VL_CHECK(openglContext()->framebuffer())

	// set x/y relative to the viewport
	x -= camera()->viewport()->x();
	y -= openglContext()->framebuffer()->height() - 1 - (camera()->viewport()->y() + camera()->viewport()->height() -1);

	// check that the click is in the viewport
	int w = camera()->viewport()->width();
	int h = camera()->viewport()->height();

	if (x<0 || y<0 || x>=w || y>=h)
		return;

	mMouseStart.x() = x;
	mMouseStart.y() = y;

	if (mTransform)
	{
		mStartMatrix = mTransform->localMatrix();
		mPivot = mStartMatrix.getT();
	}
	else
		mStartMatrix = camera()->modelingMatrix();

	mStartCameraPos = camera()->modelingMatrix().getT();
	mStartPivot = mPivot;

	if (btn == MiddleButton)
	{
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
			//		  mPivot = intersector.intersections()[0]->intersectionPoint();
		}

		openglContext()->update();
	}
}
//-----------------------------------------------------------------------------
void PerspectiveTrackballManipulator::mouseUpEvent(EMouseButton btn, int /*x*/, int /*y*/)
{
	if ( camera() == NULL )
		return;

	// if the trackball is not doing anything ignore the event
	if (mode() == NoMode)
		return;

	if (btn == MiddleButton)
	{
		m_pVLBaseView->sceneManager()->tree()->eraseActor( mCrossActor);
		openglContext()->update();
	}

	// leave the mode
	if (btn == rotationButton() && mMode == RotationMode)
		mMode = NoMode;
	else
		if (btn == translationButton() && mMode == TranslationMode)
			mMode = NoMode;
		else
			if (btn == zoomButton() && mMode == ZoomMode)
				mMode = NoMode;
}
//-----------------------------------------------------------------------------
void PerspectiveTrackballManipulator::mouseMoveEvent(int x, int y)
{
	if ( camera() == NULL )
		return;

	// ignore the event if the trackball is not in any mode
	if (mode() == NoMode)
		return;

	VL_CHECK(openglContext()->framebuffer())
	// set x/y relative to the top/left cornder of the viewport
	x -= camera()->viewport()->x();
	y -= openglContext()->framebuffer()->height() - 1 - (camera()->viewport()->y() + camera()->viewport()->height() -1);

	if (mode() == RotationMode)
	{
		if (mTransform)
		{
			mTransform->setLocalMatrix( mat4::getTranslation(mPivot) * trackballRotation(x,y) * mat4::getTranslation(-mPivot) * mStartMatrix );
			mTransform->computeWorldMatrix();
			mStartMatrix = mTransform->localMatrix();
		}
		else
		{
			camera()->setModelingMatrix( mat4::getTranslation(mPivot) * trackballRotation(x,y) * mat4::getTranslation(-mPivot) * mStartMatrix );
			mStartMatrix = camera()->modelingMatrix();
		}

		mMouseStart.x() = x;
		mMouseStart.y() = y;
	}
	else
		if (mode() == ZoomMode)
		{
			float t = (y-mMouseStart.y()) / 200.0f;
			t *= zoomSpeed();
			real distance = (mStartCameraPos - mPivot).length();
			vec3 camera_pos = mStartCameraPos - camera()->modelingMatrix().getZ()*t*distance;
			mat4 m = camera()->modelingMatrix();
			m.setT(camera_pos);
			camera()->setModelingMatrix(m);
		}
		else
			if (mode() == TranslationMode)
			{
				float tx = (mMouseStart.x() - x)  / 400.0f;
				float ty = -(mMouseStart.y() - y) / 400.0f;
				tx *= translationSpeed();
				ty *= translationSpeed();
				real distance = (mStartCameraPos - mPivot).length();
				vec3 up    = camera()->modelingMatrix().getY();
				vec3 right = camera()->modelingMatrix().getX();
				mat4 m = camera()->modelingMatrix();
				m.setT(mStartCameraPos + up*distance*ty + right*distance*tx);
				camera()->setModelingMatrix(m);
				mPivot = mStartPivot + up*distance*ty + right*distance*tx;
			}

			// update the view
			openglContext()->update();
}

#pragma endregion PerspectiveTrackballManipulator


//-----------------------------------------------------------------------------
OrthographicTrackballManipulator::OrthographicTrackballManipulator(VLBaseView* pVLBaseView) :
mZoomFactor(50)
{
	m_pVLBaseView = pVLBaseView;
	m_bShift = false;
}

void OrthographicTrackballManipulator::resizeEvent( int w, int h )
{
	float nearPlane = this->camera()->nearPlane();
	float farPlane = this->camera()->farPlane();

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
	this->camera()->setProjectionMatrix(
		// compute directly an orthographic projection & center the scene on the screen
		vl::mat4::getOrtho(0, new_w, 0, new_h, nearPlane, farPlane) *
//		vl::mat4::getTranslation(new_w/2, new_h/2, 0)
		vl::mat4::getTranslation(translation.x(), translation.y(), 0)
		,PMT_OrthographicProjection);
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
	vl::mat4 projMatrix = this->camera()->projectionMatrix();
	projMatrix *= vl::mat4::getScaling(scale,scale,scale);

	if (m_bShift)
	{
		this->camera()->setProjectionMatrix(projMatrix,PMT_OrthographicProjection);
	}
	else
	{
		//need to consider the best factor for offset
		double dOffsetX = -(_x - this->camera()->viewport()->width()/2.0 )/800.0*n;
		double dOffsetY = (_y - this->camera()->viewport()->height()/2.0 ) /800.0*n;

		projMatrix *= vl::mat4::getTranslation(dOffsetX, dOffsetY, 0);

		this->camera()->setProjectionMatrix(projMatrix,PMT_OrthographicProjection);
	}
}

void OrthographicTrackballManipulator::mouseDownEvent( EMouseButton btn, int x, int y )
{
	TrackballManipulator::mouseDownEvent(btn,x,y);

	if (btn == MiddleButton)
	{
		mProjMatrixBeforePan = this->camera()->projectionMatrix();
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

			//this->camera()->setProjectionMatrix(
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

	if (btn == MiddleButton)
	{
		m_pVLBaseView->sceneManager()->tree()->eraseActor( mCrossActor);
		openglContext()->update();
	}
}

void OrthographicTrackballManipulator::mouseMoveEvent( int x, int y )
{
	_x = x;
	_y = y;

	if (this->mode() == RotationMode)
		return vl::TrackballManipulator::mouseMoveEvent(x, y);

	if (this->camera() == 0 || this->mode() == NoMode)
		return;

	// set x/y relative to the top/left corner of the viewport
	const vl::Viewport* vp = this->camera()->viewport();
	x -= vp->x();
	y -= this->openglContext()->height() - 1 - (vp->y() + vp->height() - 1);

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
		
		this->camera()->setProjectionMatrix(
			// compute directly an orthographic projection & center the scene on the screen
			mProjMatrixBeforePan*
			vl::mat4::getTranslation(tx, ty, 0),PMT_OrthographicProjection);
	}

	// update the view
	this->openglContext()->update();
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
