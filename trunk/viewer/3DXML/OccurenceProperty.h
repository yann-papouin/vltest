///////////////////////////////////////////////////////////
//  OccurenceProperty.h
//  Implementation of the Class OccurenceProperty
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_C9E6C9B8_EFAB_46bd_83D2_5D1CE24517FE__INCLUDED_)
#define EA_C9E6C9B8_EFAB_46bd_83D2_5D1CE24517FE__INCLUDED_

#include "InstancesPath.h"

class OccurenceProperty
{

public:
	OccurenceProperty();
	virtual ~OccurenceProperty();
	InstancesPath *m_InstancesPath;

private:
	InstancesPath OccurenceId;

};
#endif // !defined(EA_C9E6C9B8_EFAB_46bd_83D2_5D1CE24517FE__INCLUDED_)
