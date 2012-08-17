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
// $Header: /files/homes/master/cvs/hoops_master/acispartviewer/HSAcisSelectionSet.cpp,v 1.27 2009-06-09 18:52:37 thomas Exp $
//

// HSAcisSelectionSet.cpp : implementation of the HPSelectionSet class
// 
#include "stdafx.h"
#include "resource.h"
#include "HSInclude.h"

#include "HSSelectionSet.h"
#include "HSAcisSelectionSet.h"

//#include "topolbrowserdlg.h"
#include "HSAcisSelectionSet.h"

#include "HSolidModel.h"
#include "HSolidView.h"
#include "vlist.h"


HSAcisSelectionSet::HSAcisSelectionSet(HBaseView* view) : HSSelectionSet(view)
{
	m_SelectLevel = BODY_TYPE;	// set default level to body
	m_bMultiSelectActive = false;
	SetAllowEntitySelection(true);
}

void HSAcisSelectionSet::Select(HC_KEY key, int num_include_keys, HC_KEY * include_keys, bool emit_message)
{
	HC_KEY	keys[40000];
	int		count = 0, c;

	ENTITY* entity = 0;
	KERN_CLASS_TYPEDEF SelectLevel = m_SelectLevel;

	if (m_bMultiSelectActive)
	{
		char type[MVO_BUFFER_SIZE];
		HC_Show_Key_Type(key, type);
		if (strcmp(type, "segment") == 0)
			SelectLevel = BODY_TYPE;
		if (strcmp(type, "shell") == 0)
			SelectLevel = FACE_TYPE;
		else if (strcmp(type, "marker") == 0)
			SelectLevel = VERTEX_TYPE;
		else 
			SelectLevel = EDGE_TYPE;
	}


	if (((HSolidModel *) m_pView->GetModel())->m_bSolidModel != true)
	{
		char	type[MVO_BUFFER_SIZE];
		HC_KEY	segkey;

		HC_Show_Key_Type(key, type);
		if (!streq(type, "segment"))
			segkey = HC_KShow_Owner_By_Key(key);
		else
			segkey = key;

		HSelectionSet::Select(segkey, num_include_keys, include_keys, emit_message); 
	}
	else
	{
		if (m_pView->GetRenderMode() == HRenderHiddenLine && SelectLevel==FACE_TYPE)
		{
			entity = HA_Compute_Entity_Pointer(key, FACE_TYPE);
			if (entity)
			{
				vlist_add_last(m_pSolidSelection, (void*)entity);
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");

				for (c = 0; c < count; c++)
					HSelectionSet::Select(keys[c], num_include_keys, include_keys, emit_message); // call base class fcn for each key

				ENTITY_LIST edges;
				int num_edges;
				outcome o = api_get_edges( entity, edges );
				CHECK_OUTCOME(o);
				num_edges = edges.iteration_count();
				for (int i=0;i<num_edges;i++)
				{
					count = HA_Compute_Geometry_Keys(edges[i], 40000, keys, "edges");
					for (c = 0; c < count; c++)
						HSelectionSet::Select(keys[c], num_include_keys, include_keys, emit_message); // call base class fcn for each key
				}
			}			 			
		}
		else
		{
			if(SelectLevel == KERN_INSTANCE_TYPE)
			{
				// TODO: Use the component HA apis when they are available
				HC_KEY* pReversePath = new HC_KEY[num_include_keys];
				for(int i = 0; i < num_include_keys; i++)
				{
					pReversePath[i] = include_keys[num_include_keys-1-i];
				}
				component_entity_handle* pCompEntity = HA_Compute_Component_Entity_Pointer(num_include_keys, pReversePath);
				if(pCompEntity && pCompEntity->component())
					count = HA_Compute_Component_Keys(pCompEntity->component(), 40000, keys);
			}
			else
			{			
				
				entity = HA_Compute_Entity_Pointer(key, SelectLevel);
				if (entity)
				{
					vlist_add_last(m_pSolidSelection, (void*)entity);
					if (SelectLevel == BODY_TYPE)  // && body segments is true 
					{
						count = HA_Compute_Geometry_Keys(entity, 1, keys, "bodies");
					}
					else if (SelectLevel == FACE_TYPE)
					{
						count = HA_Compute_Geometry_Keys(entity, 40000, keys, "faces");
						if (m_bShowFacesAsLines)
						{
							ENTITY_LIST edges;
							int num_edges;
							outcome o = api_get_edges( entity, edges );
							CHECK_OUTCOME(o);
							num_edges = edges.iteration_count();
							for (int i=0;i<num_edges;i++)
							{
								int ecount = HA_Compute_Geometry_Keys(edges[i], 40000-count, &keys[count], "edges");
								count += ecount;
							}
						}
					}
					else if (SelectLevel == EDGE_TYPE)
						count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");
					else if (SelectLevel == VERTEX_TYPE)
						count = HA_Compute_Geometry_Keys(entity, 40000, keys, "vertices");

				}
			}
			assert(count < 40000);

			if (count)
			{
				for (c = 0; c < count; c++)
				{ 
					HSelectionSet::Select(keys[c], num_include_keys, include_keys, emit_message); // call base class fcn for each key
				}
			}
			else
			{
				// if there is no modeler entity corresponding to this key here, let base class handle
				// this is added here so that we are able to select cutting planes, notes etc. in modeler partviewer 
				// Rajesh B (2-May-03)
//				HSolidModel *model = (HSolidModel *)m_pView->GetModel();
// 				MultiSelectManager *msm = model->GetMultiSelectManager();
// 				m_bMultiSelectActive = true;
// 				key = msm->MultiSelect(key, m_pView);
// 				m_bMultiSelectActive = false;
				HSelectionSet::Select(key, num_include_keys, include_keys, emit_message); 
			}
		}
	}
}

void HSAcisSelectionSet::Select(HC_KEY key, const char* segpath, HC_KEY include_key, HC_KEY includer_key, bool emit_message)
{
	HC_KEY	keys[40000];
	int		count = 0, c;
	
	ENTITY* entity = 0;
	
	if (((HSolidModel *) m_pView->GetModel())->m_bSolidModel != true)
	{
		char	type[MVO_BUFFER_SIZE];
		HC_KEY	segkey;
		
		HC_Show_Key_Type(key, type);
		if (!streq(type, "segment"))
			segkey = HC_KShow_Owner_By_Key(key);
		else
			segkey = key;
		
		HSelectionSet::Select(segkey, 0, NULL, emit_message); 
	}
	else
	{
		if (m_pView->GetRenderMode() == HRenderHiddenLine && m_SelectLevel==FACE_TYPE)
		{
			entity = HA_Compute_Entity_Pointer(key, FACE_TYPE);
			if (entity)
			{
				vlist_add_last(m_pSolidSelection, (void*)entity);
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");
				
				for (c = 0; c < count; c++)
					HSelectionSet::Select(keys[c], 0, NULL, emit_message); // call base class fcn for each key
				
				ENTITY_LIST edges;
				int num_edges;
				outcome o = api_get_edges( entity, edges );
				CHECK_OUTCOME(o);
				num_edges = edges.iteration_count();
				for (int i=0;i<num_edges;i++)
				{
					count = HA_Compute_Geometry_Keys(edges[i], 40000, keys, "edges");
					for (c = 0; c < count; c++)
						HSelectionSet::Select(keys[c], 0, NULL, emit_message); // call base class fcn for each key
				}
			}			 			
		}
		else
		{
			entity = HA_Compute_Entity_Pointer(key, m_SelectLevel);
			if (entity)
			{
				vlist_add_last(m_pSolidSelection, (void*)entity);
				if (m_SelectLevel == BODY_TYPE)  // && body segments is true 
				{
					count = HA_Compute_Geometry_Keys(entity, 1, keys, "bodies");
				}
				else if (m_SelectLevel == FACE_TYPE)
				{
					count = HA_Compute_Geometry_Keys(entity, 40000, keys, "faces");
					if (m_bShowFacesAsLines)
					{
						ENTITY_LIST edges;
						int num_edges;
						outcome o = api_get_edges( entity, edges );
						CHECK_OUTCOME(o);
						num_edges = edges.iteration_count();
						for (int i=0;i<num_edges;i++)
						{
							int ecount = HA_Compute_Geometry_Keys(edges[i], 40000-count, &keys[count], "edges");
							count += ecount;
						}
					}
				}
				else if (m_SelectLevel == EDGE_TYPE)
					count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");
				else if (m_SelectLevel == VERTEX_TYPE)
					count = HA_Compute_Geometry_Keys(entity, 40000, keys, "vertices");
				
			}
			assert(count < 40000);
			
			if (count)
			{
				for (c = 0; c < count; c++)
				{ 
						HSelectionSet::Select(keys[c], 0, NULL, emit_message); // call base class fcn for each key
				}
			}
			else
				// if there is no modeler entity corresponding to this key here, let base class handle
				// this is added here so that we are able to select cutting planes, notes etc. in modeler partviewer 
				// Rajesh B (2-May-03)
				HSelectionSet::Select(key, 1, &include_key, emit_message); 
			
		}
	}
}


void HSAcisSelectionSet::Select(ENTITY* entity, bool emit_message)
{
	HC_KEY	keys[40000];
	int		count = 0, c;
	
	int eclass;	
	eclass = entity->identity();
	HC_KEY incl_keys[1] = {m_pView->GetIncludeLinkKey()};
	
	if (m_pView->GetRenderMode() == HRenderHiddenLine && eclass == FACE_TYPE)
	{
		if (entity)
		{
			vlist_add_last(m_pSolidSelection, (void*)entity);
			count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");
			
			for (c = 0; c < count; c++)
				HSelectionSet::Select(keys[c], 1, incl_keys, emit_message); // call base class fcn for each key
			
			ENTITY_LIST edges;
			int num_edges;
			outcome o = api_get_edges( entity, edges );
			CHECK_OUTCOME(o);
			num_edges = edges.iteration_count();
			for (int i=0;i<num_edges;i++)
			{
				count = HA_Compute_Geometry_Keys(edges[i], 40000, keys, "edges");
				for (c = 0; c < count; c++) 
					HSelectionSet::Select(keys[c], 1, incl_keys, emit_message); // call base class fcn for each key
			}
		}			 			
	}
	else
	{
		if (entity)
		{
			vlist_add_last(m_pSolidSelection, (void*)entity);
			if (eclass == BODY_TYPE)  // && body segments is true 
				count = HA_Compute_Geometry_Keys(entity, 1, keys, "bodies");
			else if (eclass == FACE_TYPE)
			{
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "faces");
				if (m_bShowFacesAsLines)
				{
					ENTITY_LIST edges;
					int num_edges;
					outcome o = api_get_edges( entity, edges );
					CHECK_OUTCOME(o);
					num_edges = edges.iteration_count();
					for (int i=0;i<num_edges;i++)
					{
						int ecount = HA_Compute_Geometry_Keys(edges[i], 40000-count, &keys[count], "edges");
						count += ecount;
					}
				}
			}			
			else if (eclass == EDGE_TYPE)
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");
			else if (eclass == VERTEX_TYPE)
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "vertices");
			
		}
		
		assert(count < 40000);
		
		if (count)
		{
			for (c = 0; c < count; c++)
			{
						HSelectionSet::Select(keys[c], 1, incl_keys, emit_message); // call base class fcn for each key
			}
		}
	}
}


void HSAcisSelectionSet::Select(component_handle* component, bool emit_message)
{
	unsigned long n_sel_keys = 0;
	n_sel_keys = HA_Compute_Component_Key_Count(component);
	if( n_sel_keys > 0 )
	{
		// highlight the corresponding segments
		HC_KEY*	sel_keys = new HC_KEY[n_sel_keys];
		HA_Compute_Component_Keys(component, n_sel_keys, sel_keys);
		for(unsigned long i = 0; i < n_sel_keys; i++)
			HSelectionSet::Select(sel_keys[i], 0, NULL, emit_message); // call base class fcn for each key
		delete[] sel_keys;

		// add to the selection list
		// BIG NOTE: component_handle* CANNOT BE ADDED TO THE SELECTION LIST BECAUSE IT IS 
		// NOT AN ENTITY (UNFORTUNATELY). INSTEAD WE ARE ADDING AN EE_LIST (ENTITY-derived CLASS)
		// WHICH IS THE LIST OF ALL ASM_MODEL_REF THAT ARE REQUIRED TO IDENTIFY A component
		// Rajesh B. (13-Mar-07)
		HSolidModel* pModel = ((HSolidModel *) m_pView->GetModel());
		if(pModel)
		{
			EE_LIST* model_refs = pModel->GetEEListForComponent(component);
			if(model_refs)
				vlist_add_last(m_pSolidSelection, (void*)model_refs);
		}

	}
}

void HSAcisSelectionSet::RefreshSolidSelection()
{
	ENTITY* entities[40000];
	HC_KEY	keys[40000];
	int count;
	int i;
	int size=vlist_count(m_pSolidSelection);
	for (i=0;i<size;i++)
		entities[i]= (ENTITY*)vlist_nth_item(m_pSolidSelection, i);
	DeSelectAll();
	
	
	for (i=0;i<size;i++)
	{
		vlist_add_last(m_pSolidSelection, (void*)entities[i]);
		
		int eclass;
		eclass = entities[i]->identity();
		
		if (eclass == BODY_TYPE)  // && 'body segments is true 
			count = HA_Compute_Geometry_Keys(entities[i], 1, keys, "bodies");
		else if (eclass == FACE_TYPE)
		{
			count = HA_Compute_Geometry_Keys(entities[i], 40000, keys, "faces");
			if (m_bShowFacesAsLines /*|| HP_Get_Hash_Level()==1*/)
			{	
				HC_KEY	keys[1000];
				ENTITY_LIST edges;
				int num_edges;
				outcome o = api_get_edges( entities[i], edges );
				CHECK_OUTCOME(o);
				num_edges = edges.iteration_count();
				for (int j=0;j<num_edges;j++)
				{
					int count = HA_Compute_Geometry_Keys(edges[j], 1000, keys, "edges");
					for (int c = 0; c < count; c++)
						HSelectionSet::Select(keys[c], 0, NULL, true); // call base class fcn for each key
				}
			}
		}
		else if (eclass == EDGE_TYPE)
			count = HA_Compute_Geometry_Keys(entities[i], 40000, keys, "edges");
		else if (eclass == VERTEX_TYPE)
			count = HA_Compute_Geometry_Keys(entities[i], 40000, keys, "vertices");
		
		
		for (int c = 0; c < count; c++)  
			HSelectionSet::Select(keys[c], 0, NULL, true); // call base class fcn for each key
	}
}


void HSAcisSelectionSet::DeSelect(HC_KEY key, int num_include_keys, HC_KEY * include_keys, bool emit_message)
{
	HC_KEY keys[40000];
	int count = 0, c;

	if (((HSolidModel *) m_pView->GetModel())->m_bSolidModel != true)
	{
		char	type[MVO_BUFFER_SIZE];
		HC_KEY	segkey;

		HC_Show_Key_Type(key, type);
		if (!streq(type, "segment"))
			segkey = HC_KShow_Owner_By_Key(key);
		else
			segkey = key;

		HSelectionSet::DeSelect(segkey, num_include_keys, include_keys, emit_message);
	}
	else
	{
		// TODO: Handle de-selection of component_handle
		ENTITY*	entity = 0;
		int		n = 0;
		entity = HA_Compute_Entity_Pointer(key, m_SelectLevel);

		if (entity)
		{
			vlist_remove(m_pSolidSelection, (void*)entity);

			if (m_SelectLevel == BODY_TYPE)  // && 'body segments is true 
				count = HA_Compute_Geometry_Keys(entity, 1, keys, "bodies");
			else if (m_SelectLevel == FACE_TYPE)
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "faces");
			else if (m_SelectLevel == EDGE_TYPE)
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");
			else if (m_SelectLevel == VERTEX_TYPE)
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "vertices");

		}

		assert(count < 40000);

		if (count)
		{
			for (c = 0; c < count; c++)
			{
				HSelectionSet::DeSelect(keys[c], emit_message);	// call base class fcn for each key
			}
		}
		else
			// call the base-class anyway to deselect
			HSelectionSet::DeSelect(key, num_include_keys, include_keys, emit_message);
	}
}


void HSAcisSelectionSet::DeSelect(HC_KEY key, bool emit_message )
{
	HC_KEY keys[40000];
	int count = 0, c;
	
	if (((HSolidModel *) m_pView->GetModel())->m_bSolidModel != true)
	{
		char	type[MVO_BUFFER_SIZE];
		HC_KEY	segkey;
		
		HC_Show_Key_Type(key, type);
		if (!streq(type, "segment"))
			segkey = HC_KShow_Owner_By_Key(key);
		else
			segkey = key;
		
		HSelectionSet::DeSelect(segkey, emit_message);
	}
	else
	{
		ENTITY*	entity = 0;
		int		n = 0;
		entity = HA_Compute_Entity_Pointer(key, m_SelectLevel);
		
		if (entity)
		{
			vlist_remove(m_pSolidSelection, (void*)entity);
			
			if (m_SelectLevel == BODY_TYPE)  // && 'body segments is true 
				count = HA_Compute_Geometry_Keys(entity, 1, keys, "bodies");
			else if (m_SelectLevel == FACE_TYPE)
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "faces");
			else if (m_SelectLevel == EDGE_TYPE)
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");
			else if (m_SelectLevel == VERTEX_TYPE)
				count = HA_Compute_Geometry_Keys(entity, 40000, keys, "vertices");
			
		}
		
		assert(count < 40000);
		
		if (count)
		{
			for (c = 0; c < count; c++)
			{
				HSelectionSet::DeSelect(keys[c], emit_message);	// call base class fcn for each key
			}
		}
		else
			// call the base-class anyway to deselect
			HSelectionSet::DeSelect(key, emit_message);
	}
}


void HSAcisSelectionSet::DeSelectEntity(ENTITY* entity, bool emit_message )
{
	HC_KEY keys[40000];
	int count = 0, c;
	int n = 0;
	
	
	if (entity > 0)
	{
		vlist_remove(m_pSolidSelection, (void*) entity);
		
		if (m_SelectLevel == BODY_TYPE)  // && 'body segments is true 
			count = HA_Compute_Geometry_Keys(entity, 1, keys, "bodies");
		else if (m_SelectLevel == FACE_TYPE)
			count = HA_Compute_Geometry_Keys(entity, 40000, keys, "faces");
		else if (m_SelectLevel == EDGE_TYPE)
			count = HA_Compute_Geometry_Keys(entity, 40000, keys, "edges");
		else if (m_SelectLevel == VERTEX_TYPE)
			count = HA_Compute_Geometry_Keys(entity, 40000, keys, "vertices");
		
	}
	
	assert(count < 40000);
	
	if (count)
	{
		for (c = 0; c < count; c++)
		{
			HSelectionSet::DeSelect(keys[c], emit_message);	// call base class fcn for each key
		}
	}
}

void HSAcisSelectionSet::DeSelectComponent(component_handle* component, bool emit_message )
{
	if (component != NULL)
	{
		// remove from selection list
		HSolidModel* pModel = (HSolidModel*)(m_pView->GetModel());
		if( pModel )
		{
			EE_LIST* pModelRefs = pModel->GetEEListForComponent(component);
			if(pModelRefs)
				vlist_remove(m_pSolidSelection, (void*) pModelRefs);
		}

		// de-highlight each key
		unsigned long n_sel_keys = 0;
		n_sel_keys = HA_Compute_Component_Key_Count(component);
		if( n_sel_keys > 0 )
		{
			// highlight the corresponding segments
			HC_KEY*	sel_keys = new HC_KEY[n_sel_keys];
			HA_Compute_Component_Keys(component, n_sel_keys, sel_keys);
			for(unsigned long i = 0; i < n_sel_keys; i++)
				HSelectionSet::DeSelect(sel_keys[i], 0, NULL, emit_message); // call base class fcn for each key
			delete[] sel_keys;
		}
	}

}
bool HSAcisSelectionSet::IsSelected(HC_KEY key, int num_include_keys, const HC_KEY * include_keys) const
{
	if (((HSolidModel *) m_pView->GetModel())->m_bSolidModel != true)
	{
		char	type[MVO_BUFFER_SIZE];
		HC_Show_Key_Type(key, type);
		if (!streq(type, "segment"))
			key = HC_KShow_Owner_By_Key(key);

		return HSelectionSet::IsSelected(key, num_include_keys, include_keys);
	}
	else
	{
		// TODO: check for component
		if (m_SelectLevel == BODY_TYPE)  // && body segments is true
		{
			ENTITY* entity = 0;
			int count;

			entity = HA_Compute_Entity_Pointer(key, m_SelectLevel);

			if (entity)
			{
				count = HA_Compute_Geometry_Keys(entity, 1, &key, "bodies");
				assert(count == 1);
			}
		}
		return HSelectionSet::IsSelected(key, num_include_keys, include_keys);
	}
}

bool HSAcisSelectionSet::IsSelected(HC_KEY key)
{
	
	if (((HSolidModel *) m_pView->GetModel())->m_bSolidModel != true)
	{
		char	type[MVO_BUFFER_SIZE];
		HC_Show_Key_Type(key, type);
		if (!streq(type, "segment"))
			key = HC_KShow_Owner_By_Key(key);
	}
	else
	{
		if (m_SelectLevel == BODY_TYPE)  // && body segments is true
		{
			ENTITY* entity = 0;
			int count;
			
			entity = HA_Compute_Entity_Pointer(key, m_SelectLevel);
			
			if (entity)
			{
				count = HA_Compute_Geometry_Keys(entity, 1, &key, "bodies");
				assert(count == 1);
			}
		}
	}
	// call base class function with the computed key
	return HSelectionSet::IsSelected(key,0,0);
}


bool HSAcisSelectionSet::IsSelected(ENTITY* pEntity)
{
	int size = vlist_count(m_pSolidSelection);
	for( int i = 0; i < size; i++ )
	{
		if( pEntity == ((ENTITY*)vlist_nth_item(m_pSolidSelection, i)) )
			return true;
	}
	return false;
}

bool HSAcisSelectionSet::IsSelected(component_handle* pComponent)
{
	HSolidModel* pModel = (HSolidModel*) m_pView->GetModel();
	EE_LIST* pModelRefs = pModel->GetEEListForComponent(pComponent);

	int size = vlist_count(m_pSolidSelection);
	for( int i = 0; i < size; i++ )
	{
		if( ((ENTITY*)pModelRefs) == ((ENTITY*)vlist_nth_item(m_pSolidSelection, i)) )
			return true;
	}
	return false;
}


