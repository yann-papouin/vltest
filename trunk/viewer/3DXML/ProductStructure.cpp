///////////////////////////////////////////////////////////
//  ProductStructure.cpp
//  Implementation of the Class ProductStructure
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#include "ProductStructure.h"


ProductStructure::ProductStructure()
{

}

ProductStructure::~ProductStructure()
{

}

unsigned int ProductStructure::GetRoot(){

	return root;
}
void ProductStructure::SetRoot(unsigned int newVal){

	root = newVal;
}