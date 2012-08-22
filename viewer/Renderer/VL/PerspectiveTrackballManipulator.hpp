#ifndef PerspectiveTrackballManipulator_INCLUDE_ONCE
#define PerspectiveTrackballManipulator_INCLUDE_ONCE

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
	void SetOldBufferFrameSize(int cx,int cy){mOldSizeX = cx,mOldSizeY=cy;};

private:
	Transform* mIntersectionPoint;
	VLBaseView* m_pVLBaseView;
	Actor* mCrossActor;

	int mOldSizeX;
	int mOldSizeY;
};

#endif
