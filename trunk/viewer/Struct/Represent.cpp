#include <assert.h>
#include "struct/Represent.h"
//#include "QtDebug"

// Default constructor
Represent::Represent()
: m_pIsLoaded(new bool(false))
, m_pNumberOfRepresentation(new int(1))
, m_pFileName(new std::string())
, m_pName(new std::string())
//, m_pDateTime(new QDateTime)
{

}

// Copy Constructor
Represent::Represent(const Represent& rep)
: m_pIsLoaded(rep.m_pIsLoaded)
, m_pNumberOfRepresentation(rep.m_pNumberOfRepresentation)
, m_pFileName(rep.m_pFileName)
, m_pName(rep.m_pName)
// , m_pDateTime(rep.m_pDateTime)
{
	++(*m_pNumberOfRepresentation);
}

// Assignement operator
Represent& Represent::operator=(const Represent& rep)
{
	if (this != &rep)
	{
		// Clear this representation
		clear();
		m_pIsLoaded= rep.m_pIsLoaded;
		m_pNumberOfRepresentation= rep.m_pNumberOfRepresentation;
		++(*m_pNumberOfRepresentation);
		m_pFileName= rep.m_pFileName;
		m_pName= rep.m_pName;
		//m_pDateTime= rep.m_pDateTime;
	}

	return *this;
}

// Destructor
Represent::~Represent()
{
	// Clear this representation
	clear();
}


//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////
// Clear current representation
void Represent::clear()
{
	assert(NULL != m_pNumberOfRepresentation);
	if ((--(*m_pNumberOfRepresentation)) == 0)
	{
		delete m_pIsLoaded;
		m_pIsLoaded= NULL;
		delete m_pNumberOfRepresentation;
		m_pNumberOfRepresentation= NULL;
		delete m_pFileName;
		m_pFileName= NULL;
		delete m_pName;
		m_pName= NULL;
		//delete m_pDateTime;
		//m_pDateTime= NULL;
	}
}
