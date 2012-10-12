// XMLParser.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "XMLParser.h"
#include "cUnpackFile.h"
#include "Point.h"
#include "Triangle.h"
#include "tinyxml.h"
#include "split.h"
#include "Poly.h"
#include "TFRep.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//
void MyDeleteFile(CString& sPath) 
{ 
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(sPath + _T("*.*"),&fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{           
		while (::FindNextFile(hFind,&fd))
		{
			//判断是否为目录
			if (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				CString name;
				name = fd.cFileName;
				//判断是否为.和..
				if ((name != ".") && (name != ".."))
				{
					//如果是真正的目录，进行递归
					MyDeleteFile(sPath + fd.cFileName + _T("\\"));
				}
			}
			else
				DeleteFile(sPath + fd.cFileName);
		}
		::FindClose(hFind);
	}
	RemoveDirectory(sPath);
}

CXMLParser::CXMLParser(const std::string& fileStr)
{
#pragma region Create temp folder
	CString szFolder;
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc;
	TCHAR szTemp[MAX_PATH];
	if(SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL,
			CSIDL_APPDATA,&pidl)))
		{
			SHGetPathFromIDList(pidl,szTemp);
			szFolder =szTemp;
			pShellMalloc->Free(pidl);
		}
		pShellMalloc->Release();
	}

	szFolder.TrimRight(_T("\\"));
	szFolder += _T("\\TF3DXML");
	MyDeleteFile(szFolder);
	CFileStatus fstatus;
	if (!CFile::GetStatus(szFolder,fstatus))
	{
		::CreateDirectory(szFolder,NULL);
	}
#pragma endregion Create temp folder

	const size_t strsize=(szFolder.GetLength()+1)*2; // 宽字符的长度;
	char * qstr= new char[strsize]; //分配空间;
	size_t sz=0;
	wcstombs_s(&sz,qstr,strsize,szFolder,_TRUNCATE);

	const size_t filestrsize=(fileStr.size()+1)*2; // 宽字符的长度;
	char * filestr= new char[filestrsize]; //分配空间;
	sz=0;
	wcstombs_s(&sz,filestr,filestrsize,CString(fileStr.c_str()),_TRUNCATE);

	cUnpackFile unpackTool;
	unpackTool.CreateDirFromZip(qstr, filestr);
	delete []qstr;
	delete []filestr;

	CFileFind finder;
	LPCTSTR pstr = szFolder;
	CString strWildcard(pstr);
	strWildcard += _T("\\*.*");
	CString m_ext_now;
	m_ext_now.Format(_T("%s"),_T("3DRep"));
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		CString name = finder.GetFileName();
		CString extend = name.Right(name.GetLength() - name.ReverseFind('.') - 1);
		if(!finder.IsDots())
		{
			if (extend == m_ext_now)//m_ext_now为你要查找的文件扩展名
			{
				CString str=finder.GetFilePath();
				// 先得到要转换为字符的长度
				const size_t strsize=(str.GetLength()+1)*2; // 宽字符的长度;
				char * pstr= new char[strsize]; //分配空间;
				size_t sz=0;
				wcstombs_s(&sz,pstr,strsize,str,_TRUNCATE);

				TiXmlDocument *myDocument = new TiXmlDocument(pstr);
				myDocument->LoadFile();
				TiXmlElement *rootElement=myDocument->RootElement();
				if(rootElement->FirstChildElement()->FirstChildElement()!=NULL && strcmp(rootElement->FirstChildElement()->FirstChildElement()->Value(),"Rep")==0)
				{
					rootElement=rootElement->FirstChildElement()->FirstChildElement();
					if(strcmp(rootElement->FirstChildElement()->Value(),"Rep")!=0)
					{
						while(rootElement!=NULL)
						{
							m_pTempGeometry=new TFRep();
							TraverseGetInformation(rootElement);
							rootElement=rootElement->NextSiblingElement();
						}
					}
					else
					{
						m_pTempGeometry=new TFRep(3);
						while (strcmp(rootElement->FirstChildElement()->Value(),"Rep")==0)
						{
							rootElement=rootElement->FirstChildElement();
						}
						while(rootElement!=NULL)
						{
							m_pTempGeometry->SetRepType(3);
							m_pTempChildGeometry=new TFRep();
							GetIdRepInformation(rootElement);
							m_pTempGeometry->AddChildRep(m_pTempChildGeometry);
							rootElement=rootElement->NextSiblingElement();
						}
					}
					m_geometryList.push_back(m_pTempGeometry);
				}
				delete []pstr;
				delete myDocument;
			}
		}
	}

	finder.Close();

	MyDeleteFile(szFolder);
}

CXMLParser::~CXMLParser()
{
	vector<TFRep*>::iterator fileIter;
	for (fileIter = m_geometryList.begin();
		fileIter != m_geometryList.end(); ++fileIter)
	{
		delete *fileIter;
		*fileIter = NULL;
	}
}

void CXMLParser::TraverseGetInformation(TiXmlElement* root)
{
	char *pointStr=(char *)root->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->FirstChildElement()->GetText();
	AddPoint(pointStr);
	char *normalStr=(char *)root->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->FirstChildElement()->NextSiblingElement()->GetText();
	AddNormals(normalStr);
	TiXmlElement* triangleElement=root->FirstChildElement()->NextSiblingElement()->FirstChildElement();
	char *m=(char *)triangleElement->Value();
	while(triangleElement!=NULL)
	{
		char *triangleStr=(char *)triangleElement->FirstAttribute()->Value();
		AddTriangle(triangleStr);
		triangleElement=triangleElement->NextSiblingElement();
	}
	TiXmlElement* polyElement=root->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->FirstChildElement();
	while(polyElement!=NULL)
	{
		char *polyStr=(char *)polyElement->FirstAttribute()->Value();
		AddPoly(polyStr);
		polyElement=polyElement->NextSiblingElement();
	}
}

void CXMLParser::GetIdRepInformation(TiXmlElement *root)
{
#pragma region VertexBuffer

	if(strcmp(root->FirstChildElement()->Value(),"VertexBuffer")==0)
	{
		char *pointStr=(char *)root->FirstChildElement()->FirstChildElement()->GetText();
		AddPoint(pointStr);
		char *normalStr=(char *)root->FirstChildElement()->FirstChildElement()->NextSiblingElement()->GetText();
		AddNormals(normalStr);
		char *triangleStr=(char *)root->FirstChildElement()->NextSiblingElement()->FirstChildElement()->NextSiblingElement()->FirstAttribute()->Next()->Value();
		AddIdTriangle(triangleStr);
	}
#pragma endregion VertexBuffer
#pragma region SurfaceAttributes

	if(strcmp(root->FirstChildElement()->Value(),"SurfaceAttributes")==0)
	{
		if(strcmp(root->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->Value(),"VertexBuffer")==0)
		{
			char *pointStr=(char *)root->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->FirstChildElement()->GetText();
			AddPoint(pointStr);
			char *normalStr=(char *)root->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->FirstChildElement()->NextSiblingElement()->GetText();
			AddNormals(normalStr);
		}
		TiXmlElement* triangleElement=root->FirstChildElement()->NextSiblingElement()->FirstChildElement();

		//此部分用于判断有些特殊的triangles没有id但是也不用","分开每个三角形
		string s(triangleElement->FirstAttribute()->Value());
		bool flag=true;
		if(s.find(",")==s.npos)
		{
			flag=false;
		}

		if(strcmp(triangleElement->FirstAttribute()->Name(),"id")==0)
		{
			while(triangleElement!=NULL)
			{
				char *triangleStr=(char *)triangleElement->FirstAttribute()->Next()->Value();
				AddIdTriangle(triangleStr);
				triangleElement=triangleElement->NextSiblingElement();
			}
		}
		else
		{
			while(triangleElement!=NULL)
			{
				char *triangleStr=(char *)triangleElement->FirstAttribute()->Value();
				if(flag==true)
				{
					AddTriangle(triangleStr);
				}
				else
				{
					AddIdTriangle(triangleStr);
				}
				triangleElement=triangleElement->NextSiblingElement();
			}
		}
	}
#pragma endregion SurfaceAttributes
#pragma region Edges
	if(strcmp(root->FirstChildElement()->Value(),"Edges")==0)
	{
		TiXmlElement* polyElement=root->FirstChildElement()->FirstChildElement()->NextSiblingElement();
		while(polyElement!=NULL)
		{
			char *polyStr=(char *)polyElement->FirstAttribute()->Value();
			AddPoly(polyStr);
			polyElement=polyElement->NextSiblingElement();
		}
	}
#pragma endregion Edges
}

void CXMLParser::AddPoint(const string &str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern, &temp);
	string pat=" ";
	for(int i=0;i<temp.size();i++)
	{
		vector<string> result;
		split(temp[i],pat, &result);
		double x,y,z;
		if(result[0]=="")
		{
			x=atof(result[1].c_str());
			y=atof(result[2].c_str());
			z=atof(result[3].c_str());
		}
		else
		{
			x=atof(result[0].c_str());
			y=atof(result[1].c_str());
			z=atof(result[2].c_str());
		}
		TFPoint *p=new TFPoint(x,y,z);
		if(m_pTempGeometry->GetRepType()==1)
		{
			m_pTempGeometry->AddPoint(p);
		}
		else
		{
			m_pTempChildGeometry->AddPoint(p);
		}
	}
}

void CXMLParser::AddNormals(const string &str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern, &temp);
	string pat=" ";
	for(int i=0;i<temp.size();i++)
	{
		vector<string> result;
		split(temp[i],pat, &result);
		double x,y,z;
		if(result[0]=="")
		{
			x=atof(result[1].c_str());
			y=atof(result[2].c_str());
			z=atof(result[3].c_str());
		}
		else
		{
			x=atof(result[0].c_str());
			y=atof(result[1].c_str());
			z=atof(result[2].c_str());
		}
		TFNormals *p=new TFNormals(x,y,z);
		if(m_pTempGeometry->GetRepType()==1)
		{
			m_pTempGeometry->AddNormals(p);
		}
		else
		{
			m_pTempChildGeometry->AddNormals(p);
		}
	}
}

void CXMLParser::AddTriangle(const string& str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern,&temp);
	string pat=" ";
	for(int i=0;i<temp.size();i++)
	{
		vector<string> result;
		split(temp[i],pat,&result);
		int p1,p2,p3;
		if(result[0]=="")
		{
			p1=atoi(result[1].c_str());
			p2=atoi(result[2].c_str());
			p3=atoi(result[3].c_str());
		}
		else
		{
			p1=atoi(result[0].c_str());
			p2=atoi(result[1].c_str());
			p3=atoi(result[2].c_str());
		}
		TFTriangle *p=new TFTriangle(p1,p2,p3);
		m_pTempGeometry->AddTriangle(p);
	}
}

void CXMLParser::AddIdTriangle(const string &str)
{
	vector<string> temp;
	string pattern=" ";
	split(str,pattern,&temp);
	int x,y,z;
	for(int i=0;i<temp.size();i++)
	{
		if(i%3==2)
		{
			z=atoi(temp[i].c_str());
			TFTriangle *p=new TFTriangle(x,y,z);
			m_pTempChildGeometry->AddTriangle(p);
		}
		else if(i%3==0)
		{
			x=atoi(temp[i].c_str());
		}
		else
		{
			y=atoi(temp[i].c_str());
		}
	}
}

void CXMLParser::AddPoly(const string& str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern,&temp);
	string pat=" ";
	vector<TFPoint*> v_pt;
	for(int i=0;i<temp.size();i++)
	{
		vector<string> result;
		split(temp[i],pat,&result);
		double x,y,z;
		if(result[0]=="")
		{
			x=atof(result[1].c_str());
			y=atof(result[2].c_str());
			z=atof(result[3].c_str());
		}
		else
		{
			x=atof(result[0].c_str());
			y=atof(result[1].c_str());
			z=atof(result[2].c_str());
		}
		TFPoint *pt=new TFPoint(x,y,z);
		v_pt.push_back(pt);
	}
	TFPoly* p=new TFPoly(v_pt);
	if(m_pTempGeometry->GetRepType()==1)
	{
		m_pTempGeometry->AddPolyline(p);
	}
	else
	{
		m_pTempChildGeometry->AddPolyline(p);
	}
}

