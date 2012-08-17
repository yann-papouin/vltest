//
// Copyright (c) 2000 by Tech Soft 3D, LLC.
// The information contained herein is confidential and proprietary to
// Tech Soft 3D, LLC., and considered a trade secret as defined under
// civil and criminal statutes.  Tech Soft 3D shall pursue its civil
// and criminal remedies in the event of unauthorized use or misappropriation
// of its trade secrets.  Use of this information by anyone other than
// authorized employees of Tech Soft 3D, LLC. is granted only under a
// written non-disclosure agreement, expressly prescribing the scope and
// manner of such use.
//
// $Header: /files/homes/master/cvs/hoops_master/shared/partviewer_common/HSolidView.cpp,v 1.143 2009-10-23 17:30:33 nathan Exp $
//

// HSolidView.cpp : implementation of the HSolidView class
// 

// standard includes
#include "StdAfx.h"
#include "resource.h"
#include <assert.h>
#include <math.h>
#include "hc.h"
#include "HSInclude.h"
#include "HUtility.h"
#include "HEventManager.h"
#include "HBhvBehaviorManager.h"
// HOOPS/MVO includes
#include "HMarkupManager.h"
#include "HSharedKey.h"
#include "HUtilityGeomHandle.h"

#include "HSolidView.h"
#include "HSolidModel.h"
#include "HSSelectionSet.h"
#include "HUtilityLocaleString.h"

#ifdef ACIS
#include "ha_bridge.h"
#include "HSAcisSelectionSet.h"
#endif // ACIS

#ifdef INTEROP
//#include "HPMIUtility.h"
#include "InterOpFileIO.h"
#endif // INTEROP

HSolidView::HSolidView(HBaseModel *model,
    const char * 		alias,	
    const char *		driver_type,
    const char *		instance_name,
    void *		window_handle,
    void *		colormap) : HBaseView(model, alias, driver_type, instance_name, window_handle, colormap)

{
	m_optimize_shells = false;
	m_bhidden_exists = false;
	m_pSweetenKeyList = 0;
	m_bSmoothTransition = true;	
	m_collab_mode = false;
	m_bShowCollisions = false;
	m_ClashList = 0;
	m_pViewOperator = NULL;
}

HSolidView::~HSolidView()
{
	if (m_pSweetenKeyList)
		delete []m_pSweetenKeyList;
	ClearClashList();
	delete_vlist(m_ClashList);
}


void HSolidView::InitSelection(HSSelectionSet *ss)
{
	if (m_pSelection)
		delete m_pSelection;
	m_pSelection = ss;
    m_pSelection->Init();

}
 
// app-specific init function
void HSolidView::Init()
{
	// call base's init function first to get the default HOOPS hierarchy for the view
	HBaseView::Init();
 	GetModel()->GetEventManager()->RegisterHandler((HAnimationListener *)this, HAnimationListener::GetType(), HLISTENER_PRIORITY_NORMAL);

	// create our app-specific Selection object and initialize
#ifdef ACIS
	m_pSelection = new HSAcisSelectionSet(this);
#endif // ACIS

    m_pSelection->Init();

	m_pMarkupManager = new HMarkupManager(this);

	//set window background
	SetWindowColor(HPoint(0.75,0.75,0.75));

    // app-specific scene Defaults
    HC_Open_Segment_By_Key(m_ViewKey);
		HC_Set_Selectability("everything = off");
	HC_Close_Segment();


	// set up some scene defaults
	HC_Open_Segment_By_Key(m_SceneKey);
	  HC_Set_Rendering_Options("no color interpolation, color index interpolation");
	  HC_Set_Visibility ("lights = (faces = on, edges = off), markers = off, faces=on, edges=off, lines=off, text = on");

	  // 7/9/2011 mwu : 
	  char color[256];
//	  sprintf(color,"r=%f g=%f b=%f",189/255.0,196/255.0,224/255.0);
	  sprintf(color,"r=%f g=%f b=%f",155/255.0,155/255.0,187/255.0);
	  HC_Set_Color(color);

	  HC_Set_Color("lines = black");
	HC_Close_Segment();

    // windowspace (overlay) defaults
    HC_Open_Segment_By_Key(m_WindowspaceKey);
	  HC_Set_Color_By_Index ("geometry", 3);
	  HC_Set_Color_By_Index ("window contrast", 1);
	  HC_Set_Color_By_Index ("windows", 1);
	  HC_Set_Visibility("markers=on");
	  HC_Set_Marker_Symbol("+");
	  HC_Set_Selectability("off");
    HC_Close_Segment();

    SetZoomLimit();

}

void HSolidView::DeleteSelectionList(bool emit_message)
{
	// if there is no solid model, call the base class
	HSolidModel * pModel = (HSolidModel *)GetModel();
	if ( (!pModel->m_bSolidData) || (pModel->m_bPlainSolidModel))
	{
		HBaseView::DeleteSelectionList(emit_message);
		return;
	}

	int				i, numSolidSelections, numHoopsSelections;
	char			type[MVO_BUFFER_SIZE];
	HC_KEY			tmp;
	HSSelectionSet* selection = 0;
#ifdef ACIS
	ENTITY*			current = 0;
#endif // ACIS
	// get ptr to selection object
	selection = (HSSelectionSet *)GetSelection();

	numHoopsSelections = selection->GetSize();
//	numSolidSelections = selection->GetSolidListSize();
	numSolidSelections = 0; // 07/12/2011 modified by mwu : bug

	if (numHoopsSelections == 0)
		return;

	// loop through the list of selected HOOPS primitives, filtering
	// out the ones associated with solid model entities, and deleting
	// the non-solid model primitives directly using Delete_By_Key
	// The primitives associated with solid model entities need to be
	// deleted using the HOOPS/GM deletion functions so that the HOOPS<->GM 
	// mapping tables will remain in sync

	for (i = numHoopsSelections; i > 0; i--)
	{
		HC_KEY key = selection->GetAt(i-1);

		// if it's a HOOPS primitive that is not associated with a solid model entity,
		// deselect and delete it
#ifdef ACIS
		current = HA_Compute_Entity_Pointer(key, BODY_TYPE);
		if (current == 0)
#endif // ACIS
		{
			selection->DeSelect(key, false); // the message emit business is done at the end
			int size;
			if (size = HUtility::IsChildOf(m_CuttingPlanesKey, key))
			{
				HC_KEY *keylist = new HC_KEY[size];
				HUtility::IsChildOf(m_CuttingPlanesKey, key, keylist);
				key = keylist[size-2];
				delete [] keylist;
			}			
			HC_Delete_By_Key(key);
		}
	}


	HC_Open_Segment_By_Key(m_CuttingPlanesKey);
		HC_Begin_Contents_Search ("...", "cutting planes");
		if (!HC_Find_Contents (type, &tmp)) 
			SetCuttingPlanesExist(false); 
		HC_End_Contents_Search ();
	HC_Close_Segment();

#ifdef CP_Intersect
    m_bUpdateCPGeom = true;
#endif
	

	// loop through the list of selected solid model entities,
	// delete the HOOPS geometry associated with each
	// and then delete the entity
	for (i = numSolidSelections; i > 0; i--)
	{

#ifdef ACIS
		current = (ENTITY*)selection->GetAtSolidEntity(i-1);
		if (current)
		{
			selection->DeSelectEntity(current);
			((HSolidModel*)GetModel())->DeleteAcisEntity(current);
		}
#endif // ACIS


	}

	if( emit_message )
		EmitDeleteSelectionListMessage();

	// Reset the HSSelectionSet and HSelectionSet lists since we've deleted all the entities that
	// were in them  (we can't DeSelect the entities because they no longer exist)
	selection->Reset();

    SetGeometryChanged();

	Update();
}

void HSolidView::RegenerateBodies(void)
{
#ifdef ACIS
	ENTITY* entity;
#endif // ACIS
	HC_KEY	child;
	char seg_type[MVO_BUFFER_SIZE];

 

	HC_Begin_Contents_Search (".", "segment, include");
	while (HC_Find_Contents (seg_type, &child) )
	{

		if (strcmp(seg_type,"include")==0)
		{
 			HC_KEY ikey = HC_KShow_Include_Segment(child);
			HC_Open_Segment_By_Key(ikey);
				RegenerateBodies();
			HC_Close_Segment();
		}
		else
		{

#ifdef ACIS
		HC_KEY body_key;
		entity = HA_Compute_Entity_Pointer(child, BODY_TYPE);
		if (entity)
		{
			ENTITY_LIST elist;

			elist.add(entity);

			// We might be in collabration, so let the peers know we are deleting
			// something. Better way to do this will be to write a wrapper over
			// HA_Delete_xxx
	 		HA_Compute_Geometry_Keys(entity, 1, &body_key, "bodies");		
			HA_Delete_Entity_Geometry(elist);

			HA_Render_Entity (entity);
			// We have inserted a new geometry (via HA_Render call). Let our 
			// collabrative peers know about that. 
			HA_Compute_Geometry_Keys(entity, 1, &body_key, "bodies");
			continue;
		}
#endif // ACIS


		HC_Open_Segment_By_Key(child);
			RegenerateBodies();
		HC_Close_Segment();
		}
	}
	HC_End_Contents_Search();
	
}

#ifndef HOOPS_ONLY
void HSolidView::SetOptimizations(bool optimize, bool no_update)
{
	if (no_update)
	{
		m_optimize_shells = optimize;
		return;
	}
		
	if (optimize != m_optimize_shells &&  GetRenderMode()!=HRenderHiddenLine)		
	{
		m_optimize_shells = optimize;

		// if optimize is true, set 'merge faces' mode so that all faces in a body will
		// be merged into a single HOOPS shell primitive; also set the select level to body
		// level if it was previously face-level, since we can no longer select on 
		// individiual faces
		KERN_CLASS_TYPEDEF class_type;


#ifdef ACIS
//		int class_type;

		ha_rendering_options &roptions=HA_Get_Rendering_Options();
		if (optimize)
		{
			roptions.SetMergeFacesMode(TRUE);

			class_type = ((HSSelectionSet *)GetSelection())->GetSelectLevel();
			if (class_type == FACE_TYPE)
				((HSSelectionSet *)GetSelection())->SetSelectLevel(BODY_TYPE);
		}
		else
			roptions.SetMergeFacesMode(FALSE);
#endif // ACIS

		// deselect all geometry prior to regenerating the model
		GetSelection()->DeSelectAll();
	 
		HC_Open_Segment_By_Key(GetModelKey());
			RegenerateBodies();
		HC_Close_Segment();	

		// we might be in collab, ask the peers to flush all the geometries
		// of model key and retransmit them the same
		if( m_pSharedKey )
		{
			m_pSharedKey->Reset();
			EmitFlushMessage();
			EmitSegment(GetModelKey(), true);
		}
	}
}

#endif // HOOPS_ONLY



bool HSolidView::GetOptimizations(void)
{
	return m_optimize_shells;
}
  
void HSolidView::EnableHiddenLine()
{
#ifdef ACIS
	OutputDebugString(_T("Precise Hidden Line not yet implemented in ACIS Part Viewer"));
	return;
#endif

	// do not do any precise hidden line fancy if we are in a collabrative session
	if( m_collab_mode )
		return;

	// do not use precise hidden line rendering if we do not have a solid model - Rajesh B
	HSolidModel * pModel = (HSolidModel *)GetModel();
	assert(pModel);
	if ( (!pModel->m_bSolidData) || (pModel->m_bPlainSolidModel))
		return;

	m_ProjTmp=GetProjMode();
	SetProjMode(ProjOrthographic);

	HC_Open_Segment_By_Key(pModel->GetModelKey());	
		HC_Set_Visibility("everything = off");
		HC_Set_Selectability("faces = on!");
	HC_Close_Segment();
 
 	float px,py,pz,tx,ty,tz; 
 	HC_Open_Segment_By_Key(GetSceneKey());		
		HC_Show_Net_Camera_Position(&px, &py, &pz);
 		HC_Show_Net_Camera_Target(&tx, &ty, &tz);
 	HC_Close_Segment();

	char visibility[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(GetSceneKey());
	HC_Show_Net_Visibility(visibility);
	HC_Close_Segment();

	if (px==m_pos.x && py==m_pos.y && pz==m_pos.z && 
		tx==m_tar.x && ty==m_tar.y && tz==m_tar.z && m_bhidden_exists)
	{
		HC_Open_Segment_By_Key(pModel->GetModelKey());
			HC_Open_Segment("main");
				HC_Set_Visibility(visibility);
			HC_Close_Segment();
		HC_Close_Segment();

		((HSSelectionSet *)GetSelection())->RefreshSolidSelection(); 	
	}
	else 
	{	  			 
 		UpdateHiddenLine();  	
 	}

  	m_bhidden_exists=false;
}

void HSolidView::DisableHiddenLine()
{
#ifdef ACIS
	OutputDebugString(_T("Precise Hidden Line not yet implemented in ACIS Part Viewer"));
	return;
#endif

	// do not do any precise hidden line fancy if we are in a collabrative session
	if( m_collab_mode )
		return;

	HSolidModel * pModel = (HSolidModel *)GetModel();
	assert(pModel);


	// do not use precise hidden line rendering if we do not have a solid model - Rajesh B
	if ( (!pModel->m_bSolidData) || (pModel->m_bPlainSolidModel))
		return;


 	int size = 0;

	//we need to rerender all the bodies in the current partition
#ifdef ACIS
  	ENTITY_LIST body; 
	body = pModel->GetEntityList();
	size = body.iteration_count();
#endif // ACIS

	if (size == 0)
		return;

 	SetProjMode(m_ProjTmp);
	
	HC_Open_Segment_By_Key(pModel->GetModelKey());
		HC_Open_Segment("main");
			HC_Set_Visibility("everything = off");
		HC_Close_Segment();
		HC_UnSet_Visibility();
		HC_UnSet_Selectability();
	HC_Close_Segment();

	((HSSelectionSet *)GetSelection())->RefreshSolidSelection(); 	
	m_bhidden_exists=true;
}



#ifdef ACIS

void  HSolidView::UpdateHiddenLine() 
{
	// TODO: precise hidden line implementation
	return;
}
#endif // ACIS


void Compute_Shell_Normal(HC_KEY skey, HPoint &tnormal)
{
	// if the key doesn't exist, normal doesn't either
	if (!HC_Show_Existence_By_Key(skey, "self"))
	{
		tnormal.Set(0,0,0);
		return;
	}

	char ktype[MVO_BUFFER_SIZE];
	HC_Show_Key_Type(skey,ktype);
	if (strcmp(ktype,"shell")!=0)
	{
		tnormal.Set(0,0,0);
		return;
	}
	
	
	int pcount,flen;
	HC_Show_Shell_Size(skey,&pcount,&flen);
	HPoint *points = new HPoint[pcount];
	int *flist = new int[flen];

	HC_Show_Shell(skey, &pcount, points, &flen, flist);	
	int flist_pos=0;
	unsigned short numfaces=0;
 	tnormal.Set(0,0,0);
 	while (flist_pos < flen)
	{
		HPoint fpoints[1000];
		int fpcount = flist[flist_pos++];

		for (int i=0;i<fpcount;i++)
		{
			fpoints[i].Set(points[flist[flist_pos]].x,points[flist[flist_pos]].y,points[flist[flist_pos]].z);
			flist_pos++;
		}
		HPoint normal;
 		HC_Compute_Polygon_Normal(fpcount,fpoints,"right",&normal);
		tnormal.x=tnormal.x+normal.x;
		tnormal.y=tnormal.y+normal.y;
		tnormal.z=tnormal.z+normal.z;
		numfaces++;
	}
	tnormal.x/=numfaces;
	tnormal.y/=numfaces;
	tnormal.z/=numfaces;
	
	if (fabs(tnormal.x)+fabs(tnormal.y)+fabs(tnormal.z)<0.1)
		tnormal.x = tnormal.y=tnormal.z = 0;
	else
 		HC_Compute_Normalized_Vector(&tnormal,&tnormal);

	if( flist )
		delete[] flist;
	if( points )
		delete[] points;
}
 

void HSolidView::FitWorldToHOOPSSelection()
{
	FitSelection();
}

#ifdef ACIS
// reset the camera to optimally view the current selection
void HSolidView::FitWorldToSelection()
{
	HPoint min,max;
	HPoint center;
	HPoint camera;
	HPoint target;
	HPoint viewingVector;
	HPoint diagonalVector;
	float diagonalLength;
	float tmp;

	HSSelectionSet* selection = (HSSelectionSet*) (GetSelection());
	int solid_size = selection->GetSolidListSize();

	if (!solid_size)
		return;
	ENTITY* entity = (ENTITY*) selection->GetAtSolidEntity(0);
	int eclass;
	eclass = entity->identity();
	if (eclass == VERTEX_TYPE)
	{
		return;
	}
	
	SPAposition min_pt;
	SPAposition max_pt;
	ENTITY_LIST elist;
	elist.add( entity );
	outcome o = api_get_entity_box( elist, NULL, min_pt, max_pt );
	CHECK_OUTCOME(o);

	min.x = (float)min_pt.x();
	min.y = (float)min_pt.y();
	min.z = (float)min_pt.z();
	max.x = (float)max_pt.x();
	max.y = (float)max_pt.y();
	max.z = (float)max_pt.z();
	if (min.x > max.x) {
		tmp = min.x; min.x = max.x; max.x = tmp; }
	if (min.y > max.y) {
		tmp = min.y; min.y = max.y; max.y = tmp; }
	if (min.z > max.z) {
		tmp = min.z; min.z = max.z; max.z = tmp; }

	HPoint normal;

	if (eclass == FACE_TYPE)
	{
		ENTITY_LIST vertices;
		normal.Set(0,0,0);
		int nvertices;
		outcome o = api_get_vertices( entity, vertices );
		nvertices = vertices.iteration_count();
		if (nvertices>2)
		{
			HPoint *p = new HPoint[nvertices];
			for (int j=0;j<nvertices;j++)
			{
				APOINT* point = ((VERTEX*)vertices[j])->geometry();
				SPAposition ps = point->coords();
				p[j].Set((float)ps.x(),(float)ps.y(),(float)ps.z());
			}
			
			HC_Compute_Polygon_Normal(nvertices,p,"right",&normal);
			delete p;
		}
		else
		{
			
			HC_KEY shell = selection->GetAt(0);
	 		Compute_Shell_Normal(shell,normal);
		}
	}
	
	for (int i=1;i<solid_size;i++)
	{	
		ENTITY* entity = (ENTITY*) selection->GetAtSolidEntity(i);
		
		ENTITY_LIST elist;
		elist.add( entity );
		outcome o = api_get_entity_box( elist, NULL, min_pt, max_pt );
		CHECK_OUTCOME(o);
		
		min_pt.x() < min.x ? min.x = (float)min_pt.x() : 0;
		min_pt.y() < min.y ? min.y = (float)min_pt.y() : 0;
		min_pt.z() < min.z ? min.z = (float)min_pt.z() : 0;
		max_pt.x() > max.x ? max.x = (float)max_pt.x() : 0;
		max_pt.y() > max.y ? max.y = (float)max_pt.y() : 0;
		max_pt.z() > max.z ? max.z = (float)max_pt.z() : 0;
	}
	
	HC_Open_Segment_By_Key (m_SceneKey);
	
	// compute the length of the bbox diagonal vector
	// this will be used to make the camera field a bit larger
	// to ensure that objects between the camera position and target
	// will still not get clipped (this is for perspective viewing mode)

	HPoint co,to,uo,cn,tn,un;
	float wo, ho, wn, hn;
	char proj[MVO_BUFFER_SIZE];

	HC_Show_Net_Camera(&co, &to, &uo, &wo, &ho, proj);

	diagonalVector.Set(
		max.x - min.x,
		max.y - min.y,
		max.z - min.z);

	diagonalLength = (float) sqrt(
		diagonalVector.x * diagonalVector.x +
		diagonalVector.y * diagonalVector.y +
		diagonalVector.z * diagonalVector.z);

	center.x = 0.5F * (min.x + max.x);
	center.y = 0.5F * (min.y + max.y);
	center.z = 0.5F * (min.z + max.z);

	HC_Show_Net_Camera_Position (&camera.x, &camera.y, &camera.z);
	HC_Show_Net_Camera_Target (&target.x, &target.y, &target.z);
  
	viewingVector.Set(
		target.x - camera.x,
		target.y - camera.y,
		target.z - camera.z);

	HC_Compute_Normalized_Vector (&viewingVector, &viewingVector);

	// reset camera position to lie on the viewing vector, at a distance of
	// 2.5 * diagonal length away from the center of the selection centroid
	HC_Set_Camera_Position(center.x - 2.5f * viewingVector.x * diagonalLength,
						   center.y - 2.5f * viewingVector.y * diagonalLength,
						   center.z - 2.5f * viewingVector.z * diagonalLength);

	// set the camera target to be the centroid of the selection
	HC_Set_Camera_Target (center.x, center.y, center.z);

	// set the camera field to be the diagonal length of the selections's bounding box
	HC_Set_Camera_Field (diagonalLength, diagonalLength);

	HC_Show_Net_Camera(&cn, &tn, &un, &wn, &hn, proj);

	if (eclass == FACE_TYPE && !(normal.x==0 && normal.y==0 && normal.z==0))
	{
		HPoint temp1;
		temp1.Set(cn.x-tn.x,cn.y-tn.y,cn.z-tn.z);
		float length = (float)HC_Compute_Vector_Length(&temp1);
		cn.x=tn.x+normal.x*length;
		cn.y=tn.y+normal.y*length;
		cn.z=tn.z+normal.z*length;
		HC_Set_Camera_Position(cn.x,cn.y,cn.z);
		HPoint p1,p2;
		p1.Set(cn.x-tn.x,cn.y-tn.y,cn.z-tn.z);
		p2.Set(co.x-to.x,co.y-to.y,co.z-to.z);
		float mat[4][4];
		HUtility::ComputeVectorToVectorRotationMatrix((float *)mat,p1,p2);
		HPoint un2;
		un2.Set(un.x,un.y,un.z);
		HC_Compute_Transformed_Points(1,&uo,(float *)mat,&un);
		
		HC_Set_Camera_Up_Vector(un.x,un.y,un.z);
	}

	if( GetSmoothTransition() )
		HUtility::SmoothTransition(co,to,uo, wo, ho, cn,tn, un, wn, hn, this);

	HC_Close_Segment();

	CameraPositionChanged( true, GetSmoothTransition());
}
#endif // ACIS

void HSolidView::SetCamera(int cameranum)
{
	HC_Open_Segment_By_Key (m_SceneKey);
	HC_Show_Net_Camera_Target (&cameras[cameranum].t.x, &cameras[cameranum].t.y, &cameras[cameranum].t.z);
	HC_Show_Net_Camera_Position (&cameras[cameranum].p.x, &cameras[cameranum].p.y, &cameras[cameranum].p.z);
	HC_Show_Net_Camera_Up_Vector (&cameras[cameranum].u.x, &cameras[cameranum].u.y, &cameras[cameranum].u.z);
	HC_Show_Net_Camera_Field(&cameras[cameranum].w,&cameras[cameranum].h);
	HC_Close_Segment();
	cameras[cameranum].active = true;
}
 
void HSolidView::MoveCamera(int cameranum)
{
	if (cameras[cameranum].active)
	{
		HC_Open_Segment_By_Key (m_SceneKey);
		HPoint co,to,uo;
		float wo, ho;
		char proj[MVO_BUFFER_SIZE];

		HC_Show_Net_Camera(&co, &to, &uo, &wo, &ho, proj);
		
		if( GetSmoothTransition() )
		{
			HUtility::SmoothTransition(co,to,uo, wo, ho, cameras[cameranum].p,cameras[cameranum].t,cameras[cameranum].u,cameras[cameranum].w, cameras[cameranum].h, this);
 			CameraPositionChanged(true, true);
		}
		else
		{
 			HC_Set_Camera_Target(cameras[cameranum].t.x,cameras[cameranum].t.y,cameras[cameranum].t.z);
 			HC_Set_Camera_Position(cameras[cameranum].p.x,cameras[cameranum].p.y,cameras[cameranum].p.z);
 			HC_Set_Camera_Up_Vector(cameras[cameranum].u.x,cameras[cameranum].u.y,cameras[cameranum].u.z);
 			CameraPositionChanged(true, false);
			Update();
		}

		HC_Close_Segment();
	}
}
 


void HSolidView::CameraPositionChanged(bool emit_message, bool smooth_camera_transition)
{

	// Let the edit class handle the task of 
	// reposition the lights which follow the camera
//	CEditLightsDlg::RepositionLights( this );
	// call the base class
	HBaseView::CameraPositionChanged( emit_message, smooth_camera_transition );
}


//======================================================================
//	Function :HSolidView::FitWorld
//	Description :	Overridden FitWorld. This will always fire CameraPositionChanged
//					with emit_message and smooth_transition set to true. So, this 
//					is now defacto for partviewers
//	Return :	
//	Parameters : HPoint
//	Note :
//======================================================================
void HSolidView::FitWorld(HPoint * center_out)
{
	HBaseView::FitWorld(center_out);
}


//======================================================================
//	Function :HSolidView::SelectFromMessage
//	Description :	Overridden this particular one. We might be in body
//					level selection while our master client might want
//					us to select a face. To get away with this, here's is
//					the intuitive strategy. I overrode this function to 
//					call my_sel->SelectFromMessage rather than my_sel->Select
//	Return :void	-	
//	Parameters :
//			const char *in_data	-	
//			unsigned int data_length	-	
//	Note :
//======================================================================
void HSolidView::SelectFromMessage(const char *in_data, unsigned int data_length)
{
    char data[MVO_BUFFER_SIZE];
	int isSelect = false;
	long ident = 0;
	HC_KEY key = 0;


    memcpy(data, in_data, data_length);
    data[data_length] = '\0';
	    
    sscanf(data, "%d %d", &isSelect, &ident);

	if (m_pSharedKey && (key = m_pSharedKey->GetKeyFromIdent(ident)))
	{
		HSSelectionSet* my_sel = (HSSelectionSet*) GetSelection();
		assert( my_sel );
		if (isSelect)
			my_sel->SelectFromMessage(key, "", INVALID_KEY, INVALID_KEY);
		else
			my_sel->DeSelectFromMessage(key);

		Update();
	}
}

//======================================================================
//	Function :		Flush
//	Description :	Override the base Flush to remove any solid modeler
//					objects present
//	Return :void	-	
//	Parameters :
//			bool emit_message=false	-	
//	Note :
//======================================================================
void HSolidView::Flush(bool emit_message)
{
	HSolidModel* model = (HSolidModel*) GetModel();
	assert(model);
	if( model )
		model->DeleteAllEntities();

	HBaseView::Flush(emit_message);
}

void HSolidView::FlushFromMessage(const char *in_data, unsigned int data_length)
{
	HSolidModel* model = (HSolidModel*) GetModel();
	assert(model);
	if( model )
		model->DeleteAllEntities();

	HBaseView::FlushFromMessage(in_data, data_length);
}


/*!
  Sets the visibility of "lines" on the scene key
  \param on_off
*/
void HSolidView::SetVisibilityLines(bool on_off)
{
    HC_Open_Segment_By_Key (GetSceneKey());
        if (on_off) 
            HC_Set_Visibility ("lines = on");
        else 
            HC_Set_Visibility ("lines = off");
    HC_Close_Segment ();

	SetGeometryChanged();
}

/*!
  returns the current visibility settings of lines on the scene key.
  \return bool
*/
bool HSolidView::GetVisibilityLines()
{
	char cval[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key (GetSceneKey());
		HC_Show_One_Net_Visibility("lines", cval);
    HC_Close_Segment ();
	if (strstr(cval,"on"))
		return true;
	else
		return false;
}
/*!
  returns the current visibility settings of lines on the scene key.
  \return bool
*/
bool HSolidView::GetVisibilityVertices()
{
	char cval[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key (GetSceneKey());
		HC_Show_One_Net_Visibility("vertices", cval);
    HC_Close_Segment ();
	if (strstr(cval,"on"))
		return true;
	else
		return false;
}

void HSolidView::SetVisibilityVertices(bool on_off)
{
    HC_Open_Segment_By_Key (GetSceneKey());
        if (on_off) 
            HC_Set_Visibility ("vertices = on");
        else 
            HC_Set_Visibility ("vertices = off");
    HC_Close_Segment ();

	SetGeometryChanged();
}

/*!
  returns the current visibility settings of lines on the scene key.
  \return bool
*/
bool HSolidView::GetVisibilityMarkersOnly()
{
	char cval[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key (GetSceneKey());
		HC_Show_One_Net_Visibility("markers only", cval);
    HC_Close_Segment ();
	if (strstr(cval,"on"))
		return true;
	else
		return false;
}

void HSolidView::SetVisibilityMarkersOnly(bool on_off)
{
    HC_Open_Segment_By_Key (GetSceneKey());
        if (on_off) 
            HC_Set_Visibility ("markers only = on");
        else 
            HC_Set_Visibility ("markers only = off");
    HC_Close_Segment ();

	SetGeometryChanged();
}


/*!
  Sets the visibility of "edges" on the scene key
  \param on_off
*/
void HSolidView::SetVisibilityEdges(bool on_off)
{
    HC_Open_Segment_By_Key (GetSceneKey());
        if (on_off) 
            HC_Set_Visibility ("edges = on");
        else 
            HC_Set_Visibility ("edges = off");
    HC_Close_Segment ();

	SetGeometryChanged();
}

/*!
  returns the current visibility settings of edges on the scene key.
  \return bool
*/
bool HSolidView::GetVisibilityEdges()
{
	char cval[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key (GetSceneKey());
		HC_Show_One_Net_Visibility("edges", cval);
    HC_Close_Segment ();
	if (strstr(cval,"on"))
		return true;
	else
		return false;
}

/*!
  Sets the visibility of "Faces" on the scene key
  \param on_off
*/
void HSolidView::SetVisibilityFaces(bool on_off)
{
    HC_Open_Segment_By_Key (GetSceneKey());
        if (on_off) 
            HC_Set_Visibility ("Faces = on");
        else 
            HC_Set_Visibility ("Faces = off");
    HC_Close_Segment ();

	SetGeometryChanged();
}

/*!
  returns the current visibility settings of Faces on the scene key.
  \return bool
*/
bool HSolidView::GetVisibilityFaces()
{
	char cval[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key (GetSceneKey());
		HC_Show_One_Net_Visibility("Faces", cval);
    HC_Close_Segment ();
	if (strstr(cval,"on"))
		return true;
	else
		return false;
}

/*!
  Sets the visibility of "Markers" on the scene key
  \param on_off
*/
void HSolidView::SetVisibilityMarkers(bool on_off)
{
    HC_Open_Segment_By_Key (GetSceneKey());
        if (on_off) 
            HC_Set_Visibility ("Markers = on");
        else 
            HC_Set_Visibility ("Markers = off");
    HC_Close_Segment ();

	SetGeometryChanged();
}

/*!
  returns the current visibility settings of Markers on the scene key.
  \return bool
*/
bool HSolidView::GetVisibilityMarkers()
{
	char cval[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key (GetSceneKey());
		HC_Show_One_Net_Visibility("Markers", cval);
    HC_Close_Segment ();
	if (strstr(cval,"on"))
		return true;
	else
		return false;
}

void HSolidView::SetShadowMode (HShadowMode shadowMode)
{
	// in collabration, you are only allowed to switch 
	// shadow off. You are not allowed to create one.
	if( m_collab_mode && (shadowMode != HShadowNone) )
		return;
	else
		HBaseView::SetShadowMode(shadowMode);
}

int HSolidView::ObjectCollisionEvent(HBhvTargetObject *tob)
{
	if (m_bShowCollisions)
	{	
		HC_Open_Segment("?style library");
		HC_Open_Segment("collisionstyle");
		HC_Flush_Contents(".", "everything");
		HC_Style_Segment("?Style Library/mvo_highlight_style_segment");
		HC_Close_Segment();
		HC_Close_Segment();
		HC_KEY stylekey;
		char stylepath[4096];
		HC_Open_Segment_By_Key(tob->GetTargetKey());
		HC_Begin_Contents_Search(".", "styles");
		
		while (HUtility::FindContents(stylekey))
		{
			HC_Show_Style_Segment(stylekey, stylepath);
			if (strstr(stylepath, "collisionstyle"))
				HC_Delete_By_Key(stylekey);
			
		}
		HC_End_Contents_Search();
		
		HC_Style_Segment("?Style Library/collisionstyle");
		HC_Close_Segment();
	}
	
	return HLISTENER_PASS_EVENT;
}

int HSolidView::ObjectNoCollisionEvent(HBhvTargetObject *tob)
{
 
	HC_KEY stylekey;
	char stylepath[4096];
	HC_Open_Segment_By_Key(tob->GetTargetKey());
	HC_Begin_Contents_Search(".", "styles");

	while (HUtility::FindContents(stylekey))
	{
		HC_Show_Style_Segment(stylekey, stylepath);
		if (strstr(stylepath, "collisionstyle"))
			HC_Delete_By_Key(stylekey);

	}
	HC_End_Contents_Search();
	HC_Close_Segment();
	
	return HLISTENER_PASS_EVENT;
}

void HSolidView::Update () {
#ifdef INTEROP
	((HSolidModel *)GetModel())->GetInterOpFileIOHandler()->Update();
//	m_pmi->Update();
#endif // INTEROP
	HBaseView::Update();
}
 

ClashItem * HSolidView::AddClash(HC_KEY k1, HC_KEY k2)
{
	ClashItem *item = new ClashItem(k1,k2);
	vlist_add_last(m_ClashList, item);
	return item;
}

void HSolidView::ClearClashList()
{
	if (m_ClashList)
	{
		START_LIST_ITERATION(ClashItem, m_ClashList)
			delete temp;
		END_LIST_ITERATION(m_ClashList);
		delete_vlist(m_ClashList);
	}
	m_ClashList = new_vlist(malloc,free);
}

void HSolidView::ClassifyClashes() 
{
	HC_Begin_Shell_Selection();
	HShellWrapper *myshell1 = 0;
	HC_KEY lastshell = -1;
	HPoint min,max;
	GetSceneExtents(min, max);
	HPoint delta;
	delta.Set((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f,
		(min.z + max.z) / 2.0f);
	float emargin = HC_Compute_Vector_Length(&delta) /5000.0f;
 	START_LIST_ITERATION(ClashItem, m_ClashList)


		float matrix[16];

		if (!temp->m_bHardClash)
		{
		if (lastshell!= temp->m_shell1)
		{
			delete myshell1;
			myshell1 = new HShellWrapper(temp->m_shell1);
			HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(temp->m_shell1));
			HC_Show_Net_Modelling_Matrix(matrix);
			HC_Close_Segment();
			HC_Compute_Transformed_Points(myshell1->m_plen, myshell1->m_Points,matrix,  myshell1->m_Points);
		}
		lastshell = temp->m_shell1;

		HShellWrapper myshell2(temp->m_shell2);
		HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(temp->m_shell2));
		HC_Show_Net_Modelling_Matrix(matrix);
		HC_Close_Segment();
		HC_Compute_Transformed_Points(myshell2.m_plen, myshell2.m_Points,matrix,  myshell2.m_Points);

		int *result2 = new int[myshell2.m_plen];
		float *result = new float[myshell2.m_plen];
		HC_Compute_Points_Inside_Shell(
			myshell2.m_plen, myshell2.m_Points,0,			
			myshell1->m_plen, myshell1->m_Points, myshell1->m_flen, myshell1->m_Flist, "tolerance = 0.00005",
			result2);
		
		HC_Compute_Points_Distance_From_Shell(
			myshell2.m_plen, myshell2.m_Points,
			emargin, 
			myshell1->m_plen, myshell1->m_Points, myshell1->m_flen, myshell1->m_Flist,"",
			result);
		bool hard = false;
		for (int i=0;i<myshell2.m_plen;i++)
		{
			if (result2[i] == -1 && result[i]<0 )
			{
				hard = true;
				break;
			}
		}
		temp->m_bHardClash = hard;

		delete [] result;
		delete [] result2;
		}

 		END_LIST_ITERATION(m_ClashList);
	HC_End_Shell_Selection();
	delete myshell1;
}

static int CZLevelCompare(void const * a, void const * b)
{
	HC_KEY za;
	HC_KEY zb;
	
	za = *(HC_KEY*)a;
	zb = *(HC_KEY*)b;

	int plen, flen;
	int plen2, flen2;
	HC_Show_Shell_Size(za, &plen, &flen);
	HC_Show_Shell_Size(zb, &plen2, &flen2);
 

	if(plen > plen2)
		return -1;
	if(plen < plen2)
		return 1;
 	return 0;
}

void HSolidView::CalculateSelectedClashes(bool doSoft, bool ExtraPrecision, HSelectionSet *tool, HSelectionSet *target)
{
	int numtools = 0;
	HC_KEY *toolshells;
	for (unsigned int i=0;i<tool->GetSize();i++)
	{
		char type[MVO_BUFFER_SIZE];
		HC_KEY key = tool->GetAt(i);
		HC_Show_Key_Type(key, type);
		if (strcmp(type, "shell") == 0)
			numtools++;
		else
		{
			HC_Open_Segment_By_Key(key);
			HC_Begin_Contents_Search("...", "shells");
			int num;
			HC_Show_Contents_Count(&num);
			HC_End_Contents_Search();
			HC_Close_Segment();
			numtools+=num;
		}
	}
	toolshells = new HC_KEY[numtools];
	numtools = 0;
	for (unsigned int i=0;i<tool->GetSize();i++)
	{
		char type[MVO_BUFFER_SIZE];
		HC_KEY key = tool->GetAt(i);
		HC_Show_Key_Type(key, type);
		if (strcmp(type, "shell") == 0)
 			toolshells[numtools++] = key;
 		else
		{
			HC_Open_Segment_By_Key(key);
			HC_Begin_Contents_Search("...", "shells");
			int num;
			HC_Show_Contents_Count(&num);
			for (int j=0;j<num;j++)
			{
				HUtility::FindContents(toolshells[numtools]);
				numtools++;
			}
			HC_End_Contents_Search();
			HC_Close_Segment();			
		}
	}

	HC_KEY tempsegmentkey = HC_KOpen_Segment("/tempcollision");
	HC_Flush_Contents(".", "everything");
	HC_Set_Selectability("faces = on");
	HC_KEY *targetowner;
	targetowner = new HC_KEY[target->GetSize()];


	for (unsigned int i=0;i<target->GetSize();i++)
	{
		float matrix[16];
		HC_KEY ownerkey = HC_KShow_Owner_By_Key(target->GetAt(i));
		targetowner[i] = ownerkey;
		HC_Open_Segment_By_Key(ownerkey);
		HC_Show_Net_Modelling_Matrix(matrix);
		HC_Close_Segment();
		HC_Open_Segment("");		
		HC_Move_By_Key(target->GetAt(i), ".");
		HC_Set_Modelling_Matrix(matrix);
		HC_Close_Segment();
	}
	HC_Close_Segment();
		

	CalculateClashes(doSoft, ExtraPrecision, tempsegmentkey, numtools, toolshells);
	delete [] toolshells;
	for (unsigned int i=0;i<target->GetSize();i++)
	{
		HC_Open_Segment_By_Key(targetowner[i]);
		HC_Move_By_Key(target->GetAt(i), ".");
		HC_Close_Segment();
	}
	HC_Delete_By_Key(tempsegmentkey);
	delete [] targetowner;
}

void HSolidView::CalculateClashes(bool doSoft, bool ExtraPrecision, HC_KEY startsegment, int shellnum, HC_KEY *shelllist) 
{
	
	int j;
	ClearClashList();

	HC_Begin_Shell_Selection();
	int snum = 0;
	float matrix[16];
	HC_KEY shellkey;
	HC_KEY *shells;
	if (shelllist)
	{
		shells = shelllist;
		snum = shellnum;
	}
	else
	{
		HC_Open_Segment_By_Key(GetModelKey());
		HC_Begin_Contents_Search("...", "shells");
		HC_Show_Contents_Count(&snum);
		shells = new HC_KEY[snum];
		snum = 0;
		while (HUtility::FindContents(shellkey))	
			shells[snum++] = shellkey;
		HC_End_Contents_Search();
		HC_Close_Segment();
	}
	qsort(shells, snum, sizeof(HC_KEY), CZLevelCompare);
	
	for (int i=0;i<snum;i++)
	{
		HShellWrapper myshell(shells[i]);
		HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(shells[i]));
		HC_Show_Net_Modelling_Matrix(matrix);
		HC_Close_Segment();
		HC_Compute_Transformed_Points(myshell.m_plen, myshell.m_Points,matrix,  myshell.m_Points);
		if (startsegment == INVALID_KEY)
			HC_Open_Segment_By_Key(GetViewKey());
		else
			HC_Open_Segment_By_Key(startsegment);
		HC_QSet_Heuristics (".", "no related selection limit" );
		int scount = HC_Compute_Selection_By_Shell("v", ".", myshell.m_plen, myshell.m_Points,
			myshell.m_flen, myshell.m_Flist);

		struct vlist_s * tempitemlist = new_vlist(malloc, free);

		for (j=0;j<scount;j++)
		{
			HC_KEY key2;
			int o1,o2,o3;
			HC_Show_Selection_Element(&key2, &o1,&o2,&o3);
			bool alreadydone = false;
			for (int k=0;k<i;k++)
			{
				if (key2 == shells[k])
				{
					alreadydone = true;
					break;
				}
			}
			if (shells[i] != key2 && !alreadydone)
			{
 				ClashItem *item = AddClash(shells[i],key2);
				vlist_add_last(tempitemlist, item);	
				if (doSoft)
					item->m_bHardClash = false;
				else
					item->m_bHardClash = true;
			}

			HC_Find_Related_Selection();
		}
 		
		if (doSoft)
		{
			scount = HC_Compute_Selection_By_Shell_With_Options("v", ".", myshell.m_plen, myshell.m_Points,
				myshell.m_flen, myshell.m_Flist,"tolerance = -1");

			for (j=0;j<scount;j++)
			{
				HC_KEY key2;
				int o1,o2,o3;
				HC_Show_Selection_Element(&key2, &o1,&o2,&o3);

				START_LIST_ITERATION(ClashItem, tempitemlist)
					if (temp->m_shell2 == key2)
						temp->m_bHardClash = true;
				END_LIST_ITERATION(tempitemlist);
				
	 
				HC_Find_Related_Selection();
			}
		}
 		HC_Close_Segment();
		
		delete_vlist(tempitemlist);
	}
	HC_End_Shell_Selection();
	if (!shelllist)
		delete [] shells;
	if (doSoft && ExtraPrecision)
		ClassifyClashes();
}


static FramerateMode FramerateCStringToMode(CString mode)
{
	if(mode=="Fixed Framerate")
		return FramerateFixed;
	else
		return FramerateTarget;
}
static HLRMode HLRCString2Enum(CString hlr)
{
	if(hlr == "Analytic")
		return AnalyticHiddenLine;
	else if(hlr == "Fast")
		return FastHiddenLine;
	else 
		return FakeHiddenLine;	
}

static DisplayListType DisplayListCString2Enum(CString dl)
{
	if(dl == "Geometry")
		return DisplayListGeometry;
	else if(dl == "Segment")
		return DisplayListSegment;
	else 
		return DisplayListOff;	
}

static CString FramerateModeToCString(FramerateMode mode)
{
	if(mode==FramerateFixed)
		return "Fixed Framerate";
	else
		return "Target Framerate";
}

static CString HLREnum2CString(HLRMode hlr)
{
	if(hlr == AnalyticHiddenLine)
		return "Analytic";
	else if(hlr == FastHiddenLine)
		return "Fast";
	else 
		return "Fake";
}

static CString DisplayListEnum2CString(DisplayListType dl)
{
	if(dl == DisplayListGeometry)
		return "Geometry";
	else if(dl == DisplayListSegment)
		return "Segment";
	else 
		return "Off";
}

static CString MultiThreadingEnum2CString(MultiThreadingType mt)
{
	if(mt == MultiThreadingFull || mt == MultiThreadingBasic)
		return "Full";
	else 
		return "Off";
}

static MultiThreadingType MultiThreadingCString2Enum(CString mt)
{
	if(mt == "Full")
		return MultiThreadingFull;
	else if(mt == "Basic")
		return MultiThreadingBasic;
	else 
		return MultiThreadingOff;	
}
