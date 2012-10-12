#include "StdAfx.h"
#include "Poly.h"

TFPoly::TFPoly(void)
{
}

TFPoly::~TFPoly(void)
{
	vector<TFPoint*>::iterator iter;
	for (iter = v.begin();
		iter != v.end(); ++iter)
	{
		delete *iter;
		*iter = NULL;
	}
	v.clear();
}

TFPoly::TFPoly(vector<TFPoint*> &vec)
{
	v=vec;
}

vector<TFPoint*> TFPoly::GetPolyPoint()
{
	return this->v;
}

