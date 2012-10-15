#include "StdAfx.h"
#include "TFFan.h"

TFFan::TFFan(void)
{
}

TFFan::TFFan(vector<int> &p)
{
	this->vec_fan=p;
}

TFFan::~TFFan(void)
{
}

vector<int> TFFan::GetFans() const
{
	return this->vec_fan;
}
