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
//! \file SelectionSet.h interface for the SelectionSet class.

#ifndef SelectionSet_H_
#define SelectionSet_H_

#include "struct/occurence.h"

#include <vector>
#include <set>
#include <hash_map>


class WorldHandle;

//////////////////////////////////////////////////////////////////////
//! \class SelectionSet
/*! \brief SelectionSet : Occurence and primitive selection set */
//////////////////////////////////////////////////////////////////////
class SelectionSet
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////

public:
	//! Construct the selection set associated to the given WorldHandle
	SelectionSet(WorldHandle* pWorld);
	virtual ~SelectionSet();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Return true if this selection set is empty
	bool isEmpty() const;

	//! Return the number of occurence in this selection set
	inline int size() const
	{return count();}

	//! Return the number of occurence in this selection set
	int count() const;

	//! Return the list of selected occurences
	std::vector<Occurence*> occurencesList() const;

	//! Return the list of occurences with selected primitive
	std::vector<Occurence*> occurencesListWithSelectedPrimitive() const;

	//! Return the set of primitive id of the given Occurence/3DviewInstance id
	std::set<unsigned int> selectedPrimitivesId(unsigned int instanceId) const;

	//! Return true if the given Occurence/3DviewInstance id has selected primitives
	bool hasPrimitiveSelected(unsigned int instanceId) const;

	//! Return true if this selection set contains the given occurence
	bool contains(const Occurence* pOccurence) const
	{return contains(pOccurence->id());}

	//! Return true if this selection set contains the given occurence id
	bool contains(unsigned int occurenceId) const
	{return m_OccurenceHash.find(occurenceId) != m_OccurenceHash.end();}

//@}
//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Insert the given Occurence into the selection set and return true on success
	/*! The given occurence must belongs to this selection set's world*/
	bool insert(Occurence* pOccurence);

	//! Insert the given Occurence id into the selection set and return true on success
	/*! The given occurence id must belongs to this selection set's world*/
	bool insert(unsigned int occurenceId);

	//! Remove the given occurence from the selection set and return true on success
	/*! The given occurence must belongs to this selection set's world*/
	bool remove(Occurence* pOccurence);

	//! Remove the given occurence from the selection set and return true on success
	/*! The given occurence id must belongs to this selection set's world*/
	bool remove(unsigned int occurenceId);

	//! Clear this selection set
	void clear();

	//! Insert the given primitive id to the given Occurence return true on success
	/*! The given occurence must belongs to this selection set's world*/
	bool insertPrimitiveId(Occurence* pOccurence, unsigned int primitiveId);

	//! Insert the given primitive id to the given Occurence id return true on success
	/*! The given occurence id must belongs to this selection set's world*/
	bool insertPrimitiveId(unsigned int occurenceId, unsigned int primitiveId);

	//! Insert the given set of primitive id to the given Occurence
	/*! The given occurence must belongs to this selection set's world*/
	void insertSetOfPrimitivesId(Occurence* pOccurence, const std::set<unsigned int>& setOfPrimitivesId);

	//! Insert the given set of primitive id to the given Occurence id
	/*! The given occurence id must belongs to this selection set's world*/
	void insertSetOfPrimitivesId(unsigned int occurenceId, const std::set<unsigned int>& setOfPrimitivesId);

	//! Remove the given primitive id to the given Occurence return true on success
	/*! The given occurence must belongs to this selection set's world
	 *  If the set of primitive only contains the given primitive ID
	 *  the given occurence is removed from this selection set
	 */
	bool removePrimitiveId(Occurence* pOccurence, unsigned int primitiveId);

	//! Remove the given primitive id to the given Occurence return true on success
	/*! The given occurence must belongs to this selection set's world
	 *  If the set of primitive only contains the given primitive ID
	 *  the given occurence is removed from this selection set
	 */
	bool removePrimitiveId(unsigned int occurenceId, unsigned int primitiveId);


//@}

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! The worldHandle attached to this selection set
	WorldHandle* m_pWorldHandle;

	//! Hash table of selected occurence
	stdext::hash_map<unsigned int, Occurence*> m_OccurenceHash;

	//! Hash table of instance id to set of primitive id
	stdext::hash_map<unsigned int, std::set<unsigned int> > m_InstanceToPrimitiveId;

};

#endif /* SelectionSet_H_ */
