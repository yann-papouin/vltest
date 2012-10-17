#ifndef OrthographicTrackballManipulator_INCLUDE_ONCE
#define OrthographicTrackballManipulator_INCLUDE_ONCE

#include <vlGraphics/TrackballManipulator.hpp>

class VLBaseView;
using namespace vl;

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

	float GetZoomFactor() const { return mZoomFactor; }
	void SetZoomFactor(float val) { mZoomFactor = val; }
	void SetOldBufferFrameSize(int cx,int cy){mOldSizeX = cx,mOldSizeY=cy;};

private:
	VLBaseView* m_pVLBaseView;
	Transform* mIntersectionPoint;
	Actor* mCrossActor;
	bool m_bShift;
	float mZoomFactor;

	vl::mat4 mProjMatrixBeforePan;
	//int _x;
	//int _y;
	int mOldSizeX;
	int mOldSizeY;
};
#endif
