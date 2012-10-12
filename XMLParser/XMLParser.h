// XMLParser.h : main header file for the XMLParser DLL
//

#pragma once


#ifdef XMLPARSER_EXPORTS
#define XMLDLL_EXPORT __declspec(dllexport)
#else
#define XMLDLL_EXPORT __declspec(dllimport)
#endif

class TiXmlElement;
class TFRep;

#include <vector>
#include <string>

using namespace std;

class XMLDLL_EXPORT CXMLParser
{
public:
	CXMLParser(const std::string& fileStr);

	~CXMLParser();
// Overrides
public:
	vector<TFRep*> GetGeometryList() const
	{
		return m_geometryList;
	}

private: 
	vector<TFRep*>				m_geometryList;
	TFRep*						m_pTempGeometry;
	TFRep*						m_pTempChildGeometry;

protected:
	void TraverseGetInformation(TiXmlElement *root);	//��������������Ϣ
	void GetIdRepInformation(TiXmlElement *root);
	void AddPoint(const string &str);	//���ҵ��ĵ����������
	void AddNormals(const string &str);
	void AddTriangle(const string &str);
	void AddIdTriangle(const string &str);
	void AddPoly(const string &str);
};
