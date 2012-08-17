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
// $Header: /files/homes/master/cvs/hoops_master/shared/partviewer_common/HSSelectionSet.cpp,v 1.25 2007-08-16 19:57:02 nathan Exp $
//

// HSSelectionSet.cpp : implementation of the HPSelectionSet class
// 


#include "stdafx.h"

#include <assert.h>
#include "resource.h"

#include "HSInclude.h"

#include "HSolidView.h"
// #ifndef HOOPS_ONLY
// #include "TopolBrowserDlg.h"
// #endif // HOOPS_ONLY


#include "HSSelectionSet.h"
#include "HSolidModel.h"
#include "HBaseOperator.h"

#include "hc.h"
#include "vlist.h" 




HSSelectionSet::HSSelectionSet(HBaseView* view, bool bReferenceSelection)
	:HSelectionSet(view, bReferenceSelection)
{
	m_pSolidSelection = 0;
	m_bShowFacesAsLines = false;
	SetAllowEntitySelection(false);
}



HSSelectionSet::~HSSelectionSet()
{
	if( m_pSolidSelection )
	{
		delete_vlist(m_pSolidSelection);
		m_pSolidSelection = 0;
	}
}

// create a new list  object
void HSSelectionSet::Init()
{
	m_pSolidSelection = new_vlist(malloc, free);

	HSelectionSet::Init();
}


long HSSelectionSet::GetAtSolidEntity(int index)
{
	return (long) vlist_nth_item( m_pSolidSelection, index );
}


int HSSelectionSet::GetSolidListSize()
{
	return vlist_count( m_pSolidSelection );
}




// deselect all items as in DeSelect
void HSSelectionSet::DeSelectAll()
{
	if( m_pSolidSelection )
	{
		delete_vlist(m_pSolidSelection);
		m_pSolidSelection = new_vlist(malloc, free);
	}

	HSelectionSet::DeSelectAll();
	m_pView->EmitDeSelectAllMessage();
}


void HSSelectionSet::Reset()
{
	if( m_pSolidSelection )
	{
		delete_vlist(m_pSolidSelection);
		m_pSolidSelection = new_vlist(malloc, free);
	}

	HSelectionSet::Reset();
}


//======================================================================
//	Author :Rajesh B			Created : 8/22/2001 2:38:18 PM
//======================================================================
//	Function :		HSSelectionSet::SelectFromMessage
//	Description :	If it is a call to select something from HNet message,
//					by pass our selection functionality and just call the 
//					vanilla Select. JUST SELECT THIS KEY!
//					This is to work around the case where the master client
//					is in some selection mode (face) and slave is in some (body)
//	Return :void	-	
//	Parameters :
//			HC_KEY key	-	
//			char* segpath	-	
//			HC_KEY include_key	-	
//			HC_KEY includer_key	-	
//			bool emit_message	-	
//	Note :
//======================================================================
void HSSelectionSet::SelectFromMessage(HC_KEY key, char* segpath, HC_KEY include_key, HC_KEY includer_key, bool emit_message)
{
	HSelectionSet::Select(key, segpath, INVALID_KEY, INVALID_KEY, emit_message);
}

//======================================================================
//	Author :Rajesh B			Created : 8/22/2001 2:40:31 PM
//======================================================================
//	Function :		HSSelectionSet::DeSelectFromMessage
//	Description :	See the Description for SelectFromMessage
//	Return :void	-	
//	Parameters :
//			HC_KEY key	-	
//			bool emit_message	-	
//	Note :
//======================================================================
void HSSelectionSet::DeSelectFromMessage(HC_KEY key, bool emit_message)
{
	HSelectionSet::DeSelect(key, emit_message);
}
