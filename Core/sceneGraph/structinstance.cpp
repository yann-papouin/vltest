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

//! \file structinstance.cpp implementation of the StructInstance class.

#include "structinstance.h"
#include "structreference.h"
#include "structoccurence.h"
#include <iostream>

// Default constructor
StructInstance::StructInstance(StructReference* pStructReference)
: m_pNumberOfInstance(NULL)
, m_pStructReference(pStructReference)
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name()
, m_pAttributes(NULL)
{
	if (NULL == m_pStructReference)
	{
		m_pStructReference= new StructReference();
	}
	m_Name= m_pStructReference->name();

	if (m_pStructReference->hasStructInstance())
	{
		m_pNumberOfInstance= m_pStructReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);
	//cout<< "StructInstance::StructInstance : " << (*m_pNumberOfInstance) << " " << m_pNumberOfInstance;
}

// Create instance with a rep
StructInstance::StructInstance(Rep* pRep)
: m_pNumberOfInstance(NULL)
, m_pStructReference(new StructReference(pRep))
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name(m_pStructReference->name())
, m_pAttributes(NULL)
{
	if (m_pStructReference->hasStructInstance())
	{
		m_pNumberOfInstance= m_pStructReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);
}

// Copy constructor
StructInstance::StructInstance(const StructInstance& structInstance)
: m_pNumberOfInstance(structInstance.m_pNumberOfInstance)
, m_pStructReference(structInstance.m_pStructReference)
, m_ListOfOccurences()
, m_RelativeMatrix(structInstance.m_RelativeMatrix)
, m_Name(structInstance.name())
, m_pAttributes(NULL)
{
	//cout<< "Instance Copy constructor";
	assert(NULL != m_pStructReference);
	// Copy attributes if necessary
	if (NULL != structInstance.m_pAttributes)
	{
		m_pAttributes= new Attributes(*(structInstance.m_pAttributes));
	}

	++(*m_pNumberOfInstance);

	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);
}

// Copy constructor
StructInstance::StructInstance(StructInstance* pStructInstance)
: m_pNumberOfInstance(pStructInstance->m_pNumberOfInstance)
, m_pStructReference(pStructInstance->m_pStructReference)
, m_ListOfOccurences()
, m_RelativeMatrix(pStructInstance->m_RelativeMatrix)
, m_Name(pStructInstance->name())
, m_pAttributes(NULL)
{
	//cout<< "Instance Copy constructor";
	assert(NULL != m_pStructReference);
	// Copy attributes if necessary
	if (NULL != pStructInstance->m_pAttributes)
	{
		m_pAttributes= new Attributes(*(pStructInstance->m_pAttributes));
	}

	++(*m_pNumberOfInstance);

	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);
}

// Create empty instance
StructInstance::StructInstance(const std::string& name)
: m_pNumberOfInstance(NULL)
, m_pStructReference(NULL)
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name(name)
, m_pAttributes(NULL)
{
}

// Set the reference of an empty instance
void StructInstance::setReference(StructReference* pStructReference)
{
	assert(NULL == m_pStructReference);
	assert(NULL == m_pNumberOfInstance);
	m_pStructReference= pStructReference;
	if (m_pStructReference->hasStructInstance())
	{
		m_pNumberOfInstance= m_pStructReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);

	if (m_Name.empty())
	{
		m_Name= pStructReference->name();
	}
}

// Destructor
StructInstance::~StructInstance()
{
	if(m_pNumberOfInstance != NULL)
	{
		// Update number of instance
		if ((--(*m_pNumberOfInstance)) == 0)
		{
			delete m_pStructReference;
			delete m_pNumberOfInstance;
		}
		else
		{
			// Inform reference that an instance has been deleted
			m_pStructReference->structInstanceDeleted(this);
			assert(m_pStructReference->hasStructInstance());
		}
		delete m_pAttributes;
	}
	else 
		cout<< "StructInstance::~StructInstance() of empty instance";

}
