#include "StdAfx.h"
#include "TFInstance3D.h"

TFInstance3D::TFInstance3D(void)
{
}

TFInstance3D::~TFInstance3D(void)
{
}

TFInstance3D::TFInstance3D(string name,int id,int num_IsAggregatedBy,int num_IsInstanceOf)
{
	this->name=name;
	this->id=id;
	this->num_IsAggregatedBy=num_IsAggregatedBy;
	this->num_IsInstanceOf=num_IsInstanceOf;
}

string TFInstance3D::GetName() const
{
	return name;
}

int TFInstance3D::GetId() const
{
	return id;
}

int TFInstance3D::GetAggregatedNum() const
{
	return num_IsAggregatedBy;
}

int TFInstance3D::GetIsInstanceNum() const
{
	return num_IsInstanceOf;
}
