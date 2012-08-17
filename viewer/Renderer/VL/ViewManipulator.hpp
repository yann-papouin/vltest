#ifndef ViewManipulator_INCLUDE_ONCE
#define ViewManipulator_INCLUDE_ONCE

#include <vlGraphics/UIEventListener.hpp>
#include <vlGraphics/Camera.hpp>
#include <vlCore/Vector3.hpp>
#include <vlGraphics/TrackballManipulator.hpp>
#include <vlGraphics/Applet.hpp>

class VLBaseView;


//------------------------------------------------------------------------------
// TrackballManipulator
//------------------------------------------------------------------------------
/**
* This class lets you rotate a Camera or a Transform node using a virtual trackball.
* If you set a Transform node to manipulate, using the function setTransform(), the trackball
* will manipulate the given Transform (rotation only, panning and zooming will always affect 
* the bound Camera). If no Transform is specified or a NULL one is passed to the function 
* setTransform() then the trackball will manipulate the current camera.
* \note In any case, before using a TrackballManipulator you have to specify a Camera object 
* using the function setCamera().
* \note The Transform is expected to contain only rotation and translation information. 
* Other transformations like shearing, scaling, projection, and so on can produce unspecified results.
* \sa GhostCameraManipulator
*/
using namespace vl;

class PerspectiveTrackballManipulator: public vl::TrackballManipulator
{
	VL_INSTRUMENT_CLASS(PerspectiveTrackballManipulator, vl::TrackballManipulator)
public:
	//! Constructor.
	PerspectiveTrackballManipulator(VLBaseView* pVLBaseView);

	// --- UIEventListener ---
	virtual void mouseDownEvent(EMouseButton, int x, int y);
	virtual void mouseUpEvent(EMouseButton, int x, int y);
	virtual void mouseMoveEvent(int x, int y);
	virtual void mouseWheelEvent(int) {}
private:
	Transform* mIntersectionPoint;
	VLBaseView* m_pVLBaseView;
	Actor* mCrossActor;
};

class OrthographicTrackballManipulator : public vl::TrackballManipulator
{
	VL_INSTRUMENT_CLASS(OrthographicTrackballManipulator, vl::TrackballManipulator)
public:
	//! Constructor.
	OrthographicTrackballManipulator(VLBaseView* pVLBaseView);

	// --- UIEventListener ---
	virtual void mouseDownEvent(EMouseButton, int x, int y);
	virtual void mouseUpEvent(EMouseButton, int x, int y);
	  virtual void mouseMoveEvent(int x, int y);
	  virtual void mouseWheelEvent(int n);
	  virtual void resizeEvent(int w, int h);
	  virtual void keyPressEvent( unsigned short, EKey key);
	  virtual void keyReleaseEvent( unsigned short, EKey key);
	  void setProjection();
	  int GetZoomFactor() const { return _zoomFactor; }
	  void SetZoomFactor(int val) { _zoomFactor = val; }

	  vl::mat4 mOldProjMatrix;

private:
	int _zoomFactor;
	Transform* mIntersectionPoint;
	VLBaseView* m_pVLBaseView;
	Actor* mCrossActor;
	bool m_bShift;

	int _x;
	int _y;
};


#endif
