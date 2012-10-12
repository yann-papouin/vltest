#include "StdAfx.h"
#include "Point.h"

TFPoint::TFPoint(void)
{
}

TFPoint::~TFPoint(void)
{
}

TFPoint::TFPoint(double x,double y,double z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}

void TFPoint::SetX(double x)
{
	this->x=x;
}

void TFPoint::SetY(double y)
{
	this->y=y;
}

void TFPoint::SetZ(double z)
{
	this->z=z;
}

double TFPoint::GetX()
{
	return x;
}

double TFPoint::GetY()
{
	return y;
}

double TFPoint::GetZ()
{
	return z;
}