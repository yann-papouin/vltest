// standard includes 
#include "stdafx.h"


#include "InterOpFileIO.h"
#include <string.h>
#include <fstream>
#include <string>
#include <process.h>

// InterOp includes
#include "SPAIConverter.h"
#include "SPAIDocument.h"
#include "SPAIDocumentAssemblyInfo.h"
#include "SPAIDocumentBRepInfo.h"
#include "SPAIDocumentHeaderInfo.h"
#include "SPAIFile.h"
#include "SPAIOptions.h"
#include "SPAIOptionName.h"
#include "SPAIProgressCallback.h"
#include "SPAIUnit.h"
#include "SPAIValue.h"
#include "HSInclude.h"

#ifdef ACIS 
#include "SPAIAcisDocument.h"
#endif // ACIS

// HOOPS includes
//   utilities
#include "vhash.h"
#include "varray.h"
//   MVO
#include "HUtilityLocaleString.h"
//   App level

#include "HSolidModel.h"
#include "GenViewerView.h"
//#include "InterOpAsmImportDlg.h"
#include "InterOpFileTranslationProgressDlg.h"
#include "InterOpImportDlg.h"
#include "InterOpAsmImportDlg.h"

// modeling kernel includes
#ifdef ACIS
#include "transf.hxx"
#include "matrix.hxx"
#include "entity_handle.hxx"
#include "entity_handle_list.hxx"
#include "comp_handle.hxx"
#include "asm_cleanup_api.hxx"
#include "asm_model.hxx"
#include "asm_api.hxx"
#include "asm_assembly.hxx"
#include "acis_ha_part_entity_mgr.hxx"
#include "entity_mgr_factory.hxx"
#include "default_entity_mgr.hxx"
#include "ha_bridge.h"
#include "ha_bridge_asm.h"
#include "ha_rend_options.h"
#include "ckoutcom.hxx"
#endif // ACIS

/**********************************************************************************/

// static member variable initialization
void * InterOpFileIO::progress_callback = NULL;
bool InterOpFileIO::init_trans_call = true;
const wchar_t * InterOpFileIO::curr_trans_file = NULL;

/**********************************************************************************/

PartCommonInfo::PartCommonInfo () {
	path_name = NULL;
#ifdef ACIS
	elist = NULL;
#endif // ACIS
	is_asm = false;
	missing = false;
	needs_pmi = true;
	brep_info = NULL;
	queried_brep_info = false;
	translation_failed = false;
	is_ug = false;
}

PartCommonInfo::~PartCommonInfo () {
	if (path_name) {
		delete[] path_name;
		path_name = NULL;
	}
#ifdef ACIS
	if (elist) {
		delete elist;
		elist = NULL;
	}
#endif // ACIS

	if (brep_info) {
		delete brep_info;
		brep_info = NULL;
	}
}

void PartCommonInfo::AddPathName ( const wchar_t *str ) {
	if (path_name == str)
		return;
	if (path_name)
		delete[] path_name;
	path_name = new wchar_t [wcslen(str) + 1];
	wcscpy(path_name, str);
}

void PartCommonInfo::GetBRepInfo () {
	if (queried_brep_info)
		return;
	SPAIDocument src(path_name);
	if (is_ug){
		src.SetType("UG");
	}
	brep_info = new SPAIDocumentBRepInfo(src);
	queried_brep_info = true;
}

/**********************************************************************************/

#ifdef ACIS
TransfHandlePair::TransfHandlePair ( SPAtransf t, entity_handle * eh ) {
	transform = t;
	handle = eh;
}

TransfHandlePair::~TransfHandlePair () {}
#endif // ACIS

/**********************************************************************************/

PartInstance::PartInstance ( PartCommonInfo * init ) {
	common = init;
#ifdef ACIS
	transforms = new VArray<TransfHandlePair *>();
#endif // ACIS
	part_hash = NULL;
	model = NULL;
	init_render = true;
	is_expanded = false;
}

PartInstance::~PartInstance () {
	if (part_hash) {
		part_hash->MapFunction(DeleteInstances, NULL);
		delete part_hash;
		part_hash = NULL;
	}
	if (transforms) {
		delete transforms;
	}
}

void PartInstance::RenderAssembly ( PartInstance * pinst, const char * reflink_id, const void * const user_data ) {
	RenderData *rend_data = (RenderData *)user_data;
#ifdef ACIS
	entity_mgr_factory* import_factory; 
	asmi_get_entity_mgr_factory( import_factory );
	outcome o;
#endif // ACIS
	if (rend_data->parent == NULL) {
		if (pinst->model == NULL) {
#ifdef ACIS
			((default_entity_mgr_factory*)import_factory)->mgr_owns_history = false;
			((default_entity_mgr_factory*)import_factory)->mgr_owns_ents	= false;
			((default_entity_mgr_factory*)import_factory)->create_mgr_history = true;
			asm_model *main_assembly = NULL;
			asm_model_info assembly_info;
			assembly_info.model_has_asm = true;
			assembly_info.set_model_name(L"root");

			asm_model_entity_mgr* my_mgr = import_factory->create_entity_mgr(assembly_info);

			outcome o = asmi_model_create(my_mgr, true, main_assembly);
			check_outcome(o);
			pinst->model = main_assembly;			
#endif // ACIS
		}
		rend_data->ref_part_translated = false;
		rend_data->parent = pinst->model;
		pinst->part_hash->MapFunction(RenderAssembly, rend_data);
		if (rend_data->ref_part_translated == false)
			return;
#ifdef ACIS
		ha_rendering_options& ha_rend_opts = HA_Get_Rendering_Options();
		ha_rend_opts.SetRenderWCSsMode(false);

		HA_Render_Model(pinst->model);
#endif // ACIS
		pinst->init_render = false;
		
		return;
	}
#ifdef ACIS
	asm_model *assembly = rend_data->parent;
	asm_model_info model_info;
#endif // ACIS
	bool add_refs = false;
	
	if (pinst->common->is_asm) {
		if (pinst->model == NULL) {
#ifdef ACIS
			model_info.model_has_asm = pinst->common->is_asm;
			asm_model_entity_mgr* pinst_entity_mgr = import_factory->create_entity_mgr(model_info );
			o = asmi_model_create(pinst_entity_mgr, true, pinst->model);
			CHECK_OUTCOME(o);
#endif // ACIS
		}
		rend_data->parent = pinst->model;
		bool old_ref_part_translated = rend_data->ref_part_translated;
		rend_data->ref_part_translated = false;
		pinst->part_hash->MapFunction(RenderAssembly, rend_data);
		rend_data->parent = assembly;
		if (rend_data->ref_part_translated)
			add_refs = true;
		rend_data->ref_part_translated = (rend_data->ref_part_translated || old_ref_part_translated);
	}

	else if (pinst->model == NULL && 
#ifdef ACIS
		pinst->common->elist
#endif // ACIS
		) {
#ifdef ACIS

		model_info.model_has_asm = pinst->common->is_asm;
		asm_model_entity_mgr* pinst_entity_mgr = import_factory->create_entity_mgr(model_info );

		pinst_entity_mgr->add_entities(*(pinst->common->elist));
		o = asmi_model_create(pinst_entity_mgr, false, pinst->model);
		check_outcome(o);
		add_refs = true;
		rend_data->ref_part_translated = true;
#endif // ACIS
	}

	if (add_refs) {
#ifdef ACIS
		entity_handle *pinst_mref = NULL;
#endif // ACIS
		unsigned long i;
		for ( i = 0 ; i < (unsigned long)pinst->transforms->Count() ; i++ ) {
#ifdef ACIS
			o = asmi_model_add_model_ref(assembly, pinst->transforms->GetData(i)->transform, pinst->model, pinst_mref);
			pinst->transforms->GetData(i)->handle = pinst_mref;
			const char *err_msg;
			if (o.get_error_info())
				err_msg = o.get_error_info()->error_message();
			check_outcome(o);
#endif // ACIS
		}
	}
}

void PartInstance::RenderPart ( PartInstance * pinst, const char * reflink_id, const void * const user_data ) {
#ifdef ACIS
	ha_rendering_options& ha_rend_opts = HA_Get_Rendering_Options();
	ha_rend_opts.SetRenderWCSsMode(false);	

	HA_Render_Entities(*(pinst->common->elist));
#endif // ACIS
}

void PartInstance::GatherAssemblyInfo ( PartInstance * pinst, const char * reflink_id, const void * const user_data ) {
	AsmInfo *asm_info = (AsmInfo *)user_data;
	if (pinst->common->is_asm)
		asm_info->sub_asm_count++;
	else
		asm_info->part_count++;
}

/**********************************************************************************/

ConversionData::ConversionData () {
	status = SPAX_S_OK;
	bad_file = NULL;
	prt_is_ug = false;
	standard_asm = false;
}

ConversionData::~ConversionData () {
	if (bad_file) {
		delete [] bad_file;
		bad_file = NULL;
	}
}

void ConversionData::BadFileName ( const wchar_t * str ) {
	if (bad_file == str)
		return;
	if (bad_file)
		delete [] bad_file;
	bad_file = new wchar_t [wcslen(str) + 1];
	wcscpy(bad_file, str);
}

RenderData::RenderData () {
	ref_part_translated = false;
	parent = NULL;
}

InterOpFileIO::InterOpFileIO ( HSolidModel * pHModel ) 
{
	m_pHModel = pHModel;
	m_pHView = NULL;
	part_hash = NULL;
	part_common_hash = NULL;
	asm_import = false;
	model_changed = false;
	convert_asm_parts = false; 
	asm_xml_conversion = false;
	asm_xml_conversion_finished = false;
	conv_data = NULL;
}

InterOpFileIO::~InterOpFileIO () 
{
	if (part_common_hash) {
		part_common_hash->MapFunction(PartCommonInfo::DeleteCommonInfo, NULL);
		delete part_common_hash;
		part_common_hash = NULL;
	}
	if (part_hash) {
		part_hash->MapFunction(PartInstance::DeleteInstances, NULL);
		delete part_hash;
		part_hash = NULL;
	}
	if (conv_data) {
		delete conv_data;
		conv_data = NULL;
	}
}

bool InterOpFileIO::CreatePartHash ( const wchar_t * filename ) 
{
	progress_callback = new CInterOpFileTranslationProgressDlg(AfxGetMainWnd());
	((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice(0.0f);
	((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice(H_FORMAT_TEXT("Generating assembly XML for %s file . . .\n", filename));
#if 0
	init_trans_call = true;
	_beginthread(XMLGenerationProgress, 0, NULL);
#endif
	
	SPAIDocument src(filename);
	if (GetPrtIsUG()){
		src.SetType("UG");
	}	
	SPAIDocumentAssemblyInfo doc_asm_struct(src);
	wchar_t xml_file[MVO_BUFFER_SIZE];
//	wsprintf(LPSTR(xml_file), LPSTR("%s.xml"), filename);
	swprintf(xml_file, L"%s.xml", filename);

	SPAIFile xml_asm_struct(xml_file);
	SPAIResult result = doc_asm_struct.SaveAs(xml_asm_struct);
	const char *err_msg;
	if (result.IsFailure()) {
		err_msg = result.GetMessage();
		
		char strMB[MVO_BUFFER_SIZE];
		sprintf(strMB, "Unable to generate EBOM XML.  Aborting translation.");
		AfxMessageBox( strMB, MB_ICONINFORMATION | MB_OK );

		delete (CInterOpFileTranslationProgressDlg *)progress_callback;

		return false;
	}
#if 0
	init_trans_call = false;
#endif
	((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice(0.90f);
	((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice("Done.\n\nParsing XML file . . .\n");

	std::ifstream in_file(xml_file);
	std::string line;
	const char *str_start, *str_end;
	char *file_path,*reflink_id, *type;
	int str_size;
	float transform[12];
	PartInstance *pinst = NULL;
	PartCommonInfo *pcinfo = NULL;
	VArray<VStringHash<PartInstance *> *> *parent_part_hash_stack = new VArray<VStringHash<PartInstance *> *>();
	bool read = true;
	bool missing_parts = false;
	// skip header tags
	int i;
	for ( i = 0 ; i < 6 ; i++ )
		getline(in_file, line);
	while (in_file) {
		if (read)
			getline(in_file, line);
		read = true;
		str_start = NULL;
		if (str_start = strstr(line.c_str(), "<Child Name=")) {
			// jump down to transform definition
			getline(in_file, line);
			getline(in_file, line);
			for ( i = 0 ; i < 12 ; i++, getline(in_file, line) ) {
				sscanf(line.c_str() + 4, "%f</", &transform[i]);
			}
#ifdef ACIS
			SPAposition origin(transform[9], transform[10], transform[11]);
			SPAunit_vector new_x_axis(transform[0], transform[1], transform[2]);
			SPAunit_vector new_y_axis(transform[3], transform[4], transform[5]);
			SPAtransf coord_sys = coordinate_transf(origin, new_x_axis, new_y_axis);
#endif // ACIS
	
			// grab the reflink id
			str_start = line.c_str() + 13;
			str_end = strstr(str_start, "\"/>");
			str_size = str_end - str_start;
			reflink_id = new char [str_size + 1];
			strncpy(reflink_id, str_start, str_size);
			reflink_id[str_size] = '\0';

			// add this to our the common part info
			if (!part_common_hash->LookupItem(reflink_id, &pcinfo)) {
				pcinfo = new PartCommonInfo();
				pcinfo->needs_pmi = false;
				if (this->GetPrtIsUG())
					pcinfo->is_ug = true;
				part_common_hash->InsertItem(reflink_id, pcinfo);
			}
			// insert the transformation (or append it to the list)
			if (!part_hash->LookupItem(reflink_id, &pinst)) {
				pinst = new PartInstance(pcinfo);
				part_hash->InsertItem(reflink_id, pinst);
			}
#ifdef ACIS
			pinst->transforms->Append(new TransfHandlePair(coord_sys));
#endif // ACIS
			delete[] reflink_id;
			// see if the next part is in a sibling of the part we just grabbed or if it is a child
			//  of the part we just grabbed
			getline(in_file, line);
			if (!(str_start = strstr(line.c_str(), "</Child>"))) {
				// make sure we don't read the next line
				read = false;
				parent_part_hash_stack->Append(part_hash);
				pinst->common->is_asm = true;
				pinst->part_hash = new VStringHash<PartInstance *>();
				part_hash = pinst->part_hash;
			}
		}
		else if (str_start = strstr(line.c_str(), "</Child>")) {
			part_hash = parent_part_hash_stack->Pop();
		}
		else if (str_start = strstr(line.c_str(), "<Document Name=\"")) {
			// start grabbing the relevant information
			str_end = strstr(line.c_str(), "\" Type=\"");
			// maybe we're missing the part
			if (str_end == NULL) {
				pinst->common->missing = true;
				missing_parts = true;
				continue;
			}
			// since the part is there, grab the relevant information
			str_start += 16;	// offset to get to start of the reflink id
			//str_end -= 0;		// offset to get to end of the reflink id
			str_size = str_end - str_start;
			reflink_id = new char [str_size + 1];
			strncpy(reflink_id, str_start, str_size);
			reflink_id[str_size] = '\0';

			// determine whether the part is a single entity or an assembly
			str_start = str_end;
			str_start += 8;		// offset to get to start of type
			str_end = strstr(str_start, "\"");
			//str_end -= 0;		// offset to get to end of type
			str_size = str_end - str_start;
			type = new char [str_size + 1];
			strncpy(type, str_start, str_size);
			type[str_size] = '\0';

			bool is_asm = false;
			if (streq(type, "sat") || streq(type, "sab"))
			{
				str_start = strstr(line.c_str(), "FilePath=\"");
				str_end = strstr(str_start, "\" SourceUuid=\"");
				//FilePath=_ is 10 chars so adding 10 moves the pointer 
				//		to the beginning of the string we want
				//      e.g. FilePath="C:\Files\CATIAV5\ball.sat"
				str_start += 10;						
			}
			else {
				// we've got an assembly
				is_asm = true;
				
				str_start = str_end;
				str_start += 14;	// offset to get to the start of the file path
				str_end = strstr(str_start, "\"");
				//str_end -= 0;		// offset to get to the end of the file path
			}

			str_size = str_end - str_start;
			file_path = new char [str_size + 1];
			strncpy(file_path, str_start, str_size); //		wcscpy
			file_path[str_size] = '\0';


			wchar_t* wide_file_path = new wchar_t[str_size +1];
			//+1 to include null terminator above
			mbstowcs(wide_file_path, file_path, str_size +1);
			part_common_hash->LookupItem(reflink_id, &pcinfo);
			pcinfo->AddPathName(wide_file_path);

			if (GetPrtIsUG())
				pcinfo->is_ug = true;
			if (is_asm) {
				pinst->common->is_asm = true;

				// jump down one line since we are done with this assembly
				getline(in_file, line);
			}
			else {								
				// jump down four lines because we're done with this part
				for ( i = 0 ; i < 4 ; i++ )
					getline(in_file, line);
			}
			
			delete [] type;
			delete [] file_path;
			delete [] reflink_id;
		}
	}
	in_file.close();

	((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice(1.0f);
	((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice("Done.\n");
	delete (CInterOpFileTranslationProgressDlg *)progress_callback;
	
	delete parent_part_hash_stack;

	if (missing_parts) {
		char strMB[MVO_BUFFER_SIZE];

		sprintf(strMB, "WARNING: There were missing files detected when\nparsing the assembly structure.  Parts\nmay be missing from final translation.");
		AfxMessageBox( strMB, MB_ICONINFORMATION | MB_OK );
	}

	return true;
}

int InterOpFileIO::ConvertParts ( PartInstance * pinst, const char * reflink_id, const void * const user_data ) 
{
	ConversionData *cdata = (ConversionData *)user_data;
#ifdef ACIS
	if (pinst->common->elist)
#endif // ACIS
		return VHASH_MAP_RETURN_CONTINUE;
	if (pinst->common->is_asm) {
		pinst->part_hash->MapFunctionWithReturn(ConvertParts, user_data);
	}
	else {
#ifdef ACIS
		cdata->status &= TranslateFile(pinst->common->path_name, pinst->common->elist, cdata);
#endif // ACIS
	}

	unsigned int err_crit = 0;
	if (cdata->status.IsFailure())
		err_crit = cdata->status.GetCriticity();
	if (err_crit == 1 || err_crit == 2) { // SPAX_ERROR_CRITICAL || SPAX_ERROR_FATAL
		if (!pinst->common->is_asm)
			cdata->BadFileName(pinst->common->path_name);
		pinst->common->translation_failed = true;
		return VHASH_MAP_RETURN_STOP;
	}
	else
		return VHASH_MAP_RETURN_CONTINUE;
}

SPAIResult InterOpFileIO::TranslateFile ( const wchar_t * filename, ENTITY_LIST *& elist, ConversionData * cdata ) 
{
	SPAIOptions options;

	SPAIOptionName attributes("TranslateAttributes");
	SPAIValue yes(true);
	options.Add(attributes, yes);

	SPAIOptionName translate_ids("TranslateEntityIDs");
	options.Add(translate_ids, yes);

	SPAIOptionName representation("Representation");
	SPAIValue rep_val;
	if (cdata->standard_asm)
		rep_val = "Assembly";
	else
		rep_val = "BRep+PMI+Manufacturing";
	options.Add(representation, rep_val);

	SPAIDocument src(filename);
	if (cdata->prt_is_ug)
		src.SetType("UG");
	wchar_t *log_file = new wchar_t [wcslen(filename) + 14];
#ifdef ACIS
	swprintf(log_file, L"%s.acis.log", filename);
//	wsprintf(LPSTR(log_file), LPSTR("%s.acis.log"), filename);
#endif // ACIS
	SPAIFile console(log_file);
	delete [] log_file;
#ifdef ACIS
	SPAIAcisDocument dst;
#endif // ACIS

	init_trans_call = true;
	progress_callback = new CInterOpFileTranslationProgressDlg(AfxGetMainWnd());
	curr_trans_file = filename;

	SPAIResult result = SPAX_S_OK;
	SPAIConverter converter;
	result &= converter.SetOptions(options);
	result &= converter.SetProgressCallback(TranslationProgress);
	result &= converter.StartLog(console);
	result &= converter.Convert(src, dst);
	result &= converter.StopLog(console);
	
	delete (CInterOpFileTranslationProgressDlg *)progress_callback;

#ifdef ACIS
	result &= dst.GetEntities(elist);
	result &= dst.DetachEntities();
#endif // ACIS
	return result;
}

HFileInputResult InterOpFileIO::Import ( const wchar_t * filename, const wchar_t * extension, HBaseView * pHView ) 
{
	m_pHView = pHView;
	SPAIResult result = SPAX_S_OK;
	SPAIDocument src(filename);	
	char reflink_id[4096];

	asm_import = false;

	if (conv_data)
		delete conv_data;
	conv_data = new ConversionData;		

	if (!wcsnicmp(extension, L"catproduct", wcslen(L"catproduct"))) {
		asm_import = true;
	}
	else {
		if (!wcsnicmp(extension, L"prt", wcslen(L"prt"))) {
			CInterOpImportDlg iop_import_dlg;
			if (iop_import_dlg.DoModal(this) == IDOK) {
				if (conv_data->prt_is_ug)
					src.SetType("UG");
			}
		}
		SPAIDocumentHeaderInfo info;
		result &= src.GetHeaderInfo(info);
		if (result.IsSuccess()) {
			SPAIValue gen_val;
			result = SPAX_S_OK;
			result = info.GetFileName(gen_val);
			if (result.IsSuccess())
				sprintf(reflink_id, "%s", (const char *)gen_val);
			else
				sprintf(reflink_id, "no_reflink_id");
			result = SPAX_S_OK;
			result = info.GetDocumentType(gen_val);
			if (result.IsSuccess()) {
				const char *doc_type = gen_val;
				if (strstr(doc_type, "Assembly"))
					asm_import = true;
			}
			else {
				TCHAR strMB[MVO_BUFFER_SIZE];

				sprintf(strMB, _T("Unable to query Document Type.\nTranslation may fail due to representation.\n"));
				AfxMessageBox( strMB, MB_ICONINFORMATION | MB_OK );

				sprintf(reflink_id, "no_reflink_id");
			}
		}
		else {
			TCHAR strMB[MVO_BUFFER_SIZE];

			sprintf(strMB, _T("Unable to query Header Info.\nTranslation may fail due to representation.\n"));
			AfxMessageBox( strMB, MB_ICONINFORMATION | MB_OK );

			sprintf(reflink_id, "no_reflink_id");
		}
	}

	if (part_common_hash)
		delete part_common_hash;
	if (part_hash)
		delete part_hash;
	part_common_hash = new VStringHash<PartCommonInfo *>();
	part_hash = new VStringHash<PartInstance *>();

	if (asm_import) {
		CInterOpAsmImportDlg iop_asm_import_dlg;
		if (iop_asm_import_dlg.DoModal(this) == IDOK) {
			if (!convert_asm_parts)
			{
				asm_xml_conversion = true;
				asm_xml_conversion_finished = false;
			}
			else {
				conv_data->standard_asm = true;
				return BRepImport(reflink_id, filename);
			}
		}
		return AssemblyImport(filename);		
	}
	else
		return BRepImport(reflink_id, filename);
}

HFileInputResult InterOpFileIO::AssemblyImport ( const wchar_t * filename, bool dump_asm_struct ) 
{
	if (!CreatePartHash(filename))
		return InputFail;

	if (dump_asm_struct) {
		std::ofstream out_file(H_FORMAT_TEXT("%s.asm_struct.log", filename));
#if 0
		part_hash->MapFunction(OutputParts, &out_file);
#endif
	}

	PartCommonInfo *root_pci = new PartCommonInfo();
	root_pci->AddPathName(filename);
	root_pci->is_asm = true;
	root_pci->needs_pmi = false;
	SPAIDocument src(filename);
	if (conv_data->prt_is_ug)
		src.SetType("UG");
	SPAIDocumentHeaderInfo header_info;
	SPAIResult result = src.GetHeaderInfo(header_info);
	SPAIValue reflink_id_val;
	char reflink_id[4096];
	if (result.IsSuccess()) {
		header_info.GetFileName(reflink_id_val);
		sprintf(reflink_id, "%s", (const char *)reflink_id_val);
	}
	else {
		sprintf(reflink_id, "no_part_name");
	}
	part_common_hash->InsertItem(reflink_id, root_pci);

	VStringHash<PartInstance *> *root_hash = new VStringHash<PartInstance *>();
	PartInstance *root = new PartInstance(root_pci);
	root->part_hash = part_hash;
	root_hash->InsertItem(reflink_id, root);
	part_hash = root_hash;

	HFileInputResult success = InputOK;

//	if (convert_asm_parts) {
	if (1) {
		part_hash->MapFunctionWithReturn(ConvertParts, (const void * const)conv_data);
		unsigned int err_crit = 0;
		if (conv_data->status.IsFailure())
			err_crit = conv_data->status.GetCriticity();

		if (err_crit == 1 || err_crit == 2) 
		{ // SPAX_ERROR_CRITICAL || SPAX_ERROR_FATAL
			TCHAR strMB[MVO_BUFFER_SIZE];

			sprintf(strMB, _T("Translation for Assembly import failed.  For specifics see the log file for the part:\n%s"), conv_data->bad_file);
			AfxMessageBox( strMB, MB_ICONINFORMATION | MB_OK );
			success = InputFail;
		}
		else {
			RenderAssembly();
			success = InputOK;	
		}
	}

#ifdef ACIS		
	m_pHModel->GetModelList().add(root->model);
#endif // ACIS

	asm_xml_conversion_finished = true;
	return success;
}

HFileInputResult InterOpFileIO::BRepImport ( const char * reflink_id, const wchar_t * filename ) 
{
	HFileInputResult success;
	
	PartCommonInfo *pcinfo = new PartCommonInfo();
	pcinfo->AddPathName(filename);
	if (conv_data->standard_asm)
		pcinfo->needs_pmi = false;
	if(conv_data->prt_is_ug)
		pcinfo->is_ug = true;

	part_common_hash->InsertItem(reflink_id, pcinfo);
	PartInstance *pinst = new PartInstance(pcinfo);
	part_hash->InsertItem((const char *)reflink_id, pinst);
	
	part_hash->MapFunctionWithReturn(ConvertParts, (const void * const)conv_data);
	unsigned int err_crit = 0;
	if (conv_data->status.IsFailure())
		err_crit = conv_data->status.GetCriticity();
	if (err_crit == 1 || err_crit == 2) { // SPAX_ERROR_CRITICAL || SPAX_ERROR_FATAL
		TCHAR strMB[MVO_BUFFER_SIZE];

		sprintf(strMB, _T("Translation for BRep import failed.  For specifics see the log file for the part:\n%s"), conv_data->bad_file);
		AfxMessageBox( strMB, MB_ICONINFORMATION | MB_OK );
		success = InputFail;
	}
	else {
		part_hash->MapFunction(PartInstance::RenderPart, NULL);
#ifdef ACIS
		m_pHModel->GetEntityList().add(*(pinst->common->elist));
#endif // ACIS
		success = InputOK;
	}
	
	return success;
}

void InterOpFileIO::RenderAssembly () 
{
	if (part_hash) {
		RenderData *rend_data = new RenderData();
		part_hash->MapFunction(PartInstance::RenderAssembly, rend_data);
		delete rend_data;
	}
}

void InterOpFileIO::RenderPart () 
{
	if (part_hash)
		part_hash->MapFunction(PartInstance::RenderPart, NULL);
}

void InterOpFileIO::ShowInterOpPartBrowser () 
{
	if (asm_import && asm_xml_conversion && asm_xml_conversion_finished) {
// 		CSolidHoopsFrame *frame = (CSolidHoopsFrame *)AfxGetMainWnd();
// 		frame->ShowInterOpPartBrowserDialogBar();
		asm_xml_conversion = false;
	}
}

void InterOpFileIO::Update () 
{
	if (asm_import && model_changed) {
		HC_Open_Segment_By_Key(m_pHModel->GetModelKey());
			RenderAssembly();
		HC_Close_Segment();
		model_changed = false;
	}
}

void InterOpFileIO::TranslationProgress ( double progress, SPAXBoolean & abort ) 
{
	if (init_trans_call) {
		char file_name[256];
		sprintf(file_name, "Reading File . . ."/*, curr_trans_file*/);				
		((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice(file_name);
		init_trans_call = false;
	}
	((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice(static_cast<float>(progress));
	abort = 0;
}


void InterOpFileIO::XMLGenerationProgress ( void * args ) 
{
	unsigned int counter = 0;
	float progress;
	while (1) {
		progress = static_cast<float>(counter / 4.0);
		((CInterOpFileTranslationProgressDlg *)progress_callback)->Notice(progress);
		counter = (counter + 1) % 5;
		if (!init_trans_call)
			break;
	}
}

/**********************************************************************************/