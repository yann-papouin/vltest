#ifndef REP_H_
#define REP_H_

#include "../config.h"
#include <string>
//////////////////////////////////////////////////////////////////////
//! \class Rep
/*! \brief Rep : Abstract class for a reference represention*/
//////////////////////////////////////////////////////////////////////
class GVCORE_EXPORT Rep
{
public:
	enum Type
	{
		VBOGEOM= 1
	};
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default constructor
	Rep();

	//! Copy constructor
	Rep(const Rep&);

	//! Assignement operator
	virtual Rep &operator=(const Rep&);

	//! Clone the representation
	virtual Rep* clone() const = 0;

	//! Return a deep copy of the representation
	virtual Rep* deepCopy() const = 0;

	//! Destructor
	virtual ~Rep();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return true if the representation is the last
	inline bool isTheLast() const
	{return 1 == (*m_pNumberOfRepresentation);}

	//! Return true if representations are equals
	inline bool operator==(const Rep& rep)
	{
		return (rep.m_pNumberOfRepresentation == m_pNumberOfRepresentation);
	}

	//! Return the representation file name
	inline std::string fileName() const
	{return (*m_pFileName);}

	//! Return the type of representation
	virtual int type() const =0;

	//! Return the name of the rep
	inline std::string name() const
	{return (*m_pName);}

	//! Return true if the representation is empty
	virtual bool isEmpty() const= 0;

	//! Return true if the representation as been loaded
	inline bool isLoaded() const
	{return *m_pIsLoaded;}

	//! Return the rep file las modified date and time
	//inline QDateTime lastModified() const
	//{return *m_pDateTime;}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Set the representation FileName
	inline void setFileName(const std::string& fileName)
	{(*m_pFileName)= fileName;}

	//! Set the representation Name
	inline void setName(const std::string& name)
	{(*m_pName)= name;}

	//! Load the representation
	virtual bool load()= 0;

	//! UnLoad the representation
	virtual bool unload()= 0;

	//! Replace the representation
	virtual void replace(Rep*)= 0;

	//! Set the last modified date and time
	//inline void setLastModified(const QDateTime& dateTime)
	//{*m_pDateTime= dateTime;}

//@}
//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////
private:
	//! Clear current representation
	void clear();
//////////////////////////////////////////////////////////////////////
// protected members
//////////////////////////////////////////////////////////////////////
protected:

	//! Flag to know if the representation has been loaded
	bool* m_pIsLoaded;

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:

	//! Number of this representation
	int* m_pNumberOfRepresentation;

	//! The File Name of this representation
	std::string* m_pFileName;

	//! The Name of the rep
	std::string* m_pName;

	//! The Date and time of the rep
	//QDateTime* m_pDateTime;

};

#endif /* REP_H_ */
