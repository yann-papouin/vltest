#include "stdafx.h"

#include "HSolidModel.h"
#include "ha_bridge.h"
#include "InterOp/InterOpFileIO.h"
#include "vhash.h"
#include "vlists.hxx"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif


HSolidModel::HSolidModel()
#ifdef ACIS
			: m_modelList(ASM_NO_HOLD)
#endif // ACIS
{
	m_bSolidModel = false;
	m_bSolidData = false;
	m_bPlainSolidModel = false;
	SetBRepGeometry(false);

	HC_Define_System_Options("license = (customer = spatial corp., product = (im, kanji, classic), key = 125A9FEC-EC764B7-5B58077F-3F5C7F)");


#ifdef ACIS
	mhash_CompHandle_2_EEList = 0;
	curr_hs = &model_hs;
	SetHistoryStream();
#endif // ACIS

#ifdef INTEROP
	iop_fio = new InterOpFileIO(this);
#endif // INTEROP
}

HSolidModel::~HSolidModel()
{
	// Model cleanup : delete all the entities from the 
	// partition associated with this model
	DeleteAllEntities();
#ifdef INTEROP
	delete iop_fio;
#endif // INTEROP
}

//======================================================================
//	Function :		HSolidModel::DeleteAllEntities
//	Description :	Deletes all the entities in the current model from the 
//					modeller and associated HOOPS geometry
//	Return :void	-	
//	Parameters :
//	Note :
//======================================================================
void HSolidModel::DeleteAllEntities()
{
#ifdef ACIS
	HA_Delete_Entity_Geometry(m_entityList);

	SetHistoryStream();

	DELTA_STATE *ds;
	outcome o = api_note_state(ds);
	CHECK_OUTCOME(o);

	o = api_del_entity_list(m_entityList);
	CHECK_OUTCOME(o);

	o = api_note_state(ds);
	CHECK_OUTCOME(o);

	m_entityList.clear();

	if(mhash_CompHandle_2_EEList)
	{
		vlist_s *temp_list = new_vlist(malloc,free);
		vhash_to_vlist(mhash_CompHandle_2_EEList, temp_list, malloc);
		vhash_pair_t * handle_list_pair;
		while(handle_list_pair = (vhash_pair_t *) vlist_remove_first(temp_list)) 
		{
			EE_LIST *model_refs = (EE_LIST *)handle_list_pair->item;
			if(model_refs)
				api_del_entity(model_refs);
		}

		delete_vhash(mhash_CompHandle_2_EEList);
		delete_vlist(temp_list);
		mhash_CompHandle_2_EEList = 0;
	}
#endif // ACIS
}

#ifdef ACIS
EE_LIST* HSolidModel::GetEEListForComponent(component_handle* comp_handle)
{
	EE_LIST* ret_eelist = 0;
	if( !mhash_CompHandle_2_EEList )
		mhash_CompHandle_2_EEList = new_vhash(512, malloc, free);

	if( VHASH_STATUS_SUCCESS != vhash_lookup_item(mhash_CompHandle_2_EEList, 
												  comp_handle, (void**) &ret_eelist) )
	{
		// create a EE_LIST of ASM_MODEL_REFs
		outcome o;

		asm_model* root_model = 0;
		entity_handle_list model_refs;
		o = asmi_component_get_root_model(comp_handle, root_model);
		o = asmi_component_get_path(comp_handle, root_model, model_refs);
// #ifdef DEBUG_NEW
// #undef new DEBUG_NEW
		ret_eelist = ACIS_NEW EE_LIST();
// #define new DEBUG_NEW
// #endif
		ret_eelist->set_ownership(false);
		ret_eelist->init();
		model_refs.init();
		for (entity_handle* ent_handle = model_refs.first(); 
			ent_handle != NULL; ent_handle = model_refs.next())
		{
			ENTITY* ent = ent_handle->entity_ptr();
			ret_eelist->add(ent);
		}

		vhash_insert_item(mhash_CompHandle_2_EEList, (void*) comp_handle, (void*)ret_eelist);
	}

	return ret_eelist;
}
#endif	// ACIS


#ifdef ACIS
void HSolidModel::SetHistoryStream ( HISTORY_STREAM * new_hs ) {
	outcome o;
	if (new_hs && new_hs != curr_hs) {
		o = api_set_default_history(new_hs);
		CHECK_OUTCOME(o);
		curr_hs = new_hs;
	}
	else {
		o = api_set_default_history(curr_hs);
		CHECK_OUTCOME(o);
	}
}

void HSolidModel::DeleteAcisEntity( ENTITY* entity)
{
	ENTITY_LIST elist;
	elist.add(entity);
	HA_Delete_Entity_Geometry(elist);
	// delete the ACIS body
	api_del_entity(entity);
	m_entityList.remove(entity);
}
void HSolidModel::AddAcisEntity( ENTITY* entity)
{
	m_entityList.add(entity);
}
void HSolidModel::RemoveAcisEntity( ENTITY* entity)
{
	m_entityList.remove(entity);
}
#endif // ACIS


#ifdef ACIS
logical HSolidModel::HA_Read_Asat_File(const char* fname, asm_model_list& model_list)
{
	FILE * fp = NULL;
	ENTITY* entity = 0;
	outcome oc;
	logical res = TRUE;

	fp = fopen(fname, "r");
	res = (fp != NULL);

	if (res)
	{
		entity_mgr_factory* pEntity_Mgr_Factory;
		outcome o = asmi_get_entity_mgr_factory(pEntity_Mgr_Factory);
		check_outcome(o);

		// Retrieve a list of all models from file
		asm_restore_options ropts;
		ropts.set_entity_mgr_factory( pEntity_Mgr_Factory );
		oc =  asmi_restore_model_list(fp, TRUE, model_list, &ropts);
		res = oc.ok();
	}

	if (fp)
		fclose(fp);

	return res;
}

#endif // ACIS


// our application-specific read function
HFileInputResult HSolidModel::Read(const wchar_t * FileName,
								   HBaseView * pHView,
								   bool clean_include,
								   HInputHandlerOptions * options) 
{   
	// This process is going to take sometime, convey that to the user
	CWaitCursor show_hourglass_cursor_through_this_function;

	HFileInputResult success = InputOK;
	wchar_t extension[64];
	wchar_t const * ext = wcsrchr(FileName, L'.');

	if(!ext)
		return InputBadFileName;

	++ext; //move one past the dot;

	// in case we have a Pro/E file which ends with a number, e.g., *.prt.1 or *.xas.1
	bool good_filename = true;
	wchar_t *filename_copy = 0;
	if ( good_filename && ext[0] >= L'0' && ext[0] <= L'9' ) { 
		if (filename_copy == 0) {
			int original_filename_len = (int)wcslen(FileName);
			filename_copy = new wchar_t [original_filename_len + 1];
			swprintf(filename_copy, original_filename_len, L"%ls", FileName);
			filename_copy[original_filename_len] = '\0';
		}
		wchar_t *bad_ext = wcsrchr(filename_copy, L'.');
		bad_ext[0] = '\0';
		ext = wcsrchr(filename_copy, L'.');
		if (!ext)
			good_filename = false;
		else
			ext++;
	}

	if (ext)
		swprintf(extension, 64, L"%ls", ext);

	if (filename_copy)
		delete [] filename_copy;

	if (!good_filename)
		return InputBadFileName;


	HC_Open_Segment_By_Key(GetModelKey());	
	// we should maybe read ach file into it's own subsegment of the main model segment 
	// however I am keeping with reading it into the modelkey directly so that reading
	// and writing out a file doesn't result in nesting occuring.  Gavin
#ifdef INTEROP
	if (wcsieq(extension, L"catpart") || wcsieq(extension, L"catproduct") || wcsieq(extension, L"cgr") ||
		wcsieq(extension, L"model")   || wcsieq(extension, L"session")    || wcsieq(extension, L"exp") ||
		wcsieq(extension, L"prt")     || wcsieq(extension, L"xpr")        ||
		wcsieq(extension, L"asm")     || wcsieq(extension, L"xas")        ||
		wcsieq(extension, L"igs")     || wcsieq(extension, L"iges")       ||
		wcsieq(extension, L"stp")     || wcsieq(extension, L"step")       ||
		wcsieq(extension, L"vda")     ||
		wcsieq(extension, L"sldprt")  || wcsieq(extension, L"sldasm")	  ||
		wcsieq(extension, L"ipt")	  || wcsieq(extension, L"iam")
#ifdef ACIS
		|| wcsieq(extension, L"x_t") || wcsieq(extension, L"x_b")
#endif // ACIS
		) {
			m_bSolidModel = true;
			m_bSolidData = true;
			SetBRepGeometry(true);
			SetModelHandedness(HandednessSetLeft);
#ifdef ACIS
			DELTA_STATE *ds;
			outcome o = api_note_state(ds);
			CHECK_OUTCOME(o);
#endif // ACIS
			success = iop_fio->Import(FileName, extension, (HBaseView *)pHView);
#ifdef ACIS
			o = api_note_state(ds);
			CHECK_OUTCOME(o);

			SetHistoryStream();
			o = api_note_state(ds);
			CHECK_OUTCOME(o);
#endif // ACIS
	}
#endif // INTEROP
#ifdef ACIS
#ifdef INTEROP
	else
#endif // INTEROP
		if  (!wcsnicmp(extension, L"sat", wcslen(L"sat")))
		{  
			ENTITY_LIST elist;
			ENTITY* entity;

			m_bSolidModel = true;
			m_bSolidData = true;
			SetBRepGeometry(true);
			SetModelHandedness(HandednessSetLeft);

			logical read_sat_result = HA_Read_Sat_File(H_ASCII_TEXT(FileName), elist);	// read an SAT file
			if( read_sat_result == FALSE )
			{
				AfxMessageBox(_T("Failed to read SAT file. The file may be corrupt or from later version of ACIS."));
				success = InputFail;
			}

			elist.init();						// set next() function to return first element in "list"
			while (entity = elist.next())
			{
				m_entityList.add(entity);
			}
		} 
		else if( !wcsnicmp(extension, L"asat", wcslen(L"asat")) )
		{  
			m_bSolidModel = true;
			m_bSolidData = true;
			SetBRepGeometry(true);
			SetModelHandedness(HandednessSetLeft);

			logical read_asat_result = HA_Read_Asat_File(H_ASCII_TEXT(FileName), m_modelList);
			if( read_asat_result == FALSE )
			{
				AfxMessageBox(_T("Failed to read ASAT file. The file may be corrupt or from later version of ACIS."));
				success = InputFail;
			}
			else
			{
				// render the entities
				m_modelList.init();
				asm_model* this_model = m_modelList.next();
				while (this_model)
				{
					MODEL_BEGIN(this_model)
						HA_Render_Model(this_model, "entity");
					this_model->get_top_level_entities(m_entityList, TRUE);
					HA_Render_Entities(m_entityList);
					MODEL_END(ASM_NO_CHANGE)

					this_model = m_modelList.next();
				}
			}

		} 

#endif // ACIS
#ifdef HOOPS_ONLY
		if(	wcsieq(extension, L"") )
		{
			AfxMessageBox(_T("Failed to read file. Unrecognized file extension."));
		}
#endif // HOOPS_ONLY			
		else
		{
			// we don't know what extensions the base class is capable of handling
			// let it try and read the file whatever the extension might be

//#ifndef HOOPS_ONLY				
//			if (wcsieq(extension, _T(KERN_STREAM_FILE)))
//				m_bSolidModel = true;
//			else
//#endif
			m_bSolidModel = true;

			SetBRepGeometry(false);
			if (wcsieq(extension, L"dwg") || wcsieq(extension, L"dxf")|| wcsieq(extension, L"dgn"))
			{
				//HInputHandlerOptions options;
				//wchar_t * p_dwg_textures_dir = 0;
				//if(CAppSettings::DWGTextureDirectory.IsEmpty() == FALSE)
				//{
				//	H_WCS wcs(CAppSettings::DWGTextureDirectory);
				//	p_dwg_textures_dir = wcsdup(wcs.encodedText());
				//}
				//options.m_pHBaseView = view;
				//options.m_pExtendedData = (void*) p_dwg_textures_dir;
				//options.m_bInputViaHSF = CAppSettings::bImportDWGviaBstream;
				//options.m_Deviation = atof(H_ASCII_TEXT(CAppSettings::DWGDeviation));
				//options.m_bImportMetadata = CAppSettings::bImportDWGXData;
				//options.m_bMergeEntityShells = true;
				//options.m_bImportAllLayouts = CAppSettings::bImportAllLayouts;

				////For DGN
				//options.m_bIgnoreOffLayer = CAppSettings::bIgnoreInvisibleLayers;
				//options.m_bIgnoreOffEntity = CAppSettings::bIgnoreInvisibleEntity;
				//options.m_coneFactor = atof(H_ASCII_TEXT(CAppSettings::DGNFactorForCone));
				//options.m_factorForSmartSolids = atof(H_ASCII_TEXT(CAppSettings::DGNFactorForSmartSolids ));

				//success = HBaseModel::Read(FileName, 0, true, &options);

				//if(p_dwg_textures_dir)
				//	free(p_dwg_textures_dir);
			}
			else
			{					
				//HInputHandlerOptions opt;
				//// PRC options
				//opt.m_pPRCPMIConnector = m_pPMIConnector;
				//opt.m_pPRCTopologyManager = m_TopologyManager;
				//opt.m_bPRCMergeFaces = !CAppSettings::bPrcFaceEdgeVertexSelectability;
				//opt.m_pPRCMultiModelSelectionCallback = MultiModelSelectionCallback;

				//// Other options
				//opt.m_pConnector = m_pConnector;
				//opt.m_tk = GetStreamFileTK();
				//opt.m_pHBaseView = view;
				//opt.m_bImportMetadata = CAppSettings::bImportDWGXData;;
				success = HBaseModel::Read(FileName/*, view, true, &opt*/);
				/*m_pPRCAsmModelFile = opt.m_pPRCAsmModelFile;
				HObjectManager::FileLoadHObjectCheck(GetModelKey());
				if (opt.m_bPRCWrongVersion)
				{
					MessageBox(0, H_TEXT(L"PRC Import requires functionality that is not available in your version of Acrobat.\nPlease update to the latest version of Acrobat to view this file."), _T("ERROR!"), MB_OK);
					success = InputOK;
				}*/
			}
		}
		// update the polygon handedness for this model
		//UpdateModelHandedness();


		HC_Close_Segment();
#if 0
		if( m_bSolidModel )
			GenerateLODsForBodies();
#endif
		return success;
}

//======================================================================
//	Function :		HSolidModel::SetModelHandedness
//	Description :	Sets the given modelhandedness on to the model key
//					This handedness get hardcoded in hmf/hsf files when
//					you save it.
//	Return :void	-	
//	Parameters :
//			ModelHandedness handedness	-	
//	Note :
//======================================================================
void HSolidModel::SetModelHandedness(ModelHandedness handedness)
{
	m_model_handedness = handedness; 

	HC_Open_Segment_By_Key(GetModelKey());

	if( m_model_handedness == HandednessSetLeft )
		HC_Set_Heuristics("polygon handedness = left");
	else if( m_model_handedness == HandednessSetRight )
		HC_Set_Heuristics("polygon handedness = Right");
	else if( m_model_handedness == HandednessSetNone )
		HC_Set_Heuristics("no polygon handedness");

	HC_Close_Segment();

}



//======================================================================
//	Function :		HSolidModel::SetBRepGeometry
//	Description :	Override the base class function to add setting of
//				user value on the segment. This helps in HNet, when we
//				emit this segment the peers understand our rendermode
//				setting better.
//	Return :void	-	
//	Parameters :
//			bool brep	-	
//	Note :
//======================================================================
void HSolidModel::SetBRepGeometry(bool brep)
{
	if (brep)
	{
		HC_KEY key = GetModelKey();
		HC_Open_Segment_By_Key(key);
		HC_Set_User_Value(1L);
		HC_Close_Segment();
	}

	HBaseModel::SetBRepGeometry(brep);
}