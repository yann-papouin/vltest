///////////////////////////////////////////////////////////
//  InstancesPath.h
//  Implementation of the Class InstancesPath
//  Created on:      28-¾ÅÔÂ-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_EB49595F_8DDE_4818_B714_D867BB8DE5AC__INCLUDED_)
#define EA_EB49595F_8DDE_4818_B714_D867BB8DE5AC__INCLUDED_

class InstancesPath
{

public:
	InstancesPath();
	virtual ~InstancesPath();

	archiveURI Getid();
	void Setid(archiveURI newVal);

private:
	archiveURI id;

};
#endif // !defined(EA_EB49595F_8DDE_4818_B714_D867BB8DE5AC__INCLUDED_)
