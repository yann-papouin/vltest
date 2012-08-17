#ifndef _INTEROPFILEIO_H
#define _INTEROPFILEIO_H

#ifdef ACIS
class ENTITY_LIST;
#endif // ACIS
class SPAIDocumentBRepInfo;

class PartCommonInfo {
public:
	wchar_t *path_name;
#ifdef ACIS
	ENTITY_LIST *elist;
#endif // ACIS
	bool is_asm;
	bool missing;
	bool needs_pmi;
	SPAIDocumentBRepInfo *brep_info;
	bool queried_brep_info;
	bool translation_failed;
	bool is_ug;

	PartCommonInfo ();
	~PartCommonInfo ();
	
	void AddPathName ( const wchar_t *str );
	void GetBRepInfo ();
	
	static void DeleteCommonInfo ( PartCommonInfo * pcinfo, const char * reflink_id, const void * const user_data ) {
		delete pcinfo;
	}
};

#ifdef ACIS
#include "transf.hxx"
class entity_handle;

class TransfHandlePair {
public:
	SPAtransf transform;
	entity_handle *handle;

	TransfHandlePair ( SPAtransf t, entity_handle * eh = NULL );
	~TransfHandlePair ();
};
#endif // ACIS

#ifdef ACIS
class asm_model;
#endif // ACIS
template <typename T> class VArray;
template <typename T> class VStringHash;

class PartInstance {
public:
	PartCommonInfo *common;
	VStringHash<PartInstance *> *part_hash;
#ifdef ACIS
	VArray<TransfHandlePair *> *transforms;
#endif // ACIS

#ifdef ACIS
	asm_model *model;
#endif // ACIS

	bool init_render;
	bool is_expanded;

	class AsmInfo {
	public:
		unsigned int part_count;
		unsigned int sub_asm_count;

		AsmInfo () {
			part_count = 0;
			sub_asm_count = 0;
		}

		~AsmInfo () {}
	};

	PartInstance ( PartCommonInfo * init = NULL );
	~PartInstance ();

	static void DeleteInstances ( PartInstance * pinst, const char * reflink_id, const void * const user_data ) {
		delete pinst;
	}

	static void RenderAssembly ( PartInstance * pinst, const char * reflink_id, const void * const user_data );
	static void RenderPart ( PartInstance * pinst, const char * reflink_id, const void * const user_data );
#if 0
	static void MapEntitiesToParts ( PartInstance * pinst, const char * reflink_id, const void * const user_data );
#endif
	static void GatherAssemblyInfo ( PartInstance * pinst, const char * reflink_id, const void * const user_data );
};

#include "HTools.h"
#include "HGlobals.h"
#include "SPAXBoolean.h"
#undef GetMessage
#include "SPAIResult.h"
class HSolidModel;
class HBaseView;

class ConversionData {
public:
	SPAIResult status;
	wchar_t *bad_file;
	bool prt_is_ug;
	bool standard_asm;

	ConversionData ();
	~ConversionData ();
	void BadFileName ( const wchar_t *str );
};

class RenderData {
public:
	bool ref_part_translated;
#ifdef ACIS
	asm_model *parent;
#endif // ACIS

	RenderData ();
	~RenderData () {}
};

class InterOpFileIO {
private:
	HSolidModel *m_pHModel;
	HBaseView *m_pHView;
	VStringHash<PartInstance *> *part_hash;
	VStringHash<PartCommonInfo *> *part_common_hash;
	bool asm_import;
	bool model_changed;
	bool convert_asm_parts;
	bool asm_xml_conversion;
	bool asm_xml_conversion_finished;
	static void *progress_callback;
	static bool init_trans_call;
	static const wchar_t *curr_trans_file;

	ConversionData *conv_data;
public:
	InterOpFileIO ( HSolidModel * pHModel );
	~InterOpFileIO ();
	
#if 0
	static void SaveParts ( PartInstance * pinst, const char * reflink_id, const void * const user_data );
	static void OutputParts ( PartInstance * pinst, const char * reflink_id, const void * const user_data );
#endif
	bool CreatePartHash ( const wchar_t * filename );
	static int ConvertParts ( PartInstance * pinst, const char * reflink_id, const void * const user_data );
#ifdef ACIS
	static SPAIResult TranslateFile ( const wchar_t * filename, ENTITY_LIST *& elist, ConversionData * cdata );
#endif // ACIS

	HFileInputResult Import ( const wchar_t * filename, const wchar_t * extension, HBaseView * pHView );
	HFileInputResult AssemblyImport ( const wchar_t * filename, bool dump_asm_struct = false );
	HFileInputResult BRepImport ( const char * reflink_id, const wchar_t * filename );
	void RenderAssembly ();
	void RenderPart ();
	void SetModelChanged ( bool changed ) { model_changed = changed; }
	void ShowInterOpPartBrowser ();
	void Update ();

	VStringHash<PartInstance *> * GetPartHash () { return part_hash; }
	void SetPartHash ( VStringHash<PartInstance *> * new_part_hash ) { part_hash = new_part_hash; }
#if 0
	VHash<ENTITY *, PartInstance *> * GetEntHash () { return ent_hash; }
	void SetEntHash ( VHash<ENTITY *, PartInstance *> * new_ent_hash ) { ent_hash = new_ent_hash; }
#endif	
	bool GetPrtIsUG () { return conv_data->prt_is_ug; }
	void SetPrtIsUG ( bool is_ug ) { conv_data->prt_is_ug = is_ug; }
	bool GetConvertAsmParts () { return convert_asm_parts; }
	void SetConvertAsmParts ( bool convert ) { convert_asm_parts = convert; }
	static void TranslationProgress ( double progress, SPAXBoolean & abort );
	static void XMLGenerationProgress ( void * args );
};

#endif // _INTEROPFILEIO_H