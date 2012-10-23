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
class TFReference3D;
class TFInstance3D;

#include <vector>
#include <string>

using namespace std;

struct ReferenceTreeElement
{
	string value;
	int id;
	string instancename;
	int instanceId;
	ReferenceTreeElement *FirstChildElement;
	ReferenceTreeElement *NextSimblingElement;
};

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
	TFRep*							m_pTempGeometry;
	vector<TF3DRepFile*>			m_fileList;
	TF3DRepFile*					m_pTempFile;
	vector<TFReference3D*>			m_reference3DList;
	vector<TFInstance3D*>			m_instance3DList;
	vector<TFReference3D*>			m_treeNodeList;		//树的结点List，因为m_reference3DList不包含重复结点，这里将节点找全
	ReferenceTreeElement*			m_root;
	vector<TFReference3D*>			m_copy;				//辅助寻找结点的copy

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
	void AddReference3D(TiXmlElement *root);
	void AddInstance3D(TiXmlElement *root);
	void FindAllTreeNode();
	void CopyReference3D(int id);
	void LinkTreeNode(ReferenceTreeElement *root,ReferenceTreeElement *father);
	void ReleaseTreeNode(ReferenceTreeElement *root);
};
