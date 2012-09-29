//! \file Occurence.cpp implementation of the Occurence class.

#include "struct/Occurence.h"
//#include "3dviewcollection.h"
#include "struct/Reference.h"
#include "struct/Instance.h"
#include "struct/represent3d.h"
#include "struct/WorldHandle.h"
//#include "VLBaseView.h"
//#include "../errorlog.h"

unsigned int GenUid(void)
{
	static unsigned int Id= 0;
	Id++;
	return Id;
}

// Default constructor
Occurence::Occurence()
: m_Uid(GenUid())
, m_pWorldHandle(NULL)
, m_pNumberOfOccurence(new int(1))
, m_pInstance(new Instance())
, m_pParent(NULL)
, m_Childs()
, m_AbsoluteMatrix()
, m_OccurenceNumber(0)
, m_IsVisible(true)
//, m_pRenderProperties(NULL)
, m_AutomaticCreationOf3DViewInstance(true)
{
	// Update instance
	m_pInstance->OccurenceCreated(this);
}

// Default constructor
Occurence::Occurence(Instance* pInstance, WorldHandle* pWorldHandle, unsigned int shaderId)
: m_Uid(GenUid())
, m_pWorldHandle(pWorldHandle)
, m_pNumberOfOccurence(NULL)
, m_pInstance(pInstance)
, m_pParent(NULL)
, m_Childs()
, m_AbsoluteMatrix()
, m_OccurenceNumber(0)
, m_IsVisible(true)
//, m_pRenderProperties(NULL)
, m_AutomaticCreationOf3DViewInstance(true)
{
	// Update the number of occurences
	if (pInstance->hasOccurence())
	{
		Occurence* pFirstOccurence= pInstance->firstOccurenceHandle();
		m_pNumberOfOccurence= pFirstOccurence->m_pNumberOfOccurence;
		++(*m_pNumberOfOccurence);
		std::vector<Occurence*> childs= pFirstOccurence->m_Childs;
		const int size= childs.size();
		for (int i= 0; i < size; ++i)
		{
			Occurence* pChild= childs.at(i)->clone(m_pWorldHandle, true);
			addChild(pChild);
		}
	}
	else
	{
		m_pNumberOfOccurence= new int(1);
	}

	setName(m_pInstance->name());

	// Inform the world Handle
	if (NULL != pWorldHandle)
	{
//		m_pWorldHandle->addOccurence(this, shaderId);
	}

	// Update Absolute matrix
	updateAbsoluteMatrix();

	// Update instance
	m_pInstance->OccurenceCreated(this);
}
// Construct Occurence with the specified Represent3D
Occurence::Occurence(Represent3D* pRep)
: m_Uid(GenUid())
, m_pWorldHandle(NULL)
, m_pNumberOfOccurence(new int(1))
, m_pInstance(NULL)
, m_pParent(NULL)
, m_Childs()
, m_AbsoluteMatrix()
, m_OccurenceNumber(0)
, m_IsVisible(true)
//, m_pRenderProperties(NULL)
, m_AutomaticCreationOf3DViewInstance(true)
{
	m_pInstance= new Instance(pRep);
	setName(m_pInstance->name());

	// Update instance
	m_pInstance->OccurenceCreated(this);
}

// Copy constructor
Occurence::Occurence(WorldHandle* pWorldHandle, const Occurence& occurence, bool shareInstance)
: m_Uid(GenUid())
, m_pWorldHandle(pWorldHandle)
, m_pNumberOfOccurence(NULL)
, m_pInstance(NULL)
, m_pParent(NULL)
, m_Childs()
, m_AbsoluteMatrix(occurence.m_AbsoluteMatrix)
, m_OccurenceNumber(0)
, m_IsVisible(occurence.m_IsVisible)
//, m_pRenderProperties(NULL)
, m_AutomaticCreationOf3DViewInstance(occurence.m_AutomaticCreationOf3DViewInstance)
{
	if (shareInstance)
	{
		m_pInstance= occurence.m_pInstance;
		m_pNumberOfOccurence= occurence.m_pNumberOfOccurence;
		++(*m_pNumberOfOccurence);
	}
	else
	{
		m_pNumberOfOccurence= new int(1);
		m_pInstance= new Instance(occurence.m_pInstance);
	}


	// Test if Occurence has representation and has a shader
	unsigned int shaderId= 0;
	bool instanceIsSelected= false;
	//if ((NULL != m_pWorldHandle) && (NULL != Occurence.m_pWorldHandle) && Occurence.m_pWorldHandle->collection()->contains(Occurence.id()))
	//{
	//	3DViewInstance* p3DViewInstance= Occurence.m_pWorldHandle->collection()->instanceHandle(Occurence.id());

	//	if(Occurence.m_pWorldHandle->collection()->isInAShadingGroup(Occurence.id()))
	//	{
	//		shaderId= Occurence.m_pWorldHandle->collection()->shadingGroup(Occurence.id());
	//	}

	//	instanceIsSelected= p3DViewInstance->isSelected();
	//	// Test the rendering properties
	//	if (! p3DViewInstance->renderPropertiesHandle()->isDefault())
	//	{
	//		m_pRenderProperties= new RenderProperties(*(p3DViewInstance->renderPropertiesHandle()));
	//	}
	//}
	//else if (NULL != Occurence.m_pRenderProperties)
	//{
	//	m_pRenderProperties= new RenderProperties(*(Occurence.m_pRenderProperties));
	//}

	// Inform the world Handle
	if (NULL != m_pWorldHandle)
	{
//		m_pWorldHandle->addOccurence(this, instanceIsSelected, shaderId);
//		if (NULL != m_pRenderProperties)
//		{
//// 			m_pWorldHandle->collection()->instanceHandle(id())->setRenderProperties(*m_pRenderProperties);
//			delete m_pRenderProperties;
//			m_pRenderProperties= NULL;
//		}
	}

	// Update Absolute matrix
	updateAbsoluteMatrix();


	// Create childs
	const int size= occurence.childCount();
	for (int i= 0; i < size; ++i)
	{
		Occurence* pChild= occurence.child(i)->clone(m_pWorldHandle, true);
		addChild(pChild);
	}
	updateChildrenAbsoluteMatrix();
	// Update instance
	m_pInstance->OccurenceCreated(this);
}

// Destructor
Occurence::~Occurence()
{
	//qDebug() << "Delete " << id();
	assert(m_pNumberOfOccurence != NULL);
	// Remove from the VLBaseView
	if (NULL != m_pWorldHandle)
	{
//		m_pWorldHandle->removeOccurence(this);
	}

	// Remove Childs
	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		Occurence* pChild= m_Childs.front();
		removeChild(pChild);
		delete pChild;
	}
	// Update number of occurence and instance
	if ((--(*m_pNumberOfOccurence)) == 0)
	{
		delete m_pInstance;
		delete m_pNumberOfOccurence;
	}
	else
	{
		m_pInstance->OccurenceDeleted(this);
		if (!m_pInstance->hasOccurence())
		{

//			std::stringList errorList;
//			errorList << "Occurence count error";
//			errorList << ("ref name = " + m_pInstance->reference()->name());
//			ErrorLog::addError(errorList);

			delete m_pInstance;
			//delete m_pNumberOfOccurence;
		}
	}

	//delete m_pRenderProperties;
}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

bool Occurence::hasRepresentation() const
{
	if ((NULL != m_pInstance) && (m_pInstance->hasOccurence()))
	{
		return this->reference()->hasRepresentation();
	}
	else return false;
}

bool Occurence::has3DViewInstance() const
{
	if ( NULL != m_pWorldHandle)
	{
//		return m_pWorldHandle->collection()->contains(m_Uid);
	}
	else return false;
}

bool Occurence::canBeAddedToChildren(Occurence* pOccurence) const
{
	bool canBeAdded= false;
	if ((NULL != m_pInstance) && (m_pInstance->hasOccurence()) && (NULL != pOccurence->m_pInstance) && (NULL != pOccurence->reference()))
	{
		if (this->reference() != pOccurence->reference())
		{
			std::set<Reference*> thisRefSet= Occurence::parentsReferences(this);
			thisRefSet.insert(this->reference());
			std::set<Reference*> childRefSet= pOccurence->childrenReferences();

		//	canBeAdded= thisRefSet == (thisRefSet - childRefSet);
		}
	}
	else
	{
		canBeAdded= true;
	}
	return canBeAdded;
}

std::vector<Occurence*> Occurence::subOccurenceList() const
{
	std::vector<Occurence*> subOccurence;
	const int childCount= m_Childs.size();
	for (int i= 0; i < childCount; ++i)
	{
		Occurence* pCurrentChild= m_Childs.at(i);
		if (pCurrentChild->hasChild())
		{
			std::vector<Occurence*>::const_iterator itor;
			for (itor = pCurrentChild->subOccurenceList().begin();itor != pCurrentChild->subOccurenceList().end();itor++)
			{
				subOccurence.push_back(*itor);
			}			
		}
		else
		{
			subOccurence.push_back(pCurrentChild);
		}
	}

	return subOccurence;
}

unsigned int Occurence::numberOfFaces() const
{
	unsigned int result= 0;
	if (hasRepresentation())
	{
		result= instance()->reference()->numberOfFaces();
	}

	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		result+= m_Childs.at(i)->numberOfFaces();
	}

	return result;
}

unsigned int Occurence::numberOfVertex() const
{
	unsigned int result= 0;
	if (hasRepresentation())
	{
		result= instance()->reference()->numberOfVertex();
	}
	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		result+= m_Childs.at(i)->numberOfVertex();
	}

	return result;
}

// Get number of materials
unsigned int Occurence::numberOfMaterials() const
{
	unsigned int result= 0;
	//std::set<Material*> materialSet;
	//if (hasRepresentation())
	//{
	//	result= Instance()->reference()->numberOfMaterials();
	//}

	//const int size= m_Childs.size();
	//for (int i= 0; i < size; ++i)
	//{
	//	materialSet.unite(m_Childs.at(i)->materialSet());
	//}
	//result= static_cast<unsigned int>(materialSet.size());

	return result;
}

// Get materials List
//std::set<Material*> Occurence::materialSet() const
//{
//	std::set<Material*> materialSet;
//	if (hasRepresentation())
//	{
//		materialSet= Instance()->reference()->materialSet();
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
Occurence* Occurence::clone(WorldHandle* pWorldHandle, bool shareInstance) const
{
	return new Occurence(pWorldHandle, *this, shareInstance);
}

// Return true if the occurence is visible
bool Occurence::isVisible() const
{
	bool isHidden= true;

// 	if ((NULL != m_pWorldHandle) && m_pWorldHandle->collection()->contains(m_Uid))
// 	{
// 		isHidden= !m_pWorldHandle->collection()->instanceHandle(m_Uid)->isVisible();
// 	}
// 	else if (childCount() > 0)
// 	{
// 		const int size= childCount();
// 		int i= 0;
// 		while ((i < size) && isHidden)
// 		{
// 			isHidden= isHidden && !child(i)->isVisible();
// 			++i;
// 		}
// 	}
// 	else
// 	{
// 		isHidden= !m_IsVisible;
// 	}
	return !isHidden;
}

// Return the occurence Bounding Box
AABB Occurence::boundingBox() const
{
	AABB boundingBox;

	if (NULL != m_pWorldHandle)
	{
		//if (has3DViewInstance())
		//{
		//	assert(m_pWorldHandle->collection()->contains(id()));
		//	boundingBox= m_pWorldHandle->collection()->instanceHandle(id())->boundingBox();
		//}
		//else
		//{
		//	if (hasChild())
		//	{
		//		std::list<Occurence*> childrenList= children();
		//		const int size= childrenList.size();

		//		for (int i= 0; i < size; ++i)
		//		{
		//			boundingBox.combine(childrenList.at(i)->boundingBox());
		//		}
		//	}
		//}
	}

	return boundingBox;
}

unsigned int Occurence::nodeCount() const
{
	unsigned int result= 1;
	const int size= m_Childs.size();
	for (int i= 0; i < size; ++i)
	{
		result+= m_Childs.at(i)->nodeCount();
	}
	return result;
}

std::set<Reference*> Occurence::childrenReferences() const
{
	std::set<Reference*> refChildrenSet;
	const int childCount= m_Childs.size();
	for (int i= 0; i < childCount; ++i)
	{
		Occurence* pCurrentChild= m_Childs.at(i);
		if ((NULL != pCurrentChild->instance()) && (NULL != pCurrentChild->reference()))
		{
			refChildrenSet.insert(pCurrentChild->reference());
		}
	}

	return refChildrenSet;
}

std::set<Reference*> Occurence::parentsReferences(const Occurence* pOccurence)
{
	std::set<Reference*> parentSet;
	Occurence* pParent= pOccurence->parent();
	if (NULL != pParent)
	{
		if ((NULL != pParent->instance()) && (NULL != pParent->reference()))
		{
			parentSet.insert(pParent->reference());
			parentSet.insert(Occurence::parentsReferences(pParent).begin(),Occurence::parentsReferences(pParent).end());
		}
	}

	return parentSet;
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Update the absolute matrix
Occurence* Occurence::updateAbsoluteMatrix()
{
	if (NULL != m_pParent)
	{
		m_AbsoluteMatrix= m_pParent->absoluteMatrix() * m_pInstance->relativeMatrix();
	}
	else
	{
		m_AbsoluteMatrix= m_pInstance->relativeMatrix();
	}
	// If the occurence have a representation, update it.

	//if ((NULL != m_pWorldHandle) && m_pWorldHandle->collection()->contains(m_Uid))
	//{
	//	m_pWorldHandle->collection()->instanceHandle(m_Uid)->setMatrix(m_AbsoluteMatrix);
	//}
	return this;
}

// Update children obsolute Matrix
Occurence* Occurence::updateChildrenAbsoluteMatrix()
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
void Occurence::addChild(Occurence* pChild)
{
	assert(pChild->isOrphan());
	assert((NULL == pChild->m_pWorldHandle) || (m_pWorldHandle == pChild->m_pWorldHandle));

	//qDebug() << "Add Child " << pChild->name() << "id=" << pChild->id() << " to " << name() << " id=" << id();
	// Add the child to the list of child
	// Get occurence reference
	m_Childs.push_back(pChild);
	pChild->m_pParent= this;
	if (NULL == pChild->m_pWorldHandle)
	{
		pChild->setWorldHandle(m_pWorldHandle);
	}
	pChild->updateChildrenAbsoluteMatrix();
}

// Add Child instance and returns the newly created occurence
Occurence* Occurence::addChild(Instance* pInstance)
{
	Occurence* pOccurence;
	pOccurence= new Occurence(pInstance, m_pWorldHandle);

	addChild(pOccurence);

	return pOccurence;
}

// make the occurence orphan
void Occurence::makeOrphan()
{
	//qDebug() << "Occurence::makeOrphan() " << id();
	//qDebug() << name() << " " << id();
	assert(!isOrphan());
	m_pParent->removeChild(this);
	//qDebug() << "Occurence::makeOrphan() DONE!";
}

// Remove the specified child
bool Occurence::removeChild(Occurence* pChild)
{
	bool result = false;
	assert(pChild->m_pParent == this);
//	assert(m_Childs.contains(pChild));
	pChild->m_pParent= NULL;
	pChild->detach();

	std::vector<Occurence*>::const_iterator itor;
	for (itor = m_Childs.begin();itor != m_Childs.end();itor++)
	{
		if (*itor == pChild)
		{
			m_Childs.erase(itor);
			result = true;
		}
	}

	return result;
}

// Detach the occurence from the World
void Occurence::detach()
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
	//	m_pWorldHandle->removeOccurence(this);
		m_pWorldHandle= NULL;
		if (!m_Childs.empty())
		{
			const int size= m_Childs.size();
			for (int i= 0; i < size; ++i)
			{
				m_Childs[i]->detach();
			}
		}
	}
}

// Reverse Normals of this Occurence and childs
void Occurence::reverseNormals()
{
	if (has3DViewInstance())
	{
//		m_pWorldHandle->collection()->instanceHandle(id())->reverseGeometriesNormals();
	}
}

// Check the presence of representation
bool Occurence::create3DViewInstance()
{
	bool creationSuccess= false;
	if ((NULL != m_pWorldHandle) && hasRepresentation())
	{
		Represent3D* pRepresent3D= dynamic_cast<Represent3D*>(reference()->representationHandle());
		if (NULL != pRepresent3D)
		{
			//3DViewInstance instance(*pRepresent3D);
			//instance.setName(name());
			//// Force instance representation id
			//instance.setId(id());
			//creationSuccess= m_pWorldHandle->collection()->add(instance);
			//m_pWorldHandle->collection()->setVisibility(m_Uid, m_IsVisible);
		}
	}
	return creationSuccess;
}

bool Occurence::remove3DViewInstance()
{
	if (NULL != m_pWorldHandle)
	{
	//	return m_pWorldHandle->collection()->remove(m_Uid);
	}
	else return false;
}

// Set the occurence world Handle
void Occurence::setWorldHandle(WorldHandle* pWorldHandle)
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
// 		m_pWorldHandle->collection()->setVisibility(m_Uid, m_IsVisible);
// 
		const int size= m_Childs.size();
		for (int i= 0; i < size; ++i)
		{
			m_Childs[i]->setWorldHandle(m_pWorldHandle);
		}
	}
}

// Load the representation and return true if success
bool Occurence::loadRepresentation()
{
	assert(!this->has3DViewInstance());

	bool loadSuccess= false;
	if (hasRepresentation())
	{
		Reference* pReference= this->reference();
		if (pReference->representationIsLoaded())
		{
			loadSuccess= create3DViewInstance();
		}
		else
		{
			loadSuccess=  m_pInstance->reference()->loadRepresentation();
			if (loadSuccess && !m_AutomaticCreationOf3DViewInstance)
			{
				loadSuccess= create3DViewInstance();
			}
		}
	}

	return loadSuccess;
}

// UnLoad the representation and return true if success
bool Occurence::unloadRepresentation()
{
	bool unloadResult= false;
	if (hasRepresentation())
	{
		Reference* pRef= this->reference();
		if (pRef->representationIsLoaded())
		{
			if (this->has3DViewInstance())
			{
//				unloadResult= m_pWorldHandle->collection()->remove(m_Uid);
				std::set<Occurence*> occurenceSet= pRef->setOfOccurence();
				std::set<Occurence*>::const_iterator iOcc= occurenceSet.begin();
				bool unloadReferenceRep= true;
				while (occurenceSet.end() != iOcc)
				{
					unloadReferenceRep= unloadReferenceRep && !(*iOcc)->has3DViewInstance();
					++iOcc;
				}
				if (unloadReferenceRep)
				{
					pRef->unloadRepresentation();
				}
			}
		}
	}
	return unloadResult;
}

unsigned int Occurence::updateOccurenceNumber(unsigned int n)
{
	m_OccurenceNumber= n++;
	const int childCount= m_Childs.size();
	for (int i= 0; i < childCount; ++i)
	{
		n= m_Childs[i]->updateOccurenceNumber(n);
	}
	return n;
}

void Occurence::setVisibility(bool visibility)
{
	m_IsVisible= visibility;
	if (has3DViewInstance())
	{
	//	m_pWorldHandle->collection()->setVisibility(m_Uid, m_IsVisible);
	}
	const int childCount= m_Childs.size();
	for (int i= 0; i < childCount; ++i)
	{
		m_Childs[i]->setVisibility(m_IsVisible);
	}
}

//void Occurence::setRenderProperties(const RenderProperties& renderProperties)
//{
//	delete m_pRenderProperties;
//	if (has3DViewInstance())
//	{
////		m_pWorldHandle->collection()->instanceHandle(m_Uid)->setRenderProperties(renderProperties);
//	}
//	else if (hasChild())
//	{
//		const int childCount= m_Childs.size();
//		for (int i= 0; i < childCount; ++i)
//		{
//			m_Childs[i]->setRenderProperties(renderProperties);
//		}
//	}
//	else
//	{
//		m_pRenderProperties= new RenderProperties(renderProperties);
//	}
//}

void Occurence::removeEmptyChildren()
{
	std::vector<Occurence*>::iterator iChild= m_Childs.begin();
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

void Occurence::setReference(Reference* pRef)
{
	assert(m_pInstance->reference() == NULL);
	assert((*m_pNumberOfOccurence) == 1);

	if (pRef->hasInstance())
	{
		Instance* pExistingInstance= pRef->firstInstanceHandle();
		if (pExistingInstance->hasOccurence())
		{
			Occurence* pFirstOccurence= pExistingInstance->firstOccurenceHandle();
			std::vector<Occurence*> childs= pFirstOccurence->m_Childs;
			const int size= childs.size();
			for (int i= 0; i < size; ++i)
			{
				Occurence* pChild= childs.at(i)->clone(m_pWorldHandle, true);
				addChild(pChild);
			}

			std::vector<Instance*> instances= pRef->listOfInstances();
			const int instanceCount= instances.size();
			int i= 0;
			bool continu= true;
			while (continu && (i < instanceCount))
			{
				if (m_pInstance == instances.at(i))
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

	m_pInstance->setReference(pRef);
}

Reference* Occurence::reference() const
{
	assert(NULL != m_pInstance);
	return m_pInstance->reference();
}

Instance* Occurence::instance() const
{
	return m_pInstance;
}

Occurence* Occurence::child( const int index ) const
{
	return m_Childs.at(index);
}

const std::string Occurence::name() const
{
	return m_pInstance->name();
}

void Occurence::setName( const std::string name )
{
	m_pInstance->setName(name);
}