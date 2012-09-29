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

//! \file structreference.h interface for the StructReference class.

#ifndef STRUCTREFERENCE_H_
#define STRUCTREFERENCE_H_

#include <string>
#include <set>

#include "../geometry/Rep3D.h"
//#include "3dviewinstance.h"
#include "attributes.h"
#include "structinstance.h"

#include "../config.h"

class StructOccurence;

//////////////////////////////////////////////////////////////////////
//! \class StructReference
/*! \brief StructReference : A scene graph reference node */
//////////////////////////////////////////////////////////////////////
class GVCORE_EXPORT StructReference
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default Constructor
	StructReference(const std::string& name= std::string());

	//! Create reference with representation
	StructReference(Rep*);

	//! Copy constructor
	StructReference(const StructReference&);

	//! Overload "=" operator
	StructReference& operator=(const StructReference&);

	//! Destructor
	virtual ~StructReference();
//@}
//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return true if this reference has instance
	inline bool hasStructInstance() const
	{ return !m_SetOfInstance.empty();}

	//! Return first instance handle
	inline StructInstance* firstInstanceHandle() const
	{ return *(m_SetOfInstance.begin());}

	//! Return the list of instance of this reference
	inline std::vector<StructInstance*> listOfStructInstances() const
	{ return m_SetOfInstance.toList();}

	//! Return the Set of occurence of this reference
	std::set<StructOccurence*> setOfStructOccurence() const;

	//! Return the list of occurence of this reference
	inline std::vector<StructOccurence*> listOfStructOccurence() const
	{return setOfStructOccurence().toList();}

	//! Return true if this reference has a representation
	inline bool hasRepresentation() const
	{return NULL != m_pRepresentation;}

	//! Return the representation of this reference
	/*! The representation must exists*/
	Rep* representationHandle() const;

	//! Return the name
	inline std::string name() const
	{return m_Name;}

	//! Get number of faces
	inline unsigned int numberOfFaces() const
	{
		assert(NULL != m_pRepresentation);
		Rep3D* pRep= dynamic_cast<Rep3D*>(m_pRepresentation);
		if (NULL != pRep) return pRep->faceCount();
		else return 0;
	}

	//! Get number of vertex
	inline unsigned int numberOfVertex() const
	{
		assert(NULL != m_pRepresentation);
		Rep3D* pRep= dynamic_cast<Rep3D*>(m_pRepresentation);
		if (NULL != pRep) return pRep->vertexCount();
		else return 0;
	}

	//! Get number of materials
	//inline unsigned int numberOfMaterials() const
	//{
	//	assert(NULL != m_pRepresentation);
	//	Rep3D* pRep= dynamic_cast<Rep3D*>(m_pRepresentation);
	//	if (NULL != pRep) return pRep->materialCount();
	//	else return 0;
	//}

	//! Return the number of body
	inline unsigned int numberOfBody() const
	{
		if(NULL != m_pRepresentation)
		{
			Rep3D* pRep= dynamic_cast<Rep3D*>(m_pRepresentation);
			if (NULL != pRep) return pRep->numberOfBody();
			else return 0;
		}
		else return 0;
	}

	//! Get materials List
	//inline std::set<Material*> materialSet() const
	//{
	//	assert(NULL != m_pRepresentation);
	//	Rep3D* pRep= dynamic_cast<Rep3D*>(m_pRepresentation);
	//	if (NULL != pRep) return pRep->materialSet();
	//	else return std::set<Material*>();
	//}

	//! Return true if the reference contains User attributes
	inline bool containsAttributes() const
	{ return ((NULL != m_pAttributes) && !m_pAttributes->isEmpty());}

	//! Return handle to the reference attributes
	inline Attributes* attributesHandle() const
	{return m_pAttributes;}

	//! Return the name of the representation
	std::string representationName() const;

	//! Return true if the representation is loaded
	bool representationIsLoaded() const;

	//! Return the representation file name
	std::string representationFileName() const;

	//! Return true if the representation is empty or if there is no representation
	bool representationIsEmpty() const;

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! An Instance of this reference have been created
	inline void structInstanceCreated(StructInstance* pInstance)
	{
		assert(m_SetOfInstance.find(pInstance) == m_SetOfInstance.end() );
		m_SetOfInstance.insert(pInstance);
	}

	//! An Instance of this reference have been deleted
	inline void structInstanceDeleted(StructInstance* pInstance)
	{m_SetOfInstance.erase(pInstance);}

	//! Set the reference name
	inline void setName(const std::string& name)
	{m_Name= name;}

	//! Set the reference representation
	void setRepresentation(const Rep3D& rep);

	//! Set the reference attributes
	inline void setAttributes(const Attributes& attr)
	{
		delete m_pAttributes;
		m_pAttributes= new Attributes(attr);
	}

	//! Set the representation name
	void setRepresentationName(const std::string& representationName);

	//! Load the representation
	/*! The representation must exists*/
	bool loadRepresentation();

	//! Unload the representation
	/*! The representation must exists*/
	bool unloadRepresentation();

	//! Add the given occurence as a child
	/*! Return true on success*/
	bool addChild(StructOccurence* pOccurence);


//@}

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! The Set of reference's instances
	std::set<StructInstance*> m_SetOfInstance;

	//! The representation of reference
	Rep* m_pRepresentation;

	//! The Reference Name
	std::string m_Name;

	//! The Reference attributes
	Attributes* m_pAttributes;
	
};

#endif /* STRUCTREFERENCE_H_ */
