#ifndef PerspectiveTrackballManipulator_INCLUDE_ONCE
#define PerspectiveTrackballManipulator_INCLUDE_ONCE

#include <vlGraphics/TrackballManipulator.hpp>

class VLBaseView;
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
