#pragma once

#include "Point.h"
#include <vector>
using namespace std;

class XMLDLL_EXPORT TFPoly
{
public:
	TFPoly(void);
	~TFPoly(void);
	TFPoly(vector<TFPoint*> &vec);
	vector<TFPoint*> GetPolyPoint();
private:
	vector<TFPoint*> v;
};
