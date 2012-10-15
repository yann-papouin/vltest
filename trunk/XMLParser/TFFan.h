#pragma once

#include <vector>
using namespace std;

class XMLDLL_EXPORT TFFan
{
public:
	TFFan(void);
	TFFan(vector<int> &p);
	~TFFan(void);
	vector<int> GetFans() const;
private:
	vector<int> vec_fan;

};
