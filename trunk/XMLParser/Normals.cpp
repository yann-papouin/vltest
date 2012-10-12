#include "StdAfx.h"
#include "Normals.h"

TFNormals::TFNormals(void)
{
}

TFNormals::~TFNormals(void)
{
}

TFNormals::TFNormals(double x,double y,double z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}

void TFNormals::SetX(double x)
{
	this->x=x;
}

void TFNormals::SetY(double y)
{
	this->y=y;
}

void TFNormals::SetZ(double z)
{
	this->z=z;
}

double TFNormals::GetX() const
{
	return this->x;
}

double TFNormals::GetY() const
{
	return this->y;
}

double TFNormals::GetZ() const
{
	return this->z;
}