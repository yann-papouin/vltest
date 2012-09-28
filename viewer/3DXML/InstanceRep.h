///////////////////////////////////////////////////////////
//  InstanceRep.h
//  Implementation of the Class InstanceRep
//  Created on:      28-¾ÅÔÂ-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_5596B5F8_1611_4bcb_83A1_22376B35EAA4__INCLUDED_)
#define EA_5596B5F8_1611_4bcb_83A1_22376B35EAA4__INCLUDED_

#include "InstanceRepExtension.h"
#include "Instance.h"

/**
 * <InstanceRep id="4" xsi:type="InstanceRepType" name="fitting_2_UVRInst">
 *      <PLM_ExternalID>fitting_2_UVRInst</PLM_ExternalID>
 *      <V_description>Representation Instance</V_description>
 *      <IsAggregatedBy>3</IsAggregatedBy>
 *      <IsInstanceOf>5</IsInstanceOf>
 *    </InstanceRep>
 */
class InstanceRep : public Instance
{

public:
	InstanceRep();
	virtual ~InstanceRep();

private:
	InstanceRepExtension Extension;

};
#endif // !defined(EA_5596B5F8_1611_4bcb_83A1_22376B35EAA4__INCLUDED_)
