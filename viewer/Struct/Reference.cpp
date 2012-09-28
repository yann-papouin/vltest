//! \file Reference.cpp implementation of the Reference class.

#include "struct/Reference.h"
#include "struct/Occurence.h"
#include "struct/Represent3D.h"
#include "struct/Represent.h"
#include "struct/Attributes.h"

// Default constructor
Reference::Reference(const std::string& name)
: m_SetOfInstance()
, m_pRepresentation(NULL)
, m_Name(name)
, m_pAttributes(NULL)
{

}

// Create reference with representation
Reference::Reference(Represent* pRep)
: m_SetOfInstance()
, m_pRepresentation(pRep)
, m_Name(m_pRepresentation->name())
, m_pAttributes(NULL)
{

}

// Copy constructor
Reference::Reference(const Reference& ref)
: m_SetOfInstance()
, m_pRepresentation(NULL)
, m_Name(ref.m_Name)
, m_pAttributes(new Attributes(*(ref.m_pAttributes)))
{
	if (NULL != ref.m_pRepresentation)
	{
		m_pRepresentation= ref.m_pRepresentation->clone();
	}
}

//! Overload "=" operator
Reference& Reference::operator=(const Reference& ref)
{
	if (this != &ref)
	{
		m_SetOfInstance.clear();
		delete m_pAttributes;
		m_pAttributes= NULL;

		m_Name= ref.m_Name;
		m_pAttributes= new Attributes(*(ref.m_pAttributes));

		if (NULL != ref.m_pRepresentation)
		{
			m_pRepresentation= ref.m_pRepresentation->clone();
		}
	}
	return *this;
}

Reference::~Reference()
{
	delete m_pRepresentation;
	delete m_pAttributes;
}


//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

std::set<Occurence*> Reference::setOfOccurence() const
{
	std::vector<Instance*> instanceList= listOfInstances();
	std::set<Occurence*> occurenceSet;
	const int size= instanceList.size();
	for (int i= 0; i < size; ++i)
	{
		std::vector<Occurence*> occurenceList= instanceList.at(i)->listOfOccurences();
		const int occurenceSize= occurenceList.size();
		for (int occIndex= 0; occIndex < occurenceSize; ++occIndex)
		{
			occurenceSet.insert(occurenceList[occIndex]);
		}
	}
	return occurenceSet;
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////
// Set the reference representation
void Reference::setRepresentation(const Represent3D& rep)
{
	// Unload occurence representation
	{
		std::set<Occurence*> OccurenceSet= this->setOfOccurence();
		std::set<Occurence*>::iterator iOcc= OccurenceSet.begin();
		while (OccurenceSet.end() != iOcc)
		{
			(*iOcc)->remove3DViewInstance();
			++iOcc;
		}
	}

	if(NULL == m_pRepresentation)
	{
		m_pRepresentation= new Represent3D(rep);
	}
	else
	{
		*(m_pRepresentation)= rep;
	}

	if (m_pRepresentation->isLoaded())
	{
		std::set<Occurence*> OccurenceSet= this->setOfOccurence();
		std::set<Occurence*>::iterator iOcc= OccurenceSet.begin();
		while (OccurenceSet.end() != iOcc)
		{
			Occurence* pOccurence= *iOcc;
			assert(!pOccurence->has3DViewInstance());
			if (pOccurence->useAutomatic3DViewInstanceCreation())
			{
				pOccurence->create3DViewInstance();
			}
			++iOcc;
		}
	}
}

Represent* Reference::representationHandle() const
{
	assert(NULL != m_pRepresentation);
	return m_pRepresentation;
}

std::string Reference::representationName() const
{
	if (NULL != m_pRepresentation)
	{
		return m_pRepresentation->name();
	}
	else return std::string();
}

bool Reference::representationIsLoaded() const
{
	if (NULL != m_pRepresentation)
	{
		return m_pRepresentation->isLoaded();
	}
	else return false;

}

std::string Reference::representationFileName() const
{
	if (NULL != m_pRepresentation)
	{
		return m_pRepresentation->fileName();
	}
	else return std::string();
}

bool Reference::representationIsEmpty() const
{
	if (NULL != m_pRepresentation)
	{
		return m_pRepresentation->isEmpty();
	}
	else return true;

}

void Reference::setRepresentationName(const std::string& representationName)
{
	if (NULL != m_pRepresentation)
	{
		m_pRepresentation->setName(representationName);
	}
}

bool Reference::loadRepresentation()
{
	assert(NULL != m_pRepresentation);
	if (m_pRepresentation->load())
	{
		std::set<Occurence*> OccurenceSet= this->setOfOccurence();
		std::set<Occurence*>::iterator iOcc= OccurenceSet.begin();
		while (OccurenceSet.end() != iOcc)
		{
			Occurence* pOccurence= *iOcc;
			assert(!pOccurence->has3DViewInstance());
			if (pOccurence->useAutomatic3DViewInstanceCreation())
			{
				pOccurence->create3DViewInstance();
			}
			++iOcc;
		}
		return true;
	}
	else return false;
}

bool Reference::unloadRepresentation()
{
	assert(NULL != m_pRepresentation);
	if (m_pRepresentation->unload())
	{
		std::set<Occurence*> OccurenceSet= this->setOfOccurence();
		std::set<Occurence*>::iterator iOcc= OccurenceSet.begin();
		while (OccurenceSet.end() != iOcc)
		{
			(*iOcc)->remove3DViewInstance();
			++iOcc;
		}
		return true;
	}
	else return false;
}

bool Reference::addChild(Occurence* pOccurence)
{
	if (hasInstance() && firstInstanceHandle()->hasOccurence())
	{
		Occurence* pCurrentChildOccurence= pOccurence;

		std::set<Instance*>::iterator iInstance= m_SetOfInstance.begin();
		while (m_SetOfInstance.end() != iInstance)
		{
			Instance* pCurrentInstance= *iInstance;
			std::vector<Occurence*> occurenceList= pCurrentInstance->listOfOccurences();
			const int occurenceCount= occurenceList.size();
			for (int i= 0; i < occurenceCount; ++i)
			{
				Occurence* pCurrentOccurence= occurenceList[i];

				if ((i != 0) || (NULL == pCurrentChildOccurence))
				{
//					pCurrentChildOccurence= pOccurence->clone(pCurrentOccurence->VLBaseView(), true);
				}

				pCurrentOccurence->addChild(pCurrentChildOccurence);
			}
			pCurrentChildOccurence= NULL;
			++iInstance;
		}
		return true;
	}
	else
	{
		return false;
	}
}

unsigned int Reference::numberOfFaces() const
{
	assert(NULL != m_pRepresentation);
	Represent3D* pRep= dynamic_cast<Represent3D*>(m_pRepresentation);
	if (NULL != pRep) return pRep->faceCount();
	else return 0;
}

unsigned int Reference::numberOfVertex() const
{
	assert(NULL != m_pRepresentation);
	Represent3D* pRep= dynamic_cast<Represent3D*>(m_pRepresentation);
	if (NULL != pRep) return pRep->vertexCount();
	else return 0;
}

unsigned int Reference::numberOfMaterials() const
{
	assert(NULL != m_pRepresentation);
	Represent3D* pRep= dynamic_cast<Represent3D*>(m_pRepresentation);
	if (NULL != pRep) return pRep->materialCount();
	else return 0;
}

unsigned int Reference::numberOfBody() const
{
	if(NULL != m_pRepresentation)
	{
		Represent3D* pRep= dynamic_cast<Represent3D*>(m_pRepresentation);
		if (NULL != pRep) return pRep->numberOfBody();
		else return 0;
	}
	else return 0;
}

void Reference::setName( const std::string& name )
{
	m_Name= name;
}

void Reference::InstanceDeleted( Instance* pInstance )
{
	m_SetOfInstance.erase(pInstance);
}

void Reference::InstanceCreated( Instance* pInstance )
{
	//		assert(!m_SetOfInstance.contains(pInstance));
	m_SetOfInstance.insert(pInstance);
	//		m_SetOfInstance << pInstance;
}

void Reference::setAttributes( const Attributes& attr )
{
	delete m_pAttributes;
	m_pAttributes= new Attributes(attr);
}

Attributes* Reference::attributesHandle() const
{
	return m_pAttributes;
}

bool Reference::containsAttributes() const
{
	return ((NULL != m_pAttributes) && !m_pAttributes->isEmpty());
}

std::string Reference::name() const
{
	return m_Name;
}

bool Reference::hasRepresentation() const
{
	return NULL != m_pRepresentation;
}

std::vector<Instance*> Reference::listOfInstances() const
{
	std::vector<Instance*> vec;
	std::set<Instance*>::const_iterator itor;
	for (itor=m_SetOfInstance.begin();itor!=m_SetOfInstance.end();itor++)
	{
		vec.push_back(*itor);
	}
	return vec;
}

std::vector<Occurence*> Reference::listOfOccurence() const
{
	std::vector<Occurence*> vec;
	std::set<Occurence*>::const_iterator itor;
	for (itor=setOfOccurence().begin();itor!=setOfOccurence().end();itor++)
	{
		vec.push_back(*itor);
	}
	return vec;
}

