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

#include "worldhandle.h"
#include "structreference.h"
#include <set>

WorldHandle::WorldHandle()
//: m_Collection()
: m_NumberOfWorld(1)
, m_OccurenceHash()
, m_UpVector(0,0,1)
, m_SelectionSet(this)
{

}

WorldHandle::~WorldHandle()
{

}

// Return the list of instance
std::vector<StructInstance*> WorldHandle::instances() const
{
	std::set<StructInstance*> instancesSet;
	stdext::hash_map<unsigned int, StructOccurence*>::const_iterator iOccurence= m_OccurenceHash.begin();
	while (iOccurence != m_OccurenceHash.end())
	{
		instancesSet.insert(iOccurence.value()->structInstance());
		++iOccurence;
	}
	return instancesSet.toList();
}

// Return the list of Reference
std::vector<StructReference*> WorldHandle::references() const
{
	std::set<StructReference*> referencesSet;
	stdext::hash_map<unsigned int, StructOccurence*>::const_iterator iOccurence= m_OccurenceHash.begin();
	while (iOccurence != m_OccurenceHash.end())
	{
		referencesSet.insert(iOccurence.value()->structReference());
		++iOccurence;
	}
	return referencesSet.toList();
}

// Return the number of body
int WorldHandle::numberOfBody() const
{
	std::vector<StructReference*> referenceList(references());
	const int size= referenceList.size();
	int numberOfBody= 0;
	for (int i= 0; i < size; ++i)
	{
		numberOfBody+= referenceList[i]->numberOfBody();
	}
	return numberOfBody;
}

int WorldHandle::representationCount() const
{
	std::vector<StructReference*> referenceList(references());
	const int size= referenceList.size();
	int count= 0;
	for (int i= 0; i < size; ++i)
	{
		if (referenceList[i]->hasRepresentation()) ++count;
	}
	return count;

}

// An Occurence has been added
void WorldHandle::addOccurence(StructOccurence* pOccurence, bool isSelected, GLuint shaderId)
{
	//assert(!m_OccurenceHash.contains(pOccurence->id()));
	//m_OccurenceHash.insert(pOccurence->id(), pOccurence);
	//StructReference* pRef= pOccurence->structReference();
	//assert(NULL != pRef);

	//// Add instance representation in the collection
	//if (pOccurence->useAutomatic3DViewInstanceCreation() && pRef->representationIsLoaded())
	//{
	//	//cout<< "WorldHandle::addOccurence with rep";
	//	Rep3D* pRep3D= dynamic_cast<Rep3D*>(pRef->representationHandle());
	//	3DViewInstance representation(*pRep3D);
	//	// Force instance representation id
	//	representation.setId(pOccurence->id());
	//	// Force instance representation name
	//	representation.setName(pOccurence->name());
	//	if (0 != shaderId) m_Collection.bindShader(shaderId);
	//	m_Collection.add(representation, shaderId);
	//	if (isSelected)
	//	{
	//		//cout<< pOccurence->name() << "selected";
	//		m_Collection.select(pOccurence->id());
	//	}
	//}
}

// An Occurence has been removed
void WorldHandle::removeOccurence(StructOccurence* pOccurence)
{
	//assert(m_OccurenceHash.contains(pOccurence->id()));
	//// Remove the occurence from the selection set
	//m_SelectionSet.remove(pOccurence);
	//// Remove the occurence from the main occurence hash table
	//m_OccurenceHash.remove(pOccurence->id());
	//// Remove instance representation from the collection
	//m_Collection.remove(pOccurence->id());
}

void WorldHandle::select(unsigned int occurenceId, bool propagate)
{
	//assert(m_OccurenceHash.contains(occurenceId));
	//m_SelectionSet.insert(occurenceId);
	//m_Collection.select(occurenceId);

	//const StructOccurence* pSelectedOccurence= m_OccurenceHash.value(occurenceId);
	//if (propagate && pSelectedOccurence->hasChild())
	//{
	//	std::vector<StructOccurence*> subOccurenceList= pSelectedOccurence->subOccurenceList();
	//	const int subOccurenceCount= subOccurenceList.size();
	//	for (int i= 0; i < subOccurenceCount; ++i)
	//	{
	//		const unsigned int currentOccurenceId= subOccurenceList[i]->id();
	//		if (m_Collection.contains(currentOccurenceId))
	//		{
	//			m_Collection.select(currentOccurenceId);
	//		}
	//	}
	//}
}

void WorldHandle::unselect(unsigned int occurenceId, bool propagate)
{
	//assert(m_OccurenceHash.contains(occurenceId));
	//m_SelectionSet.remove(occurenceId);
	//m_Collection.unselect(occurenceId);

	//const StructOccurence* pSelectedOccurence= m_OccurenceHash.value(occurenceId);
	//if (propagate && pSelectedOccurence->hasChild())
	//{
	//	std::vector<StructOccurence*> subOccurenceList= pSelectedOccurence->subOccurenceList();
	//	const int subOccurenceCount= subOccurenceList.size();
	//	for (int i= 0; i < subOccurenceCount; ++i)
	//	{
	//		const unsigned int currentOccurenceId= subOccurenceList[i]->id();
	//		m_Collection.unselect(currentOccurenceId);
	//	}
	//}
}

void WorldHandle::selectAllWith3DViewInstance(bool allShowState)
{
	//m_Collection.selectAll(allShowState);
	//std::vector<unsigned int> selectedId= m_Collection.selection()->keys();
	//m_SelectionSet.clear();
	//const int selectionCount= selectedId.count();
	//for (int i= 0; i < selectionCount; ++i)
	//{
	//	m_SelectionSet.insert(selectedId[i]);
	//}
}

void WorldHandle::unselectAll()
{
	//m_SelectionSet.clear();
	//m_Collection.unselectAll();
}

void WorldHandle::showHideSelected3DViewInstance()
{
	//std::vector<3DViewInstance*> selected3dviewInstance= m_Collection.selection()->values();
	//const int instanceCount= selected3dviewInstance.count();
	//for(int i= 0; i < instanceCount; ++i)
	//{
	//	//3DViewInstance* pCurrentInstance= selected3dviewInstance[i];
	//	//pCurrentInstance->setVisibility(!pCurrentInstance->isVisible());
	//}
}

void WorldHandle::setSelected3DViewInstanceVisibility(bool isVisible)
{
	//std::vector<3DViewInstance*> selected3dviewInstance= m_Collection.selection()->values();
	//const int instanceCount= selected3dviewInstance.count();
	//for(int i= 0; i < instanceCount; ++i)
	//{
	//	3DViewInstance* pCurrentInstance= selected3dviewInstance[i];
	//	pCurrentInstance->setVisibility(isVisible);
	//}
}

