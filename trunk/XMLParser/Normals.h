#pragma once
#include <vector>
using namespace std;

class XMLDLL_EXPORT TFNormals
{
public:
	TFNormals(void);
	TFNormals(double x,double y,double z);
	~TFNormals(void);
	double GetZ() const;
	double GetY() const;
	double GetX() const;
	void SetZ(double z);
	void SetY(double y);
	void SetX(double x);
private:
	double x;
	double y;
	double z;
};
