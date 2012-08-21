#include "stdafx.h"

#include <vlCore/Time.hpp>
#include <vlCore/Say.hpp>
#include <vlCore/Log.hpp>
#include <vlCore/VisualizationLibrary.hpp>

#include <vlGraphics/Rendering.hpp>
#include <vlGraphics/SceneManager.hpp>
#include <vlGraphics/RenderQueue.hpp>
#include <vlGraphics/FlatManipulator.hpp>

#include "Renderer/VL/VLBaseView.hpp"

using namespace vl;

//-----------------------------------------------------------------------------
VLBaseView::VLBaseView()
{
	VL_DEBUG_SET_OBJECT_NAME()
	mFrameCount = 0;
	mStartTime  = Time::currentTime();
	mFPS        = 0;
	mReadPixels = new ReadPixels;
	mMainAppletName = "MainAppletNoName";
	m_bPerspective = false;
//	m_bPerspective = true;

}
//-----------------------------------------------------------------------------
void VLBaseView::initialize()
{
	// if the user didn't provide one use the one installed by default
	ref<Rendering> rend = rendering() && rendering()->as<Rendering>() ? rendering()->as<Rendering>() : new Rendering;
	setRendering(rend.get());

	// installs a SceneManagerActorTree as the default scene manager
	mSceneManagerActorTree = new SceneManagerActorTree;
	rend->sceneManagers()->push_back(sceneManager());

	mFly = new GhostCameraManipulator;
	if (m_bPerspective == true)
	{
		mTrackball = new PerspectiveTrackballManipulator(this);
	}
	else
	{
		mTrackball = new OrthographicTrackballManipulator(this);
	}
	mFly->setEnabled(false);
	mTrackball->setEnabled(true);

	bindManipulators( rend->camera() );
}
//-----------------------------------------------------------------------------
void VLBaseView::updateEvent()
{
	// FPS counter
	if (Time::currentTime() - mStartTime > 0.500f)
	{
		double secs = (Time::currentTime() - mStartTime);
		mFPS = mFrameCount / secs;
		mFrameCount = 0;
		mStartTime = Time::currentTime();
	}
	mFrameCount++;

	// update the scene content
	updateScene();

	// set frame time for all the rendering
	real now_time = Time::currentTime();
	rendering()->setFrameClock( now_time );

	// execute rendering
	rendering()->render();

	// show rendering
	if ( openglContext()->hasDoubleBuffer() )
		openglContext()->swapBuffers();

	VL_CHECK_OGL();

	// useful for debugging
	// wglMakeCurrent(NULL,NULL);
}
//-----------------------------------------------------------------------------
void VLBaseView::keyReleaseEvent(unsigned short, EKey key)
{
	if (key == Key_Escape)
		openglContext()->quitApplication();
	else
		if (key == Key_T)
		{
			mFly->setEnabled(false);
			mTrackball->setEnabled(true);
		}
		else
			if (key == Key_F)
			{
				mTrackball->setEnabled(false);
				mFly->setEnabled(true);
			}
			else
				if (key == Key_F1)
					openglContext()->setFullscreen( !openglContext()->fullscreen() );
				else
					if (key == Key_F5)
					{
						mReadPixels->setup( 0, 0, openglContext()->width(), openglContext()->height(), RDB_BACK_LEFT, false );
						rendering()->onFinishedCallbacks()->push_back( mReadPixels.get() );
						mReadPixels->setRemoveAfterCall(true);
						Time time;
						String filename = Say( MainAppletName() + " - %n%02n%02n%02n%02n.png") << time.year() << time.month() << time.dayOfMonth() << time.hour() << time.second();
						mReadPixels->setSavePath( filename );
						Log::print( Say("Saved screenshot: '%s'\n") << filename );
						openglContext()->update();
					}
					else
						if (key == Key_U)
							openglContext()->update();
						else
							if (key == Key_C)
								openglContext()->setContinuousUpdate( !openglContext()->continuousUpdate() );
}
//-----------------------------------------------------------------------------
void VLBaseView::resizeEvent(int w, int h)
{
	// if a simple Rendering is attached as the rendering root than update viewport and projection matrix.
	Rendering* rend = cast<Rendering>(rendering());
	if (rend)
	{
		VL_CHECK( w == rend->renderer()->framebuffer()->width() );
		VL_CHECK( h == rend->renderer()->framebuffer()->height() );
		rend->camera()->viewport()->setWidth( w );
		rend->camera()->viewport()->setHeight( h );
		if (m_bPerspective)
		{
	//		rend->camera()->setProjectionPerspective();
		}
		else
		{
//			rend->camera()->setProjectionOrtho();
		}
	}
}
//-----------------------------------------------------------------------------
void VLBaseView::bindManipulators(Camera* camera)
{
	mFly->setCamera( camera );
	mTrackball->setCamera( camera );
	mTrackball->setTransform( NULL );
	mTrackball->setPivot( vec3(0,0,0) );
}
//-----------------------------------------------------------------------------
void VLBaseView::addedListenerEvent(OpenGLContext* ogl_context)
{
	VL_CHECK(ogl_context)
		VL_CHECK(mFly->openglContext() == NULL);
	ogl_context->addEventListener( mFly.get() );

	VL_CHECK(mTrackball->openglContext() == NULL);
	mTrackball->setPivot( vec3(0,0,0) );
	ogl_context->addEventListener( mTrackball.get() );
}
//-----------------------------------------------------------------------------
void VLBaseView::removedListenerEvent(OpenGLContext* ogl_context)
{
	ogl_context->removeEventListener( mTrackball.get() );
	ogl_context->removeEventListener( mFly.get() );
}
//-----------------------------------------------------------------------------
void VLBaseView::destroyEvent()
{
	mFly->setCamera(NULL);
	mTrackball->setCamera(NULL);
	mTrackball->setTransform(NULL);
	mRendering = NULL;
}

void VLBaseView::FitWorld()
{
	Rendering* rend = cast<Rendering>(rendering());
	if (rend)
	{
		sceneManager()->setBoundsDirty(true);
		sceneManager()->computeBounds();
		const vl::Sphere bs = sceneManager()->boundingSphere();
		vl::vec3 new_at = bs.center();
		mTrackball->setPivot(new_at);

		//Set projection matrix
		float w = (float) rend->camera()->viewport()->width();
		float h =  (float) rend->camera()->viewport()->height();
		if (bs.radius()>0)
		{
			float aspectRatio = rend->camera()->aspectRatio();
			if (aspectRatio>1)
			{
				h = bs.radius()*2.0f*1.01f;
				w = h*aspectRatio;
			}
			else
			{
				if(aspectRatio != 0)
				{
					w = bs.radius()*2.0f*1.01f;
					h = w/aspectRatio;
				}
			}
		}

		OrthographicTrackballManipulator* pOrthoTM = dynamic_cast<OrthographicTrackballManipulator*>(trackball());
		if (pOrthoTM)
		{
			float zoomFactor = 1.0f*rend->camera()->viewport()->width()/w;
			pOrthoTM->SetZoomFactor(zoomFactor);
		}

		const float nearPlane = 0.5f;
		const float farPlane  = 10000.0f;

		// install the orthographic projection
		if (rend->camera()->projectionMatrixType() == PMT_OrthographicProjection )
		{
			rend->camera()->setProjectionMatrix(
				// compute directly an orthographic projection & center the scene on the screen
				vl::mat4::getOrtho(0, w, 0, h, nearPlane, farPlane) 
				* vl::mat4::getTranslation(w / 2.0f, h / 2.0f, 0) 
				,PMT_OrthographicProjection) ;
		}
		else
		{
			// TODO: 7/19/2012 mwu Doesn't work  
			//rend->camera()->setProjectionMatrix(
			//	// compute directly an orthographic projection & center the scene on the screen
			//	rend->camera()->projectionMatrix() *
			//	vl::mat4::getTranslation(w / 2.0f, h / 2.0f, 0),PMT_PerspectiveProjection);
		}
	}
}

void VLBaseView::SetViewMode( ViewMode eViewMode,bool bFitWorld /*= true*/ )
{
	mViewMode = eViewMode;

	if (eViewMode == ViewNone) 
		return;

	if (bFitWorld)
	{
		FitWorld();
	}

	Rendering* rend = cast<Rendering>(rendering());
	if (rend)
	{
		sceneManager()->setBoundsDirty(true);
		sceneManager()->computeBounds();
		const vl::Sphere bs = sceneManager()->boundingSphere();

		/* define the camera position and orientation */
		vl::vec3 eye;  
		vl::vec3 at;
		vl::vec3 up;  
		vl::vec3 right;
		rend->camera()->getViewMatrixAsLookAt(eye, at,up,right);

		float length = (eye-at).length();

		at = bs.center();
		length = 1.1f*bs.radius();

		switch(mViewMode)
		{
		case ViewXY:
			eye = at;
			eye.z() = at.z() + length;
			up = vec3(0,1,0);
			rend->camera()->setViewMatrixLookAt(eye, at,up);
			break;

		case ViewYX:
			eye = at;
			eye.z() = at.z() - length;
			up = vec3(0,1,0);
			rend->camera()->setViewMatrixLookAt(eye, at,up);

			break;

		case ViewXZ:
			eye = at;
			eye.y() = at.y() - length;
			up = vec3(0,0,1);
			rend->camera()->setViewMatrixLookAt(eye, at,up);

			break;

		case ViewZX:
			eye = at;
			eye.y() = at.y() + length;
			up = vec3(0,0,1);
			rend->camera()->setViewMatrixLookAt(eye, at,up);

			break;

		case ViewYZ:
			eye = at;
			eye.x() = at.x() + length;
			up = vec3(0,0,1);
			rend->camera()->setViewMatrixLookAt(eye, at,up);

			break;

		case ViewZY:
			eye = at;
			eye.x() = at.x() - length;
			up = vec3(0,0,1);
			rend->camera()->setViewMatrixLookAt(eye, at,up);

			break;

		case ViewIso:
			{
				vec3 frontAxis(1,0,0);
				vec3 topAxis(0,0,1);
				vec3 rightAxis = -cross(frontAxis,topAxis);
				float newLen = length * 0.5774f;

				eye.x() = at.x()+newLen*frontAxis.x()+newLen*rightAxis.x()+newLen*topAxis.x();
				eye.y() = at.y()+newLen*frontAxis.y()+newLen*rightAxis.y()+newLen*topAxis.y();
				eye.z() = at.z()+newLen*frontAxis.z()+newLen*rightAxis.z()+newLen*topAxis.z();

				up = topAxis;
				rend->camera()->setViewMatrixLookAt(eye, at,up);
			}
			break;
		default: break;
		}
	}
}

void VLBaseView::keyPressEvent( unsigned short, EKey key)
{
}

void VLBaseView::initEvent()
{
	Rendering* rend = cast<Rendering>(rendering());
	if (rend)
	{
		if (m_bPerspective)
		{
			rend->camera()->setProjectionPerspective();
		}
		else
		{
			rend->camera()->setProjectionOrtho();
		}
	}
}

void VLBaseView::SetOldBufferFrameSize(int cx,int cy)
{
	OrthographicTrackballManipulator* pOrthoTM = dynamic_cast<OrthographicTrackballManipulator*>(trackball());
	if (pOrthoTM)
	{
		pOrthoTM->SetOldBufferFrameSize(cx,cy);
	}

	PerspectiveTrackballManipulator* pPerspTM =  dynamic_cast<PerspectiveTrackballManipulator*>(trackball());
	if (pPerspTM)
	{
		pPerspTM->SetOldBufferFrameSize(cx,cy);
	}
}
//-----------------------------------------------------------------------------
