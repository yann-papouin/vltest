///////////////////////////////////////////////////////////
//  Viewpoint.h
//  Implementation of the Class Viewpoint
//  Created on:      28-¾ÅÔÂ-2012 11:36:38
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_97920EEA_F4C0_4bd6_B90A_BBE145FA3C68__INCLUDED_)
#define EA_97920EEA_F4C0_4bd6_B90A_BBE145FA3C68__INCLUDED_

class Viewpoint
{

public:
	Viewpoint();
	virtual ~Viewpoint();

	unsigned int GetId();
	string GetName();
	void SetId(unsigned int newVal);
	void SetName(string newVal);

private:
	unsigned int id;
	string name;

};
#endif // !defined(EA_97920EEA_F4C0_4bd6_B90A_BBE145FA3C68__INCLUDED_)
