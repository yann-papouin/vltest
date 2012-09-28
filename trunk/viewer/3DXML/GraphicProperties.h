///////////////////////////////////////////////////////////
//  GraphicProperties.h
//  Implementation of the Class GraphicProperties
//  Created on:      28-¾ÅÔÂ-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_653BD427_5B97_4b10_A4A9_C653C6F8FB38__INCLUDED_)
#define EA_653BD427_5B97_4b10_A4A9_C653C6F8FB38__INCLUDED_

#include "GeneralAttributes.h"
#include "LineAttributes.h"
#include "PointAttributes.h"
#include "SurfaceAttributes.h"

class GraphicProperties
{

public:
	GraphicProperties();
	virtual ~GraphicProperties();
	SurfaceAttributes *m_SurfaceAttributes;
	GeneralAttributes *m_GeneralAttributes;
	LineAttributes *m_LineAttributes;
	PointAttributes *m_PointAttributes;

private:
	GeneralAttributes GeneralAttributes;
	LineAttributes LineAttributes;
	PointAttributes PointAttributes;
	SurfaceAttributes SurfaceAttributes;

};
#endif // !defined(EA_653BD427_5B97_4b10_A4A9_C653C6F8FB38__INCLUDED_)
