#pragma once
#include "Point.h"

class XMLDLL_EXPORT Triangle
{
public:
	Triangle(void);

	~Triangle(void);

	Triangle(int n1,int n2,int n3);

	void GetPointIdx(int& idx1,int& idx2,int& idx3);	

private:
	int			m_pointIdx1;
	int			m_pointIdx2;
	int			m_pointIdx3;
};
