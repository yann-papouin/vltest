///////////////////////////////////////////////////////////
//  SurfaceAttributes.cpp
//  Implementation of the Class SurfaceAttributes
//  Created on:      28-¾ÅÔÂ-2012 11:36:38
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "SurfaceAttributes.h"


SurfaceAttributes::SurfaceAttributes()
{

}

SurfaceAttributes::~SurfaceAttributes()
{

}

ColorType SurfaceAttributes::GetColor(){

	return Color;
}
MaterialApplication SurfaceAttributes::GetMaterialApplication(){

	return MaterialApplication;
}
void SurfaceAttributes::SetColor(ColorType newVal){

	Color = newVal;
}
void SurfaceAttributes::SetMaterialApplication(MaterialApption newVal){

	MaterialApplication = newVal;
}