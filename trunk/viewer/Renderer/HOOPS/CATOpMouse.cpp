/*******************************************************************/
/*    Copyright (c) 2003-2008 by Spatial Corp.                     */
/*******************************************************************/
#include "StdAfx.h"

#include "math.h"

//---------- HOOPS ----------
#include "HBaseOperator.h"
#include "HBaseView.h"
#include "HEventInfo.h"
#include "HOpCameraOrbit.h"
#include "HOpCameraPan.h"
#include "HOpCameraZoom.h" 

#include "CATOpMouse.h"


CATOpMouse::CATOpMouse( HBaseView* pHBaseView) : HBaseOperator( pHBaseView )
{
	m_ptDist.Set(0,0,0);
	m_ptCamera.Set(0,0,0);
	m_Width		= 0.0;
	m_Height	= 0.0;
	m_fLength	= 0.0;
	m_Angle2	= 0.0;
	m_Angle1	= 0.0;
	m_Angle3	= 0.0;
	m_eMode		= AH_CAM_UNKNOWN;
	m_bMouseMoved = false;
	m_oldMousePos.Set(0,0,0);
	m_ptOnSpherePrevious.x	= 0.0;
	m_ptOnSpherePrevious.y	= 0.0;
	m_ptOnSpherePrevious.z	= 0.0;
	m_bExistenceCross	= false;
	m_bVisibleSphere	= true;
};

int CATOpMouse::OnMouseMove(HEventInfo& event)
{
	HPoint ptEventMousePixelPos = event.GetMousePixelPos();
	HPoint ptdMousPos( m_oldMousePos.x - ptEventMousePixelPos.x, m_oldMousePos.y - ptEventMousePixelPos.y, m_oldMousePos.z - ptEventMousePixelPos.z );
	double dDist = HC_Compute_Vector_Length( &ptdMousPos );
	if (dDist < 3.0f)
	{
		return (HOP_OK);
	}

	m_bMouseMoved = true;
	if (m_eMode == AH_CAM_ROTATE )
	{
		rotate_camera(event);
	}
	else if (m_eMode == AH_CAM_ROTATE)
	{
		rotate_camera_z_axis(event);
	}
	else if (m_eMode == AH_CAM_ZOOM )
	{
		zoom_camera(event);
	}
	else if (m_eMode == AH_CAM_PAN )
	{
		pan_camera(event);
	}
	else if (event.LButton())
	{
		OnLButtonDownAndMove(event);
	}
	else if (event.RButton())
	{
		OnRButtonDownAndMove(event);
	}
	return (HOP_OK);
}

int CATOpMouse::OnMButtonDown(HEventInfo& event)
{
	HPoint ptMousePixelPos = event.GetMousePixelPos();
	m_oldMousePos.Set( ptMousePixelPos.x, ptMousePixelPos.y, ptMousePixelPos.z );

	m_bMouseMoved = false;// 7/8/2011 mwu :

	return init_pan(event);
}

int CATOpMouse::OnMButtonUp(HEventInfo& event)
{
	int retval = 0;

	if (m_eMode == AH_CAM_ROTATE)
	{
		retval = exit_rotation(event);
	}
	else if (m_eMode == AH_CAM_ZOOM)
	{
		retval = exit_zoom(event);
	}
	else if (m_eMode == AH_CAM_PAN)
	{
 		if ( m_bMouseMoved != true )
		{
			center_camera(event);
		}
	}

	retval = exit_pan(event);

	HC_Update_Display();
	// 02/12/2008 Uehara : use AGM_UPDATE_DISPLAY to check trace
//??	AGM_UPDATE_DISPLAY;

	// 04/07/2008 request to update sheape

	m_bMouseMoved = false;

	return retval;
}

int CATOpMouse::OnLButtonDown(HEventInfo& event)
{
	int retval = 0;
	if (event.MButton())
	{
		retval = init_rotation(event);
	}

	return retval;
}

int CATOpMouse::OnLButtonUp(HEventInfo& event)
{
	int retval = 0;
	if (event.MButton())
	{
		retval = exit_rotation(event);

		// 02/07/2008 Uehara : After other mouse operations are done, not execute zoom.
		//if ( !m_bMouseMoved )// 08/06/2009 modified by mwu : 
		{
			retval = init_zoom(event);
		}
		//01/30/2008 Uehara:It is necessary to call InitPan here
		//retval = InitPan(event);
	}

	// 10/24/2005 Bashi add to refresh view
	HC_Update_Display();
	// 02/12/2008 Uehara : use AGM_UPDATE_DISPLAY to check trace
	//AGM_UPDATE_DISPLAY;
	// 04/07/2008 Uehara : request to update view

	return retval;
}

int CATOpMouse::OnRButtonDown(HEventInfo& event)
{
	int retval = 0;
	if (event.MButton())
	{
		retval = init_rotation(event);
	}

	return retval;
}

int CATOpMouse::OnRButtonUp(HEventInfo& event)
{
	int retval = 0;

	if (event.MButton())
	{
		retval = exit_rotation(event);

		// 02/07/2008 Uehara : After other mouse operations are done, not execute zoom.
		//if ( !m_bMouseMoved )// 08/06/2009 modified by mwu : 
		{
			retval = init_zoom(event);
		}
		//01/30/2008 Uehara:It is necessary to call InitPan here
		//retval = InitPan(event);
	}

	// 04/07/2008 Uehara : request to update view
	HC_Update_Display();

	return retval;
}

// =================================================
// Initialization and Termination
// =================================================
int CATOpMouse::init_rotation(HEventInfo& event)
{

	m_bExistenceCross = false;
	float fSphere, fInitRotLength;
	m_eMode = AH_CAM_ROTATE;
	m_bOpStarted = true;

	// read mouse position
	m_ptFirst = event.GetMouseWindowPos();
	
	// this is simply used to determine whether or not the mouse
	// moved buring it's interaction
	m_ptRealFirst = event.GetMouseWorldPos();
	
	// remember screen mouse position
	m_ptRealOld.x = m_ptFirst.x;
	m_ptRealOld.y = m_ptFirst.y;
	
	// map screen mouse points to sphere mouse points
	fSphere = (m_ptFirst.x * m_ptFirst.x + m_ptFirst.y * m_ptFirst.y);
	fInitRotLength = (float)sqrt(fSphere);
	if (fInitRotLength > 1.0f) 
	{
		m_ptFirst.x /= fInitRotLength;
		m_ptFirst.y /= fInitRotLength;
		m_ptFirst.z = 0.0;
	}
	else
		m_ptFirst.z = (float) sqrt (1.0f - fSphere);

	create_circle(event);
	create_cross(event);

	HC_Update_Display();
	// 02/12/2008 Uehara : use AGM_UPDATE_DISPLAY to check trace
	//AGM_UPDATE_DISPLAY;

	return (HOP_OK);
}

int CATOpMouse::exit_rotation(HEventInfo& event)
{
	delete_circle();
	delete_cross();
	// 02/07/2008 Uehara Add
	m_eMode = AH_CAM_UNKNOWN;

	return (HOP_OK);
}

int CATOpMouse::init_zoom(HEventInfo& event)
{
	m_eMode = AH_CAM_ZOOM;
	HPoint ptZoomTarget;

	m_bOpStarted = true;
	m_ptNew = event.GetMouseWindowPos();

	m_ptFirst.x = m_ptNew.x;
	m_ptFirst.y = m_ptNew.y;
	m_ptFirst.z = m_ptNew.z;

 	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		// 02/08/2004 Bashi - Add: Enable LOD
		// 01/05/2009 Spatial: discontinue using LOD
//##		HC_Set_Rendering_Options( "lod=on, lodo = (clamp = 1,fallback = coarsest)" );
		HC_Show_Net_Camera_Target(&m_ptCamera.x,&m_ptCamera.y,&m_ptCamera.z);
		HC_Show_Net_Camera_Position(&ptZoomTarget.x,&ptZoomTarget.y,&ptZoomTarget.z);	
		m_ptDist.Set(ptZoomTarget.x-m_ptCamera.x, ptZoomTarget.y - m_ptCamera.y, ptZoomTarget.z - m_ptCamera.z);
		m_fLength = (float)HC_Compute_Vector_Length(&m_ptDist);
		HC_Compute_Normalized_Vector(&m_ptDist,&m_ptDist);
		HC_Show_Net_Camera_Field(&m_Width, &m_Height);
	HC_Close_Segment();

 	return (HOP_OK);
}

int CATOpMouse::exit_zoom(HEventInfo& event)
{
	// 09/05/2006 Bashi
	//if ( m_pAHBaseView != NULL )
	//{
	//	m_pAHBaseView->UpdateFaceDisplacement();
	//}

    return(HOP_READY);
}

int CATOpMouse::init_pan(HEventInfo& event)
{
	m_eMode = AH_CAM_PAN;
	m_bOpStarted = true;

    m_ptNew = event.GetMouseWorldPos(); 
									
    m_ptFirst.x = m_ptNew.x;
    m_ptFirst.y = m_ptNew.y;
    m_ptFirst.z = m_ptNew.z;
 
	// 02/08/2004 Bashi - Add: Enable LOD
	// 01/05/2009 Spatial: discontinue using LOD
	//HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	//	HC_Set_Rendering_Options( "lod=on, lodo = (clamp = 1,fallback = coarsest)" );
	//HC_Close_Segment();

	create_origin();

    return (HOP_OK);
};

int CATOpMouse::exit_pan(HEventInfo& event)
{
	delete_origin();

	// 02/08/2004 Bashi - Add: Enable LOD
//	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
//		HC_Set_Rendering_Options( "lod=on, lodo = (clamp = 0)" );
//	HC_Close_Segment();

 	m_bOpStarted = false;
	m_pView->CameraPositionChanged(true, true);
	m_eMode = AH_CAM_UNKNOWN;

	HC_Update_Display();
	// 02/12/2008 Uehara : use AGM_UPDATE_DISPLAY to check trace
//??	AGM_UPDATE_DISPLAY;

    return(HOP_READY);
};

//============================================
// Camera Manipulation
//============================================
int CATOpMouse::rotate_camera(HEventInfo& event)
{
	if (m_eMode != AH_CAM_ROTATE) return -1;

	create_cross(event);

	m_pView->Update();
	return (HOP_OK);
}

//============================================
// Oribit Camrema around Z Axis
//============================================
#include "wcs_utl.hxx"
#include "geom_utl.hxx"
#include "acistol.hxx"

int CATOpMouse::rotate_camera_z_axis(HEventInfo& event)
{
	float	fRadius = (float) 0.9;	// Radius of the circle in the circle segment
	float	fApproachFactor = (float) 0.88;	// Factor, which is related with the radius to calculate the length of the arc 　　
	float	fLimitRadius = (float) 0.1;	// The factor regarding the limitation of the cross
	float	fFactor	= (float) 3.0;	// The factor regarding how much the cross approach
	float	fWight = (float) 0;
	float	fLengthMouseWindowPos = (float) 0;
	float	fProductByAcos = (float) 0;
	float	fProductNByAcos = (float) 0;
	float	fRadiusSin = (float) 0;
	float	fLengthTempM = (float) 0;
	float	fLengthOnSphere	 = (float) 0;
	float	fAngleForArbitraryAxis = (float) 0;
	float	fAngleFirstForRoll = (float) 0;
	float	fAngleFirstForOrbit = (float) 0;
	float	fdOnSphereA, fdOnSphereB, fdOnSphereC;

	HPoint	ptMouseWindowPos = event.GetMouseWindowPos();
	HPoint	ptWorldMouseWindowPos;
	HPoint	ptWorldMouseWindowPosWithFactor;
	HPoint	ptOnSphere, ptOnSphereStart, ptOnSphereEnd, ptOnSphereStartV, ptOnSphereEndV;
	HPoint	ptTemp, ptTemp2, ptTempM, ptTempN, ptTempWithFactor, ptTemp2WithFactor, ptProductM, ptProductN;
	HPoint	ptDividedByRadius, ptProductOnCircle;

	HC_Open_Segment_By_Key (m_pView->GetSceneKey());
		HC_Open_Segment("sub-figure");
			HC_Open_Segment("cross");
		//		HC_Set_Handedness("right");
				HC_Set_Camera_By_Volume ("orthographic", -1.0, 1.0, -1.0, 1.0);
				HC_Compute_Coordinates(".","local window", &ptMouseWindowPos, "world", &ptWorldMouseWindowPos);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	fLengthMouseWindowPos = (float) HC_Compute_Vector_Length(&ptWorldMouseWindowPos);

	if (fLengthMouseWindowPos < fRadius)
	{
		// The picked point is in the circle
		ptWorldMouseWindowPosWithFactor.x = ptWorldMouseWindowPos.x;
		ptWorldMouseWindowPosWithFactor.y = ptWorldMouseWindowPos.y;
		ptWorldMouseWindowPosWithFactor.z = ptWorldMouseWindowPos.z;
	}
	else
	{
		// The picked point is out of the circle
		if ( (1 + fFactor)*fRadius > fLengthMouseWindowPos + fLimitRadius)
		{
			// The more the picked point separates from the circle, the closer the position of the cross is
			fWight = (fRadius - ((fLengthMouseWindowPos - fRadius)/fFactor)) / fLengthMouseWindowPos;
		}
		else
		{
			// The point of the cross is fixed
			fWight = (float) (fRadius - ((fFactor *fRadius) - fLimitRadius )/fFactor)/((1 + fFactor)*fRadius - fLimitRadius);
		}
		ptWorldMouseWindowPosWithFactor.x = ptWorldMouseWindowPos.x * fWight;
		ptWorldMouseWindowPosWithFactor.y = ptWorldMouseWindowPos.y * fWight;
		ptWorldMouseWindowPosWithFactor.z = ptWorldMouseWindowPos.z * fWight;
	}

	// Calculate the length between the picked point and the center point
	float fLengthPosWithFactor = (float) HC_Compute_Vector_Length(&ptWorldMouseWindowPosWithFactor);

	if( fRadius < 0 )
	{
		return 0;
	}
	fProductByAcos = (float) acos(fLengthPosWithFactor/fRadius);

	ptOnSphere.x = ptWorldMouseWindowPosWithFactor.x;
	ptOnSphere.y = ptWorldMouseWindowPosWithFactor.y;
	ptOnSphere.z = (float) fRadius * ((float) sin(fProductByAcos));

	if ( m_bExistenceCross == false )
	{
		ptTemp.x = ptTemp.y = 0;
		ptTemp.z = (float) fRadius * ((float) sin(fProductByAcos));

		ptTemp2.x = ptWorldMouseWindowPosWithFactor.x;
		ptTemp2.y = ptWorldMouseWindowPosWithFactor.y;
		ptTemp2.z = 0;

		HC_Compute_Cross_Product(&ptTemp, &ptTemp2, &ptTempM);
		fLengthTempM = (float) HC_Compute_Vector_Length(&ptTempM);
		if( fLengthTempM < 0 )
		{
			return 0;
		}
		ptTempN.x = (float) ptTempM.x/fLengthTempM;
		ptTempN.y = (float) ptTempM.y/fLengthTempM;
		ptTempN.z = (float) ptTempM.z/fLengthTempM;

		if( fRadius < 0 )
		{
			return 0;
		}
		fProductNByAcos = (float) acos(fApproachFactor/fRadius);
		fRadiusSin = (float) fRadius * ((float) sin(fProductNByAcos));

		if( fRadius < 0 )
		{
			return 0;
		}
		ptDividedByRadius.x = fApproachFactor * ptOnSphere.x/fRadius;
		ptDividedByRadius.y = fApproachFactor * ptOnSphere.y/fRadius;
		ptDividedByRadius.z = fApproachFactor * ptOnSphere.z/fRadius;

		ptOnSphereStart.x = ptDividedByRadius.x + ptTempN.x * fRadiusSin;
		ptOnSphereStart.y = ptDividedByRadius.y + ptTempN.y * fRadiusSin;
		ptOnSphereStart.z = ptDividedByRadius.z + ptTempN.z * fRadiusSin;

		ptOnSphereEnd.x = ptDividedByRadius.x - ptTempN.x * fRadiusSin;
		ptOnSphereEnd.y = ptDividedByRadius.y - ptTempN.y * fRadiusSin;
		ptOnSphereEnd.z = ptDividedByRadius.z - ptTempN.z * fRadiusSin;		

		ptTempWithFactor.x = ptWorldMouseWindowPosWithFactor.x;
		ptTempWithFactor.y = ptWorldMouseWindowPosWithFactor.y;
		ptTempWithFactor.z = (float) fRadius * ((float) sin(fProductByAcos));

		ptTemp2WithFactor.x = - ptWorldMouseWindowPosWithFactor.y;
		ptTemp2WithFactor.y = + ptWorldMouseWindowPosWithFactor.x;
		ptTemp2WithFactor.z = 0;

		HC_Compute_Cross_Product(&ptTempWithFactor, &ptTemp2WithFactor, &ptProductM);
		float fLengthProductM = (float) HC_Compute_Vector_Length(&ptProductM);
		if( fLengthProductM < 0 )
		{
			return 0;
		}
		ptProductN.x = (float) ptProductM.x/fLengthProductM;
		ptProductN.y = (float) ptProductM.y/fLengthProductM;
		ptProductN.z = (float) ptProductM.z/fLengthProductM;

		ptOnSphereStartV.x = ptDividedByRadius.x + ptProductN.x * fRadiusSin;
		ptOnSphereStartV.y = ptDividedByRadius.y + ptProductN.y * fRadiusSin;
		ptOnSphereStartV.z = ptDividedByRadius.z + ptProductN.z * fRadiusSin;

		ptOnSphereEndV.x = ptDividedByRadius.x - ptProductN.x * fRadiusSin;
		ptOnSphereEndV.y = ptDividedByRadius.y - ptProductN.y * fRadiusSin;
		ptOnSphereEndV.z = ptDividedByRadius.z - ptProductN.z * fRadiusSin;		
	}

	if( fLengthMouseWindowPos < fRadius )
	{
		// The picked point is in the circle
		;
	}
	else
	{
		// The picked point is without the circle
		// Reverse the z coordinate value
		ptOnSphere.z =  -1 * ptOnSphere.z;
		if( m_bExistenceCross == false )
		{
			ptOnSphereStartV.z = -1 * ptOnSphereStartV.z;
			ptOnSphereEndV.z = -1 * ptOnSphereEndV.z;
			ptOnSphereStart.z = -1 * ptOnSphereStart.z;
			ptOnSphereEnd.z = -1 * ptOnSphereEnd.z;
		}
	}

	fdOnSphereA		= ptOnSphere.x - m_ptOnSpherePrevious.x;
	fdOnSphereB		= ptOnSphere.y - m_ptOnSpherePrevious.y;
	fdOnSphereC		= ptOnSphere.z - m_ptOnSpherePrevious.z;
	fLengthOnSphere	= sqrt(ptOnSphere.x * ptOnSphere.x + ptOnSphere.y * ptOnSphere.y + ptOnSphere.z * ptOnSphere.z );
	fAngleForArbitraryAxis	= asin(sqrt(fdOnSphereA * fdOnSphereA+ fdOnSphereB * fdOnSphereB + fdOnSphereC * fdOnSphereC )/(fLengthOnSphere*2)) *2;
	fAngleForArbitraryAxis	= fAngleForArbitraryAxis * 180/3.141592653;

	HC_Compute_Cross_Product(&m_ptOnSpherePrevious, &ptOnSphere, &ptProductOnCircle );

	if ( m_bExistenceCross == true )
	{
		if( (fabs(ptProductOnCircle.x) > 0) || (fabs(ptProductOnCircle.y) > 0) || (fabs(ptProductOnCircle.z) > 0) )
		{
			double	a_cross_theta, a_cross_phi;

			a_cross_theta = atan(ptProductOnCircle.y/ptProductOnCircle.x) *180/3.141592653;
			a_cross_phi = asin(ptProductOnCircle.z/sqrt(ptProductOnCircle.x * ptProductOnCircle.x + ptProductOnCircle.y * ptProductOnCircle.y + ptProductOnCircle.z * ptProductOnCircle.z)) *180/3.141592653;

			fAngleFirstForRoll = 0;
			if ( (ptProductOnCircle.x > 0) && (ptProductOnCircle.y > 0) )
			{
				fAngleFirstForRoll = -(90-fabs(a_cross_theta));
			}
			else if ( (ptProductOnCircle.x < 0) && (ptProductOnCircle.y > 0) )
			{
				fAngleFirstForRoll = +(90-fabs(a_cross_theta));
			}
			else if ( (ptProductOnCircle.x < 0) && (ptProductOnCircle.y < 0) )
			{
				fAngleFirstForRoll = +(90+fabs(a_cross_theta));
			}
			else if ((ptProductOnCircle.x > 0) && (ptProductOnCircle.y < 0))
			{
				fAngleFirstForRoll = -(90+fabs(a_cross_theta));
			}

			fAngleFirstForOrbit = 0;
			if((ptProductOnCircle.z > 0))
			{
				fAngleFirstForOrbit = - fabs(a_cross_phi);
			}
			else
			{
				fAngleFirstForOrbit = + fabs(a_cross_phi);
			}
			
			HC_Open_Segment_By_Key (m_pView->GetSceneKey());

				// Go to the special place
				HC_Roll_Camera(fAngleFirstForRoll);
				HC_Orbit_Camera (0, fAngleFirstForOrbit);

				// Orbit against the arbitrary axis
				HC_Orbit_Camera (-fAngleForArbitraryAxis, 0);

				// Return to the original place
				HC_Orbit_Camera (0, -fAngleFirstForOrbit);
				HC_Roll_Camera(-fAngleFirstForRoll);

				float p1[3] = {0,0,0};
				float p2[3] = {10,0,0};

 				HC_Compute_Coordinates(".","viewpoint", &p1, "object", &p1);
 				HC_Compute_Coordinates(".","viewpoint", &p2, "object", &p2);

				double dAngle = 90.0*M_PI/180.0;//radians
				SPAtransf trans = rotate_transf(dAngle,SPAvector(p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]));
				SPAvector vec = wcs_z_axis()*trans;

				//change z_axis
				float point[3] = {0,0,10};
 				HC_Compute_Coordinates(".","object", &point, "viewpoint", &point);
				float point1[3] = {0,0,0};
 				HC_Compute_Coordinates(".","object", &point1, "viewpoint", &point1);

				HPoint vecUp(0,1,0);

				SPAvector myvec = SPAvector(point[0]-point1[0],point[1]-point1[1],point[2]-point1[2]);
				SPAunit_vector axisZ = SPAunit_vector(0,0,1);

				double angle1 = 180.0/M_PI*angle_between(myvec,axisZ);

				double angle = 0.0;
				if (myvec.y() > 0.0)
				{
					angle = 180.0/M_PI*angle_between( myvec,SPAvector(vecUp.x,vecUp.y,vecUp.z),axisZ);
				}
				else
				{
					angle = 180.0/M_PI*angle_between( myvec,SPAvector(vecUp.x,-vecUp.y,vecUp.z),axisZ);
				}
				double dAngleTemp = angle;
				
				if(angle > 180)
					dAngleTemp = 360-angle;


 				if (dAngleTemp > 0.5)
				{
					HC_Roll_Camera(-angle);
				}

			HC_Close_Segment();

			if ( m_bVisibleSphere == true )
			{
				HC_Open_Segment_By_Key (m_pView->GetSceneKey());
					HC_Open_Segment("sub-figure");
						HC_Open_Segment("cross");
							HC_Rotate_Object_Offaxis(ptProductOnCircle.x, ptProductOnCircle.y, ptProductOnCircle.z, fAngleForArbitraryAxis);
						HC_Close_Segment();
					HC_Close_Segment();
				HC_Close_Segment();
			}
	
			//Test
			HC_Update_Display();
		}
	}
	else
	{
		if ( m_bVisibleSphere == true )
		{
			HC_Open_Segment_By_Key (m_pView->GetSceneKey());
				HC_Open_Segment("sub-figure");
					HC_Open_Segment("cross");
						HC_Set_Rendering_Options("depth range=(0.05, 0.06)"); // 04/01/2005 Bashi
						HC_Set_Color("line = orange");
						HC_Set_Line_Weight(1.5);
						HC_Insert_Circular_Arc (&ptOnSphereStart, &ptOnSphere, &ptOnSphereEnd);
						HC_Insert_Circular_Arc (&ptOnSphereStartV, &ptOnSphere, &ptOnSphereEndV);
					HC_Close_Segment();
				HC_Close_Segment();
			HC_Close_Segment();
		}
		
		m_bExistenceCross = true;
	}

	// Update the menmber
	m_ptOnSpherePrevious.x = ptOnSphere.x;
	m_ptOnSpherePrevious.y = ptOnSphere.y;
	m_ptOnSpherePrevious.z = ptOnSphere.z;

	return true;
}

int CATOpMouse::zoom_camera(HEventInfo& event)
{

	if (m_eMode != AH_CAM_ZOOM)
	{
		return -1;
	}

	delete_origin();

	m_ptNew = event.GetMouseWindowPos();

	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		
		float fZoomCameraWidth, fZoomCameraHight, fdZoomFactor = m_ptNew.y - m_ptFirst.y;

		HPoint ptZoomCameraTarget, ptZoomCameraPosition, ptdZoomCameraPosCam; 
		char proj[64];
 
		HC_Show_Net_Camera_Position(&ptZoomCameraPosition.x, &ptZoomCameraPosition.y, &ptZoomCameraPosition.z);
		HC_Show_Net_Camera_Target(&ptZoomCameraTarget.x, &ptZoomCameraTarget.y, &ptZoomCameraTarget.z);
		HC_Show_Net_Camera_Field(&fZoomCameraWidth, &fZoomCameraHight);
		HC_Show_Net_Camera_Projection(proj);

		// we only want to zoom in if we are still further out than
		// the maximum zoom level and we are actually zooming in
		float maxZoomLevel = m_pView->GetZoomLimit();
		if ((fZoomCameraWidth > maxZoomLevel) || (fZoomCameraHight > maxZoomLevel) || (fdZoomFactor<0.0f))
		{
			ptdZoomCameraPosCam.Set(ptZoomCameraPosition.x - ptZoomCameraTarget.x, 
				ptZoomCameraPosition.y - ptZoomCameraTarget.y, 
				ptZoomCameraPosition.z - ptZoomCameraTarget.z);

			// if zoom_factor greater than 1 than we will zoom past the camera target.  Extra check to avoid unnecessary 
			// zoom in when the camera is close to target (sign of zoom factor indicates whether trying to zoom in/out) 
			if ((HC_Compute_Vector_Length(&ptdZoomCameraPosCam) > 0.0000001))
			{
				if (fdZoomFactor < 0.99)
				HC_Set_Camera_Position(
					m_ptCamera.x + m_ptDist.x * (m_fLength-(m_fLength*fdZoomFactor)),
					m_ptCamera.y + m_ptDist.y * (m_fLength-(m_fLength*fdZoomFactor)),
					m_ptCamera.z + m_ptDist.z * (m_fLength-(m_fLength*fdZoomFactor)));
			}
		}
		if (streq(proj, "orthographic"))
		{
		    if (!(fdZoomFactor > 0.99))
			    HC_Set_Camera_Field(m_Width - (m_Width * fdZoomFactor), m_Height - (m_Height * fdZoomFactor));
		}

	HC_Close_Segment();

	create_origin();	// 15 July 2004 hide 

	// tell the view that the camera position has changed
	m_pView->CameraPositionChanged();

 	// 03/30/2005 Bashi Add
//	m_pAHBaseView->ExecuteCameraCallback(); // 01/21/2008 Bashi modified

	m_pView->Update();
	return (HOP_OK);
}

int CATOpMouse::pan_camera(HEventInfo& event)
{
	if (m_eMode != AH_CAM_PAN) return -1;
 
	delete_origin();

	HPoint ptdPanCamera, ptPanCameraPos,ptPanCameraTarget;
	
    m_ptNew = event.GetMouseWorldPos();  
										
    ptdPanCamera.x = m_ptNew.x - m_ptFirst.x;
    ptdPanCamera.y = m_ptNew.y - m_ptFirst.y;
    ptdPanCamera.z = m_ptNew.z - m_ptFirst.z;

    HC_Open_Segment_By_Key (m_pView->GetSceneKey());
		
		HC_Show_Net_Camera_Target (&ptPanCameraTarget.x, &ptPanCameraTarget.y, &ptPanCameraTarget.z);
		HC_Show_Net_Camera_Position (&ptPanCameraPos.x, &ptPanCameraPos.y, &ptPanCameraPos.z);
	
		HC_Set_Camera_Target (ptPanCameraTarget.x - ptdPanCamera.x, ptPanCameraTarget.y - ptdPanCamera.y, ptPanCameraTarget.z - ptdPanCamera.z);
		HC_Set_Camera_Position (ptPanCameraPos.x - ptdPanCamera.x, ptPanCameraPos.y - ptdPanCamera.y, ptPanCameraPos.z - ptdPanCamera.z);

	HC_Close_Segment();

	create_origin();

	m_pView->CameraPositionChanged();

//??	m_pAHBaseView->ExecuteCameraCallback();

//	m_pView->Update();
    HC_Update_Display();

	return (HOP_OK);
}

//============================================
// CenterCamera - Change Target of Camera to the position mouse clicked and change eye position of camera accordingly
//============================================
int CATOpMouse::center_camera(HEventInfo& event)
{

    HC_KEY selectionkey = INVALID_KEY;
    int	iDummy1, iDummy2, iDummy3;
	HPoint ptCenterCameraLocalWindow;

	ptCenterCameraLocalWindow = event.GetMouseWindowPos(); // Bashi modified 06/28/2004

	// 03/25/2008 Bashi add: Turn on internal selection ( check another HC_Set_Heuristics below )
	HC_Open_Segment_By_Key( m_pView->GetSceneKey() );
		HC_Set_Heuristics("internal selection limit=1"); // HOOPS default
	HC_Close_Segment();

	// Finds the key of the database element that was selected by the user.
    HC_Open_Segment_By_Key(m_pView->GetViewKey());
		if ( HC_Compute_Selection(".",".","v", ptCenterCameraLocalWindow.x, ptCenterCameraLocalWindow.y))
		{	
			HC_Show_Selection_Element (&selectionkey, &iDummy1, &iDummy2, &iDummy3);	
		}
	HC_Close_Segment();

	// Determine offset value (vector)
	HPoint ptCenterCameraOffset, ptCenterCameraTarget;
	if (selectionkey != INVALID_KEY) // Something picked
	{
		HC_Open_Segment_By_Key (m_pView->GetSceneKey());
			HC_Show_Net_Camera_Target(&ptCenterCameraTarget.x, &ptCenterCameraTarget.y, &ptCenterCameraTarget.z);
		HC_Close_Segment();	

		// Get Position on the picked object
		HPoint ptCenterCameraCenter; 
		get_position_on_object(ptCenterCameraLocalWindow, ptCenterCameraCenter);

		ptCenterCameraOffset.x = ptCenterCameraCenter.x - ptCenterCameraTarget.x;
		ptCenterCameraOffset.y = ptCenterCameraCenter.y - ptCenterCameraTarget.y;
		ptCenterCameraOffset.z = ptCenterCameraCenter.z - ptCenterCameraTarget.z;

	}
	else // Nothing picked
	{
		// Map position
		HC_Open_Segment_By_Key (m_pView->GetSceneKey());

			HPoint ptCenterCameraScreen(0.0, 0.0, 0.0), ptCenterCameraScreenTarget, ptCenterCameraScreenCenter;
			HC_Compute_Coordinates(".","local window", &ptCenterCameraScreen, "world", &ptCenterCameraScreenTarget);
			HC_Compute_Coordinates(".","local window", &ptCenterCameraLocalWindow, "world", &ptCenterCameraScreenCenter);

		HC_Close_Segment();	
				
		ptCenterCameraOffset.x = ptCenterCameraScreenCenter.x - ptCenterCameraScreenTarget.x;
		ptCenterCameraOffset.y = ptCenterCameraScreenCenter.y - ptCenterCameraScreenTarget.y;
		ptCenterCameraOffset.z = ptCenterCameraScreenCenter.z - ptCenterCameraScreenTarget.z;
	}

	// Offset Camera position
	HPoint ptCenterCameraCenterCameraPosition;
	HC_Open_Segment_By_Key( m_pView->GetSceneKey());

		HC_Show_Net_Camera_Position(&ptCenterCameraCenterCameraPosition.x, &ptCenterCameraCenterCameraPosition.y, &ptCenterCameraCenterCameraPosition.z);
		HC_Show_Net_Camera_Target(&ptCenterCameraTarget.x, &ptCenterCameraTarget.y, &ptCenterCameraTarget.z);

		HC_Set_Camera_Position (ptCenterCameraCenterCameraPosition.x + ptCenterCameraOffset.x, ptCenterCameraCenterCameraPosition.y + ptCenterCameraOffset.y, ptCenterCameraCenterCameraPosition.z + ptCenterCameraOffset.z);
		HC_Set_Camera_Target(ptCenterCameraTarget.x + ptCenterCameraOffset.x, ptCenterCameraTarget.y + ptCenterCameraOffset.y, ptCenterCameraTarget.z + ptCenterCameraOffset.z);

	HC_Close_Segment();

	// 03/25/2008 Bashi add: Turn off internal selection to increase performance
	HC_Open_Segment_By_Key( m_pView->GetSceneKey() );
		HC_Set_Heuristics("internal selection limit=-1");
	HC_Close_Segment();

	return (HOP_OK);
}

//============================================
// Queries the exact world coordinate on the model 
//============================================
bool CATOpMouse::get_position_on_object(HPoint& ptLocalWindow, HPoint& ptExactWorld)
{
	char save_selection[MVO_BUFFER_SIZE];
	int count;
	HC_KEY key = INVALID_KEY;
	bool bSelectionFlag = false;
	HPoint	ptWindowA;

	HC_Open_Segment_By_Key (m_pView->GetSceneKey());

		// save selection settings
		if (HC_Show_Existence("selectability"))
		{
			HC_Show_Selectability(save_selection);
			bSelectionFlag = true;
		}

		HC_Set_Selectability ("geometry=on");

		if ((count = HC_Compute_Selection(m_pView->GetDriverPath(), ".", "v", ptLocalWindow.x, ptLocalWindow.y)) > 0)
		{		
			HC_Show_Selection_Position(&ptWindowA.x, &ptWindowA.y, &ptWindowA.z, &ptExactWorld.x, &ptExactWorld.y, &ptExactWorld.z);
		}
		else 
		{
			HC_Compute_Coordinates(".","local window", &ptLocalWindow, "world", &ptExactWorld);
		}

		//restore selection settings
		if (bSelectionFlag)
		{
			HC_Set_Selectability(save_selection);
		}
		else
		{
			HC_UnSet_Selectability();
		}
	
	HC_Close_Segment();
	return true;
}

//============================================
// CreateOrigin - Create 5 lines at the origin
//============================================
int CATOpMouse::create_origin()
{
	HPoint	ptTarget;
	HPoint	ptPixelA(0.0, 0.0, 0.0), ptPixelB(40.0, 0.0, 0.0);
	HPoint	ptWorldA, ptWorldB;
	HC_Open_Segment_By_Key (m_pView->GetSceneKey());
		HC_Open_Segment("sub-figure");
			HC_Open_Segment("origin");
				HC_Show_Net_Camera_Target(&ptTarget.x, &ptTarget.y, &ptTarget.z);
				HC_Compute_Coordinates(".","local pixels", &ptPixelA, "world", &ptWorldA);
				HC_Compute_Coordinates(".","local pixels", &ptPixelB, "world", &ptWorldB);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	HPoint ptdWorld(ptWorldA.x - ptWorldB.x, ptWorldA.y - ptWorldB.y, ptWorldA.z - ptWorldB.z);
	float fLengthForWorld = (float) HC_Compute_Vector_Length(&ptdWorld);
	float fLengthForWorldShort = fLengthForWorld/3; // The length ratio of visible line to non-visible line = 3 : 1  

	HPoint ptCreateOriginXfa(ptTarget.x-fLengthForWorld, ptTarget.y, ptTarget.z);
	HPoint ptCreateOriginXea(ptTarget.x-fLengthForWorldShort, ptTarget.y, ptTarget.z);
	HPoint ptCreateOriginXfb(ptTarget.x+fLengthForWorld, ptTarget.y, ptTarget.z);
	HPoint ptCreateOriginXeb(ptTarget.x+fLengthForWorldShort, ptTarget.y, ptTarget.z);

	HPoint ptCreateOriginYfa(ptTarget.x, ptTarget.y - fLengthForWorld, ptTarget.z);
	HPoint ptCreateOriginYea(ptTarget.x, ptTarget.y - fLengthForWorldShort, ptTarget.z);
	HPoint ptCreateOriginYfb(ptTarget.x, ptTarget.y + fLengthForWorld, ptTarget.z);
	HPoint ptCreateOriginYeb(ptTarget.x, ptTarget.y + fLengthForWorldShort, ptTarget.z);

	HPoint ptCreateOriginZfa(ptTarget.x, ptTarget.y, ptTarget.z + fLengthForWorld);
	HPoint ptCreateOriginZea(ptTarget.x, ptTarget.y, ptTarget.z - fLengthForWorldShort);

	if ( m_bVisibleSphere == true )
	{
		HC_Open_Segment_By_Key(m_pView->GetSceneKey());
			HC_Open_Segment("sub-figure");
				HC_Open_Segment("origin");
					HC_Set_Color("line = orange");
					HC_Set_Line_Weight(1.5);
					HC_Set_Visibility("lines = on");
					HC_Insert_Line(ptCreateOriginXfa.x, ptCreateOriginXfa.y, ptCreateOriginXfa.z, ptCreateOriginXea.x, ptCreateOriginXea.y, ptCreateOriginXea.z);
					HC_Insert_Line(ptCreateOriginXfb.x, ptCreateOriginXfb.y, ptCreateOriginXfb.z, ptCreateOriginXeb.x, ptCreateOriginXeb.y, ptCreateOriginXeb.z);
					HC_Insert_Line(ptCreateOriginYfa.x, ptCreateOriginYfa.y, ptCreateOriginYfa.z, ptCreateOriginYea.x, ptCreateOriginYea.y, ptCreateOriginYea.z);
					HC_Insert_Line(ptCreateOriginYfb.x, ptCreateOriginYfb.y, ptCreateOriginYfb.z, ptCreateOriginYeb.x, ptCreateOriginYeb.y, ptCreateOriginYeb.z);
					HC_Insert_Line(ptCreateOriginZfa.x, ptCreateOriginZfa.y, ptCreateOriginZfa.z, ptCreateOriginZea.x, ptCreateOriginZea.y, ptCreateOriginZea.z);
				HC_Close_Segment();
			HC_Close_Segment();
		HC_Close_Segment();
	}
	return true;
}

//============================================
// DeleteOrigin - Delete the origin 
// 22 July 2004 hide  
//============================================
int	CATOpMouse::delete_origin()
{
	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		HC_Open_Segment("sub-figure");
			HC_Open_Segment("origin");
				HC_Flush_Contents(".", "everything");
			HC_Close_Segment();	
		HC_Close_Segment();
	HC_Close_Segment();
	return true;
}

//============================================
// CreateCross - Creat a cross on the sphere 
//============================================
int CATOpMouse::create_cross(HEventInfo& event)
{
	float	fRadius = (float) 0.9;	// Radius of the circle in the circle segment
	float	fApproachFactor = (float) 0.88;	// Factor, which is related with the radius to calculate the length of the arc 　　
	float	fLimitRadius = (float) 0.1;	// The factor regarding the limitation of the cross
	float	fFactor	= (float) 3.0;	// The factor regarding how much the cross approach
	float	fWight = (float) 0;
	float	fLengthMouseWindowPos = (float) 0;
	float	fProductByAcos = (float) 0;
	float	fProductNByAcos = (float) 0;
	float	fRadiusSin = (float) 0;
	float	fLengthTempM = (float) 0;
	float	fLengthOnSphere	 = (float) 0;
	float	fAngleForArbitraryAxis = (float) 0;
	float	fAngleFirstForRoll = (float) 0;
	float	fAngleFirstForOrbit = (float) 0;
	float	fdOnSphereA, fdOnSphereB, fdOnSphereC;

	HPoint	ptMouseWindowPos = event.GetMouseWindowPos();
	HPoint	ptWorldMouseWindowPos;
	HPoint	ptWorldMouseWindowPosWithFactor;
	HPoint	ptOnSphere, ptOnSphereStart, ptOnSphereEnd, ptOnSphereStartV, ptOnSphereEndV;
	HPoint	ptTemp, ptTemp2, ptTempM, ptTempN, ptTempWithFactor, ptTemp2WithFactor, ptProductM, ptProductN;
	HPoint	ptDividedByRadius, ptProductOnCircle;

	HC_Open_Segment_By_Key (m_pView->GetSceneKey());
		HC_Open_Segment("sub-figure");
			HC_Open_Segment("cross");
		//		HC_Set_Handedness("right");
				HC_Set_Visibility("lines = on");
			HC_Set_Camera_By_Volume ("orthographic", -1.0, 1.0, -1.0, 1.0);
				HC_Compute_Coordinates(".","local window", &ptMouseWindowPos, "world", &ptWorldMouseWindowPos);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	fLengthMouseWindowPos = (float) HC_Compute_Vector_Length(&ptWorldMouseWindowPos);

	if ( fLengthMouseWindowPos < fRadius )
	{
		// The picked point is in the circle
		ptWorldMouseWindowPosWithFactor.x = ptWorldMouseWindowPos.x;
		ptWorldMouseWindowPosWithFactor.y = ptWorldMouseWindowPos.y;
		ptWorldMouseWindowPosWithFactor.z = ptWorldMouseWindowPos.z;
	}
	else
	{
		// The picked point is out of the circle
		if( (1 + fFactor)*fRadius > fLengthMouseWindowPos + fLimitRadius){
			// The more the picked point separates from the circle, the closer the position of the cross is
			fWight = (fRadius - ((fLengthMouseWindowPos - fRadius)/fFactor)) / fLengthMouseWindowPos;
		}else{
			// The point of the cross is fixed
			fWight = (float) (fRadius - ((fFactor *fRadius) - fLimitRadius )/fFactor)/((1 + fFactor)*fRadius - fLimitRadius);
		}
		ptWorldMouseWindowPosWithFactor.x = ptWorldMouseWindowPos.x * fWight;
		ptWorldMouseWindowPosWithFactor.y = ptWorldMouseWindowPos.y * fWight;
		ptWorldMouseWindowPosWithFactor.z = ptWorldMouseWindowPos.z * fWight;
	}

	// Calculate the length between the picked point and the center point
	float fLengthPosWithFactor = (float) HC_Compute_Vector_Length(&ptWorldMouseWindowPosWithFactor);

	if ( fRadius < 0 )
	{
		return 0;
	}
	fProductByAcos = (float) acos(fLengthPosWithFactor/fRadius);

	ptOnSphere.x = ptWorldMouseWindowPosWithFactor.x;
	ptOnSphere.y = ptWorldMouseWindowPosWithFactor.y;
	ptOnSphere.z = (float) fRadius * ((float) sin(fProductByAcos));

	if ( m_bExistenceCross == false )
	{
		ptTemp.x = ptTemp.y = 0;
		ptTemp.z = (float) fRadius * ((float) sin(fProductByAcos));

		ptTemp2.x = ptWorldMouseWindowPosWithFactor.x;
		ptTemp2.y = ptWorldMouseWindowPosWithFactor.y;
		ptTemp2.z = 0;

		HC_Compute_Cross_Product(&ptTemp, &ptTemp2, &ptTempM);
		fLengthTempM = (float) HC_Compute_Vector_Length(&ptTempM);
		if ( fLengthTempM < 0 )
		{
			return 0;
		}
		ptTempN.x = (float) ptTempM.x/fLengthTempM;
		ptTempN.y = (float) ptTempM.y/fLengthTempM;
		ptTempN.z = (float) ptTempM.z/fLengthTempM;

		if ( fRadius < 0 )
		{
			return 0;
		}
		fProductNByAcos = (float) acos(fApproachFactor/fRadius);
		fRadiusSin = (float) fRadius * ((float) sin(fProductNByAcos));

		if ( fRadius < 0 )
		{
			return 0;
		}
		ptDividedByRadius.x = fApproachFactor * ptOnSphere.x/fRadius;
		ptDividedByRadius.y = fApproachFactor * ptOnSphere.y/fRadius;
		ptDividedByRadius.z = fApproachFactor * ptOnSphere.z/fRadius;

		ptOnSphereStart.x = ptDividedByRadius.x + ptTempN.x * fRadiusSin;
		ptOnSphereStart.y = ptDividedByRadius.y + ptTempN.y * fRadiusSin;
		ptOnSphereStart.z = ptDividedByRadius.z + ptTempN.z * fRadiusSin;

		ptOnSphereEnd.x = ptDividedByRadius.x - ptTempN.x * fRadiusSin;
		ptOnSphereEnd.y = ptDividedByRadius.y - ptTempN.y * fRadiusSin;
		ptOnSphereEnd.z = ptDividedByRadius.z - ptTempN.z * fRadiusSin;		

		ptTempWithFactor.x = ptWorldMouseWindowPosWithFactor.x;
		ptTempWithFactor.y = ptWorldMouseWindowPosWithFactor.y;
		ptTempWithFactor.z = (float) fRadius * ((float) sin(fProductByAcos));

		ptTemp2WithFactor.x = - ptWorldMouseWindowPosWithFactor.y;
		ptTemp2WithFactor.y = + ptWorldMouseWindowPosWithFactor.x;
		ptTemp2WithFactor.z = 0;

		HC_Compute_Cross_Product(&ptTempWithFactor, &ptTemp2WithFactor, &ptProductM);
		float fLengthProductM = (float) HC_Compute_Vector_Length(&ptProductM);
		if ( fLengthProductM < 0 )
		{
			return 0;
		}
		ptProductN.x = (float) ptProductM.x/fLengthProductM;
		ptProductN.y = (float) ptProductM.y/fLengthProductM;
		ptProductN.z = (float) ptProductM.z/fLengthProductM;

		ptOnSphereStartV.x = ptDividedByRadius.x + ptProductN.x * fRadiusSin;
		ptOnSphereStartV.y = ptDividedByRadius.y + ptProductN.y * fRadiusSin;
		ptOnSphereStartV.z = ptDividedByRadius.z + ptProductN.z * fRadiusSin;

		ptOnSphereEndV.x = ptDividedByRadius.x - ptProductN.x * fRadiusSin;
		ptOnSphereEndV.y = ptDividedByRadius.y - ptProductN.y * fRadiusSin;
		ptOnSphereEndV.z = ptDividedByRadius.z - ptProductN.z * fRadiusSin;		
	}

	if(fLengthMouseWindowPos < fRadius)
	{
		// The picked point is in the circle
		;
	}
	else
	{
		// The picked point is without the circle
		// Reverse the z coordinate value
		ptOnSphere.z =  -1 * ptOnSphere.z;
		if (m_bExistenceCross == false)
		{
			ptOnSphereStartV.z = -1 * ptOnSphereStartV.z;
			ptOnSphereEndV.z = -1 * ptOnSphereEndV.z;
			ptOnSphereStart.z = -1 * ptOnSphereStart.z;
			ptOnSphereEnd.z = -1 * ptOnSphereEnd.z;
		}
	}

	fdOnSphereA		= ptOnSphere.x - m_ptOnSpherePrevious.x;
	fdOnSphereB		= ptOnSphere.y - m_ptOnSpherePrevious.y;
	fdOnSphereC		= ptOnSphere.z - m_ptOnSpherePrevious.z;
	fLengthOnSphere	= sqrt(ptOnSphere.x * ptOnSphere.x + ptOnSphere.y * ptOnSphere.y + ptOnSphere.z * ptOnSphere.z );
	fAngleForArbitraryAxis	= asin(sqrt(fdOnSphereA * fdOnSphereA+ fdOnSphereB * fdOnSphereB + fdOnSphereC * fdOnSphereC )/(fLengthOnSphere*2)) *2;
	fAngleForArbitraryAxis	= fAngleForArbitraryAxis * 180/M_PI;

	HC_Compute_Cross_Product(&m_ptOnSpherePrevious, &ptOnSphere, &ptProductOnCircle );

	if ( m_bExistenceCross == true )
	{
		if ( (fabs(ptProductOnCircle.x) > 0) || (fabs(ptProductOnCircle.y) > 0) || (fabs(ptProductOnCircle.z) > 0) )
		{

			double	a_cross_theta, a_cross_phi;

			a_cross_theta = atan(ptProductOnCircle.y/ptProductOnCircle.x) *180/M_PI;
			a_cross_phi = asin(ptProductOnCircle.z/sqrt(ptProductOnCircle.x * ptProductOnCircle.x + ptProductOnCircle.y * ptProductOnCircle.y + ptProductOnCircle.z * ptProductOnCircle.z)) *180/3.141592653;

			fAngleFirstForRoll = 0;
			if ( (ptProductOnCircle.x > 0) && (ptProductOnCircle.y > 0) )
			{
				fAngleFirstForRoll = -(90-fabs(a_cross_theta));
			}
			else if ( (ptProductOnCircle.x < 0) && (ptProductOnCircle.y > 0) )
			{
				fAngleFirstForRoll = +(90-fabs(a_cross_theta));
			}
			else if( (ptProductOnCircle.x < 0) && (ptProductOnCircle.y < 0) )
			{
				fAngleFirstForRoll = +(90+fabs(a_cross_theta));
			}
			else if ( (ptProductOnCircle.x > 0) && (ptProductOnCircle.y < 0) )
			{
				fAngleFirstForRoll = -(90+fabs(a_cross_theta));
			}

			fAngleFirstForOrbit = 0;
			if ( (ptProductOnCircle.z > 0) )
			{
				fAngleFirstForOrbit = - fabs(a_cross_phi);
			}
			else
			{
				fAngleFirstForOrbit = + fabs(a_cross_phi);
			}
			
			HC_Open_Segment_By_Key (m_pView->GetSceneKey());
				// Go to the special place
				HC_Roll_Camera(fAngleFirstForRoll);
				HC_Orbit_Camera (0, fAngleFirstForOrbit);

				// Orbit against the arbitrary axis
				HC_Orbit_Camera (-fAngleForArbitraryAxis, 0);

				// Return to the original place
				HC_Orbit_Camera (0, -fAngleFirstForOrbit);
				HC_Roll_Camera(-fAngleFirstForRoll);
			HC_Close_Segment();

			if ( m_bVisibleSphere == true )
			{
				HC_Open_Segment_By_Key (m_pView->GetSceneKey());
					HC_Open_Segment("sub-figure");
						HC_Open_Segment("cross");
							HC_Rotate_Object_Offaxis(ptProductOnCircle.x, ptProductOnCircle.y, ptProductOnCircle.z, fAngleForArbitraryAxis);
						HC_Close_Segment();
					HC_Close_Segment();
				HC_Close_Segment();
			}
		}
	}
	else
	{
		if ( m_bVisibleSphere == true )
		{
			HC_Open_Segment_By_Key (m_pView->GetSceneKey());
				HC_Open_Segment("sub-figure");
					HC_Open_Segment("cross");
						HC_Set_Rendering_Options("depth range=(0.05, 0.06)"); // 04/01/2005 Bashi
						HC_Set_Rendering_Options("general curve = view dependent");// 8/6/2009 mwu : for smooth curve
						HC_Set_Color("line = orange");
						HC_Set_Line_Weight(1.5);
						HC_Insert_Circular_Arc (&ptOnSphereStart, &ptOnSphere, &ptOnSphereEnd);
						HC_Insert_Circular_Arc (&ptOnSphereStartV, &ptOnSphere, &ptOnSphereEndV);
					HC_Close_Segment();
				HC_Close_Segment();
			HC_Close_Segment();
		}
		
		m_bExistenceCross = true;
	}

	// Update the menmber
	m_ptOnSpherePrevious.x = ptOnSphere.x;
	m_ptOnSpherePrevious.y = ptOnSphere.y;
	m_ptOnSpherePrevious.z = ptOnSphere.z;

	return true;
}

//============================================
// DeleteCross - Delete the cross 
// 22 July 2004 hide  
//============================================
int	CATOpMouse::delete_cross()
{
	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		HC_Open_Segment("sub-figure");
			HC_Open_Segment("cross");
				HC_Flush_Contents(".", "everything");
			HC_Close_Segment();	
		HC_Close_Segment();
	HC_Close_Segment();
	return true;
}

//============================================
// CreateCircle - Creat a circle 
//============================================
int CATOpMouse::create_circle(HEventInfo& event)
{
	if( m_bVisibleSphere == true )
	{
		// 09/10/2003 Bashi - Add
		HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		// 02/08/2004 Bashi - Add: Enable LOD
		// 01/05/2009 Spatial: discontinue using LOD
//##		HC_Set_Rendering_Options( "lod=on, lodo = (clamp = 1,fallback = coarsest)" );
		HC_Open_Segment("sub-figure");
			HC_Open_Segment("circle");
				HC_Set_Rendering_Options("depth range=(0.05, 0.06)"); // 04/01/2005 Bashi
		//		HC_Set_Handedness("right");
				HC_Set_Camera_By_Volume ("orthographic", -1.0, 1.0, -1.0, 1.0);
				HPoint ptA1,ptA2,ptA3;
				ptA1.Set(-0.9f,0.0f,0.0f);
				ptA2.Set(0.0f,-0.9f,0.0f);
				ptA3.Set(0.9f,0.0f,0.0f);
				HC_Insert_Circle(&ptA1,&ptA2,&ptA3);		
				HC_Set_Selectability ("everything = off");
				HC_Set_Visibility("cutting planes = off, faces = off, edges = on, lines = on, text = on, markers = off");
				HC_Set_Color("edges=orange");
				HC_Set_Edge_Weight(1.5);
				HC_Set_Edge_Pattern("- -");
			HC_Close_Segment();
		HC_Close_Segment();	
		HC_Close_Segment();	
	}

	return true;
}

//============================================
// DeleteCircle - Delete the circel 
//============================================
int	CATOpMouse::delete_circle()
{
	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		HC_Open_Segment("sub-figure");
			HC_Open_Segment("circle");
				HC_Flush_Contents(".", "everything");
			HC_Close_Segment();	
		HC_Close_Segment();
	HC_Close_Segment();

	return true;
}
