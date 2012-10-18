#include "StdAfx.h"
#include "TF3DRepFile.h"

TF3DRepFile::TF3DRepFile(void)
{
}

TF3DRepFile::~TF3DRepFile(void)
{
	vector<TFRep*>::iterator repIter;
	for (repIter = m_RepList.begin();
		repIter != m_RepList.end(); ++repIter)
	{
		delete *repIter;
		*repIter = NULL;
	}
	m_RepList.clear();
}

void TF3DRepFile::AddRep(TFRep *p)
{
	m_RepList.push_back(p);
}
