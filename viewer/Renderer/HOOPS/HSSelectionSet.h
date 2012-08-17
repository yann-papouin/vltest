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
// $Header: /files/homes/master/cvs/hoops_master/shared/partviewer_common/HSSelectionSet.h,v 1.28 2009-04-13 18:58:33 nathan Exp $
//

// HPSelectionSet.h : interface of the HPSelectionSet class, derived from HSelectionSet
// adds app-specific overlooading of selection functions to support PK_face, PK_body and
// PK_egde level selection/highlighting

#ifndef _HSSELECTIONSET_H
#define _HSSELECTIONSET_H

#include "HSelectionSet.h"
#include "HSInclude.h"


class HSSelectionSet :  public HSelectionSet 
{

protected:

	KERN_CLASS_TYPEDEF	m_SelectLevel;

	struct vlist_s *m_pSolidSelection;
	bool m_bShowFacesAsLines;

public:
	HSSelectionSet(HBaseView* view, bool bReferenceSelection = false);
	~HSSelectionSet();

	// overloaded virtuals
	virtual void Init();
	virtual void Select(HC_KEY key, const char* segpath, HC_KEY include_key, HC_KEY includer_key, bool emit_message = true) = 0;

	virtual void Select(long entity, bool emit_message = true){ assert(0); }
	virtual	void DeSelectEntity(long entity, bool emit_message = true){ assert(0); }

	virtual void SelectFromMessage(HC_KEY key, char* segpath, HC_KEY include_key, HC_KEY includer_key, bool emit_message = false);
	virtual void DeSelectFromMessage(HC_KEY key, bool emit_message = false);

	void SetSelectLevel(KERN_CLASS_TYPEDEF level) {
#ifdef HOOPS_ONLY
		if(level == SUBENTITY_TYPE)
			SetSelectionLevel(HSelectSubentity);
		else if(level == SEGMENT_TYPE)
			SetSelectionLevel(HSelectSegment);
		else
			SetSelectionLevel(HSelectEntity);
#endif
		m_SelectLevel = level;
	}
	KERN_CLASS_TYPEDEF GetSelectLevel() {
#ifdef HOOPS_ONLY
		if(GetAllowEntitySelection() == true)
			return ENTITY_TYPE;
		if(GetAllowSubentitySelection() == true)
			return SUBENTITY_TYPE;
#endif
		return m_SelectLevel;
	}


#ifdef ACIS
	virtual void Select(ENTITY* entity, bool emit_message = true){ assert(0); }
	virtual	void DeSelectEntity(ENTITY* entity, bool emit_message = true){ assert(0); }
#endif // ACIS



    virtual void DeSelect(HC_KEY key, bool emit_message = true) = 0;

	void DeSelectAll ();
    virtual bool IsSelected(HC_KEY key) = 0;
	void Reset();
	
	virtual long GetAtSolidEntity(int index);
    virtual int  GetSolidListSize();
	virtual void RefreshSolidSelection() = 0;
	bool GetFacesAsLines() {return m_bShowFacesAsLines; }
	void SetFacesAsLines(bool facesaslines) {m_bShowFacesAsLines = facesaslines; }

	
};

#endif





