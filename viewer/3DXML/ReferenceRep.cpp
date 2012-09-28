///////////////////////////////////////////////////////////
//  ReferenceRep.cpp
//  Implementation of the Class ReferenceRep
//  Created on:      28-¾ÅÔÂ-2012 11:36:38
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "ReferenceRep.h"


ReferenceRep::ReferenceRep()
{

}

ReferenceRep::~ReferenceRep()
{

}

RepresentationLink ReferenceRep::GetAssociatedFile(){

	return associatedFile;
}
RepresentationFormat ReferenceRep::GetFormat(){

	return format;
}
unsigned int ReferenceRep::GetId(){

	return id;
}
string ReferenceRep::GetName(){

	return name;
}
float ReferenceRep::GetVersion(){

	return version;
}
void ReferenceRep::SetAssociatedFile(RepresentationLink newVal){

	associatedFile = newVal;
}
void ReferenceRep::SetFormat(RepresentationFormat newVal){

	format = newVal;
}
void ReferenceRep::SetId(unsigned int newVal){

	id = newVal;
}
void ReferenceRep::SetName(string newVal){

	name = newVal;
}
void ReferenceRep::SetVersion(float newVal){

	version = newVal;
}