///////////////////////////////////////////////////////////
//  PointAttributes.cpp
//  Implementation of the Class PointAttributes
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "PointAttributes.h"


PointAttributes::PointAttributes()
{

}

PointAttributes::~PointAttributes()
{

}

PointSymbol PointAttributes::GetSymbolType(){

	return symbolType;
}
void PointAttributes::SetSymbolType(PointSymbol newVal){

	symbolType = newVal;
}