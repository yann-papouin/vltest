///////////////////////////////////////////////////////////
//  LineAttributes.cpp
//  Implementation of the Class LineAttributes
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "LineAttributes.h"


LineAttributes::LineAttributes()
{

}

LineAttributes::~LineAttributes()
{

}

LineType LineAttributes::GetLineType(){

	return LineType;
}
float LineAttributes::GetThickness(){

	return thickness;
}
void LineAttributes::SetLineType(LineType newVal){

	LineType = newVal;
}
void LineAttributes::SetThickness(float newVal){

	thickness = newVal;
}