///////////////////////////////////////////////////////////
//  Entity.cpp
//  Implementation of the Class Entity
//  Created on:      28-¾ÅÔÂ-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "Entity.h"


Entity::Entity()
{

}

Entity::~Entity()
{

}

unsigned int Entity::GetId(){

	return id;
}
string Entity::GetName(){

	return name;
}
void Entity::SetId(unsigned int newVal){

	id = newVal;
}
void Entity::SetName(string newVal){

	name = newVal;
}