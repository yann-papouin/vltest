#include "stdafx.h"
#include "TFStrips.h"

TFStrips::TFStrips(void)
{
}

TFStrips::~TFStrips(void)
{
}

TFStrips::TFStrips(vector<int> &p)
{
	this->m_stripsIdxs=p;
}

vector<int> TFStrips::GetStripIdxs() const
{
	return this->m_stripsIdxs;
}