#pragma once

#include "TFRep.h"
#include <vector>
#include <string>
using namespace std;

class XMLDLL_EXPORT TF3DRepFile
{
public:
	TF3DRepFile(void);

	~TF3DRepFile(void);

	void AddRep(TFRep *p);

	void SetFileName(string &name);

	vector<TFRep*> GetRepList() const
	{
		return this->m_RepList;
	}

	string GetFileName() const
	{
		return this->m_fileName;
	}

private:
	vector<TFRep*>				m_RepList;
	string						m_fileName;
};
