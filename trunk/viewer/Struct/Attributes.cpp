//! \file attributes.cpp implementation of the Attributes class.

#include "struct/attributes.h"

// Default constructor
Attributes::Attributes()
: m_AttributesHash()
, m_AttributesList()
{

}

// Copy Constructor
Attributes::Attributes(const Attributes& attr)
: m_AttributesHash(attr.m_AttributesHash)
, m_AttributesList(attr.m_AttributesList)
{

}

// Overload "=" operator
Attributes& Attributes::operator=(const Attributes& attr)
{
	if (this != &attr)
	{
		m_AttributesHash= attr.m_AttributesHash;
		m_AttributesList= attr.m_AttributesList;
	}
	return *this;
}

// Destructor
Attributes::~Attributes()
{

}
