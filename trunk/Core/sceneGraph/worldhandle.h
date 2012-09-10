/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 *****************************************************************************/

#ifndef WORLDHANDLE_H_
#define WORLDHANDLE_H_

//#include "3dviewcollection.h"
#include "structoccurence.h"
#include "selectionset.h"
#include <hash_map>

#include "../config.h"

//////////////////////////////////////////////////////////////////////
//! \class WorldHandle
/*! \brief WorldHandle : Handle of shared World*/
//////////////////////////////////////////////////////////////////////
class GVCORE_EXPORT WorldHandle
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! The default constructor
	WorldHandle();

	//! The default destructor
	~WorldHandle();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the collection
	//inline 3DViewCollection* collection()
	//{return &m_Collection;}

	//! Return the number of world associated with this handle
	inline int numberOfWorld() const
	{return m_NumberOfWorld;}

	//! Return true if there is only one world associated with this handle
	inline bool isOrphan() const
	{return m_NumberOfWorld == 0;}

	//! Return true if the specified occurence id is in this world
	inline bool containsOccurence(unsigned int id) const
	{return m_OccurenceHash.contains(id);}

	//! Return the occurence specified by an id
	/*! Id must be a valid identifier*/
	inline StructOccurence* getOccurence(unsigned int id) const
	{
		assert(m_OccurenceHash.contains(id));
		return m_OccurenceHash.value(id);
	}

	//! Return the list off occurences
	inline std::vector<StructOccurence*> occurences() const
	{return m_OccurenceHash.values();}

	//! Return the number of occurence
	inline int numberOfOccurence() const
	{return m_OccurenceHash.size();}

	//! Return the list of instance
	std::vector<StructInstance*> instances() const;

	//! Return the list of Reference
	std::vector<StructReference*> references() const;

	//! Return the number of body
	int numberOfBody() const;

	//! Return the number of representation
	int representationCount() const;

	//! Return the world Up vector
	inline vl::dvec3 upVector() const
	{return m_UpVector;}

	//! Return an handle to the selection set
	inline SelectionSet* selectionSetHandle()
	{return &m_SelectionSet;}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Increment the number of world
	inline void increment()
	{++m_NumberOfWorld;}

	//! Decrement the number of world
	inline void decrement()
	{--m_NumberOfWorld;}

	//! An Occurence has been added
	void addOccurence(StructOccurence* pOccurence, bool isSelected= false, GLuint shaderId= 0);

	//! An Occurence has been removed
	void removeOccurence(StructOccurence* pOccurence);

	//! All Occurence has been removed
	inline void removeAllOccurences()
	{
		m_OccurenceHash.clear();
	}

	//! Set the world Up Vector
	inline void setUpVector(const vl::dvec3& vect)
	{m_UpVector= vect;}

	//! Set the attached viewport of this world handle
	//inline void setAttachedViewport(Viewport* pViewport)
	//{m_Collection.setAttachedViewport(pViewport);}

	//! Select the given occurence id
	/*! The given occurence id must belong to this worldhandle*/
	void select(unsigned int occurenceId, bool propagate= true);

	//! Unselect the given occurence id
	/*! The given occurence id must belong to this worldhandle*/
	void unselect(unsigned int occurenceId, bool propagate= true);

	//! Select all occurence of this world handle
	void selectAllWith3DViewInstance(bool allShowState);

	//! Unselecte all occurence of this world handle
	void unselectAll();

	//! Show / Hide selected 3DViewInstance
	void showHideSelected3DViewInstance();

	//! Set selected 3DViewInstance visibility
	void setSelected3DViewInstanceVisibility(bool isVisible);


//@}

//////////////////////////////////////////////////////////////////////
// private members
//////////////////////////////////////////////////////////////////////
private:
	//! The Collection
//	3DViewCollection m_Collection;

//	VLBaseView mVLBaseView;

	//! Number of this world
	int m_NumberOfWorld;

	//! The hash table containing struct occurence
	stdext::hash_map<unsigned int, StructOccurence*> m_OccurenceHash;

	//! This world Up Vector
	dvec3 m_UpVector;

	//! This world selectionSet
	SelectionSet m_SelectionSet;
};

#endif /* WORLDHANDLE_H_ */
