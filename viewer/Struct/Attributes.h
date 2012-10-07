//! \file attributes.h interface for the Attributes class.

#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <assert.h>

#include <string>
#include <list>
#include <hash_map>

//////////////////////////////////////////////////////////////////////
//! \class Attributes
/*! \brief Attributes : User attributes of instance and reference */
//////////////////////////////////////////////////////////////////////
class Attributes
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
