//! \file Reference.h interface for the Reference class.

#ifndef Reference_H_
#define Reference_H_

#include <string>
#include <set>

//#include "3dviewinstance.h"
#include "struct/attributes.h"
#include "struct/Instance.h"

class Represent3D;
class Represent;
class Occurence;

//////////////////////////////////////////////////////////////////////
//! \class Reference
/*! \brief Reference : A scene graph reference node */
//////////////////////////////////////////////////////////////////////
class Reference
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default Constructor
	Reference(const std::string& name= std::string());

	//! Create reference with representation
	Reference(Represent*);

	//! Copy constructor
	Reference(const Reference&);

	//! Overload "=" operator
	Reference& operator=(const Reference&);

	//! Destructor
	virtual ~Reference();
//@}
//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return true if this reference has instance
	inline bool hasInstance() const
	{ return !m_SetOfInstance.empty();}

	//! Return first instance handle
	inline Instance* firstInstanceHandle() const
	{ return *(m_SetOfInstance.begin());}

	//! Return the list of instance of this reference
	std::vector<Instance*> listOfInstances() const;

	//! Return the Set of occurence of this reference
	std::set<Occurence*> setOfOccurence() const;

	//! Return the list of occurence of this reference
	std::vector<Occurence*> listOfOccurence() const;

	//! Return true if this reference has a representation
	bool hasRepresentation() const;

	//! Return the representation of this reference
	/*! The representation must exists*/
	Represent* representationHandle() const;

	//! Return the name
	std::string name() const;

	//! Get number of faces
	unsigned int numberOfFaces() const;

	//! Get number of vertex
	unsigned int numberOfVertex() const;

	//! Get number of materials
	unsigned int numberOfMaterials() const;

	//! Return the number of body
	unsigned int numberOfBody() const;

	//! Get materials List
	//inline std::set<Material*> materialSet() const
	//{
	//	assert(NULL != m_pRepresentation);
	//	Represent3D* pRep= dynamic_cast<Represent3D*>(m_pRepresentation);
	//	if (NULL != pRep) return pRep->materialSet();
	//	else return std::set<Material*>();
	//}

	//! Return true if the reference contains User attributes
	bool containsAttributes() const;

	//! Return handle to the reference attributes
	Attributes* attributesHandle() const;

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
	void InstanceCreated(Instance* pInstance);

	//! An Instance of this reference have been deleted
	void InstanceDeleted(Instance* pInstance);

	//! Set the reference name
	void setName(const std::string& name);

	//! Set the reference representation
	void setRepresentation(const Represent3D& rep);

	//! Set the reference attributes
	void setAttributes(const Attributes& attr);

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
	bool addChild(Occurence* pOccurence);


//@}

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! The Set of reference's instances
	std::set<Instance*> m_SetOfInstance;

	//! The representation of reference
	Represent* m_pRepresentation;

	//! The Reference Name
	std::string m_Name;

	//! The Reference attributes
	Attributes* m_pAttributes;
	
};

#endif /* Reference_H_ */
