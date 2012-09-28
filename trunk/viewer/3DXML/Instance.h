///////////////////////////////////////////////////////////
//  Instance.h
//  Implementation of the Class Instance
//  Created on:      28-¾ÅÔÂ-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_D0A391F8_370B_4ef1_A246_52C53F0E0A4F__INCLUDED_)
#define EA_D0A391F8_370B_4ef1_A246_52C53F0E0A4F__INCLUDED_

#include "Entity.h"

class Instance : public Entity
{

public:
	Instance();
	virtual ~Instance();

private:
	indexLink IsAggregatedBy;
	archiveURI IsInstanceOf;

};
#endif // !defined(EA_D0A391F8_370B_4ef1_A246_52C53F0E0A4F__INCLUDED_)
