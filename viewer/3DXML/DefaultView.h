///////////////////////////////////////////////////////////
//  DefaultView.h
//  Implementation of the Class DefaultView
//  Created on:      28-¾ÅÔÂ-2012 11:36:35
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_F58EDAD2_8DF6_4242_80B1_3A402354ACB9__INCLUDED_)
#define EA_F58EDAD2_8DF6_4242_80B1_3A402354ACB9__INCLUDED_

#include "DefaultViewProperty.h"
#include "Viewpoint.h"

class DefaultView
{

public:
	DefaultView();
	virtual ~DefaultView();
	DefaultViewProperty *m_DefaultViewProperty;
	Viewpoint *m_Viewpoint;

private:
	DefaultViewProperty DefaultViewProperty;
	Viewpoint Viewpoint;

};
#endif // !defined(EA_F58EDAD2_8DF6_4242_80B1_3A402354ACB9__INCLUDED_)
