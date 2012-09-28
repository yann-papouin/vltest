///////////////////////////////////////////////////////////
//  Viewpoint.cpp
//  Implementation of the Class Viewpoint
//  Created on:      28-¾ÅÔÂ-2012 11:36:38
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "Viewpoint.h"


Viewpoint::Viewpoint()
{

}

Viewpoint::~Viewpoint()
{

}

unsigned int Viewpoint::GetId(){

	return id;
}
string Viewpoint::GetName(){

	return name;
}
void Viewpoint::SetId(unsigned int newVal){

	id = newVal;
}
void Viewpoint::SetName(string newVal){

	name = newVal;
}