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
class TF3DRepFile;

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
	vector<TF3DRepFile*> GetFileList() const
	{
		return m_fileList;
	}

private: 
	//vector<TFRep*>				m_geometryList;
	TFRep*							m_pTempGeometry;
	//TFRep*						m_pTempChildGeometry;
	vector<TF3DRepFile*>			m_fileList;
	TF3DRepFile*					m_pTempFile;

protected:
	void TraverseGetInformation(TiXmlElement *root);	//遍历结点树获得信息
	void TraverseRep(TiXmlElement *root);
	void AddPoint(const string &str);	//将找到的点添加入向量
	void AddNormals(const string &str);
	void AddTriangle(const string &str);
	void AddIdTriangle(const string &str);
	void AddPoly(const string &str);
	void AddStrips(const string &str);
	void AddFans(const string &str);
	bool IsTriangleStrWithSplit(char *str);
};
