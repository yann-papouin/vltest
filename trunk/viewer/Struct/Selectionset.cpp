//! \file SelectionSet.cpp implementation for the SelectionSet class.
#include <assert.h>
#include "struct/SelectionSet.h"
#include "struct/worldhandle.h"

SelectionSet::SelectionSet(WorldHandle* pWorldHandle)
: m_pWorldHandle(pWorldHandle)
, m_OccurenceHash()
, m_InstanceToPrimitiveId()
{
//	assert(0 == m_pWorldHandle->collection()->selectionSize());
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

std::vector<Occurence*> SelectionSet::occurencesList() const
{
	std::vector<Occurence*> listOfOccurence;
	stdext::hash_map<unsigned int, Occurence*>::const_iterator iOcc= m_OccurenceHash.begin();
	while (m_OccurenceHash.end() != iOcc)
	{
		if (m_InstanceToPrimitiveId.find(iOcc->first)!=m_InstanceToPrimitiveId.end())
		{
			listOfOccurence.push_back(iOcc->second);
		}
		++iOcc;
	}
	return listOfOccurence;
}

std::vector<Occurence*> SelectionSet::occurencesListWithSelectedPrimitive() const
{
	std::vector<Occurence*> listOfOccurence;
	stdext::hash_map<unsigned int, std::set<unsigned int> >::const_iterator iOcc= m_InstanceToPrimitiveId.begin();
	while (m_InstanceToPrimitiveId.end() != iOcc)
	{
		stdext::hash_map<unsigned int, Occurence*>::const_iterator itor;
		if ((itor = m_OccurenceHash.find(iOcc->first))!=m_OccurenceHash.end())
		{
			Occurence* pOccurence = itor->second;
			listOfOccurence.push_back(pOccurence);
		}

		++iOcc;
	}
	return listOfOccurence;
}

std::set<unsigned int> SelectionSet::selectedPrimitivesId(unsigned int instanceId) const
{
	std::set<unsigned int> primitivesIdSet;

	stdext::hash_map<unsigned int, std::set<unsigned int> >::const_iterator itor;
	if ((itor = m_InstanceToPrimitiveId.find(instanceId))!=m_InstanceToPrimitiveId.end())
	{
		primitivesIdSet = itor->second;
	}
	return primitivesIdSet;
}

bool SelectionSet::hasPrimitiveSelected(unsigned int instanceId) const
{
	return m_InstanceToPrimitiveId.find(instanceId) != m_InstanceToPrimitiveId.end();
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////
bool SelectionSet::insert(Occurence* pOccurence)
{
	return insert(pOccurence->id());
}

bool SelectionSet::insert(unsigned int occurenceId)
{
	assert(m_pWorldHandle->containsOccurence(occurenceId));
	if (m_OccurenceHash.find(occurenceId)!=m_OccurenceHash.end())
	{
		const Occurence* pOccurence= m_pWorldHandle->getOccurence(occurenceId);
		m_OccurenceHash[occurenceId]=const_cast<Occurence*>(pOccurence);
		return true;
	}
	else return false;
}

bool SelectionSet::remove(Occurence* pOccurence)
{
	return remove(pOccurence->id());
}

bool SelectionSet::remove(unsigned int occurenceId)
{
	assert(m_pWorldHandle->containsOccurence(occurenceId));
	if (m_OccurenceHash.find(occurenceId) != m_OccurenceHash.end())
	{
		m_OccurenceHash.erase(occurenceId);
		m_InstanceToPrimitiveId.erase(occurenceId);
		return true;
	}
	else return false;
}

void SelectionSet::clear()
{
	m_OccurenceHash.clear();
	m_InstanceToPrimitiveId.clear();
}

bool SelectionSet::insertPrimitiveId(Occurence* pOccurence, unsigned int primitiveId)
{
	return insertPrimitiveId(pOccurence->id(), primitiveId);
}

bool SelectionSet::insertPrimitiveId(unsigned int occurenceId, unsigned int primitiveId)
{
	bool returnValue= false;
	insert(occurenceId);
	if (m_InstanceToPrimitiveId.find(occurenceId)!=m_InstanceToPrimitiveId.end())
	{
		if (m_InstanceToPrimitiveId[occurenceId].find(primitiveId)!=m_InstanceToPrimitiveId[occurenceId].end())
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
		m_InstanceToPrimitiveId[occurenceId]=primitiveSet;
		returnValue= true;
	}

	return returnValue;

}

void SelectionSet::insertSetOfPrimitivesId(Occurence* pOccurence, const std::set<unsigned int>& setOfPrimitivesId)
{
	insertSetOfPrimitivesId(pOccurence->id(), setOfPrimitivesId);
}

void SelectionSet::insertSetOfPrimitivesId(unsigned int occurenceId, const std::set<unsigned int>& setOfPrimitivesId)
{
	insert(occurenceId);
	if (m_InstanceToPrimitiveId.find(occurenceId)!=m_InstanceToPrimitiveId.end())
	{
		m_InstanceToPrimitiveId[occurenceId].insert(setOfPrimitivesId.begin(),setOfPrimitivesId.end());
	}
	else
	{
		m_InstanceToPrimitiveId[occurenceId] = setOfPrimitivesId;
	}
}

bool SelectionSet::removePrimitiveId(Occurence* pOccurence, unsigned int primitiveId)
{
	return removePrimitiveId(pOccurence->id(), primitiveId);
}

bool SelectionSet::removePrimitiveId(unsigned int occurenceId, unsigned int primitiveId)
{
	assert(m_pWorldHandle->containsOccurence(occurenceId));

	if (m_InstanceToPrimitiveId.find(occurenceId)!=m_InstanceToPrimitiveId.end())
	{
		if (m_InstanceToPrimitiveId[occurenceId].find(primitiveId)!=m_InstanceToPrimitiveId[occurenceId].end())
		{
			if (m_InstanceToPrimitiveId[occurenceId].size() == 1)
			{
				remove(occurenceId);
			}
			else
			{
				m_InstanceToPrimitiveId[occurenceId].erase(primitiveId);
			}
			return true;
		}
		else return false;
	}
	else return false;
}
