#pragma once
#include <vector>
using namespace std;

class XMLDLL_EXPORT TFStrips
{
public:
	TFStrips(void);

	~TFStrips(void);

	TFStrips(vector<int> &p);

	vector<int> GetStripIdxs() const;

private:
	vector<int> m_stripsIdxs;
};
