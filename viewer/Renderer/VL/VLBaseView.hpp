#ifndef VLBASEVIEW_INCLUDE_ONCE
#define VLBASEVIEW_INCLUDE_ONCE

#include <vlGraphics/UIEventListener.hpp>
#include <vlGraphics/TrackballManipulator.hpp>
#include <vlGraphics/GhostCameraManipulator.hpp>
#include <vlGraphics/SceneManagerActorTree.hpp>
#include <vlGraphics/Rendering.hpp>
#include <vlGraphics/ReadPixels.hpp>
#include <vlGraphics/EdgeRenderer.hpp>

#include <Renderer/vl/ViewManipulator.hpp>

using namespace vl;

//-----------------------------------------------------------------------------
// MainApplet
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
	RenderShaded,
	RenderShadedWithLines,
	RenderWireframe
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
	void initialize();

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
	const String& MainAppletName() const { return mMainAppletName; }

	bool GetPerspective() const { return m_bPerspective; }
	void SetPerspective(bool val) { m_bPerspective = val; }

	void FitWorld();
	void SetViewMode(ViewMode eViewMode,bool bFitWorld = true);
	void SetRenderMode(RenderMode eRenderMode);

	void SetOldBufferFrameSize(int cx,int cy);

protected:
	void bindManipulators(Camera* camera);

private:
	ref<RenderingAbstract> mRendering;
	ref<Renderer> mSolidRenderer;
	ref< EdgeRenderer > mEdgeRenderer;

	ref<GhostCameraManipulator> mFly;
	ref<TrackballManipulator> mTrackball;
	ref<SceneManagerActorTree> mSceneManagerActorTree;
	ref<ReadPixels> mReadPixels;
	String mMainAppletName;
	double mStartTime;
	double mFPS;
	int mFrameCount;
	bool m_bPerspective;
	ViewMode mViewMode;
	RenderMode mRenderMode;
};
#endif
