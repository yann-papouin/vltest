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

//! \file structreference.cpp implementation of the StructReference class.

#include "structreference.h"
#include "structoccurence.h"

// Default constructor
StructReference::StructReference(const std::string& name)
: m_SetOfInstance()
, m_pRepresentation(NULL)
, m_Name(name)
, m_pAttributes(NULL)
{


}

// Create reference with representation
StructReference::StructReference(Rep* pRep)
: m_SetOfInstance()
, m_pRepresentation(pRep)
, m_Name(m_pRepresentation->name())
, m_pAttributes(NULL)
{

}

// Copy constructor
StructReference::StructReference(const StructReference& ref)
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
StructReference& StructReference::operator=(const StructReference& ref)
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

StructReference::~StructReference()
{
	delete m_pRepresentation;
	delete m_pAttributes;
}


//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

std::set<StructOccurence*> StructReference::setOfStructOccurence() const
{
	std::vector<StructInstance*> instanceList= listOfStructInstances();
	std::set<StructOccurence*> occurenceSet;
	const int size= instanceList.size();
	for (int i= 0; i < size; ++i)
	{
		std::vector<StructOccurence*> occurenceList= instanceList.at(i)->listOfStructOccurences();
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
void StructReference::setRepresentation(const Rep3D& rep)
{
	// Unload occurence representation
	{
		std::set<StructOccurence*> structOccurenceSet= this->setOfStructOccurence();
		std::set<StructOccurence*>::iterator iOcc= structOccurenceSet.begin();
		while (structOccurenceSet.end() != iOcc)
		{
			(*iOcc)->remove3DViewInstance();
			++iOcc;
		}
	}

	if(NULL == m_pRepresentation)
	{
		m_pRepresentation= new Rep3D(rep);
	}
	else
	{
		*(m_pRepresentation)= rep;
	}

	if (m_pRepresentation->isLoaded())
	{
		std::set<StructOccurence*> structOccurenceSet= this->setOfStructOccurence();
		std::set<StructOccurence*>::iterator iOcc= structOccurenceSet.begin();
		while (structOccurenceSet.end() != iOcc)
		{
			StructOccurence* pOccurence= *iOcc;
			assert(!pOccurence->has3DViewInstance());
			if (pOccurence->useAutomatic3DViewInstanceCreation())
			{
				pOccurence->create3DViewInstance();
			}
			++iOcc;
		}
	}
}

Rep* StructReference::representationHandle() const
{
	assert(NULL != m_pRepresentation);
	return m_pRepresentation;
}

std::string StructReference::representationName() const
{
	if (NULL != m_pRepresentation)
	{
		return m_pRepresentation->name();
	}
	else return std::string();
}

bool StructReference::representationIsLoaded() const
{
	if (NULL != m_pRepresentation)
	{
		return m_pRepresentation->isLoaded();
	}
	else return false;

}

std::string StructReference::representationFileName() const
{
	if (NULL != m_pRepresentation)
	{
		return m_pRepresentation->fileName();
	}
	else return std::string();
}

bool StructReference::representationIsEmpty() const
{
	if (NULL != m_pRepresentation)
	{
		return m_pRepresentation->isEmpty();
	}
	else return true;

}

void StructReference::setRepresentationName(const std::string& representationName)
{
	if (NULL != m_pRepresentation)
	{
		m_pRepresentation->setName(representationName);
	}
}

bool StructReference::loadRepresentation()
{
	assert(NULL != m_pRepresentation);
	if (m_pRepresentation->load())
	{
		std::set<StructOccurence*> structOccurenceSet= this->setOfStructOccurence();
		std::set<StructOccurence*>::iterator iOcc= structOccurenceSet.begin();
		while (structOccurenceSet.end() != iOcc)
		{
			StructOccurence* pOccurence= *iOcc;
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

bool StructReference::unloadRepresentation()
{
	assert(NULL != m_pRepresentation);
	if (m_pRepresentation->unload())
	{
		std::set<StructOccurence*> structOccurenceSet= this->setOfStructOccurence();
		std::set<StructOccurence*>::iterator iOcc= structOccurenceSet.begin();
		while (structOccurenceSet.end() != iOcc)
		{
			(*iOcc)->remove3DViewInstance();
			++iOcc;
		}
		return true;
	}
	else return false;
}

bool StructReference::addChild(StructOccurence* pOccurence)
{
	if (hasStructInstance() && firstInstanceHandle()->hasStructOccurence())
	{
		StructOccurence* pCurrentChildOccurence= pOccurence;

		std::set<StructInstance*>::iterator iInstance= m_SetOfInstance.begin();
		while (m_SetOfInstance.end() != iInstance)
		{
			StructInstance* pCurrentInstance= *iInstance;
			std::vector<StructOccurence*> occurenceList= pCurrentInstance->listOfStructOccurences();
			const int occurenceCount= occurenceList.size();
			for (int i= 0; i < occurenceCount; ++i)
			{
				StructOccurence* pCurrentOccurence= occurenceList[i];

				if ((i != 0) || (NULL == pCurrentChildOccurence))
				{
					pCurrentChildOccurence= pOccurence->clone(pCurrentOccurence->worldHandle(), true);
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

