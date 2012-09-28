///////////////////////////////////////////////////////////
//  GeneralAttributes.cpp
//  Implementation of the Class GeneralAttributes
//  Created on:      28-¾ÅÔÂ-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "GeneralAttributes.h"


GeneralAttributes::GeneralAttributes()
{

}

GeneralAttributes::~GeneralAttributes()
{

}

bool GeneralAttributes::IsSelectable(){

	return selectable;
}
bool GeneralAttributes::IsVisible(){

	return visible;
}
void GeneralAttributes::SetSelectable(bool newVal){

	selectable = newVal;
}
void GeneralAttributes::SetVisible(bool newVal){

	visible = newVal;
}