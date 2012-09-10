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
//! \file attributes.h interface for the Attributes class.

#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <string>
#include <list>
//#include <unordered_map>
 #include <hash_map>
#include <assert.h>

//using namespace std::tr1;
using namespace std;

#include "../config.h"

//////////////////////////////////////////////////////////////////////
//! \class Attributes
/*! \brief Attributes : User attributes of instance and reference */
//////////////////////////////////////////////////////////////////////
class GVCORE_EXPORT Attributes
{

//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default Constructor
	Attributes();

	//! Copy Constructor
	Attributes(const Attributes&);

	//! Overload "=" operator
	Attributes& operator=(const Attributes&);

	//! Destructor
	virtual ~Attributes();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return true if attributes is empty
	inline bool isEmpty() const
	{return m_AttributesHash.empty();}

	//! Return the size of attributes
	inline int size() const
	{return m_AttributesHash.size();}

	//! Return true if the specified attribute exist
	bool contains(const std::string& name) const
	{return m_AttributesHash.find(name)!=m_AttributesHash.end();}

	//! Return the list of attribute name
	inline std::vector<std::string> names() const
	{return m_AttributesList;}

	//! Return the value of the specified attributes
	/*! Return NULL String if attribute doesn't exist*/
	inline std::string value(const std::string& name)
	{
		return m_AttributesHash[name];
	}

//	mapped_type& operator[](const key_type& _Keyval)


	//! Return the name of the specified attributes index
	/*! Return empty String if attribute doesn't exist*/
	inline std::string name(int at) const
	{return m_AttributesList[at];}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Insert an attribute (if the attribute exists, it's updated)
	inline void insert(const std::string& name, const std::string& value)
	{
		if (m_AttributesHash.find(name)!=m_AttributesHash.end()) m_AttributesList.push_back(name);
		//m_AttributesHash.insert(name, value);
		m_AttributesHash[name] = value;
	}

	//! Remove an attribute
	inline void remove(const std::string& name)
	{
		assert(m_AttributesHash.count(name)>0);
		m_AttributesHash.erase(name);

		std::vector<std::string>::const_iterator itor;
		for (itor=m_AttributesList.begin();itor!=m_AttributesList.end();itor++)
		{
			if (*itor == name)
			{
				m_AttributesList.erase(itor);
			}			
		}
		
// 		m_AttributesList.erase(name);
	}

	//! Clear the content of this attribute
	inline void clear()
	{
		m_AttributesHash.clear();
		m_AttributesList.clear();
	}

//@}

//////////////////////////////////////////////////////////////////////
/*! @name Operator Overload */
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Equal operator overload
	inline bool operator==(const Attributes& attr) const
	{return m_AttributesHash == attr.m_AttributesHash;}

//@}
//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! Attributes Hash table
	stdext::hash_map<std::string, std::string> m_AttributesHash;

	//! the list of attribute name
	std::vector<std::string> m_AttributesList;

};

#endif /* ATTRIBUTES_H_ */
