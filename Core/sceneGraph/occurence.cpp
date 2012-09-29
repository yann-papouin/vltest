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

//! \file structoccurence.cpp implementation of the StructOccurence class.

#include "structoccurence.h"
//#include "3dviewcollection.h"
#include "structreference.h"
#include "worldhandle.h"
// #include "../errorlog.h"

// Default constructor
StructOccurence::StructOccurence()
: m_Uid(glc::GenID())
, m_pWorldHandle(NULL)
, m_pNumberOfOccurence(new int(1))
, m_pStructInstance(new StructInstance())
, m_pParent(NULL)
, m_Childs()
, m_AbsoluteMatrix()
, m_OccurenceNumber(0)
, m_IsVisible(true)
//, m_pRenderProperties(NULL)
, m_AutomaticCreationOf3DViewInstance(true)
{
	// Update instance
	m_pStructInstance->structOccurenceCreated(this);
}


// Default constructor
StructOccurence::StructOccurence(StructInstance* pStructInstance, WorldHandle* pWorldHandle, GLuint shaderId)
: m_Uid(glc::GenID())
, m_pWorldHandle(pWorldHandle)
, m_pNumberOfOccurence(NULL)
, m_pStructInstance(pStructInstance)
, m_pParent(NULL)
, m_Childs()
, m_AbsoluteMatrix()
, m_OccurenceNumber(0)
, m_IsVisible(true)
//, m_pRenderProperties(NULL)
, m_AutomaticCreationOf3DViewInstance(true)
{
	// Update the number of occurences
	if (pStructInstance->hasStructOccurence())
	{
		StructOccurence* pFirstOccurence= pStructInstance->firstOccurenceHandle();
		m_pNumberOfOccurence= pFirstOccurence->m_pNumberOfOccurence;
		++(*m_pNumberOfOccurence);
		std::vector<StructOccurence*> childs= pFirstOccurence->m_Childs;
		const int size= childs.size();
		for (int i= 0; i < size; ++i)
		{
			StructOccurence* pChild= childs.at(i)->clone(m_pWorldHandle, true);
			addChild(pChild);
		}
	}
	else
	{
		m_pNumberOfOccurence= new int(1);
	}

	setName(m_pStructInstance->name());

	// Inform the world Handle
	if (NULL != m_pWorldHandle)
	{
		m_pWorldHandle->addOccurence(this, shaderId);
	}

	// Update Absolute matrix
	updateAbsoluteMatrix();

	// Update instance
	m_pStructInstance->structOccurenceCreated(this);
}
// Construct Occurence with the specified Rep3D
StructOccurence::StructOccurence(Rep3D* pRep)
: m_Uid(glc::GenID())
, m_pWorldHandle(NULL)
, m_pNumberOfOccurence(new int(1))
, m_pStructInstance(NULL)
, m_pParent(NULL)
, m_Childs()
, m_AbsoluteMatrix()
, m_OccurenceNumber(0)
, m_IsVisible(true)
//, m_pRenderProperties(NULL)
, m_AutomaticCreationOf3DViewInstance(true)
{
	m_pStructInstance= new StructInstance(pRep);
	setName(m_pStructInstance->name());

	// Update instance
	m_pStructInstance->structOccurenceCreated(this);
}

// Copy constructor
StructOccurence::StructOccurence(WorldHandle* pWorldHandle, const StructOccurence& structOccurence, bool shareInstance)
: m_Uid(glc::GenID())
, m_pWorldHandle(pWorldHandle)
, m_pNumberOfOccurence(NULL)
, m_pStructInstance(NULL)
, m_pParent(NULL)
, m_Childs()
, m_AbsoluteMatrix(structOccurence.m_AbsoluteMatrix)
, m_OccurenceNumber(0)
, m_IsVisible(structOccurence.m_IsVisible)
//, m_pRenderProperties(NULL)
, m_AutomaticCreationOf3DViewInstance(structOccurence.m_AutomaticCreationOf3DViewInstance)
{
	if (shareInstance)
	{
		m_pStructInstance= structOccurence.m_pStructInstance;
		m_pNumberOfOccurence= structOccurence.m_pNumberOfOccurence;
		++(*m_pNumberOfOccurence);
	}
	else
	{
		m_pNumberOfOccurence= new int(1);
		m_pStructInstance= new StructInstance(structOccurence.m_pStructInstance);
	}


	// Test if structOccurence has representation and has a shader
	GLuint shaderId= 0;
	bool instanceIsSelected= false;
	//if ((NULL != m_pWorldHandle) && (NULL != structOccurence.m_pWorldHandle) && structOccurence.m_pWorldHandle->collection()->contains(structOccurence.id()))
	//{
		//3DViewInstance* p3DViewInstance= structOccurence.m_pWorldHandle->collection()->instanceHandle(structOccurence.id());

		//if(structOccurence.m_pWorldHandle->collection()->isInAShadingGroup(structOccurence.id()))
		//{
		//	shaderId= structOccurence.m_pWorldHandle->collection()->shadingGroup(structOccurence.id());
		//}

		//instanceIsSelected= p3DViewInstance->isSelected();
		//// Test the rendering properties
		//if (! p3DViewInstance->renderPropertiesHandle()->isDefault())
		//{
		//	m_pRenderProperties= new RenderProperties(*(p3DViewInstance->renderPropertiesHandle()));
		//}
	//}
	//else if (NULL != structOccurence.m_pRenderProperties)
	//{
	//	//m_pRenderProperties= new RenderProperties(*(structOccurence.m_pRenderProperties));
	//}

	// Inform the world Handle
	if (NULL != m_pWorldHandle)
	{
		m_pWorldHandle->addOccurence(this, instanceIsSelected, shaderId);
		//if (NULL != m_pRenderProperties)
		//{
		//	m_pWorldHandle->collection()->instanceHandle(id())->setRenderProperties(*m_pRenderProperties);
		//	delete m_pRenderProperties;
		//	m_pRenderProperties= NULL;
		//}
	}

	// Update Absolute matrix
	updateAbsoluteMatrix();


	// Create childs
	const int size= structOccurence.childCount();
	for (int i= 0; i < size; ++i)
	{
		StructOccurence* pChild= structOccurence.child(i)->clone(m_pWorldHandle, true);
		addChild(pChild);
	}
	updateChildrenAbsoluteMatrix();
	// Update instance
	m_pStructInstance->structOccurenceCreated(this);
}

// Destructor
StructOccurence::~StructOccurence()
{
	//cout<< "Delete " << id();
	assert(m_pNumberOfOccurence != NULL);
	// Remove from the WorldHandle
	if (NULL != m_pWorldHandle)
	{
		m_pWorldHandle->removeOccurence(this);
	}

	// Remove Childs
	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		StructOccurence* pChild= m_Childs.front();
		removeChild(pChild);
		delete pChild;
	}
	// Update number of occurence and instance
	if ((--(*m_pNumberOfOccurence)) == 0)
	{
		delete m_pStructInstance;
		delete m_pNumberOfOccurence;
	}
	else
	{
		m_pStructInstance->structOccurenceDeleted(this);
		if (!m_pStructInstance->hasStructOccurence())
		{

			//std::stringList errorList;
			//errorList << "StructOccurence count error";
			//errorList << ("ref name = " + m_pStructInstance->structReference()->name());
			//ErrorLog::addError(errorList);

			delete m_pStructInstance;
			//delete m_pNumberOfOccurence;
		}
	}

	//delete m_pRenderProperties;
}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

bool StructOccurence::hasRepresentation() const
{
	if ((NULL != m_pStructInstance) && (m_pStructInstance->hasStructOccurence()))
	{
		return this->structReference()->hasRepresentation();
	}
	else return false;
}

bool StructOccurence::has3DViewInstance() const
{
	if ( NULL != m_pWorldHandle)
	{
		return m_pWorldHandle->collection()->contains(m_Uid);
	}
	else return false;
}

bool StructOccurence::canBeAddedToChildren(StructOccurence* pOccurence) const
{
	bool canBeAdded= false;
	if ((NULL != m_pStructInstance) && (m_pStructInstance->hasStructOccurence()) && (NULL != pOccurence->m_pStructInstance) && (NULL != pOccurence->structReference()))
	{
		if (this->structReference() != pOccurence->structReference())
		{
			std::set<StructReference*> thisRefSet= StructOccurence::parentsReferences(this);
			thisRefSet.insert(this->structReference());
			std::set<StructReference*> childRefSet= pOccurence->childrenReferences();

			canBeAdded= thisRefSet == (thisRefSet - childRefSet);
		}
	}
	else
	{
		canBeAdded= true;
	}
	return canBeAdded;
}

std::vector<StructOccurence*> StructOccurence::subOccurenceList() const
{
	std::vector<StructOccurence*> subOccurence;
	const int childCount= m_Childs.size();
	for (int i= 0; i < childCount; ++i)
	{
		StructOccurence* pCurrentChild= m_Childs.at(i);
		if (pCurrentChild->hasChild())
		{
			subOccurence.append(pCurrentChild->subOccurenceList());
		}
		else
		{
			subOccurence.append(pCurrentChild);
		}
	}

	return subOccurence;
}

unsigned int StructOccurence::numberOfFaces() const
{
	unsigned int result= 0;
	if (hasRepresentation())
	{
		result= structInstance()->structReference()->numberOfFaces();
	}

	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		result+= m_Childs.at(i)->numberOfFaces();
	}

	return result;
}

unsigned int StructOccurence::numberOfVertex() const
{
	unsigned int result= 0;
	if (hasRepresentation())
	{
		result= structInstance()->structReference()->numberOfVertex();
	}
	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		result+= m_Childs.at(i)->numberOfVertex();
	}

	return result;
}

// Get number of materials
//unsigned int StructOccurence::numberOfMaterials() const
//{
//	unsigned int result= 0;
//	std::set<Material*> materialSet;
//	if (hasRepresentation())
//	{
//		result= structInstance()->structReference()->numberOfMaterials();
//	}
//
//	const int size= m_Childs.size();
//	for (int i= 0; i < size; ++i)
//	{
//		materialSet.unite(m_Childs.at(i)->materialSet());
//	}
//	result= static_cast<unsigned int>(materialSet.size());
//
//	return result;
//}

// Get materials List
//std::set<Material*> StructOccurence::materialSet() const
//{
//	std::set<Material*> materialSet;
//	if (hasRepresentation())
//	{
//		materialSet= structInstance()->structReference()->materialSet();
//	}
//
//	const int size= m_Childs.size();
//	for (int i= 0; i < size; ++i)
//	{
//		materialSet.unite(m_Childs.at(i)->materialSet());
//	}
//
//	return materialSet;
//}

// Clone the occurence
StructOccurence* StructOccurence::clone(WorldHandle* pWorldHandle, bool shareInstance) const
{
	return new StructOccurence(pWorldHandle, *this, shareInstance);
}

// Return true if the occurence is visible
bool StructOccurence::isVisible() const
{
	bool isHidden= true;

	//if ((NULL != m_pWorldHandle) && m_pWorldHandle->collection()->contains(m_Uid))
	//{
	//	isHidden= !m_pWorldHandle->collection()->instanceHandle(m_Uid)->isVisible();
	//}
	//else if (childCount() > 0)
	//{
	//	const int size= childCount();
	//	int i= 0;
	//	while ((i < size) && isHidden)
	//	{
	//		isHidden= isHidden && !child(i)->isVisible();
	//		++i;
	//	}
	//}
	//else
	//{
	//	isHidden= !m_IsVisible;
	//}
	return !isHidden;
}

// Return the occurence Bounding Box
AABB StructOccurence::boundingBox() const
{
	AABB boundingBox;

	if (NULL != m_pWorldHandle)
	{
		if (has3DViewInstance())
		{
			//assert(m_pWorldHandle->collection()->contains(id()));
			//boundingBox= m_pWorldHandle->collection()->instanceHandle(id())->boundingBox();
		}
		else
		{
			//if (hasChild())
			//{
			//	std::vector<StructOccurence*> childrenList= children();
			//	const int size= childrenList.size();

			//	for (int i= 0; i < size; ++i)
			//	{
			//		boundingBox.combine(childrenList.at(i)->boundingBox());
			//	}
			//}
		}
	}

	return boundingBox;
}

unsigned int StructOccurence::nodeCount() const
{
	unsigned int result= 1;
	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		result+= m_Childs.at(i)->nodeCount();
	}
	return result;
}

std::set<StructReference*> StructOccurence::childrenReferences() const
{
	std::set<StructReference*> refChildrenSet;
	const int childCount= m_Childs.size();
	for (int i= 0; i < childCount; ++i)
	{
		StructOccurence* pCurrentChild= m_Childs.at(i);
		if ((NULL != pCurrentChild->structInstance()) && (NULL != pCurrentChild->structReference()))
		{
			refChildrenSet.insert(pCurrentChild->structReference());
		}
	}

	return refChildrenSet;
}

std::set<StructReference*> StructOccurence::parentsReferences(const StructOccurence* pOccurence)
{
	std::set<StructReference*> parentSet;
	StructOccurence* pParent= pOccurence->parent();
	if (NULL != pParent)
	{
		if ((NULL != pParent->structInstance()) && (NULL != pParent->structReference()))
		{
			parentSet.insert(pParent->structReference());
		//	parentSet.unite(StructOccurence::parentsReferences(pParent));
		}
	}

	return parentSet;
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Update the absolute matrix
StructOccurence* StructOccurence::updateAbsoluteMatrix()
{
	if (NULL != m_pParent)
	{
		m_AbsoluteMatrix= m_pParent->absoluteMatrix() * m_pStructInstance->relativeMatrix();
	}
	else
	{
		m_AbsoluteMatrix= m_pStructInstance->relativeMatrix();
	}
	// If the occurence have a representation, update it.

	//if ((NULL != m_pWorldHandle) && m_pWorldHandle->collection()->contains(m_Uid))
	//{
	//	m_pWorldHandle->collection()->instanceHandle(m_Uid)->setMatrix(m_AbsoluteMatrix);
	//}
	return this;
}

// Update children obsolute Matrix
StructOccurence* StructOccurence::updateChildrenAbsoluteMatrix()
{
	updateAbsoluteMatrix();
	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		m_Childs[i]->updateChildrenAbsoluteMatrix();
	}
	return this;
}

// Add Child
void StructOccurence::addChild(StructOccurence* pChild)
{
	assert(pChild->isOrphan());
	assert((NULL == pChild->m_pWorldHandle) || (m_pWorldHandle == pChild->m_pWorldHandle));

	//cout<< "Add Child " << pChild->name() << "id=" << pChild->id() << " to " << name() << " id=" << id();
	// Add the child to the list of child
	// Get occurence reference
	m_Childs.append(pChild);
	pChild->m_pParent= this;
	if (NULL == pChild->m_pWorldHandle)
	{
		pChild->setWorldHandle(m_pWorldHandle);
	}
	pChild->updateChildrenAbsoluteMatrix();
}

// Add Child instance and returns the newly created occurence
StructOccurence* StructOccurence::addChild(StructInstance* pInstance)
{
	StructOccurence* pOccurence;
	pOccurence= new StructOccurence(pInstance, m_pWorldHandle);

	addChild(pOccurence);

	return pOccurence;
}

// make the occurence orphan
void StructOccurence::makeOrphan()
{
	//cout<< "StructOccurence::makeOrphan() " << id();
	//cout<< name() << " " << id();
	assert(!isOrphan());
	m_pParent->removeChild(this);
	//cout<< "StructOccurence::makeOrphan() DONE!";
}

// Remove the specified child
bool StructOccurence::removeChild(StructOccurence* pChild)
{
	assert(pChild->m_pParent == this);
	assert(m_Childs.contains(pChild));
	pChild->m_pParent= NULL;
	pChild->detach();

	return m_Childs.removeOne(pChild);
}

// Detach the occurence from the World
void StructOccurence::detach()
{
	if (NULL != m_pWorldHandle)
	{
		// retrieve renderProperties if needed
		//if (m_pWorldHandle->collection()->contains(m_Uid))
		//{
		//	3DViewInstance* pInstance= m_pWorldHandle->collection()->instanceHandle(m_Uid);
		//	if (!pInstance->renderPropertiesHandle()->isDefault())
		//	{
		//		assert(NULL == m_pRenderProperties);
		//		m_pRenderProperties= new RenderProperties(*(pInstance->renderPropertiesHandle()));
		//	}
		//}
		//m_pWorldHandle->removeOccurence(this);
		//m_pWorldHandle= NULL;
		//if (!m_Childs.isEmpty())
		//{
		//	const int size= m_Childs.size();
		//	for (int i= 0; i < size; ++i)
		//	{
		//		m_Childs[i]->detach();
		//	}
		//}
	}
}

// Reverse Normals of this Occurence and childs
void StructOccurence::reverseNormals()
{
	if (has3DViewInstance())
	{
		//m_pWorldHandle->collection()->instanceHandle(id())->reverseGeometriesNormals();
	}
}

// Check the presence of representation
bool StructOccurence::create3DViewInstance()
{
	bool creationSuccess= false;
	if ((NULL != m_pWorldHandle) && hasRepresentation())
	{
		//Rep3D* pRep3D= dynamic_cast<Rep3D*>(structReference()->representationHandle());
		//if (NULL != pRep3D)
		//{
		//	3DViewInstance instance(*pRep3D);
		//	instance.setName(name());
		//	// Force instance representation id
		//	instance.setId(id());
		//	creationSuccess= m_pWorldHandle->collection()->add(instance);
		//	m_pWorldHandle->collection()->setVisibility(m_Uid, m_IsVisible);
		//}
	}
	return creationSuccess;
}

bool StructOccurence::remove3DViewInstance()
{
	if (NULL != m_pWorldHandle)
	{
		return m_pWorldHandle->collection()->remove(m_Uid);
	}
	else return false;
}

// Set the occurence world Handle
void StructOccurence::setWorldHandle(WorldHandle* pWorldHandle)
{
	// Check if world handles are equal
	if (m_pWorldHandle == pWorldHandle) return;

	if (NULL != m_pWorldHandle)
	{
		m_pWorldHandle->removeOccurence(this);
	}

	m_pWorldHandle= pWorldHandle;

	if (NULL != m_pWorldHandle)
	{
		m_pWorldHandle->addOccurence(this);
//		m_pWorldHandle->collection()->setVisibility(m_Uid, m_IsVisible);

		const int size= m_Childs.size();
		for (int i= 0; i < size; ++i)
		{
			m_Childs[i]->setWorldHandle(m_pWorldHandle);
		}
	}
}

// Load the representation and return true if success
bool StructOccurence::loadRepresentation()
{
	assert(!this->has3DViewInstance());

	bool loadSuccess= false;
	if (hasRepresentation())
	{
		StructReference* pReference= this->structReference();
		if (pReference->representationIsLoaded())
		{
			loadSuccess= create3DViewInstance();
		}
		else
		{
			loadSuccess=  m_pStructInstance->structReference()->loadRepresentation();
			if (loadSuccess && !m_AutomaticCreationOf3DViewInstance)
			{
				loadSuccess= create3DViewInstance();
			}
		}
	}

	return loadSuccess;
}

// UnLoad the representation and return true if success
bool StructOccurence::unloadRepresentation()
{
	bool unloadResult= false;
	if (hasRepresentation())
	{
		StructReference* pRef= this->structReference();
		if (pRef->representationIsLoaded())
		{
			if (this->has3DViewInstance())
			{
				//unloadResult= m_pWorldHandle->collection()->remove(m_Uid);
				//std::set<StructOccurence*> occurenceSet= pRef->setOfStructOccurence();
				//std::set<StructOccurence*>::const_iterator iOcc= occurenceSet.begin();
				//bool unloadReferenceRep= true;
				//while (occurenceSet.end() != iOcc)
				//{
				//	unloadReferenceRep= unloadReferenceRep && !(*iOcc)->has3DViewInstance();
				//	++iOcc;
				//}
				//if (unloadReferenceRep)
				//{
				//	pRef->unloadRepresentation();
				//}
			}
		}
	}
	return unloadResult;
}

unsigned int StructOccurence::updateOccurenceNumber(unsigned int n)
{
	m_OccurenceNumber= n++;
	const int childCount= m_Childs.size();
	for (int i= 0; i < childCount; ++i)
	{
		n= m_Childs[i]->updateOccurenceNumber(n);
	}
	return n;
}

void StructOccurence::setVisibility(bool visibility)
{
	m_IsVisible= visibility;
	if (has3DViewInstance())
	{
//		m_pWorldHandle->collection()->setVisibility(m_Uid, m_IsVisible);
	}
	const int childCount= m_Childs.size();
	for (int i= 0; i < childCount; ++i)
	{
		m_Childs[i]->setVisibility(m_IsVisible);
	}
}

//void StructOccurence::setRenderProperties(const RenderProperties& renderProperties)
//{
	//delete m_pRenderProperties;
	//if (has3DViewInstance())
	//{
	//	m_pWorldHandle->collection()->instanceHandle(m_Uid)->setRenderProperties(renderProperties);
	//}
	//else if (hasChild())
	//{
	//	const int childCount= m_Childs.size();
	//	for (int i= 0; i < childCount; ++i)
	//	{
	//		m_Childs[i]->setRenderProperties(renderProperties);
	//	}
	//}
	//else
	//{
	//	m_pRenderProperties= new RenderProperties(renderProperties);
	//}
//}

void StructOccurence::removeEmptyChildren()
{
	std::vector<StructOccurence*>::iterator iChild= m_Childs.begin();
	while (m_Childs.end() != iChild)
	{
		if (!((*iChild)->hasChild()) && !((*iChild)->hasRepresentation()))
		{
			delete *iChild;
			iChild= m_Childs.erase(iChild);
		}
		else
		{
			(*iChild)->removeEmptyChildren();
			++iChild;
		}
	}
}

void StructOccurence::setReference(StructReference* pRef)
{
	assert(m_pStructInstance->structReference() == NULL);
	assert((*m_pNumberOfOccurence) == 1);

	if (pRef->hasStructInstance())
	{
		StructInstance* pExistingInstance= pRef->firstInstanceHandle();
		if (pExistingInstance->hasStructOccurence())
		{
			StructOccurence* pFirstOccurence= pExistingInstance->firstOccurenceHandle();
			std::vector<StructOccurence*> childs= pFirstOccurence->m_Childs;
			const int size= childs.size();
			for (int i= 0; i < size; ++i)
			{
				StructOccurence* pChild= childs.at(i)->clone(m_pWorldHandle, true);
				addChild(pChild);
			}

			std::vector<StructInstance*> instances= pRef->listOfStructInstances();
			const int instanceCount= instances.size();
			int i= 0;
			bool continu= true;
			while (continu && (i < instanceCount))
			{
				if (m_pStructInstance == instances.at(i))
				{
					continu= false;
					delete m_pNumberOfOccurence;
					m_pNumberOfOccurence= instances.at(i)->firstOccurenceHandle()->m_pNumberOfOccurence;
					++(*m_pNumberOfOccurence);
				}
				++i;
			}
		}
	}

	m_pStructInstance->setReference(pRef);
}
