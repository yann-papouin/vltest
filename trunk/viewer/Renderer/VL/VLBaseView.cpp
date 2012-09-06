#include "stdafx.h"

#include <vlCore/Time.hpp>
#include <vlCore/Say.hpp>
#include <vlCore/Log.hpp>
#include <vlCore/VisualizationLibrary.hpp>
#include <vlcore/ResourceDatabase.hpp>

#include <vlGraphics/Rendering.hpp>
#include <vlGraphics/SceneManager.hpp>
#include <vlGraphics/RenderQueue.hpp>
#include <vlGraphics/FlatManipulator.hpp>
#include <vlGraphics/GeometryPrimitives.hpp>

#include "Renderer/VL/VLBaseView.hpp"
#include "Renderer/VL/AxisRenderer.hpp"

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
void VLBaseView::initialize(vl::Framebuffer* frameBuffer)
{
	// if the user didn't provide one use the one installed by default
	ref<Rendering> rend = rendering() && rendering()->as<Rendering>() ? rendering()->as<Rendering>() : new Rendering;
	setRendering(rend.get());

	/* set background */
	rend->camera()->viewport()->setClearColor( vl::gray );

	ref<vl::Transform> mTransform = new vl::Transform;
	rend->setTransform(mTransform.get());

	mEffect = new vl::Effect;
	mEffect->shader()->enable(vl::EN_DEPTH_TEST);
	// add a Light to the scene, since no Transform is associated to the Light it will follow the camera 

	vl::ref<vl::Light> light = new vl::Light;
	// since the fourth component is 0 OpenGL considers this a direction
	light->setPosition(vl::fvec4(0, 0, 1,0)); 

	mEffect->shader()->setRenderState( light.get(), 0 );
	// enable the standard OpenGL lighting 
	mEffect->shader()->enable(vl::EN_LIGHTING);

	// "gocMaterial" stands for "get-or-create Material"
	// set the front and back material color 
	mEffect->shader()->gocLightModel()->setTwoSide(true);
	//mEffect->shader()->gocMaterial()->setDiffuse( vl::green );
	mEffect->shader()->gocMaterial()->setBackDiffuse(vl::red);
//	mEffect->shader()->gocShadeModel()->set(SM_FLAT);

	/*solid renderer*/
	mSolidRenderer = rend->renderer();
	mSolidRenderer->setFramebuffer(frameBuffer);

	/*edge renderer*/
	mEdgeRenderer = new EdgeRenderer;

	mEdgeRenderer->setClearFlags(CF_CLEAR_DEPTH);
	// target the same opengl window
	mEdgeRenderer->setFramebuffer(mSolidRenderer->framebuffer());
	// enqueue the EdgeRenderer in the rendering, will be executed after mSolidRenderer

	// hidden line and crease options
	mEdgeRenderer->setShowHiddenLines(true);
	mEdgeRenderer->setShowCreases(true);

	// style options
	mEdgeRenderer->setLineWidth(1.0f);
	mEdgeRenderer->setSmoothLines(true);
	mEdgeRenderer->setDefaultLineColor(black);

	rend->renderers().push_back( mEdgeRenderer.get() );

	/*axis renderer*/
	ref< AxisRenderer > mAxisRenderer = new AxisRenderer;
	mAxisRenderer->setFramebuffer( mSolidRenderer->framebuffer());
	rend->renderers().push_back( mAxisRenderer.get() );

	/* set render mode */
	setRenderMode(RenderGouraudWithEdges);

	// installs a SceneManagerActorTree as the default scene manager
	mSceneManagerActorTree = new SceneManagerActorTree;
	rend->sceneManagers()->push_back(sceneManager());

	mFly = new GhostCameraManipulator;
	mFly->setEnabled(false);

	if (m_bPerspective == true)
	{
		mTrackball = new PerspectiveTrackballManipulator(this);
	}
	else
	{
		mTrackball = new OrthographicTrackballManipulator(this);
	}
	mTrackball->setEnabled(true);

	bindManipulators( rend->camera() );
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
						String filename = Say( mainAppletName() + " - %n%02n%02n%02n%02n.png") << time.year() << time.month() << time.dayOfMonth() << time.hour() << time.second();
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
	}
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
void VLBaseView::keyPressEvent( unsigned short, EKey key)
{
}
//-----------------------------------------------------------------------------
void VLBaseView::destroyEvent()
{
	mFly->setCamera(NULL);
	mTrackball->setCamera(NULL);
	mTrackball->setTransform(NULL);
	mRendering = NULL;
}

void VLBaseView::fitWorld()
{
	Rendering* rend = cast<Rendering>(rendering());
	if (rend)
	{
		sceneManager()->setBoundsDirty(true);
		sceneManager()->computeBounds();
		const vl::Sphere bs = sceneManager()->boundingSphere();
		vl::vec3 new_at = bs.center();
		mTrackball->setPivot(new_at);


		// 8/24/2012 mwu :  
		//vl::vec3 eye,at,up,right;
		//rend->camera()->getViewMatrixAsLookAt(eye,at,up,right);
		//mTrackball->adjustView(sceneManager(),at,up);

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
			rend->camera()->setProjectionMatrix(
// 				// compute directly an orthographic projection & center the scene on the screen
// 				rend->camera()->projectionMatrix() *
// 				vl::mat4::getTranslation(w / 2.0f, h / 2.0f, 0),PMT_PerspectiveProjection);
			vl::mat4::getOrtho(0, w, 0, h, nearPlane, farPlane) 
				* vl::mat4::getTranslation(w / 2.0f, h / 2.0f, 0) 
				,PMT_PerspectiveProjection) ;

		}
	}
}

void VLBaseView::setViewMode( ViewMode eViewMode,bool bFitWorld /*= true*/ )
{
	mViewMode = eViewMode;

	if (eViewMode == ViewNone) 
		return;

	if (bFitWorld)
	{
		fitWorld();
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
		length = 25.0f*bs.radius();//when it is small, some problems with hidden line and silhouette calculation

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

	openglContext()->update();
}

void VLBaseView::setOldBufferFrameSize(int cx,int cy)
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

void VLBaseView::setRenderMode( RenderMode eRenderMode )
{
	if(mEffect->lod(0)->size() > 1)
	{
		mEffect->lod(0)->pop_back();
	}

	switch(eRenderMode)
	{
	case RenderWireframe:
		mSolidRenderer->setEnableMask(0);
		mEdgeRenderer->setClearFlags(CF_CLEAR_COLOR_DEPTH);
		mEdgeRenderer->setEnableMask(0xFFFFFFFF);

		break;

	case RenderGouraud:
		mSolidRenderer->setEnableMask(0xFFFFFFFF);
		mEdgeRenderer->setEnableMask(0);

		break;

	case RenderHiddenLine:
		mSolidRenderer->setEnableMask(0);
		mEdgeRenderer->setClearFlags(CF_CLEAR_COLOR_DEPTH);
		mEdgeRenderer->setEnableMask(0xFFFFFFFF);
		mEdgeRenderer->setShowHiddenLines(false);

		break;

	case RenderWireframeWithSilhouette:
		mSolidRenderer->setEnableMask(0);
		mEdgeRenderer->setClearFlags(CF_CLEAR_COLOR_DEPTH);
		mEdgeRenderer->setEnableMask(0xFFFFFFFF);
		mEdgeRenderer->setShowHiddenLines(true);

		break;

	case RenderTriangulation:

		/* display mesh */
#if defined(VL_OPENGL)
		mEffect->lod(0)->push_back( new Shader );
		mEffect->shader(0,1)->setRenderState( new Light, 0 );
		mEffect->shader(0,1)->enable(EN_LIGHTING);
		mEffect->shader(0,1)->gocMaterial()->setDiffuse(vl::blue);
		mEffect->shader(0,1)->gocPolygonMode()->set(vl::PM_LINE, vl::PM_LINE);
		mEffect->shader(0,1)->gocPolygonOffset()->set(-1.0f, 0);
		mEffect->shader(0,1)->enable(vl::EN_POLYGON_OFFSET_LINE);
		mEffect->shader(0,1)->enable(EN_DEPTH_TEST);
#endif

		mSolidRenderer->setEnableMask(0xFFFFFFFF);
		mEdgeRenderer->setEnableMask(0);

		break;

	case RenderGouraudWithEdges:
		mSolidRenderer->setEnableMask(0xFFFFFFFF);
		/*edge renderer*/
		mEdgeRenderer->setClearFlags(CF_CLEAR_DEPTH);
		mEdgeRenderer->setEnableMask(0xFFFFFFFF);
		mEdgeRenderer->setShowHiddenLines(false);

		break;

	case RenderPhong:
		mSolidRenderer->setEnableMask(0xFFFFFFFF);
		mEdgeRenderer->setEnableMask(0);

		break;

	case RenderPhongWithEdges:
		mSolidRenderer->setEnableMask(0xFFFFFFFF);
		mEdgeRenderer->setEnableMask(0);

		break;

		break;
	default:
	    break;
	}
}

//clear all actors
void VLBaseView::flush( bool bUpdate )
{
	ActorCollection actors;
	sceneManager()->tree()->extractActors(actors);

	for (int i=0;i<actors.size();i++)
	{
		sceneManager()->tree()->eraseActor(actors[i].get());	
	}

	if (bUpdate)
	{
		openglContext()->update();
	}
}

void VLBaseView::loadFile(const std::string& strPathName )
{
	// resets the scene
	sceneManager()->tree()->actors()->clear();

	ref<ResourceDatabase> resource_db = vl::loadResource(strPathName.c_str());
	if (!resource_db)
		return;

	std::vector< ref<Actor> > actors;
	resource_db->get<Actor>(actors);
	for(unsigned i=0; i<actors.size(); ++i)
	{
		ref<Actor> actor = actors[i].get();

		// compute normals
		vl::ref<vl::Geometry> geom = cast<vl::Geometry>(actor->lod(0));
		if (geom)
		{
			geom->computeNormals();
		}

		// define a reasonable Shader
		actor->effect()->shader()->setRenderState( new Light, 0 );
		actor->effect()->shader()->enable(EN_DEPTH_TEST);
		actor->effect()->shader()->enable(EN_LIGHTING);
		actor->effect()->shader()->gocLightModel()->setTwoSide(true);
		// add the actor to the scene
		sceneManager()->tree()->addActor( actor.get() );
	}

	setViewMode(ViewIso,true);

}

void VLBaseView::makeBox()
{
//	vl::ref<vl::Geometry> geom = vl::makeBox( vl::vec3(0,0,0),  vl::vec3(10,10,10), true);

	vl::ref<vl::Geometry> geom = vl::makeCone(vec3(0,0,0),10,20)  ;

	// compute normals
	geom->computeNormals();

	sceneManager()->tree()->addActor( geom.get(), mEffect.get());
}

void VLBaseView::fileDroppedEvent( const std::vector<String>& files )
{
	// resets the scene
	sceneManager()->tree()->actors()->clear();
	// resets the EdgeRenderer cache
	mEdgeRenderer->clearCache();

	for(unsigned int i=0; i<files.size(); ++i)
	{
		ref<ResourceDatabase> resource_db = loadResource(files[i],true);

		if (!resource_db || resource_db->count<Actor>() == 0)
		{
			Log::error("No data found.\n");
			continue;
		}

		std::vector< ref<Actor> > actors;
		resource_db->get<Actor>(actors);
		for(unsigned i=0; i<actors.size(); ++i)
		{
			ref<Actor> actor = actors[i].get();

			// compute normals
			vl::ref<vl::Geometry> geom = cast<vl::Geometry>(actor->lod(0));
			if (geom)
			{
				geom->computeNormals();
			}
			// define a reasonable Shader
			actor->effect()->shader()->setRenderState( new Light, 0 );
			actor->effect()->shader()->enable(EN_DEPTH_TEST);
			actor->effect()->shader()->enable(EN_LIGHTING);
			actor->effect()->shader()->gocLightModel()->setTwoSide(true);
			// add the actor to the scene
			sceneManager()->tree()->addActor( actor.get() );
		}
	}
	setViewMode(ViewIso,true);
}
//-----------------------------------------------------------------------------
