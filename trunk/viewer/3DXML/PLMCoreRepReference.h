///////////////////////////////////////////////////////////
//  PLMCoreRepReference.h
//  Implementation of the Class PLMCoreRepReference
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_DE45A50D_ADED_4907_BC8D_DCB67FC39C32__INCLUDED_)
#define EA_DE45A50D_ADED_4907_BC8D_DCB67FC39C32__INCLUDED_

#include "RepresentationLink.h"
#include "Reference.h"

class PLMCoreRepReference : public Reference
{

public:
	PLMCoreRepReference();
	virtual ~PLMCoreRepReference();

	float GetVersion();
	void SetVersion(float newVal);

private:
	RepresentationLink associatedFile;
	RepresentationFormat format;
	float version;

};
#endif // !defined(EA_DE45A50D_ADED_4907_BC8D_DCB67FC39C32__INCLUDED_)
