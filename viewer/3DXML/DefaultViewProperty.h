///////////////////////////////////////////////////////////
//  DefaultViewProperty.h
//  Implementation of the Class DefaultViewProperty
//  Created on:      28-¾ÅÔÂ-2012 11:36:35
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_1F3047BE_4B18_49a7_8312_98F757F24E7E__INCLUDED_)
#define EA_1F3047BE_4B18_49a7_8312_98F757F24E7E__INCLUDED_

#include "GraphicProperties.h"
#include "OccurenceProperty.h"

class DefaultViewProperty : public OccurenceProperty
{

public:
	DefaultViewProperty();
	virtual ~DefaultViewProperty();
	GraphicProperties *m_GraphicProperties;

private:
	GraphicProperties GraphicProperties;
	Matrix4x3 RelativePosition;

};
#endif // !defined(EA_1F3047BE_4B18_49a7_8312_98F757F24E7E__INCLUDED_)
