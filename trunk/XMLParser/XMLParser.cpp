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
#include "TF3DRepFile.h"
#include "TFReference3D.h"
#include "TFInstance3D.h"
#include <atlconv.h>

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
	HANDLE hFind = ::FindFirstFile(sPath + _T("\\*.*"),&fd);
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
				DeleteFile(sPath + _T("\\") + fd.cFileName);
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

	cUnpackFile unpackTool;
	unpackTool.CreateDirFromZip(qstr,fileStr.c_str());
	delete []qstr;
	//delete []filestr;

	CFileFind finder;
	LPCTSTR pstr = szFolder;
	CString strWildcard(pstr);
	strWildcard += _T("\\*.*");
	CString m_ext_now;
	m_ext_now.Format(_T("%s"),_T("3DRep"));
	CString m_str_3dxml;	//查找装配树的.3dxml文件
	m_str_3dxml.Format(_T("%s"),_T("3dxml"));
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
					m_pTempFile=new TF3DRepFile();
					string filename;
					CT2A xx(name);
					filename = xx;
					m_pTempFile->SetFileName(filename);
					TraverseRep(rootElement->FirstChildElement()->FirstChildElement());
					m_fileList.push_back(m_pTempFile);
				}
				delete myDocument;
				delete []pstr;
			}
			if(extend == m_str_3dxml)
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
				if(strcmp(rootElement->FirstChildElement()->NextSiblingElement()->Value(),"ProductStructure")==0)
				{
					rootElement=rootElement->FirstChildElement()->NextSiblingElement()->FirstChildElement();
					m_root=new ReferenceTreeElement();
					m_root->value=(char *)rootElement->FirstAttribute()->Next()->Next()->Value();
					m_root->id=atoi(rootElement->FirstAttribute()->Next()->Value());
					m_root->instancename="";
					m_root->FirstChildElement=NULL;
					m_root->NextSimblingElement=NULL;
					while(rootElement!=NULL)
					{
						if(strcmp(rootElement->Value(),"Reference3D")==0)
						{
							AddReference3D(rootElement);
						}
						if(strcmp(rootElement->Value(),"Instance3D")==0)
						{
							AddInstance3D(rootElement);
						}
						rootElement=rootElement->NextSiblingElement();
					}
					FindAllTreeNode();
					ReferenceTreeElement *p=m_root;
					LinkTreeNode(p,NULL);
				}
			}
		}
	}

	finder.Close();

	MyDeleteFile(szFolder);
}

CXMLParser::~CXMLParser()
{
	vector<TF3DRepFile*>::iterator fileIter;
	for (fileIter = m_fileList.begin();
		fileIter != m_fileList.end(); ++fileIter)
	{
		delete *fileIter;
		*fileIter = NULL;
	}

	vector<TFInstance3D*>::iterator instanceIter;
	for(instanceIter = m_instance3DList.begin();
	   instanceIter != m_instance3DList.end(); ++instanceIter)
	{
		delete *instanceIter;
		*instanceIter = NULL;
	}

	vector<TFReference3D*>::iterator referenceIter;
	for(referenceIter = m_treeNodeList.begin();
		referenceIter!= m_treeNodeList.end(); ++referenceIter)
	{
		delete *referenceIter;
		*referenceIter = NULL;
	}

	ReleaseTreeNode(m_root);
}

void CXMLParser::TraverseGetInformation(TiXmlElement* root)
{
	TiXmlElement *childroot=root->FirstChildElement();
	while(childroot!=NULL)
	{
		if(strcmp(childroot->Value(),"VertexBuffer")==0)
		{
			char *pointStr=(char *)childroot->FirstChildElement()->GetText();
			AddPoint(pointStr);
			char *normalStr=(char *)childroot->FirstChildElement()->NextSiblingElement()->GetText();
			AddNormals(normalStr);
		}
		else if(strcmp(childroot->Value(),"Faces")==0)
		{
			TiXmlElement *triangleElem=childroot->FirstChildElement();
			while (triangleElem!=NULL)
			{
				if(strcmp(triangleElem->Value(),"Face")==0)
				{
					TiXmlAttribute *triangleAttribute=triangleElem->FirstAttribute();
					while(triangleAttribute!=NULL)
					{
						if(strcmp(triangleAttribute->Name(),"triangles")==0)
						{
							char *triangleStr=(char *)triangleAttribute->Value();
							if(IsTriangleStrWithSplit(triangleStr)==true)
							{
								AddTriangle(triangleStr);
							}
							else
							{
								AddIdTriangle(triangleStr);
							}
						}
						if(strcmp(triangleAttribute->Name(),"strips")==0)
						{
							char *stripStr=(char *)triangleAttribute->Value();
							AddStrips(stripStr);
						}
						if(strcmp(triangleAttribute->Name(),"fans")==0)
						{
							char *fanStr=(char *)triangleAttribute->Value();
							AddFans(fanStr);
						}
						triangleAttribute=triangleAttribute->Next();
					}
				}
				triangleElem=triangleElem->NextSiblingElement();
			}
		}
		else if(strcmp(childroot->Value(),"Edges")==0)
		{
			TiXmlElement *polyElem=childroot->FirstChildElement();
			while(polyElem!=NULL)
			{
				if(strcmp(polyElem->Value(),"Polyline")==0)
				{
					char *polyStr=(char *)polyElem->FirstAttribute()->Value();
					AddPoly(polyStr);
				}
				polyElem=polyElem->NextSiblingElement();
			}
		}
		/*else if (strcmp(childroot->Value(),"PolygonalLOD")==0)
		{
			TiXmlElement *triangleElem=childroot->FirstChildElement()->FirstChildElement();
			while (triangleElem!=NULL)
			{
				if(strcmp(triangleElem->Value(),"Face")==0)
				{
					TiXmlAttribute *triangleAttribute=triangleElem->FirstAttribute();
					while(triangleAttribute!=NULL)
					{
						if(strcmp(triangleAttribute->Name(),"triangles")==0)
						{
							char *triangleStr=(char *)triangleAttribute->Value();
							if(IsTriangleStrWithSplit(triangleStr)==true)
							{
								AddTriangle(triangleStr);
							}
							else
							{
								AddIdTriangle(triangleStr);
							}
						}
						if(strcmp(triangleAttribute->Name(),"strips")==0)
						{
							char *stripStr=(char *)triangleAttribute->Value();
							AddStrips(stripStr);
						}
						if(strcmp(triangleAttribute->Name(),"fans")==0)
						{
							char *fanStr=(char *)triangleAttribute->Value();
							AddFans(fanStr);
						}
						triangleAttribute=triangleAttribute->Next();
					}
				}
				triangleElem=triangleElem->NextSiblingElement();
			}
		}*/
		childroot=childroot->NextSiblingElement();
	}
}

void CXMLParser::AddPoint(const string &str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern, &temp);
	string pat=" ";
	for(unsigned int i=0;i<temp.size();i++)
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
		m_pTempGeometry->AddPoint(p);
	}
}

void CXMLParser::AddNormals(const string &str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern, &temp);
	string pat=" ";
	for(unsigned int i=0;i<temp.size();i++)
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
		m_pTempGeometry->AddNormals(p);
	}
}

void CXMLParser::AddTriangle(const string& str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern,&temp);
	string pat=" ";
	for(unsigned int i=0;i<temp.size();i++)
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
	for(unsigned int i=0;i<temp.size();i++)
	{
		if(i%3==2)
		{
			z=atoi(temp[i].c_str());
			TFTriangle *p=new TFTriangle(x,y,z);
			m_pTempGeometry->AddTriangle(p);
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
	for(unsigned int i=0;i<temp.size();i++)
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
	m_pTempGeometry->AddPolyline(p);
}

void CXMLParser::AddStrips(const string &str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern,&temp);
	string pat=" ";
	for(unsigned int i=0;i<temp.size();i++)
	{
		vector<string> result;
		split(temp[i],pat,&result);
		vector<int> v;
		for(unsigned int j=0;j<result.size();j++)
		{
			int n=atoi(result[j].c_str());
			v.push_back(n);
		}
		TFStrips *p=new TFStrips(v);
		m_pTempGeometry->AddStrips(p);
	}
}

void CXMLParser::AddFans(const string &str)
{
	vector<string> temp;
	string pattern=",";
	split(str,pattern,&temp);
	string pat=" ";
	for(unsigned int i=0;i<temp.size();i++)
	{
		vector<string> result;
		split(temp[i],pat,&result);
		vector<int> v;
		for(unsigned int j=0;j<result.size();j++)
		{
			int n=atoi(result[j].c_str());
			v.push_back(n);
		}
		TFFan *p=new TFFan(v);
		m_pTempGeometry->AddFans(p);
	}
}

bool CXMLParser::IsTriangleStrWithSplit(char *str)
{
	bool flag=false;
	for(int i=0;str[i]!='\0';i++)
	{
		if(str[i]==',')
		{
			flag=true;
			break;
		}
	}
	return flag;
}

void CXMLParser::TraverseRep(TiXmlElement *root)
{
	while (root!=NULL && strcmp(root->Value(),"Rep")==0)
	{
		if(strcmp(root->FirstChildElement()->Value(),"Rep")==0)
		{
			TraverseRep(root->FirstChildElement());
		}
		else
		{
			m_pTempGeometry=new TFRep();
			TraverseGetInformation(root);
			m_pTempFile->AddRep(m_pTempGeometry);
		}
		root=root->NextSiblingElement();
	}
}

void CXMLParser::AddReference3D(TiXmlElement *root)
{
	string name((char *)root->FirstAttribute()->Next()->Next()->Value());
	int id=atoi(root->FirstAttribute()->Next()->Value());
	TFReference3D *p=new TFReference3D(name,id);
	m_reference3DList.push_back(p);
}

void CXMLParser::AddInstance3D(TiXmlElement *root)
{
	string name((char *)root->FirstAttribute()->Next()->Next()->Value());
	int id=atoi(root->FirstAttribute()->Next()->Value());
	int num_Aggregated=atoi(root->FirstChildElement()->GetText());
	int num_Instance=atoi(root->FirstChildElement()->NextSiblingElement()->GetText());
	TFInstance3D *p=new TFInstance3D(name,id,num_Aggregated,num_Instance);
	m_instance3DList.push_back(p);
}

void CXMLParser::FindAllTreeNode()
{
	for(unsigned int i=0;i<m_instance3DList.size();i++)
	{
		for(unsigned int j=0;j<m_reference3DList.size();j++)
		{
			if(m_reference3DList[j]->GetId()==m_instance3DList[i]->GetIsInstanceNum())
			{
				if(m_reference3DList[j]->GetIstanceId()==-1)
				{
					m_reference3DList[j]->SetInstanceName(m_instance3DList[i]->GetName());
					m_reference3DList[j]->SetIstanceId(m_instance3DList[i]->GetId());
					m_reference3DList[j]->SetAggretedId(m_instance3DList[i]->GetAggregatedNum());
					m_treeNodeList.push_back(m_reference3DList[j]);
				}
				else
				{
					TFReference3D *newRefer=new TFReference3D(*m_reference3DList[j]);
					newRefer->SetInstanceName(m_instance3DList[i]->GetName());
					newRefer->SetIstanceId(m_instance3DList[i]->GetId());
					newRefer->SetAggretedId(m_instance3DList[i]->GetAggregatedNum());
					newRefer->SetIsMul(1);
					m_treeNodeList.push_back(newRefer);
				}
			}
		}
	}
	m_copy=m_treeNodeList;
	for(unsigned int i=0;i<m_copy.size();i++)
	{
		if(m_copy[i]->GetIsMul()==1)
		{
			CopyReference3D(m_copy[i]->GetId());
		}
	}
}

void CXMLParser::CopyReference3D(int id)
{
	for(unsigned int i=0;i<m_copy.size();i++)
	{
		if(m_copy[i]->GetAggretedId()==id)
		{
			TFReference3D *q=new TFReference3D(*m_copy[i]);
			q->SetInstanceName(m_copy[i]->GetInstaceName());
			q->SetIstanceId(m_copy[i]->GetIstanceId());
			q->SetAggretedId(m_copy[i]->GetAggretedId());
			m_treeNodeList.push_back(q);
			CopyReference3D(m_copy[i]->GetId());
		}
	}
}

void CXMLParser::LinkTreeNode(ReferenceTreeElement *root,ReferenceTreeElement *father)
{
	for(unsigned int i=0;i<m_treeNodeList.size();i++)
	{
		if(m_treeNodeList[i]->GetAggretedId()==root->id && m_treeNodeList[i]->GetFlag()==false)
		{
			ReferenceTreeElement *p=new ReferenceTreeElement();
			p->id=m_treeNodeList[i]->GetId();
			p->value=m_treeNodeList[i]->GetName();
			p->instancename=m_treeNodeList[i]->GetInstaceName();
			p->FirstChildElement=NULL;
			p->NextSimblingElement=NULL;
			p->instanceId=m_treeNodeList[i]->GetIstanceId();
			root->FirstChildElement=p;
			m_treeNodeList[i]->SetFlag(true);
			break;
		}
	}
	if(father!=NULL)
	{
		for(unsigned int i=0;i<m_treeNodeList.size();i++)
		{
			if(m_treeNodeList[i]->GetAggretedId()==father->id && m_treeNodeList[i]->GetFlag()==false)
			{
				bool f=true;
				ReferenceTreeElement *temp=father->FirstChildElement;

				while(temp!=NULL)
				{
					if(temp->instanceId==m_treeNodeList[i]->GetIstanceId())
					{
						f=false;
						break;
					}
					temp=temp->NextSimblingElement;
				}
				if(f==true)
				{
					ReferenceTreeElement *p=new ReferenceTreeElement();
					p->id=m_treeNodeList[i]->GetId();
					p->value=m_treeNodeList[i]->GetName();
					p->instancename=m_treeNodeList[i]->GetInstaceName();
					p->FirstChildElement=NULL;
					p->NextSimblingElement=NULL;
					p->instanceId=m_treeNodeList[i]->GetIstanceId();
					root->NextSimblingElement=p;
					m_treeNodeList[i]->SetFlag(true);
					break;
				}
			}
		}
	}
	if(root->FirstChildElement!=NULL)
	{
		LinkTreeNode(root->FirstChildElement,root);
	}
	if(root->NextSimblingElement!=NULL)
	{
		LinkTreeNode(root->NextSimblingElement,father);
	}
}

void CXMLParser::ReleaseTreeNode(ReferenceTreeElement *root)
{
	if(root!=NULL)
	{
		ReferenceTreeElement *p=root->FirstChildElement;
		ReferenceTreeElement *q=root->NextSimblingElement;
		delete root;
		ReleaseTreeNode(p);
		ReleaseTreeNode(q);
	}
}