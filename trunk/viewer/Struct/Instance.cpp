//! \file Instance.cpp implementation of the Instance class.

#include "struct/Instance.h"
#include "struct/Reference.h"
#include "struct/Occurence.h"
#include "struct/Represent.h"

// Default constructor
Instance::Instance(Reference* pReference)
: m_pNumberOfInstance(NULL)
, m_pReference(pReference)
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name()
, m_pAttributes(NULL)
{
	if (NULL == m_pReference)
	{
		m_pReference= new Reference();
	}
	m_Name= m_pReference->name();

	if (m_pReference->hasInstance())
	{
		m_pNumberOfInstance= m_pReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pReference->InstanceCreated(this);
	//qDebug() << "Instance::Instance : " << (*m_pNumberOfInstance) << " " << m_pNumberOfInstance;
}

// Create instance with a rep
Instance::Instance(Represent* pRep)
: m_pNumberOfInstance(NULL)
, m_pReference(new Reference(pRep))
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name(m_pReference->name())
, m_pAttributes(NULL)
{
	if (m_pReference->hasInstance())
	{
		m_pNumberOfInstance= m_pReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pReference->InstanceCreated(this);
}

// Copy constructor
Instance::Instance(const Instance& Instance)
: m_pNumberOfInstance(Instance.m_pNumberOfInstance)
, m_pReference(Instance.m_pReference)
, m_ListOfOccurences()
, m_RelativeMatrix(Instance.m_RelativeMatrix)
, m_Name(Instance.name())
, m_pAttributes(NULL)
{
	//qDebug() << "Instance Copy constructor";
	assert(NULL != m_pReference);
	// Copy attributes if necessary
	if (NULL != Instance.m_pAttributes)
	{
		m_pAttributes= new Attributes(*(Instance.m_pAttributes));
	}

	++(*m_pNumberOfInstance);

	// Inform reference that an instance has been created
	m_pReference->InstanceCreated(this);
}

// Copy constructor
Instance::Instance(Instance* pInstance)
: m_pNumberOfInstance(pInstance->m_pNumberOfInstance)
, m_pReference(pInstance->m_pReference)
, m_ListOfOccurences()
, m_RelativeMatrix(pInstance->m_RelativeMatrix)
, m_Name(pInstance->name())
, m_pAttributes(NULL)
{
	//qDebug() << "Instance Copy constructor";
	assert(NULL != m_pReference);
	// Copy attributes if necessary
	if (NULL != pInstance->m_pAttributes)
	{
		m_pAttributes= new Attributes(*(pInstance->m_pAttributes));
	}

	++(*m_pNumberOfInstance);

	// Inform reference that an instance has been created
	m_pReference->InstanceCreated(this);
}

// Create empty instance
Instance::Instance(const std::string& name)
: m_pNumberOfInstance(NULL)
, m_pReference(NULL)
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name(name)
, m_pAttributes(NULL)
{
}

// Set the reference of an empty instance
void Instance::setReference(Reference* pReference)
{
	assert(NULL == m_pReference);
	assert(NULL == m_pNumberOfInstance);
	m_pReference= pReference;
	if (m_pReference->hasInstance())
	{
		m_pNumberOfInstance= m_pReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pReference->InstanceCreated(this);

	if (m_Name.empty())
	{
		m_Name= pReference->name();
	}
}

// Destructor
Instance::~Instance()
{
	if(m_pNumberOfInstance != NULL)
	{
		// Update number of instance
		if ((--(*m_pNumberOfInstance)) == 0)
		{
			delete m_pReference;
			delete m_pNumberOfInstance;
		}
		else
		{
			// Inform reference that an instance has been deleted
			m_pReference->InstanceDeleted(this);
			assert(m_pReference->hasInstance());
		}
		delete m_pAttributes;
	}
	//else 
	//	cout<< "Instance::~Instance() of empty instance";
}
