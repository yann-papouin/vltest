#include <stdafx.h>
#include <vlGraphics/OpenGLContext.hpp>
#include <vlGraphics/Actor.hpp>
#include <vlGraphics/SceneManager.hpp>
#include <vlGraphics/Rendering.hpp>
#include <vlGraphics/RayIntersector.hpp>

#include "Renderer/VL/PerspectiveTrackballManipulator.hpp"
#include "Renderer/VL/GeometryPrimitivesEx.hpp"
#include "Renderer/VL/VLBaseView.hpp"

#pragma warning (disable:4244) 

using namespace vl;

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
