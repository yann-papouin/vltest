///////////////////////////////////////////////////////////
//  Entity.h
//  Implementation of the Class Entity
//  Created on:      28-¾ÅÔÂ-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_B110A401_B18F_4624_9F02_750AE801406C__INCLUDED_)
#define EA_B110A401_B18F_4624_9F02_750AE801406C__INCLUDED_

class Entity
{

public:
	Entity();
	virtual ~Entity();

	unsigned int GetId();
	string GetName();
	void SetId(unsigned int newVal);
	void SetName(string newVal);

private:
	string C_created;
	string C_modified;
	unsigned int id;
	string name;
	string PLM_ExternalID;
	string V_description;
	string V_Name;

};
#endif // !defined(EA_B110A401_B18F_4624_9F02_750AE801406C__INCLUDED_)
