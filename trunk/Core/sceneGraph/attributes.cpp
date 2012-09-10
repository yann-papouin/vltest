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
//! \file attributes.cpp implementation of the Attributes class.

#include "attributes.h"

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
