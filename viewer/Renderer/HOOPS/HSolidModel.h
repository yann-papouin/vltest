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
// $Header: /files/homes/master/cvs/hoops_master/shared/partviewer_common/HSolidModel.h,v 1.48 2009-07-22 18:46:59 guido Exp $
//

// HSolidModel.h : interface of the HParaModel class, derived from HBaseModel
// Adds application-specific data and members for each model

#ifndef _HSOLIDMODEL_H
#define _HSOLIDMODEL_H

#include "HDB.h"
#include "HBaseModel.h"
#include "HTools.h"
//#include "CAppSettings.h"
// #include "HStream.h"
#include "HSInclude.h"
//#include "HSUtility.h"
#ifdef ACIS
#include "ha_bridge.h"
#include "asm_model_list.hxx"
class asm_model;
class EE_LIST;
class component_handle;
class HISTORY_STREAM;
#endif // ACIS

#ifdef INTEROP
class InterOpFileIO;
#endif // INTEROP

class HUtilityXMLGenerator;
class HUtilityXMLTag;
class HIOConnector;

// ModelHandedness
enum ModelHandedness
{
	HandednessSetLeft,
	HandednessSetRight,
	HandednessSetNone,
	HandednessNotSet
};

class HSolidModel : public HBaseModel
{
private:
#ifdef ACIS
	ENTITY_LIST m_entityList;		// list of entities (bodies) in this model
	asm_model_list m_modelList;		// list of the top-level ACIS models in ASAT file

	// stores a hash map of component_handle* vs. EE_LIST (which is the list of ASM_MODEL_REFs 
	// for the component.
	struct vhash_s *mhash_CompHandle_2_EEList;

	HISTORY_STREAM model_hs;		// default HISTORY_STREAM for this model
	HISTORY_STREAM *curr_hs;		// pointer to the current HISTORY_STREAM
#endif // ACIS
#ifdef INTEROP
	InterOpFileIO *iop_fio;
#endif // INTEROP

	ModelHandedness m_model_handedness;

public:
	HSolidModel();
	~HSolidModel();
	virtual HFileInputResult Read(const __wchar_t * FileName, HBaseView * pHView=0, bool clean_include=true, HInputHandlerOptions * options = 0);

#ifdef ACIS
	HISTORY_STREAM * GetHistoryStream () { return curr_hs; }
	void SetHistoryStream ( HISTORY_STREAM * new_hs = 0 );
	
	void DeleteAcisEntity( ENTITY* entity);
	void AddAcisEntity( ENTITY* entity);
	void RemoveAcisEntity( ENTITY* entity);

	ENTITY_LIST& GetEntityList() { return m_entityList; }
	asm_model_list& GetModelList() { return m_modelList; }

	EE_LIST* GetEEListForComponent(component_handle* comp_handle);

	logical HA_Read_Asat_File(const char* fname, asm_model_list& model_list);
	asm_model* HSolidModel::get_asm_model(void);
#endif // ACIS


#ifdef INTEROP
	InterOpFileIO * GetInterOpFileIOHandler () { return iop_fio; }
#endif // INTEROP

	void DeleteAllEntities();

	bool	m_bSolidModel;
	bool	m_bSolidData;
	bool	m_bPlainSolidModel;
	ModelHandedness GetModelHandedness() { return m_model_handedness; }
	void SetModelHandedness(ModelHandedness handedness);

	virtual void SetBRepGeometry(bool brep);

};


#endif // _HSOLIDMODEL_H




