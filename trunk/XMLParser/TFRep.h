#pragma once

#include "Point.h"
#include "Normals.h"
#include "Triangle.h"
#include "Poly.h"
using namespace std;

class XMLDLL_EXPORT TFRep
{
public:
	TFRep(void);
	TFRep(int flag);
	~TFRep(void);
	void AddChildRep(TFRep *p);

	void AddPoint(TFPoint *p);

	void AddNormals(TFNormals *p);

	void AddTriangle(TFTriangle *p);

	void AddPolyline(TFPoly *p);

	void SetRepType(int f);

	int GetRepType();

	vector<TFRep*> GetChildRepList() const
	{
		return m_childRepList;
	}

	vector<TFPoint*> GetPointList() const
	{
		return m_pointList;
	}

	vector<TFNormals*> GetNormalList() const
	{
		return m_normalList;
	}

	vector<TFTriangle*>	GetTriangleList() const
	{
		return m_TriangleList;
	}

	vector<TFPoly*>	GetPolyList() const
	{
		return m_polyList;
	}

private:
	vector<TFRep*>				m_childRepList;

	vector<TFPoint*>			m_pointList;

	vector<TFNormals*>			m_normalList;

	vector<TFTriangle*>			m_TriangleList;

	vector<TFPoly*>				m_polyList;

	//标记Rep是何种类型的，1代表是无id的，2代表有id的，3代表是父Rep
	int							m_iRepType;	
};
