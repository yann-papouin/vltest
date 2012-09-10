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
//! \file selectionset.cpp implementation for the SelectionSet class.

#include "selectionset.h"
#include "worldhandle.h"

SelectionSet::SelectionSet(WorldHandle* pWorldHandle)
: m_pWorldHandle(pWorldHandle)
, m_OccurenceHash()
, m_InstanceToPrimitiveId()
{
	//assert(0 == m_pWorldHandle->collection()->selectionSize());
}

SelectionSet::~SelectionSet()
{

}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

bool SelectionSet::isEmpty() const
{
	return m_OccurenceHash.empty();
}

int SelectionSet::count() const
{
	return m_OccurenceHash.size() - m_InstanceToPrimitiveId.size();
}

std::vector<StructOccurence*> SelectionSet::occurencesList() const
{
	std::vector<StructOccurence*> listOfOccurence;
	tr1::unordered_map<unsigned int, StructOccurence*>::const_iterator iOcc= m_OccurenceHash.begin();
	while (m_OccurenceHash.end() != iOcc)
	{
		if (!m_InstanceToPrimitiveId.contains(iOcc.key()))
		{
			listOfOccurence.append(iOcc.value());
		}
		++iOcc;
	}
	return listOfOccurence;
}

std::vector<StructOccurence*> SelectionSet::occurencesListWithSelectedPrimitive() const
{
	std::vector<StructOccurence*> listOfOccurence;
	tr1::unordered_map<unsigned int, std::set<unsigned int> >::const_iterator iOcc= m_InstanceToPrimitiveId.begin();
	while (m_InstanceToPrimitiveId.end() != iOcc)
	{
		listOfOccurence.append(m_OccurenceHash.value(iOcc.key()));
		++iOcc;
	}
	return listOfOccurence;
}

std::set<unsigned int> SelectionSet::selectedPrimitivesId(unsigned int instanceId) const
{
	std::set<unsigned int> primitivesIdSet;
	if (m_InstanceToPrimitiveId.contains(instanceId))
	{
		primitivesIdSet= m_InstanceToPrimitiveId.value(instanceId);
	}
	return primitivesIdSet;
}

bool SelectionSet::hasPrimitiveSelected(unsigned int instanceId) const
{
	return m_InstanceToPrimitiveId.contains(instanceId);
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////
bool SelectionSet::insert(StructOccurence* pOccurence)
{
	return insert(pOccurence->id());
}

bool SelectionSet::insert(unsigned int occurenceId)
{
	assert(m_pWorldHandle->containsOccurence(occurenceId));
	if (!m_OccurenceHash.contains(occurenceId))
	{
		StructOccurence* pOccurence= m_pWorldHandle->getOccurence(occurenceId);
		m_OccurenceHash.insert(occurenceId, pOccurence);
		return true;
	}
	else return false;
}

bool SelectionSet::remove(StructOccurence* pOccurence)
{
	return remove(pOccurence->id());
}

bool SelectionSet::remove(unsigned int occurenceId)
{
	assert(m_pWorldHandle->containsOccurence(occurenceId));
	if (m_OccurenceHash.contains(occurenceId))
	{
		m_OccurenceHash.remove(occurenceId);
		m_InstanceToPrimitiveId.remove(occurenceId);
		return true;
	}
	else return false;
}

void SelectionSet::clear()
{
	m_OccurenceHash.clear();
	m_InstanceToPrimitiveId.clear();
}

bool SelectionSet::insertPrimitiveId(StructOccurence* pOccurence, unsigned int primitiveId)
{
	return insertPrimitiveId(pOccurence->id(), primitiveId);
}

bool SelectionSet::insertPrimitiveId(unsigned int occurenceId, unsigned int primitiveId)
{
	bool returnValue= false;
	insert(occurenceId);
	if (m_InstanceToPrimitiveId.contains(occurenceId))
	{
		if (!m_InstanceToPrimitiveId[occurenceId].contains(primitiveId))
		{
			m_InstanceToPrimitiveId[occurenceId].insert(primitiveId);
			returnValue= true;
		}
		else returnValue= false;
	}
	else
	{
		std::set<unsigned int> primitiveSet;
		primitiveSet.insert(primitiveId);
		m_InstanceToPrimitiveId.insert(occurenceId, primitiveSet);
		returnValue= true;
	}

	return returnValue;

}

void SelectionSet::insertSetOfPrimitivesId(StructOccurence* pOccurence, const std::set<unsigned int>& setOfPrimitivesId)
{
	insertSetOfPrimitivesId(pOccurence->id(), setOfPrimitivesId);
}

void SelectionSet::insertSetOfPrimitivesId(unsigned int occurenceId, const std::set<unsigned int>& setOfPrimitivesId)
{
	insert(occurenceId);
	if (m_InstanceToPrimitiveId.contains(occurenceId))
	{
		m_InstanceToPrimitiveId[occurenceId].unite(setOfPrimitivesId);
	}
	else
	{
		m_InstanceToPrimitiveId.insert(occurenceId, setOfPrimitivesId);
	}
}

bool SelectionSet::removePrimitiveId(StructOccurence* pOccurence, unsigned int primitiveId)
{
	return removePrimitiveId(pOccurence->id(), primitiveId);
}

bool SelectionSet::removePrimitiveId(unsigned int occurenceId, unsigned int primitiveId)
{
	assert(m_pWorldHandle->containsOccurence(occurenceId));

	if (m_InstanceToPrimitiveId.contains(occurenceId))
	{
		if (m_InstanceToPrimitiveId[occurenceId].contains(primitiveId))
		{
			if (m_InstanceToPrimitiveId[occurenceId].size() == 1)
			{
				remove(occurenceId);
			}
			else
			{
				m_InstanceToPrimitiveId[occurenceId].remove(primitiveId);
			}
			return true;
		}
		else return false;
	}
	else return false;
}
