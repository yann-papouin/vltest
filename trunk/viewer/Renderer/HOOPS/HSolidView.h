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
// $Header: /files/homes/master/cvs/hoops_master/shared/partviewer_common/HSolidView.h,v 1.55 2009-08-28 20:31:17 nathan Exp $
//

// HSolidView.h : interface of the HSolidView class, derived from HBaseView
// Adds application-specific data and members for each view

#ifndef _HSOLIDVIEW_H
#define _HSOLIDVIEW_H

#include "HBaseView.h"
#include "HUtility.h"

#ifdef ACIS
#include "ha_bridge.h"
#endif // ACIS
class HSSelectionSet;

class ClashItem
{
public:
	ClashItem(HC_KEY s1, HC_KEY s2)
	{
		m_shell1 = s1;
		m_shell2 = s2;
		m_bHardClash = true;
	}
	HC_KEY m_shell1;
	HC_KEY m_shell2;
 	bool m_bHardClash;
};

class CameraPos
{
public:
	HPoint p,t,u;
	float w, h;
	bool active;

	CameraPos() { active = false; }
};

class HSolidView : public HBaseView, public HAnimationListener
{
private:
	bool		m_optimize_shells;
	HPoint		m_pos,m_tar;
 	bool		m_bhidden_exists;
	bool		m_bUseLOD;
	void		RegenerateBodies(void);
	ProjMode	m_ProjTmp;
	CameraPos	cameras[10];

	bool		m_collab_mode;
	bool		m_bShowCollisions;

public:

	HSolidView(HBaseModel *model,
		const char * 		alias = 0,	
		const char *		driver_type = 0,
		const char *		instance_name = 0,
		void *		window_handle = 0,
		void *		colormap = 0);
	~HSolidView();

	void	Init();
	
 	void  SetCamera(int cameranum);
	void  MoveCamera(int cameranum);

	HSSelectionSet* GetSolidSelection() { return (HSSelectionSet*)m_pSelection; }
	void	DeleteSelectionList(bool emit_message=false);

	void	SetOptimizations(bool optimize, bool no_update = false);
	bool	GetOptimizations(void);

	void	UpdateHiddenLine(); 
	void	EnableHiddenLine();
	void	DisableHiddenLine();
 	void	FitWorldToSelection();
	void	FitWorldToHOOPSSelection();
	void	ClassifyClashes();	

	virtual void Update();
	virtual void CameraPositionChanged(bool emit_message=false, bool smooth_camera_transition=false); 
	virtual void FitWorld(HPoint * center_out = 0);

	virtual void SelectFromMessage(const char *in_data, unsigned int data_length);
	virtual void Flush(bool emit_message=false);
	virtual void FlushFromMessage(const char *in_data, unsigned int data_length);

	virtual void SetVisibilityLines(bool on_off);
	virtual bool GetVisibilityLines();

	virtual void SetVisibilityEdges(bool on_off);
	virtual bool GetVisibilityEdges();
	virtual void SetVisibilityFaces(bool on_off);
	virtual bool GetVisibilityFaces();
	virtual void SetVisibilityMarkers(bool on_off);
	virtual bool GetVisibilityMarkers();

	virtual void SetVisibilityVertices(bool on_off);
	virtual bool GetVisibilityVertices();

	virtual void SetVisibilityMarkersOnly(bool on_off);
	virtual bool GetVisibilityMarkersOnly();

	virtual void SetShadowMode (HShadowMode);	

	int ObjectCollisionEvent(HBhvTargetObject *tob);
	int ObjectNoCollisionEvent(HBhvTargetObject *tob);


#ifdef ACIS
	ENTITY* Compute_Selected_Instance();
	bool FindBox(ENTITY* entity, SPAbox* box);
#endif // ACIS

	void InitSelection(HSSelectionSet *ss);
	void SetShowCollisions(bool sc) { m_bShowCollisions = sc; }
	bool GetShowCollisions() { return m_bShowCollisions; }
	

	HC_KEY		*m_pSweetenKeyList;	// keys to HOOPS entities that need to be sweetened.  Should be private but public for the moment

	void SetCollabrationMode(bool true_or_false){ m_collab_mode = true_or_false; }
	bool GetCollabrationMode(){ return m_collab_mode; }
	
	ClashItem* AddClash(HC_KEY key1, HC_KEY key2);
	void CalculateSelectedClashes(bool doSoft, bool ExtraPrecision, HSelectionSet *tool, HSelectionSet *target);
	void CalculateClashes(bool doSoft, bool ExtraPrecision, HC_KEY startsegment = INVALID_KEY, int numshells = 0, HC_KEY *shelllist = 0);
	void ClearClashList();
	struct vlist_s*			m_ClashList;	

	//for default view operator
	HBaseOperator* m_pViewOperator;
	virtual void SetViewOperator(HBaseOperator *op) { m_pViewOperator = op; }
	virtual HBaseOperator *GetViewOperator() { return m_pViewOperator; }	
};

#endif 




