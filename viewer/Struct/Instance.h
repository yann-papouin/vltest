//! \file Instance.h interface for the Instance class.

#ifndef Instance_H_
#define Instance_H_

#include <assert.h>
#include <list>
#include <string>

#include "vlCore/Matrix4.hpp"
#include "vlCore/Vector3.hpp"
//#include "3dviewinstance.h"

#include "struct/attributes.h"

class Reference;
class Occurence;
class Represent;

//////////////////////////////////////////////////////////////////////
//! \class Instance
/*! \brief Instance : A scene graph instance node */
//////////////////////////////////////////////////////////////////////
class Instance
{
public:
	//! Default constructor
	Instance(Reference* pRef= NULL);

	//! Create instance with a rep
	Instance(Represent*);

	//! Copy constructor
	Instance(const Instance&);

	//! Copy constructor
	Instance(Instance*);

	//! Create empty instance
	Instance(const std::string&);

	//! Set the reference of an empty instance
	void setReference(Reference*);

	// Destructor
	virtual ~Instance();
//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return true if this instance have occurence
	inline bool hasOccurence() const
	{ return !m_ListOfOccurences.empty();}

	//! Return the number of occurence
	inline int numberOfOccurence() const
	{return m_ListOfOccurences.size();}

	//! Return first occurence handle
	inline Occurence* firstOccurenceHandle() const
	{ return m_ListOfOccurences.front();}

	//! Return the relative matrix of this instance
	inline vl::dmat4 relativeMatrix() const
	{ return m_RelativeMatrix;}

	//! Return the reference of this instance
	inline Reference* reference() const
	{ return m_pReference;}

	//! Return the list off occurence of this instance
	inline std::vector<Occurence*> listOfOccurences() const
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
	inline void OccurenceCreated(Occurence* pOccurence)
	{
//		assert(!m_ListOfOccurences.contains(pOccurence));
		std::vector<Occurence*>::const_iterator itor;
		for (itor=m_ListOfOccurences.begin();itor!=m_ListOfOccurences.end();itor++)
		{
			if (*itor == pOccurence)
			{
				assert(false);
			}			
		}
		m_ListOfOccurences.push_back(pOccurence);
	}

	inline void OccurenceDeleted(Occurence *pOccurence)
	{
		std::vector<Occurence*>::const_iterator itor;
		for (itor=m_ListOfOccurences.begin();itor!=m_ListOfOccurences.end();itor++)
		{
			if (*itor == pOccurence)
			{
				m_ListOfOccurences.erase(itor);
			}			
		}
	}

	//! Move the instance by specified matrix
	inline Instance* move(const vl::dmat4& matrix)
	{
		m_RelativeMatrix= matrix * m_RelativeMatrix;
		return this;
	}

	//! Translate Instance
	inline Instance* translate(double Tx, double Ty, double Tz)
	{
		m_RelativeMatrix= vl::dmat4::getTranslation(Tx, Ty, Tz) * m_RelativeMatrix;
		return this;
	}

	//! Translate Instance
	inline Instance* translate(const vl::dvec3 & v)
	{
		return translate(v.x(), v.y(), v.z());
	}

	//! Replace the instance Matrix
	inline Instance* setMatrix(const vl::dmat4 &SetMat)
	{
		m_RelativeMatrix= SetMat;
		return this;
	}

	//! Reset the instance Matrix
	inline Instance* resetMatrix()
	{
		m_RelativeMatrix = vl::dmat4::getIdentity();
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
	Reference* m_pReference;

	//! The list of instance's occurences
	std::vector<Occurence*> m_ListOfOccurences;

	//! The relative matrix
	vl::dmat4 m_RelativeMatrix;

	//! The instance Name
	std::string m_Name;

	//! The Reference attributes
	Attributes* m_pAttributes;
};

#endif /* Instance_H_ */
