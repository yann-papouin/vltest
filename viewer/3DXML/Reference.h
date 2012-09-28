///////////////////////////////////////////////////////////
//  Reference.h
//  Implementation of the Class Reference
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_B09C7148_3AE0_44ee_9198_9BFB12633939__INCLUDED_)
#define EA_B09C7148_3AE0_44ee_9198_9BFB12633939__INCLUDED_

#include "Entity.h"

class Reference : public Entity
{

public:
	Reference();
	virtual ~Reference();

private:
	string V_maturity;
	string V_version;

};
#endif // !defined(EA_B09C7148_3AE0_44ee_9198_9BFB12633939__INCLUDED_)
