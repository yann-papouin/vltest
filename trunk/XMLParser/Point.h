#pragma once

class XMLDLL_EXPORT TFPoint
{
public:
	TFPoint(void);
	TFPoint(double x,double y,double z);
	~TFPoint(void);
	double GetZ();
	double GetY();
	double GetX();
	void SetZ(double z);
	void SetY(double y);
	void SetX(double x);
private:
	double x;
	double y;
	double z;
};
