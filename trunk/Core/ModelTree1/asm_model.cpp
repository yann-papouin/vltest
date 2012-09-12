/*******************************************************************/
/*    Copyright (c) 1989-2008 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// 09-Apr-02 jce: various changes to support the "handle uses proxy" story S4755

#include "asm_model.hxx"
#include "asm_model_entity_mgr.hxx"
#include "asm_assembly.hxx"
#include "asm_model_ref.hxx"
#include "fileio.hxx"
#include "savres.hxx"
#include "errorbase.hxx"
#include "entity_handle.hxx"
#include "entity_proxy.hxx"
#include "asm_model_mgr.hxx"
#include "comp_handle.hxx"
#include "comp_handle_list.hxx"
#include "comp_ent_handle.hxx"
#include "comp_ent_handle_list.hxx"
#include "asm_list_opts.hxx"
#include "api.hxx"
#include "asm_api.hxx"
#include "ckoutcom.hxx"
#include "box_opts.hxx"
#include "get_box_prot.hxx"
#include "base_containers_prot.hxx"
#include "at_comp_prop.hxx"
#include "asm_util.hxx"
#include "asm_savres_priv.hxx"
#include "bulletin.hxx"

#include "asm_error.err"

// create a global model stack to keep track of MODEL_BEGIN/END nesting
LOCAL_PROC SPAvoid_ptr_array active_model_stack; // stack of parts that were active when activate_vf was called

LOCAL_PROC void initialize_active_model_stack()
{
	if(active_model_stack.Size() ==  0)
	{
		active_model_stack.Push(NULL);
	}
}

entity_handle* asm_model::get_assembly_handle()
{
	if(!this || !mgr_ptr)
		{return NULL;}

	ASM_ASSEMBLY const* asm_ptr = mgr_ptr->get_assembly_ptr();

	return get_entity_handle(asm_ptr);
}

//////////////////////////
// internal utility methods for use by model
//////////////////////////
//void asm_model::add_count()
//{
//	use_cnt++;
//}

void asm_model::rem_count()
{
	if(!this)
	{
		return;
	}

	use_cnt--;

	// if the manager for this model has gone away and the use count
	// goes to zero, then delete the model so we don't leak
	if ( !owning_model_mgr && ( use_cnt == 0 ) )
	{
		if(mgr_ptr)
		{ 
			// should never happen - should have been unbound when model was released
			mgr_ptr->unbind(); 
		}
		ACIS_DELETE this;
	}
}

//int asm_model::use_count()
//{
//	return use_cnt; 
//}

////////////////////////
// model manager methods
////////////////////////
asm_model_mgr* asm_model::get_owning_manager() const
{
	if(!this)
	{
		return NULL;
	}

	return owning_model_mgr;
}

// notify model that owning manager is going away
void asm_model::release_owning_manager()
{
	if(!this)
	{
		return;
	}

	// we can't call clear in the destructor since we may cause an access
	// violation during static cleanup because the history stream is already
	// gone, so call it before we call the destructor when we know the history
	// streams are still present
	//
	// we always call clear() here since we want the use counts of sub-models
	// to be decremented so they can be cleaned up
	// JHS - changed to unbind() to ensure that released models are unbound from managers
	if ( mgr_ptr )
	{
		mgr_ptr->unbind();
//		mgr_ptr->clear();
	}

	// remove the model from the assembly hierarchy
//	remove_from_tree();

	// tell model manager that *this is being released
	if ( owning_model_mgr )
	{
		owning_model_mgr->remove( this );
	}
	// null out owning manager pointer to indicate this has been released
	owning_model_mgr = NULL;

	// if the manager for this model is going away and the use count
	// is zero, then delete the model so we don't leak
	// This must be done last to prevent access violations.
	if ( use_count() == 0 )
	{
		ACIS_DELETE this;
	}
}

//////////////////////////
// methods for performing save and restore
//////////////////////////
// ++++ methods to make sure no tombstones appear in lookups for +++++++
// ++++ entity and component handles while saving               +++++++                                                                       
// method to get list of handles to save
void asm_model::get_lookup_entity_handles(entity_handle_list & handles) const// clears then adds "no-tombstone" list to handles
{
	if(!this)
	{
		return;
	}

	// use an explicit loop to transfer the handles, rather than relying on add
	{ // local scope to loop over entity handles
		for(entity_handle* hdl=ent_handles.first(); hdl; hdl=ent_handles.next())
		{
			handles.add(hdl);
		}
	}
}

int asm_model::lookup_entity_handles_count() const
{
	if(!this)
	{
		return 0;
	}

	return ent_handles.iteration_count();
}

// method to get list of handles to save
void asm_model::get_lookup_component_handles(component_handle_list & handles) const // clears then adds "no-tombstone" list to handles
{
	if(!this)
	{
		return;
	}

	// use an explicit loop to transfer the handles, rather than relying on add
	{ // local scope to loop over component handles
		for(component_handle* hdl=comp_handles.first(); hdl; hdl=comp_handles.next())
		{
			handles.add(hdl);
		}
	}
}
// method to get list of handles to save
void asm_model::get_lookup_component_entity_handles(component_entity_handle_list & handles) const // clears then adds "no-tombstone" list to handles
{
	if(!this)
	{
		return;
	}

	// use an explicit loop to transfer the handles, rather than relying on add
	{ // local scope to loop over component entity handles
		for(component_entity_handle* hdl=comp_ent_handles.first(); hdl; hdl=comp_ent_handles.next())
		{
			handles.add(hdl);
		}
	}
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//////////////////////////
// private destructor only allows model_ref to delete
//////////////////////////
asm_model::~asm_model()
{
	// delete restore information if present
	if(restore_sat_sequence_numbers)
	{
		ACIS_DELETE restore_sat_sequence_numbers;
		restore_sat_sequence_numbers = NULL;
	}

	if(restore_model_info_ptr)
	{
		ACIS_DELETE restore_model_info_ptr;
		restore_model_info_ptr = NULL;
	}

	if(seq_table)
	{
		ACIS_DELETE seq_table;
		seq_table = NULL;
	}

	if ( mgr_ptr )
	{
//		mgr_ptr->unbind();
	}

	asm_cleanup_options co;
	cleanup_handles( &co );

	// notify any handles that could not be cleaned up that the model is going away
	for ( entity_handle* eh = ent_handles.first(); eh; eh = ent_handles.next() )
	{
		eh->release_owning_model();
	}
	for ( component_handle* ch = comp_handles.first(); ch; ch = comp_handles.next() )
	{
		ch->release_owning_model();
	}
	for ( component_entity_handle* ceh = comp_ent_handles.first(); ceh; ceh = comp_ent_handles.next() )
	{
		ceh->release_owning_model();
	}

	// remove the model from the assembly hierarchy
	// shouldn't do anything, since use-count should be zero, i.e. no mrefs anywhere
	remove_from_tree();

	// tell model manager that *this is dying
	if ( owning_model_mgr )
	{
		owning_model_mgr->remove( this );
	}
}

//////////////////////////
// private constructor only allows model to create
//////////////////////////

asm_model::asm_model( asm_model_entity_mgr* ent_mgr ) : restore_sat_sequence_numbers(NULL)
{
	owning_model_mgr = sg_asm_model_mgr();
	mgr_ptr = ent_mgr;
	data_ptr = NULL;
	save_file_ptr = NULL;
	restore_file_ptr = NULL;
	restore_model_info_ptr = NULL;

	// turn on holding for assembly and mref lists
	// this is to ensure that they always have a handle (provides back-pointer to model)
	assembly_handles.set_hold_state(ASM_HOLD);
	mref_handles.set_hold_state(ASM_HOLD);

	seq_table = ACIS_NEW model_entity_handle_seq_table;

	box_valid = FALSE;
	//use_cnt = 0;

	// ugly way to use counted lists
	asm_list_options lo;
	lo.set_count_flag(TRUE);
	asm_model_list temp_list( &lo );
	owning_models = temp_list;
	sub_models = temp_list;

	// KLUDGE - used during atomic save
	sequence_number_within_save_file = -1;
	first_entity_handle_id_within_save_file = -1;

}
void asm_model::hold_assembly_handle(entity_handle* asm_handle)
{
	if(!this || !asm_handle)
	{
		return;
	}

//	entity_handle* asm_handle = get_entity_handle(asm_ptr);
	assembly_handles.add(asm_handle);
}

void asm_model::unhold_assembly_handle(entity_handle* asm_handle)
{
	if(!this)
	{
		return;
	}

	if(asm_handle)
	{
		assembly_handles.remove(asm_handle);
	}
}

void asm_model::hold_mref_handle(entity_handle* mref_handle)
{
	if(!this || !mref_handle)
	{
		return;
	}

//	entity_handle* mref_handle = get_entity_handle(mref_ptr);
	mref_handles.add(mref_handle);
}

void asm_model::unhold_mref_handle(entity_handle* mref_handle)
{
	if(!this)
	{
		return;
	}

	if(mref_handle)
	{
		mref_handles.remove(mref_handle);
	}
}

void asm_model::link_sub_model(asm_model* sub_model)
{
	asm_model* owning_model = this;
	if(!owning_model || !sub_model)
	{
		return;
	}

	EXCEPTION_BEGIN
		int owning_cnt = -1;
		int sub_cnt = -1;
		EXCEPTION_TRY
			owning_model->add_sub_model( sub_model, sub_cnt );
			sub_model->add_owning_model( owning_model, owning_cnt );
		EXCEPTION_CATCH_FALSE
		// if second add failed, undo first add
		if ( sub_cnt == owning_cnt + 1 )
		{
			owning_model->remove_sub_model( sub_model, sub_cnt );
		}
	EXCEPTION_END
}

void asm_model::unlink_sub_model(asm_model* sub_model)
{
	asm_model* owning_model = this;
	if(!owning_model || !sub_model)
	{
		return;
	}

	EXCEPTION_BEGIN
		int owning_cnt = -1;
		int sub_cnt = -1;
		EXCEPTION_TRY
			owning_model->remove_sub_model( sub_model, sub_cnt );
			sub_model->remove_owning_model( owning_model, owning_cnt );
		EXCEPTION_CATCH_FALSE
		// if second remove failed, undo first remove
		if ( sub_cnt == owning_cnt - 1 )
		{
			owning_model->add_sub_model( sub_model, sub_cnt );
		}
	EXCEPTION_END
}

void asm_model::set_restore_model_info(asm_model_info const & model_info)
{
	if(!restore_model_info_ptr)
	{
		restore_model_info_ptr = ACIS_NEW asm_model_info(model_info);
	}
	else
	{
		(*restore_model_info_ptr) = model_info;
	}
}

asm_model_info const & asm_model::restore_model_info() const
{
	if(!restore_model_info_ptr)
	{
		sys_error(ASM_INVALID_MODEL);
	}

	return *restore_model_info_ptr;
}

// KLUDGE - why is this a method of asm_model?  Is it static?
asm_model* asm_model::create( asm_model_entity_mgr* ent_mgr )
{
	asm_model* new_model = ACIS_NEW asm_model( ent_mgr );
	asm_model_mgr* mdl_mgr = sg_asm_model_mgr();
	mdl_mgr->add( new_model );
	if ( ent_mgr )
	{
		ent_mgr->set_model( new_model );
	}

	return new_model;
}

logical asm_model::unbind_mgr(asm_model_entity_mgr* mgr)
{
	if(!this)
	{
		// treat *this as owning a NULL mgr pointer
		return mgr == NULL;
	}

	if( mgr_ptr == mgr)
	{
		mgr_ptr = NULL;
		// Should probably check if model_ref_count == 0 here and destroy if it does.
		// That would mean that customer code should never delete an asm_model
		// (plus it should never hold an asm_model in case it stales out)
		return TRUE;
	}
	else
	{
		// don't sys_error since this is probably being called from a destructor
		// return FALSE instead
		return FALSE;
	}
}

void asm_model::get_immediate_owning_models(asm_model_list& models) const
{
	if(!this)
	{
		return;
	}

	// copy each member since output may not be a counted list
	for ( asm_model* model = owning_models.first(); model; model = owning_models.next() )
	{
		models.add( model );
	}
}

void asm_model::get_immediate_sub_models(asm_model_list& models, logical include_lost) const
{
	if(!this)
	{
		return;
	}

	// copy each member since output may not be a counted list
	{
		for ( asm_model* model = sub_models.first(); model; model = sub_models.next() )
		{
			models.add( model );
		}
	}

	// check for lost mrefs that might depend on more models
	if(include_lost)
	{
		for(entity_handle* eh = mref_handles.first(); eh; eh = mref_handles.next())
		{
			// only interested in entity handles whose entities are lost
			// these will claim to have a NULL entity but the raw entity pointer will be non-NULL
            // Updated as part of the "handle uses proxy" story S4755
			if(eh->entity_ptr() || !eh->pproxy || !eh->pproxy->pent)
			{
				continue;
			}

			ENTITY* ent = eh->pproxy ? eh->pproxy->pent : NULL;

			API_MODEL_BEGIN(this)
			if(is_ASM_MODEL_REF(ent)) // found one
			{
				ASM_MODEL_REF* mref = (ASM_MODEL_REF*)ent;
				models.add(mref->model());  // add its end model to the list
			}
			API_MODEL_END(ASM_NO_CHANGE)
			check_outcome(result);
		}
	}
}

void asm_model::get_top_level_entities(ENTITY_LIST& entities, logical include_assemblies) const
{
	// Using the method, rather than the pointer, so that we register loaded entities 
	// when necessary
	entities.clear();

	if(!this)
	{
		return;
	}

	if ( mgr() )
		mgr_ptr->get_top_level_entities( entities, include_assemblies );
}

void asm_model::get_top_level_entities(entity_handle_list& entities, logical include_assemblies) const
{
	entities.clear();

	if(!this)
	{
		return;
	}

	API_MODEL_BEGIN(this)
		ENTITY_LIST ents;
		get_top_level_entities( ents, include_assemblies );
		
		for ( ENTITY* ent = ents.first(); ent; ent = ents.next() )
		{
			entity_handle* ent_hdl = get_entity_handle( ent );
			entities.add( ent_hdl );
		}
	API_MODEL_END(ASM_NO_CHANGE);
	check_outcome(result);
}

asm_model_entity_mgr* asm_model::mgr() const
{
	if(!this)
	{
		return NULL;
	}

	return mgr_ptr;
}


asm_save_file* asm_model::save_file() const
{
	if(!this)
	{
		return NULL;
	}

	return save_file_ptr;
}

void asm_model::set_save_file(asm_save_file* sfile) 
{
	if(!this)
	{
		return;
	}

	save_file_ptr = sfile;
}

asm_restore_file* asm_model::restore_file() const
{
	if(!this)
	{
		return NULL;
	}

	return restore_file_ptr;
}

void asm_model::set_restore_file(asm_restore_file* rfile) 
{
	if(!this)
	{
		return;
	}

	restore_file_ptr = rfile;
}

logical asm_model::is_usable() const
{
//	if(!mgr() || !get_owning_manager())
	if( !this                                    // NULL pointer
		|| !get_owning_manager()                 // model has been released
		|| (!mgr() && !restore_model_info_ptr) ) // no manager, but not due to failed restore
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

logical asm_model::is_complete() const
{
	if(!is_usable() || !mgr())
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void asm_model::make_safe_after_failed_restore()
{
	for ( entity_handle* eh = ent_handles.first(); eh; eh = ent_handles.next() )
	{
		eh->clear_ptr();
	}
}
/*
asm_model_status asm_model::model_status() const
{

	// should only have NULL mgr pointer
	// caveats:
	//    What if customer has lost the mgr?
	if(mgr() == NULL)
		{return ASM_MODEL_EMPTY;}

	// if there's a mgr and it has an assembly, then this is an assembly mgr
	if ( mgr()->get_assembly_ptr() )
		{return ASM_MODEL_ASSEMBLY;}

	// otherwise it's a PART mgr
	return ASM_MODEL_PART;
}
*/

// entity handle management

entity_handle* asm_model::get_entity_handle(ENTITY const* entity) const
{
	return get_entity_handle(entity,TRUE);
}

entity_handle* asm_model::get_entity_handle(ENTITY const* entity, logical check_entity_owner) const
{
	if(!entity)
	{
		return NULL;
	}

	entity_handle* ret_eh = entity->handle();

	asm_model* non_const_this = (asm_model*)this;
	// if not found, make new handle
	if ( !ret_eh )
	{
		if(!this)
		{
			sys_error(ASM_NULL_MODEL);
		}

		
		// safe since asm_model_entity_mgr::owns_entity() returns false for NULL mgr
		if ((!check_entity_owner) || mgr()->owns_entity(entity) )
		{
			// cast away const - we're treating the list of entity_handles as mutable when adding
			ENTITY* non_const_entity = (ENTITY*)entity;
			ret_eh = ACIS_NEW entity_handle( non_const_this, non_const_entity ); 
			non_const_this->ent_handles.add( ret_eh );
		}
		else
		{
			sys_error(ASM_ENTITY_NOT_OWNED_BY_MODEL);
		}
	}
	else if ( ret_eh->get_owning_model() != this)
	{
		sys_error(ASM_ENTITY_NOT_OWNED_BY_MODEL);
	}
	return ret_eh;
}



void asm_model::get_entity_handles( entity_handle_list& handles ) const
{
	if(!this)
	{
		return;
	}

	// copy the list of instantiated entity_handles to the return argument
	handles = ent_handles;
}

LOCAL_PROC entity_handle* 
lookup_entity(const entity_handle_list& eh_list, const ENTITY* ent)
{
    entity_handle* answer = NULL;

    for (entity_handle* eh = eh_list.first(); eh != NULL; eh_list.next())
    {
        ENTITY* list_ent = eh->entity_ptr();
        if (ent == list_ent)
        {
            answer = eh;
            break;
        }
    }

    return answer;
}

LOCAL_PROC component_handle* 
lookup_comp_util(const component_handle_list& ch_list, entity_handle_list const & model_refs)
{
	// NOTE that this routine gets zero-length paths correct, since there should only be one (if any) zero-length path in ch_list
    component_handle* answer = NULL;

    for (component_handle* ch = ch_list.first(); ch != NULL && answer == NULL; ch = ch_list.next())
	{
		// if paths are different lengths, no chance of a match
		if(ch->get_model_ref_path_length() != model_refs.iteration_count())
		{
			continue;
		}

		answer = ch; // try this one for a match

		// get its path
		entity_handle_list oh_path;
		answer->get_model_ref_path(oh_path);

		// check each element of the path for a match
		int index = 0;
		for(entity_handle* mref = model_refs.first(); mref; mref = model_refs.next())
		{
			if(mref != answer->get_model_ref_path_element(index))
			{
				answer = NULL; // nope - this isn't a match
				break; // try next ch
			}
			index ++;
		}
	}

    return answer;
}

LOCAL_PROC component_entity_handle* 
lookup_comp_ent_util( const component_entity_handle_list& ceh_list, entity_handle* ent, component_handle* comp )
{
	component_entity_handle* answer = NULL;

    for ( component_entity_handle* ceh = ceh_list.first(); ceh != NULL; ceh = ceh_list.next() )
    {
        component_handle* ch = ceh->component();

        if ( ch == comp )
        {
            entity_handle* eh = ceh->entity();
// I don't know why this was comparing entity pointers rather than handles - I think it was just wrong (JHS)
//            if ( ent && eh && ( ent->entity_ptr() == eh->entity_ptr() ) )
            if ( ent == eh )
            {
                answer = ceh;
                break;
            }
        }
    }
    return answer;
}

component_handle* asm_model::lookup_comp(entity_handle_list const & mrefs) const
{
	return lookup_comp_util(comp_handles, mrefs);
}

component_entity_handle* asm_model::lookup_comp_ent(component_handle* comp, entity_handle* ent) const
{
// begin FAST_CE_LOOKUP
	return comp->lookup_ce(ent);
//	return lookup_comp_ent_util(comp_ent_handles, ent, comp);
// end FAST_CE_LOOKUP
}


component_handle* asm_model::get_component_handle(entity_handle* model_ref_path[], int n_model_refs)
{
	if(!this)
	{
		return NULL;
	}

	component_handle* answer = NULL;

	entity_handle_list model_refs;
	for(int ii = 0; ii<n_model_refs; ii++)
	{
		model_refs.add(model_ref_path[ii]);
	}

	answer = get_component_handle(model_refs);
	return answer;
}

component_handle* asm_model::get_component_handle(entity_handle_list & model_refs)
{
	if(!this)
	{
		return NULL;
	}

	// check path for correctness
	{
		asm_model* previous_model = this;
		for(entity_handle* mref = model_refs.first(); mref; mref = model_refs.next())
		{
			// check this element of the path
			ENTITY* ent = mref->entity_ptr();
			if(ent == NULL || !is_ASM_MODEL_REF(ent))
			{
				sys_error(BAD_COMP_PATH);
			}

			if(previous_model != mref->get_owning_model())
			{
				sys_error(BAD_COMP_PATH);
			}

			previous_model = ((ASM_MODEL_REF*)ent)->model();
			//previous_model = sg_asm_model_ref_get_model( (ASM_MODEL_REF*)ent );
		}
	}

	// see if we already have a handle for this path
	component_handle* answer = lookup_comp_util(comp_handles, model_refs);

    if (answer == NULL)
    {
		// check for zero-length path
		if(model_refs.iteration_count() != 0)
		{
			answer = ACIS_NEW component_handle(model_refs);
		}
		else
		{
			answer = ACIS_NEW component_handle(this);
		}
        comp_handles.add(answer);
    }
	return answer;
}

component_entity_handle* asm_model::get_component_entity_handle(entity_handle* ent, component_handle* comp)
{
	if ( !this || !ent || !comp )
	{
		return NULL;
	}
	if ( ent->get_owning_model() != comp->get_end_model() )
	{
		sys_error( BAD_COMP_ENT_PATH );
	}
	if ( comp->get_owning_model() != this )
	{
		sys_error( BAD_COMP_ENT_PATH );
	}

	component_entity_handle* answer = NULL;

	//answer = lookup_comp_ent_util( comp_ent_handles, ent, comp );
	answer = lookup_comp_ent( comp, ent );

    if ( answer == NULL )
    {
        answer = ACIS_NEW component_entity_handle( ent, comp );
        comp_ent_handles.add( answer );
    }

	return answer;
}

LOCAL_PROC logical cleanup_entity_handles( entity_handle_list& ent_handles )
{
	logical change_made = FALSE;
	for ( entity_handle* eh = ent_handles.first(); eh != NULL; eh = ent_handles.next() )
	{
		if ( eh )
		{
			//change_made = eh->cleanup();
			change_made = sg_asm_entity_handle_cleanup( eh );
		}
	}
	return change_made;
}

LOCAL_PROC logical cleanup_component_handles( component_handle_list& comp_handles )
{
	logical change_made = FALSE;
	for ( component_handle* ch = comp_handles.first(); ch != NULL; ch = comp_handles.next() )
	{
		if ( ch )
		{
			change_made = ch->cleanup();
		}
	}
	return change_made;
}

LOCAL_PROC logical cleanup_component_entity_handles( component_entity_handle_list& comp_ent_handles )
{
	logical change_made = FALSE;
	for ( component_entity_handle* ceh = comp_ent_handles.first(); ceh != NULL; ceh = comp_ent_handles.next() )
	{
		if ( ceh )
		{
			change_made = ceh->cleanup();
		}
	}
	return change_made;
}

logical asm_model::cleanup_handles( asm_cleanup_options const* co )
{
	if(!this)
	{
		return FALSE;
	}

	asm_cleanup_options def_co;
	if ( co )
	{
		def_co = *co;
	}

	logical ent_change = FALSE, comp_change = FALSE, comp_ent_change = FALSE;

	if ( def_co.get_clean_component_entity_handles() )
	{
		comp_ent_change = cleanup_component_entity_handles( comp_ent_handles);
	}
	if ( def_co.get_clean_component_handles() )
	{
		comp_change = cleanup_component_handles( comp_handles );
	}
	if ( def_co.get_clean_entity_handles() )
	{
		ent_change = cleanup_entity_handles( ent_handles );
	}

	return comp_ent_change || comp_change || ent_change;
}

logical asm_model::cleanup_model( asm_cleanup_options const* co )
{
	if(!this)
	{
		return FALSE;
	}

	logical change = FALSE;

	if ( use_count() == 0 )
	{
		// we can't call clear in the destructor since we may cause an access
		// violation during static cleanup because the history stream is already
		// gone, so call it before we call the destructor when we know the history
		// streams are still present
		if ( mgr_ptr )
		{
			// changed to use unbind() so that destructor is never called on a bound model
			mgr_ptr->unbind();
//			mgr_ptr->clear();
		}
		ACIS_DELETE this;	// destructor does cleanup
		change = TRUE;
	}
	else
	{
		change = cleanup_handles( co );
	}

	return change;
}

void asm_model::remove_entity_handle( entity_handle* handle )
{
	if(!this)
	{
		return;
	}

	if ( handle )
		ent_handles.remove( handle );
}

void asm_model::remove_component_handle( component_handle* handle )
{
	if(!this)
	{
		return;
	}

	if ( handle )
	{
		comp_handles.remove( handle );
	}
}

void asm_model::remove_component_entity_handle( component_entity_handle* handle )
{
	if(!this)
	{
		return;
	}

	if ( handle )
		comp_ent_handles.remove( handle );
}

asm_model_info asm_model::get_model_info() const
{
	if(!this)
	{
		sys_error( ASM_NULL_MODEL);
	}

	asm_model_entity_mgr* ent_mgr = mgr();
	if ( !ent_mgr )
	{
//		sys_error( MODEL_WITHOUT_MGR );
		asm_model_info dummy = restore_model_info();
		return dummy;
	}
	return ent_mgr->get_model_info();
}

double asm_model::get_units() const
{
	return get_model_info().model_units;
}

double asm_model::get_resabs() const
{
	return get_model_info().model_resabs;
}

double asm_model::get_resnor() const
{
	return get_model_info().model_resnor;
}

void asm_model::register_loaded_entities() const
{			
	if(!this)
	{
		return;
	}

	if (mgr_ptr)
	{
//		mgr_ptr->register_restored_entities(restore_ents);
		mgr_ptr->add_entities(restore_ents, TRUE);
	}
	clear_restore_info();
}

// used during restore to re-hold mrefs and assemblies
void asm_model::hold_mrefs_and_assemblies()
{			
	if(!this)
	{
		return;
	}

	{
		for(entity_handle* eh = ent_handles.first(); eh; eh = ent_handles.next())
		{
			// we need raw entity pointer, so use data member rather than method
			// the method filters out entities which have been lost, and those are exactly the
			// ones that we're looking for
            // Updated as part of the "handle uses proxy" story S4755
			ENTITY* ent = eh->pproxy ? eh->pproxy->pent : NULL;
			if(is_ASM_ASSEMBLY(ent))
			{
				hold_assembly_handle(eh);
			}
			else if(is_ASM_MODEL_REF(ent))
			{
				hold_mref_handle(eh);
			}
		}
	}
}

void asm_model::done_restoring()
{
	if(!this)
	{
		return;
	}

	if (mgr_ptr)
	{
		// KLUDGE - need to figure out who should activate model
		MODEL_BEGIN(this)
			// scan model-refs to build links with sub-models
			// has a MODEL_BEGIN(this) block, so nesting cuts down on spurious activations.
			hold_mrefs_and_assemblies();
			link_models_post_restore();
			mgr_ptr->done_restoring();
		MODEL_END(ASM_NO_CHANGE)
	}
}

void asm_model::add_entities(ENTITY_LIST& ents)
{			
	if(!this)
	{
		return;
	}

	if(!is_complete())
	{
		sys_error(ASM_INVALID_MODEL);
	}

	if (mgr_ptr)
	{
		mgr_ptr->add_entities(ents);
	}
}

void asm_model::clear_restore_info() const
{			
	if(!this)
	{
		return;
	}

	restore_ents.clear();
}

void asm_model::refresh_box_cache( SPAboxing_options* boxing_opts )
{
	if(!this)
	{
		return;
	}

	entity_mgr_box = calculate_box( boxing_opts );
	set_box_cache_state(TRUE);
}

void asm_model::set_box_cache_state(logical is_valid) const
{
	if(!this)
	{
		return;
	}

	box_valid = is_valid;
}

logical asm_model::box_cache_valid( SPAboxing_options* /*boxing_opts*/ ) const
{
	if(!this)
	{
		return FALSE;
	}

	// KLUDGE - don't cache for now
	//return box_valid;
	return FALSE;
}

SPAbox asm_model::get_box( SPAboxing_options* boxing_opts )
{
	SPAbox ret_box;
	if ( !box_cache_valid( boxing_opts ) )
	{
		refresh_box_cache( boxing_opts );
	}

	// protect against NULL this pointer
	if(this)
	{
		ret_box = entity_mgr_box;
	}
	return ret_box;
}

SPAbox asm_model::get_box( SPAboxing_options* boxing_opts ) const
{
	SPAbox ret_box;
	// protect against NULL this pointer
	if(this)
	{
		ret_box = entity_mgr_box;
	}

	if ( !box_cache_valid( boxing_opts ) )
	{
		ret_box = calculate_box( boxing_opts );
	}
	return ret_box;
}

SPAbox asm_model::calculate_box( SPAboxing_options* boxing_opts ) const
{
	SPAbox ret_box;

	if(!this)
	{
		return ret_box;
	}

	API_MODEL_BEGIN( (asm_model*)this )		// throw away const-ness

	// get the part data
	ENTITY_LIST ents;
	get_top_level_entities( ents, FALSE );
	
	// calculate the box
	if ( ents.iteration_count() > 0 )
	{
		SPAposition min_pt, max_pt;
		get_entity_box( ents, min_pt, max_pt, boxing_opts );
		ret_box = SPAbox( min_pt, max_pt );
	}

	API_MODEL_END( ASM_NO_CHANGE )
	check_outcome( result );	// resignal an error

	return ret_box;
}

void asm_model::add_owning_model( asm_model* model, int& add_count )
{
	if(!this)
	{
		sys_error(ASM_NULL_MODEL);
	}

	logical was_added = FALSE;
	owning_models.add_notify( model, was_added, add_count );
}

void asm_model::remove_owning_model( asm_model* model, int& add_count )
{
	if(!this)
	{
		sys_error(ASM_NULL_MODEL);
	}

	logical was_removed = FALSE;
	owning_models.remove_notify( model, was_removed, add_count );
}

void asm_model::add_sub_model( asm_model* model, int& add_count )
{
	if(!this)
	{
		sys_error(ASM_NULL_MODEL);
	}

	logical was_added = FALSE;
	sub_models.add_notify( model, was_added, add_count );
}

void asm_model::remove_sub_model( asm_model* model, int& add_count )
{
	if(!this)
	{
		sys_error(ASM_NULL_MODEL);
	}

	logical was_removed = FALSE;
	sub_models.remove_notify( model, was_removed, add_count );
}

void asm_model::remove_from_tree()
{
	if(!this)
	{
		return;
	}

	// remove this model from being a sub-model in all its owning models
	for ( asm_model* owning_model = owning_models.first(); owning_model; owning_model = owning_models.next() )
	{
		int count = 0;
		do
		{
			owning_model->remove_sub_model( this, count );
		} while ( count > 0 );
	}
	owning_models.clear();
	// remove this model from being an owning model of its all sub-models
	for ( asm_model* sub_model = sub_models.first(); sub_model; sub_model = sub_models.next() )
	{
		int count = 0;
		do 
		{
			sub_model->remove_owning_model( this, count );
		} while ( count > 0 );
	}
	sub_models.clear();
}

void asm_model::reset_model_tree_data()
{
	if(!this)
	{
		return;
	}

	owning_models.clear();
	sub_models.clear();
}

void asm_model::contents_changed( asm_event_type ev_type ) const
{
	if(!this)
	{
		return;
	}

	// reset cached info
	set_box_cache_state( FALSE );

	// propagate the change up the model tree
	for ( asm_model* model = owning_models.first(); model; model = owning_models.next() )
	{
		model->sub_model_contents_changed( ev_type, this );
	}
}

void asm_model::sub_model_contents_changed( asm_event_type ev_type, asm_model const* changed_model )
{
	if(!this)
	{
		return;
	}

	// reset cached info
	set_box_cache_state( FALSE );

	asm_model_entity_mgr* the_mgr = mgr();
	if ( the_mgr )
	{
		// callback to make customer updates
		the_mgr->sub_model_changed_vf( ev_type, (asm_model*)changed_model );  // KLUDGE - change signature to take const asm_model*
	}

	// propagate the change up the model tree
	for ( asm_model* model = owning_models.first(); model; model = owning_models.next() )
	{
		model->sub_model_contents_changed( ev_type, changed_model );
	}
}

void asm_model::begin() const
{
	// guard against NULL model pointer
	if(!this)
	{
		return;
	}

	// make sure first item on the stack is NULL
	initialize_active_model_stack();

	// get the from model from the stack
	void* dummy = NULL;
	active_model_stack.Top(dummy);
	asm_model* from_model = (asm_model*)dummy;

	// and push this model onto the stack as the new "from model"
	active_model_stack.Push((asm_model*)this); // cast away const to avoid defining a new container

	asm_model_entity_mgr* ent_mgr = mgr();
	if ( ent_mgr )
	{
		ent_mgr->nested_activate_vf(from_model);
	}
}

void asm_model::end(outcome& result, asm_event_type ev_type) const
{
	end(result, ev_type, NULL);
}

void asm_model::end(outcome& result, asm_event_type ev_type, asm_event_info* ev_info) const
{
	// guard against NULL model pointer
	if(!this)
	{
		return;
	}

	asm_model_entity_mgr* ent_mgr = mgr();
	if ( ent_mgr ) // Can't error out if NULL, since this can happen during failed restore
	{
		// perform manager updates (ev_type can change)
		ent_mgr->entities_changed_vf( result, ev_type, ev_info );
	}

	// get position just previous to top of stack (the model we're deactivating to)
	// Note that initialize put NULL as the first entry, and activated pushed a model pointer
	// so that to_model_index cannot be less than 0;
	int to_model_index = active_model_stack.Size() - 2;

	asm_model const* to_model = (asm_model const*)active_model_stack[to_model_index];

	if ( ent_mgr ) // Can't error out if NULL, since this can happen during failed restore
	{
		// reset history stream and tolerances to original state
		ent_mgr->nested_deactivate_vf(to_model);
	}

	void* top_model = NULL;
	logical popped = active_model_stack.Pop(top_model);

	if(!popped || top_model != (void*)this)
	{
		sys_error(BAD_MODEL_NESTING);
	}

	if ( ev_type != ASM_NO_CHANGE )
	{
		// perform model updates and start notification up the assembly tree
		contents_changed( ev_type );
	}
}

// KLUDGE - there's really no good reason for this to be a method of asm_model
void asm_model::get_property_owner( component_handle* comp, ATTRIB_COMPONENT_PROP_OWNER*& owner_attrib, logical create_if_absent )
{
	owner_attrib = NULL;
	if(!this)
	{
		if(create_if_absent)
		{
			sys_error(ASM_NULL_MODEL);
		}
		return;
	}

	asm_model_entity_mgr* ent_mgr = mgr();
	if ( ent_mgr )
	{
		if(get_default_stream(FALSE) != ent_mgr->get_history())
		{
			sys_error(API_ASM_WRONG_HISTORY);
		}
		
		ASM_ASSEMBLY* assembly = ent_mgr->get_assembly_ptr();
		if(assembly)
		{
			// get the first component property_owner attribute
			owner_attrib = find_comp_prop_attrib(assembly, comp);
		}

		if(owner_attrib == NULL && create_if_absent)
		{
			if(!assembly)
			{
				sys_error(ASM_NULL_ASSEMBLY);
			}

			// ensure that the property owning model is not a proper sub-model (owned by)
			// the owning model of the component modified by the property
			if ( sg_asmi_is_sub_model( this, comp->get_owning_model(), TRUE ) )
			{
				sys_error( ASM_INVALID_PROP_OWNER );
			}

			owner_attrib = ACIS_NEW ATTRIB_COMPONENT_PROP_OWNER(assembly, comp, this);
			comp->invalidate_property_owners_cache(this);
		}
	}
}

//**************************
// Class asm_cleanup_options
//**************************

asm_cleanup_options::asm_cleanup_options()
{
	ent = TRUE;
	comp = TRUE;
	comp_ent = TRUE;
}

asm_cleanup_options::asm_cleanup_options( logical ent_handles, logical comp_handles, logical comp_ent_handles )
{
	ent = ent_handles;
	comp = comp_handles;
	comp_ent = comp_ent_handles;
}

logical asm_cleanup_options::get_clean_entity_handles() const
{	return ent;	}

void asm_cleanup_options::set_clean_entity_handles(logical value) 
{	ent = value;	}

logical asm_cleanup_options::get_clean_component_handles() const
{	return comp;	}

void asm_cleanup_options::set_clean_component_handles(logical value) 
{	comp = value;	}

logical asm_cleanup_options::get_clean_component_entity_handles() const
{	return comp_ent;	}

void asm_cleanup_options::set_clean_component_entity_handles(logical value) 
{	comp_ent = value;	}

void asm_model::link_models_post_restore()
{
	if(!this)
	{
		return;
	}

	API_MODEL_BEGIN(this)

		// assembly pointer from manager
		ASM_ASSEMBLY* asm_ptr = mgr->get_assembly_ptr();  // safe since method will return NULL if mgr == NULL
		if(asm_ptr)
		{
			// Do the actual work
			ENTITY_LIST model_refs;
			asm_ptr->get_model_refs(model_refs);
			model_refs.init();
			for (ASM_MODEL_REF* model_ref = (ASM_MODEL_REF*)model_refs.first();
				model_ref;
				model_ref = (ASM_MODEL_REF*)model_refs.next())
			{
//					sg_asm_assembly_link_model_ref( asm_ptr, model_ref );
				asm_model* sub_model = model_ref->model();
				link_sub_model(sub_model);
			}
		}
	API_MODEL_END(ASM_NO_CHANGE);
	check_outcome(result);
}

void asm_model::unlink_sub_models_post_restore()
{
	if(!this)
	{
		return;
	}

	// remove this model from being an owning model of its all sub-models
	for ( asm_model* sub_model = sub_models.first(); sub_model; sub_model = sub_models.next() )
	{
		int count = 0;
		do 
		{
			sub_model->remove_owning_model( this, count );
		} while ( count > 0 );
	}
	sub_models.clear();
}

void asm_model::write_sub_models(asm_save_options_internal* /*sopts*/)
{
	// KLUDGE - can't do this until I have cardinality query for asm_model_list
	/*
	// write number of sub-model links
	write_int(sub_models.iteration_count());

	// write each link and its cardinality
	for(asm_model* sub_model = sub_models.first(); sub_model; sub_model = sub_models.next())
	{
		write_pointer((void*) INTEXTEND save_mgr->add(sub_model));
		write_int( // this is the part that I can't do
	}
	*/
}

void asm_model::read_sub_models()
{
	// KLUDGE - can't do this until I have cardinality query for asm_model_list
	/*
	// write number of sub-model links
	write_int(sub_models.iteration_count());

	// write each link and its cardinality
	for(asm_model* sub_model = sub_models.first(); sub_model; sub_model = sub_models.next())
	{
		write_pointer((void*) INTEXTEND save_mgr->add(sub_model));
		write_int( // this is the part that I can't do
	}
	*/
}

void asm_model::get_all_owning_models( logical include_self, asm_model_list& models )
{
	models.clear();
	if(!this)
	{
		return;
	}

	if ( include_self )
	{
		models.add( this );
	}
	else
	{
		models.add( owning_models );
	}

	for ( asm_model* model = models.first(); model; model = models.next() )
	{
		models.add( model->owning_models );
	}
}
