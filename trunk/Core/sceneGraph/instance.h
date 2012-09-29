
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

//! \file structinstance.h interface for the StructInstance class.

#ifndef STRUCTINSTANCE_H_
#define STRUCTINSTANCE_H_

#include <list>
#include <string>
#include "attributes.h"

#include "vlCore/config.hpp"
#include "vlCore/Vector4.hpp"
#include "vlCore/Matrix4.hpp"

#include "../config.h"
using namespace vl;

class StructReference;
class StructOccurence;
class Rep;

//////////////////////////////////////////////////////////////////////
//! \class StructInstance
/*! \brief StructInstance : A scene graph instance node */
//////////////////////////////////////////////////////////////////////
class GVCORE_EXPORT StructInstance
{
public:
	//! Default constructor
	StructInstance(StructReference* pRef= NULL);

	//! Create instance with a rep
	StructInstance(Rep*);

	//! Copy constructor
	StructInstance(const StructInstance&);

	//! Copy constructor
	StructInstance(StructInstance*);

	//! Create empty instance
	StructInstance(const std::string&);

	//! Set the reference of an empty instance
	void setReference(StructReference*);

	// Destructor
	virtual ~StructInstance();
//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return true if this instance have occurence
	inline bool hasStructOccurence() const
	{ return !m_ListOfOccurences.empty();}

	//! Return the number of occurence
	inline int numberOfOccurence() const
	{return m_ListOfOccurences.size();}

	//! Return first occurence handle
	inline StructOccurence* firstOccurenceHandle() const
	{ return m_ListOfOccurences.front();}

	//! Return the relative matrix of this instance
	inline dmat4 relativeMatrix() const
	{ return m_RelativeMatrix;}

	//! Return the reference of this instance
	inline StructReference* structReference() const
	{ return m_pStructReference;}

	//! Return the list off occurence of this instance
	inline std::vector<StructOccurence*> listOfStructOccurences() const
	{ return m_ListOfOccurences;}

	//! Return the instance name
	inline std::string name() const
	{return m_Name;}

	//! Return true if the reference contains User attributes
	inline bool containsAttributes() const
	{ return ((NULL != m_pAttributes) && !m_pAttributes->isEmpty());}

	//! Return handle to the reference attributes
	inline Attributes* attributesHandle() const
	{return m_pAttributes;}

	//! Return the number of usage of this instance
	inline int usageCount() const
	{return *m_pNumberOfInstance;}

//@}
//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! An occurence of this instance have been created
	inline void structOccurenceCreated(StructOccurence* pOccurence)
	{
//		assert(!m_ListOfOccurences.find(pOccurence) != m_ListOfOccurences.end());

		std::vector<StructOccurence*>::const_iterator itor;
		for (itor=m_ListOfOccurences.begin();itor!=m_ListOfOccurences.end();itor++)
		{
			if (*itor == pOccurence)
			{
				assert(false);
			}			
		}

		m_ListOfOccurences.push_back(pOccurence);
	}

	inline void structOccurenceDeleted(StructOccurence *pOccurence)
	{
		std::vector<StructOccurence*>::const_iterator itor;
		for (itor=m_ListOfOccurences.begin();itor!=m_ListOfOccurences.end();itor++)
		{
			if (*itor == pOccurence)
			{
				m_ListOfOccurences.erase(itor);
			}			
		}
	}

	//! Move the instance by specified matrix
	inline StructInstance* move(const vl::dmat4& matrix)
	{
		m_RelativeMatrix= matrix * m_RelativeMatrix;
		return this;
	}

	//! Translate Instance
	inline StructInstance* translate(double Tx, double Ty, double Tz)
	{
		m_RelativeMatrix= vl::dmat4::getTranslation(Tx, Ty, Tz) * m_RelativeMatrix;
		return this;
	}

	//! Translate Instance
	inline StructInstance* translate(const vl::dvec3& v)
	{
		return translate(v.x(), v.y(), v.z());
	}

	//! Replace the instance Matrix
	inline StructInstance* setMatrix(const vl::dmat4 &SetMat)
	{
		m_RelativeMatrix= SetMat;
		return this;
	}

	//! Reset the instance Matrix
	inline StructInstance* resetMatrix()
	{
		m_RelativeMatrix = dmat4::getIdentity();
		return this;
	}

	//! Set the instance name
	inline void setName(const std::string& name)
	{m_Name= name;}

	//! Set the instance attributes
	void setAttributes(const Attributes& attr)
	{
		delete m_pAttributes;
		m_pAttributes= new Attributes(attr);
	}


//@}

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! Number of this Instance
	int* m_pNumberOfInstance;

	//! The Struct reference of this instance
	StructReference* m_pStructReference;

	//! The list of instance's occurences
	std::vector<StructOccurence*> m_ListOfOccurences;

	//! The relative matrix
	vl::dmat4 m_RelativeMatrix;

	//! The instance Name
	std::string m_Name;

	//! The Reference attributes
	Attributes* m_pAttributes;
};

#endif /* STRUCTINSTANCE_H_ */
