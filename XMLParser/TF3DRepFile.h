#pragma once

#include "TFRep.h"
#include <vector>
using namespace std;

class XMLDLL_EXPORT TF3DRepFile
{
public:
	TF3DRepFile(void);

	~TF3DRepFile(void);

	void AddRep(TFRep *p);

	vector<TFRep*> GetRepList() const
	{
		return m_RepList;
	}

private:
	vector<TFRep*>				m_RepList;
};
