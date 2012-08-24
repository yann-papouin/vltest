#ifndef VLBASEVIEW_INCLUDE_ONCE
#define VLBASEVIEW_INCLUDE_ONCE

#include <vlCore/Transform.hpp>

#include <vlGraphics/UIEventListener.hpp>
#include <vlGraphics/TrackballManipulator.hpp>
#include <vlGraphics/GhostCameraManipulator.hpp>
#include <vlGraphics/SceneManagerActorTree.hpp>
#include <vlGraphics/Rendering.hpp>
#include <vlGraphics/ReadPixels.hpp>
#include <vlGraphics/EdgeRenderer.hpp>
#include <vlGraphics/Effect.hpp>

#include <Renderer/vl/OrthographicTrackballManipulator.hpp>
#include <Renderer/vl/PerspectiveTrackballManipulator.hpp>

using namespace vl;

//-----------------------------------------------------------------------------
// VLBaseView
//-----------------------------------------------------------------------------
enum ViewMode 
{
	ViewNone,     //!< none view mode
	ViewXY,  //!< self-explanatory
	ViewYX,  //!< self-explanatory
	ViewXZ,  //!< self-explanatory
	ViewZX,  //!< self-explanatory
	ViewYZ,  //!< self-explanatory
	ViewZY,  //!< self-explanatory
	ViewIso  //!< self-explanatory
};

enum RenderMode
{
	RenderWireframe = 1,       //!< edges/lines only
	RenderGouraud = 2,         //!< gouraud (smooth) shading
	RenderHiddenLine = 3,      //!< hidden line removal
	RenderWireframeWithSilhouette =4,
	RenderTriangulation = 5,   //!< edges and non-lit faces
	RenderGouraudWithEdges = 6,//!< gouraud (smooth) shading with edges turned on
	RenderPhong = 7,          //!< phong shading
	RenderPhongWithEdges = 8,          //!< phong shading with edges  turned on
	RenderUnkonwn = 0
};

class VLBaseView: public UIEventListener
{
VL_INSTRUMENT_CLASS(VLBaseView, UIEventListener)

public:
	/** Constructor */
	VLBaseView();

	/** 
	 * Initializes the default rendering (with Rendering), the default scene manager (with SceneManagerActorTree) 
	 * and camera manipulators (GhostCameraManipulator and TrackballManipulator). 
	 */
	void initialize(vl::Framebuffer* frameBuffer);

	// --- UIEventListener ---

	virtual void addedListenerEvent(OpenGLContext* openglContext);

	virtual void removedListenerEvent(OpenGLContext*);

	virtual void keyReleaseEvent(unsigned short, EKey key);

	virtual void destroyEvent();

	virtual void updateEvent();

	virtual void resizeEvent(int, int);

	virtual void initEvent();;

	virtual void enableEvent(bool) {}

	virtual void mouseMoveEvent(int, int) {}

	virtual void mouseUpEvent(EMouseButton, int, int){};

	virtual void mouseDownEvent(EMouseButton, int, int){};

	virtual void mouseWheelEvent(int) {}

	virtual void keyPressEvent(unsigned short, EKey);

	virtual void fileDroppedEvent(const std::vector<String>&) {}

	virtual void visibilityEvent(bool) {}

	// --- --- ---

	/** The rendering used by the MainApplet, by default a Rendering. */
	RenderingAbstract* rendering() { return mRendering.get(); }

	/** The rendering used by the MainApplet, by default a Rendering. */
	const RenderingAbstract* rendering() const { return mRendering.get(); }

	/** Sets the rendering used by the MainApplet, by default a Rendering. */
	void setRendering(RenderingAbstract* rendering) { mRendering = rendering; }

	/** The scene manager used by the default rendering. */
	SceneManagerActorTree* sceneManager() { return mSceneManagerActorTree.get(); }

	/** The scene manager used by the default rendering. */
	const SceneManagerActorTree* sceneManager() const { return mSceneManagerActorTree.get(); }

	/** GhostCameraManipulator used by the MainApplet, activated by the "F" key. */
	void setGhostCameraManipulator(GhostCameraManipulator* gcm) { mFly = gcm; }

	/** GhostCameraManipulator used by the MainApplet, activated by the "F" key. */
	GhostCameraManipulator* ghostCameraManipulator() { return mFly.get(); }
	const GhostCameraManipulator* ghostCameraManipulator() const { return mFly.get(); }

	/** TrackballManipulator used by the MainApplet, activated by the "T" key. */
	void setTrackball(TrackballManipulator* trackball) { mTrackball = trackball; }

	/** TrackballManipulator used by the MainApplet, activated by the "T" key. */
	TrackballManipulator* trackball() { return mTrackball.get(); }
	const TrackballManipulator* trackball() const { return mTrackball.get(); }

	/** Current average frames per second (updated every 500ms). */
	double fps() const { return mFPS; }

	/** 
	 * Override this to update the content of your scene. 
	 * Called by updateEvent() right before rendering()->render() and swapping opengl front/back buffers. 
	 * \note Since updateScene() is called by updateEvent() this function is called only if somebody
	 * requests a OpenGLContext::update() or if OpenGLContext::continuousUpdate() is set to \p true. 
	 */
	virtual void updateScene() {}

	/** Sets the MainApplet name, used for the window title and for naming screenshots. */
	void setMainAppletName(const String& app_name) { mMainAppletName = app_name; } 

	/** The MainApplet name, used for the window title and for naming screenshots. */
	const String& mainAppletName() const { return mMainAppletName; }

	bool perspective() const { return m_bPerspective; }
	void setPerspective(bool val) { m_bPerspective = val; }
	void setOldBufferFrameSize(int cx,int cy);
	void fitWorld();
	void flush(bool bUpdate = false);
	void setViewMode(ViewMode eViewMode,bool bFitWorld = true);
	void setRenderMode(RenderMode eRenderMode);
	void loadResource( const std::string& strPathName );
	void makeBox();
	vl::ref<vl::Effect> mEffect;

protected:
	void bindManipulators(Camera* camera);

private:
	ref<RenderingAbstract> mRendering;
	ref<GhostCameraManipulator> mFly;
	ref<TrackballManipulator> mTrackball;
	ref<SceneManagerActorTree> mSceneManagerActorTree;
	ref<ReadPixels> mReadPixels;


	String mMainAppletName;
	double mStartTime;
	double mFPS;
	int mFrameCount;

	ref<Renderer> mSolidRenderer;
	ref<EdgeRenderer> mEdgeRenderer;
	RenderMode mRenderMode;

	bool m_bPerspective;
	ViewMode mViewMode;
};
#endif
