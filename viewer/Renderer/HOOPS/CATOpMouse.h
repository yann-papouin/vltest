/*******************************************************************/
/*    Copyright (c) 2003-2008 by Spatial Corp.                     */
/*******************************************************************/

#ifndef _AH_OP_MOUSE_H_
#define _AH_OP_MOUSE_H_

/**
 * @CAA2Level L1
 * @CAA2Usage U1
 */

#include "HBaseOperator.h"

class HPoint;
class HEventInfo;
class HBaseView;

/**
 * Class CATOpMouse.
 *<br>
 *<b>Role</b>: This class is used to camera operation.
 */
class CATOpMouse : public HBaseOperator
{
public:
/**
 * Constructs a CATOpMouse.
 */
	CATOpMouse( HBaseView* pBaseView);

/**
 * @nodoc
 */
	~CATOpMouse(){};

protected:
/**
 * @nodoc
 */
	virtual const char* GetName(){return "CATOpMouse";};

/**
 * @nodoc
 */
	virtual int OnLButtonDown(HEventInfo& event);

/**
 * @nodoc
 */
	virtual int OnRButtonDown(HEventInfo& event);

/**
 * @nodoc
 */
	virtual int OnMButtonDown(HEventInfo& event);

/**
 * @nodoc
 */
	virtual int OnLButtonUp(HEventInfo& event);

/**
 * @nodoc
 */
	virtual int OnRButtonUp(HEventInfo& event);

/**
 * @nodoc
 */
	virtual int OnMButtonUp(HEventInfo& event);

/**
 * @nodoc
 */
	virtual int OnLButtonDownAndMove(HEventInfo& event){ return (HOP_OK);};

/**
 * @nodoc
 */
	virtual int OnRButtonDownAndMove(HEventInfo& event){ return (HOP_OK);};

/**
 * @nodoc
 */
	virtual int OnMouseMove(HEventInfo& event);

/**
 * @nodoc
 */
	int init_rotation(HEventInfo& event);

/**
 * @nodoc
 */
	int	exit_rotation(HEventInfo& event);

/**
 * @nodoc
 */
	int	init_zoom(HEventInfo& event);

/**
 * @nodoc
 */
	int exit_zoom(HEventInfo& event);

/**
 * @nodoc
 */
	int	init_pan(HEventInfo& event);

/**
 * @nodoc
 */
	int exit_pan(HEventInfo& event);

/**
 * @nodoc
 */
	int	rotate_camera(HEventInfo& event);

/**
 * @nodoc
 */
	int	rotate_camera_z_axis(HEventInfo& event);

/**
 * @nodoc
 */
	int	zoom_camera(HEventInfo& event);

/**
 * @nodoc
 */
	int	pan_camera(HEventInfo& event);

/**
 * @nodoc
 */
	int create_origin();

/**
 * @nodoc
 */
	int create_cross(HEventInfo& event); 

/**
 * @nodoc
 */
	int	delete_origin();

/**
 * @nodoc
 */
	int	delete_cross();

/**
 * @nodoc
 */
	int center_camera(HEventInfo& event);

/**
 * @nodoc
 */
	int create_circle(HEventInfo& event);

/**
 * @nodoc
 */
	int	delete_circle();

/**
 * @nodoc
 */
	bool get_position_on_object(HPoint& localwindow, HPoint& exactworld);

protected:
	HBaseView* m_pHBaseView;
	bool m_bMouseMoved;

private:
	enum ahCamMode{ AH_CAM_ROTATE, AH_CAM_ZOOM, AH_CAM_PAN, AH_CAM_UNKNOWN};
	ahCamMode	m_eMode;
	HPoint	m_ptDist, m_ptCamera;
	float	m_Width, m_Height, m_fLength;
	float	m_Angle2, m_Angle1, m_Angle3;
	HPoint	m_ptRealOld;
	HPoint	m_ptRealFirst;
	HPoint	m_oldMousePos;	// in pixel space
	HPoint	m_ptOnSpherePrevious;
	bool	m_bExistenceCross;

public:
	bool	m_bVisibleSphere;

};

#endif