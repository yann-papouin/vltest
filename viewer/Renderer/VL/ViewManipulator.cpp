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
_zoomFactor(50)
{
	m_pVLBaseView = pVLBaseView;
	m_bShift = false;
}

void OrthographicTrackballManipulator::mouseMoveEvent( int x, int y )
{
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
		const double zoomFactor = _zoomFactor > 0 ? _zoomFactor :
			(1 / std::abs(_zoomFactor));

		float tx =1.0f * (mMouseStart.x() - x) /zoomFactor ;
		float ty = -1.0f * (mMouseStart.y() - y)/zoomFactor;
		tx *= translationSpeed();
		ty *= translationSpeed();	
		
		this->camera()->setProjectionMatrix(
			// compute directly an orthographic projection & center the scene on the screen
			mOldProjMatrix*
			vl::mat4::getTranslation(-tx, -ty, 0),PMT_OrthographicProjection);
	}

	// update the view
	this->openglContext()->update();
}

void OrthographicTrackballManipulator::setProjection()
{
	//const int nearPlane = 1;
	//const int farPlane  = 1000;

	//// these are actually not needed since we dont call setProjectionAsOrtho()
	//// but is good practice to keep things up to date anyway...
	//this->camera()->setNearPlane(nearPlane);
	//this->camera()->setFarPlane(farPlane);

	float nearPlane = this->camera()->nearPlane();
	float farPlane = this->camera()->farPlane();

	// make view X units high and compute the width as whatever it takes to keep the aspect ratio.
	const double zoomFactor = _zoomFactor > 0 ? _zoomFactor :
		(1 / std::abs(_zoomFactor));

	vl::mat4 projMatrix = this->camera()->projectionMatrix();
	//const int w = (int)this->camera()->aspectRatio() * zoomFactor;
	//const int h = (int)zoomFactor;
	const int w = this->camera()->viewport()->width()/zoomFactor;
	const int h = this->camera()->viewport()->height()/zoomFactor;

	double tx,ty;
	tx = w/2; // center the view
	ty = h/2;

	if (mode()== ZoomMode && !m_bShift)
	{
		//vec3 win(_x,_y,0);

		//vec3 win2(w/2,h/2,0);

		//const int oldw = this->camera()->viewport()->width()/_zoomFactorOld;
		//const int oldh = this->camera()->viewport()->height()/_zoomFactorOld;


		//tx = w/2+(_x-oldw/2);
		//ty = h/2+(_y-oldh/2);
		//project the point to the viewplane;

		//HUtility::AdjustPositionToPlane(this, mwp, adjusted.target);
		//adjusted.target += (adjusted.target - mwp) * zDelta;

		//float const diagonal_len = sqrt(pow(adjusted.field_width, 2) + pow(adjusted.field_height, 2));
		//HVector dir_to_position = orig.position - orig.target;
		//HC_Compute_Normalized_Vector(&dir_to_position, &dir_to_position);
		//adjusted.position = adjusted.target + dir_to_position * 2.5 * diagonal_len;
	}

	//projMatrix *= vl::mat4::getOrtho(0, w, 0, h, nearPlane, farPlane);
	//this->camera()->setProjectionMatrix(projMatrix,PMT_OrthographicProjection);


	// install the orthographic projection
	this->camera()->setProjectionMatrix(
		// compute directly an orthographic projection & center the scene on the screen
		vl::mat4::getOrtho(0, w, 0, h, nearPlane, farPlane) *
		vl::mat4::getTranslation(tx, ty, 0),PMT_OrthographicProjection);
}

void OrthographicTrackballManipulator::resizeEvent( int w, int h )
{
	const double zoomFactor = _zoomFactor > 0 ? _zoomFactor :
		(1 / std::abs(_zoomFactor));

	this->setProjection();
}

void OrthographicTrackballManipulator::mouseWheelEvent( int n )
{
	//_zoomFactor += 2 * n;
	//_zoomFactor = _zoomFactor != 0 ? _zoomFactor : 1;

	double scale = 1.0* (_zoomFactor+2*n)/_zoomFactor;

	vl::mat4 projMatrix = this->camera()->projectionMatrix();
	projMatrix *= vl::mat4::getScaling(scale,scale,scale);
	this->camera()->setProjectionMatrix(projMatrix,PMT_OrthographicProjection);

//	this->setProjection();
}

void OrthographicTrackballManipulator::mouseDownEvent( EMouseButton btn, int x, int y )
{
	TrackballManipulator::mouseDownEvent(btn,x,y);

	if (btn == MiddleButton)
	{
		mOldProjMatrix = this->camera()->projectionMatrix();
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
			//const double zoomFactor = _zoomFactor > 0 ? _zoomFactor :
			//	(1 / std::abs(_zoomFactor));

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
