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
// $Header: /files/homes/master/cvs/hoops_master/acispartviewer/HSAcisSelectionSet.h,v 1.10 2008-11-26 21:18:04 guido Exp $
//

#ifndef _HSACISSELECTIONSET_H__RAJESH_B__20010203__0953__
#define _HSACISSELECTIONSET_H__RAJESH_B__20010203__0953__

class component_handle;

class HSAcisSelectionSet :  public HSSelectionSet 
{
public:

	HSAcisSelectionSet(HBaseView* view);
 
	// overloaded functions which handle instancing
	virtual void Select(HC_KEY key, int num_include_keys, HC_KEY * include_keys, bool emit_message);
	virtual void DeSelect(HC_KEY key, int num_include_keys, HC_KEY * include_keys, bool emit_message);
	virtual bool IsSelected(HC_KEY key, int num_include_keys, const HC_KEY * include_keys) const;

	// ENTITY select functions
	void Select(ENTITY* entity, bool emit_message = true);
	void DeSelectEntity(ENTITY* entity, bool emit_message = true);
	bool IsSelected(ENTITY* pEntity);

	// component_handle selection functions
	void Select(component_handle* component, bool emit_message = true);
	void DeSelectComponent(component_handle* component, bool emit_message = true);
	bool IsSelected(component_handle* pComponent);

	// deprecated old select functions
	virtual void Select(HC_KEY key, const char* segpath, HC_KEY include_key, HC_KEY includer_key, bool emit_message = true);
	virtual void DeSelect(HC_KEY key, bool emit_message = true);
	virtual bool IsSelected(HC_KEY key);

	virtual void RefreshSolidSelection();
	bool m_bMultiSelectActive;
 	
};

#endif	// _HSACISSELECTIONSET_H__RAJESH_B__20010203__0953__



