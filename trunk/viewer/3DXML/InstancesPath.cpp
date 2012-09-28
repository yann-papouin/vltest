///////////////////////////////////////////////////////////
//  InstancesPath.cpp
//  Implementation of the Class InstancesPath
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "InstancesPath.h"


InstancesPath::InstancesPath()
{

}

InstancesPath::~InstancesPath()
{

}

archiveURI InstancesPath::Getid(){

	return id;
}
void InstancesPath::Setid(archiveURI newVal){

	id = newVal;
}