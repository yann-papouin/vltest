#include "StdAfx.h"
#include "TFRep.h"

TFRep::TFRep(void)
{
	m_iRepType=1;
}

TFRep::TFRep(int flag)
{
	this->m_iRepType=flag;
}

TFRep::~TFRep(void)
{
	vector<TFRep*>::iterator repIter;
	for (repIter = m_childRepList.begin();
		repIter != m_childRepList.end(); ++repIter)
	{
		delete *repIter;
		*repIter = NULL;
	}
	m_childRepList.clear();

	vector<TFPoint*>::iterator pointIter;
	for (pointIter = m_pointList.begin();
		pointIter != m_pointList.end(); ++pointIter)
	{
		delete *pointIter;
		*pointIter = NULL;
	}
	m_pointList.clear();

	vector<TFNormals*>::iterator normalIter;
	for (normalIter = m_normalList.begin();
		normalIter != m_normalList.end(); ++normalIter)
	{
		delete *normalIter;
		*normalIter = NULL;
	}
	m_normalList.clear();

	vector<TFTriangle*>::iterator triangleIter;
	for (triangleIter = m_TriangleList.begin();
		triangleIter != m_TriangleList.end(); ++triangleIter)
	{
		delete *triangleIter;
		*triangleIter = NULL;
	}
	m_TriangleList.clear();

	vector<TFPoly*>::iterator polyIter;
	for (polyIter = m_polyList.begin();
		polyIter != m_polyList.end(); ++polyIter)
	{
		delete *polyIter;
		*polyIter = NULL;
	}
	m_polyList.clear();
}

void TFRep::AddChildRep(TFRep *p)
{
	this->m_childRepList.push_back(p);
}

void TFRep::AddPoint(TFPoint *p)
{
	this->m_pointList.push_back(p);
}

void TFRep::AddNormals(TFNormals *p)
{
	this->m_normalList.push_back(p);
}

void TFRep::AddTriangle(TFTriangle *p)
{
	this->m_TriangleList.push_back(p);
}

void TFRep::AddPolyline(TFPoly *p)
{
	this->m_polyList.push_back(p);
}

void TFRep::SetRepType(int f)
{
	this->m_iRepType=f;
}

int TFRep::GetRepType()
{
	return m_iRepType;
}