#include <assert.h>
#include "struct/Worldhandle.h"
#include "struct/reference.h"
#include "struct/Represent3D.h"
#include <set>
#include <vector>
#include <hash_map>

WorldHandle::WorldHandle()
: m_NumberOfWorld(1)
, m_OccurenceHash()
// , m_UpVector(glc::Z_AXIS)
, m_SelectionSet(this)
{

}

WorldHandle::~WorldHandle()
{

}

// Return the list of instance
std::vector<Instance*> WorldHandle::instances() const
{
	std::vector<Instance*> vecInstances;
//	std::set<Instance*> instancesSet;
	stdext::hash_map<unsigned int, Occurence*>::const_iterator iOccurence= m_OccurenceHash.begin();
	while (iOccurence != m_OccurenceHash.end())
	{
//		instancesSet.insert(iOccurence->second->instance());
		vecInstances.push_back(iOccurence->second->instance());
		++iOccurence;
	}
	return vecInstances;
}

// Return the list of Reference
std::vector<Reference*> WorldHandle::references() const
{
	std::vector<Reference*> vecReferences;
//	std::set<Reference*> referencesSet;
	stdext::hash_map<unsigned int, Occurence*>::const_iterator iOccurence= m_OccurenceHash.begin();
	while (iOccurence != m_OccurenceHash.end())
	{
//		referencesSet.insert(iOccurence->second->reference());
		vecReferences.push_back(iOccurence->second->reference());
		++iOccurence;
	}
	return vecReferences;
}

// Return the number of body
int WorldHandle::numberOfBody() const
{
	std::vector<Reference*> referenceList(references());
	const int size= referenceList.size();
	int numberOfBody= 0;
	for (int i= 0; i < size; ++i)
	{
		numberOfBody+= referenceList.at(i)->numberOfBody();
	}
	return numberOfBody;
}

int WorldHandle::representationCount() const
{
	std::vector<Reference*> referenceList(references());
	const int size= referenceList.size();
	int count= 0;
	for (int i= 0; i < size; ++i)
	{
		if (referenceList.at(i)->hasRepresentation()) ++count;
	}
	return count;

}

// An Occurence has been added
void WorldHandle::addOccurence(Occurence* pOccurence, bool isSelected, unsigned int shaderId)
{
	assert(m_OccurenceHash.find(pOccurence->id()) != m_OccurenceHash.end());
	m_OccurenceHash[pOccurence->id()]= pOccurence;
	Reference* pRef= pOccurence->reference();
	assert(NULL != pRef);

	// Add instance representation in the collection
	//if (pOccurence->useAutomatic3DViewInstanceCreation() && pRef->representationIsLoaded())
	//{
	//	//qDebug() << "WorldHandle::addOccurence with rep";
	//	Represent3D* pRepresent3D= dynamic_cast<Represent3D*>(pRef->representationHandle());
	//	3DViewInstance representation(*pRepresent3D);
	//	// Force instance representation id
	//	representation.setId(pOccurence->id());
	//	// Force instance representation name
	//	representation.setName(pOccurence->name());
	//	if (0 != shaderId) m_Collection.bindShader(shaderId);
	//	m_Collection.add(representation, shaderId);
	//	if (isSelected)
	//	{
	//		//qDebug() << pOccurence->name() << "selected";
	//		m_Collection.select(pOccurence->id());
	//	}
	//}
}

// An Occurence has been removed
void WorldHandle::removeOccurence(Occurence* pOccurence)
{
	assert(m_OccurenceHash.find(pOccurence->id()) != m_OccurenceHash.end());
	// Remove the occurence from the selection set
//	m_SelectionSet.erase(pOccurence);
	// Remove the occurence from the main occurence hash table
//	m_OccurenceHash.erase(pOccurence->id());
	// Remove instance representation from the collection
//	m_Collection.erase(pOccurence->id());
}

void WorldHandle::select(unsigned int occurenceId, bool propagate)
{
	assert(m_OccurenceHash.find(occurenceId) != m_OccurenceHash.end());
	m_SelectionSet.insert(occurenceId);
//	m_Collection.select(occurenceId);

	const Occurence* pSelectedOccurence= m_OccurenceHash[occurenceId];
	if (propagate && pSelectedOccurence->hasChild())
	{
		std::vector<Occurence*> subOccurenceList= pSelectedOccurence->subOccurenceList();
		const int subOccurenceCount= subOccurenceList.size();
		for (int i= 0; i < subOccurenceCount; ++i)
		{
			const unsigned int currentOccurenceId= subOccurenceList.at(i)->id();
// 			if (m_Collection.contains(currentOccurenceId))
// 			{
// 				m_Collection.select(currentOccurenceId);
// 			}
		}
	}
}

void WorldHandle::unselect(unsigned int occurenceId, bool propagate)
{
	assert(m_OccurenceHash.find(occurenceId)!=m_OccurenceHash.end());
	m_SelectionSet.remove(occurenceId);
//	m_Collection.unselect(occurenceId);

	const Occurence* pSelectedOccurence= m_OccurenceHash[occurenceId];
	if (propagate && pSelectedOccurence->hasChild())
	{
		std::vector<Occurence*> subOccurenceList= pSelectedOccurence->subOccurenceList();
		const int subOccurenceCount= subOccurenceList.size();
		for (int i= 0; i < subOccurenceCount; ++i)
		{
			const unsigned int currentOccurenceId= subOccurenceList.at(i)->id();
	//		m_Collection.unselect(currentOccurenceId);
		}
	}
}

void WorldHandle::selectAllWith3DViewInstance(bool allShowState)
{
//	m_Collection.selectAll(allShowState);
// 	std::vector<unsigned int> selectedId= m_Collection.selection()->keys();
// 	m_SelectionSet.clear();
// 	const int selectionCount= selectedId.count();
// 	for (int i= 0; i < selectionCount; ++i)
// 	{
// 		m_SelectionSet.insert(selectedId.at(i));
// 	}
}

void WorldHandle::unselectAll()
{
	m_SelectionSet.clear();
//	m_Collection.unselectAll();
}

void WorldHandle::showHideSelected3DViewInstance()
{
	//std::vector<3DViewInstance*> selected3dviewInstance= m_Collection.selection()->values();
	//const int instanceCount= selected3dviewInstance.count();
	//for(int i= 0; i < instanceCount; ++i)
	//{
	//	3DViewInstance* pCurrentInstance= selected3dviewInstance.at(i);
	//	pCurrentInstance->setVisibility(!pCurrentInstance->isVisible());
	//}
}

void WorldHandle::setSelected3DViewInstanceVisibility(bool isVisible)
{
	//std::vector<3DViewInstance*> selected3dviewInstance= m_Collection.selection()->values();
	//const int instanceCount= selected3dviewInstance.count();
	//for(int i= 0; i < instanceCount; ++i)
	//{
	//	3DViewInstance* pCurrentInstance= selected3dviewInstance.at(i);
	//	pCurrentInstance->setVisibility(isVisible);
	//}
}

const Occurence* WorldHandle::getOccurence( unsigned int id ) const
{
	assert(m_OccurenceHash.find(id) != m_OccurenceHash.end());

	const Occurence* pOccurence/*= m_OccurenceHash[id]*/;

	return pOccurence;
}

std::vector<Occurence*> WorldHandle::occurences() const
{
	std::vector<Occurence*> vecOccurences;
	stdext::hash_map<unsigned int,Occurence*>::const_iterator itor;
	for (itor = m_OccurenceHash.begin();itor!=m_OccurenceHash.end();itor++)
	{
		vecOccurences.push_back(itor->second);
	}
	
	return vecOccurences;
//	return m_OccurenceHash.values();
}

int WorldHandle::numberOfOccurence() const
{
	return m_OccurenceHash.size();
}

bool WorldHandle::containsOccurence( unsigned int id ) const
{
	return m_OccurenceHash.find(id) != m_OccurenceHash.end();
}

SelectionSet* WorldHandle::selectionSetHandle()
{
	return &m_SelectionSet;
}

