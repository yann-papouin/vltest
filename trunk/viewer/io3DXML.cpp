#include "stdafx.h"
#include "io3DXML.hpp"
#include <vlCore/Log.hpp>
#include <vlCore/Say.hpp>
#include <vlCore/VisualizationLibrary.hpp>
#include <vlCore/FileSystem.hpp>
#include <vlCore/TextStream.hpp>
#include <vlCore/VirtualFile.hpp>
#include <vlGraphics/Effect.hpp>
#include <vlGraphics/Actor.hpp>
#include <vlCore/LoadWriterManager.hpp>
#include <stdio.h>

#include "XMLParser.h"
#include "TF3DRepFile.h"
#include "assert.h"
#include "MainFrm.h"
#include "gui/MessageRecord.h"
//#include "glc_factory.h"
//#include "geometry\glc_geometry.h"
//#include "geometry\glc_geometry.h"

using namespace vl;
//-----------------------------------------------------------------------------
ref<ResourceDatabase> vl::load3DXML(const String& path)
{
	ref<VirtualFile> file = defFileSystem()->locateFile(path);

	if (file)
		return load3DXML( file.get() );
	else
	{
		Log::error( Say("Could not locate '%s'.\n") << path );
		return NULL;
	}
}
//-----------------------------------------------------------------------------
ref<ResourceDatabase> vl::load3DXML(VirtualFile* file)
{
	File3DXMLLoader File3DXML;
	ref<ResourceDatabase> res_db = File3DXML.load3DXML(file);
	return res_db;
}
//-----------------------------------------------------------------------------
void FillTriangleVertAndNormal(TFRep*& curGeom,
						  std::vector<fvec3>& verts,
						  std::vector<fvec3>& norms)
{
	fvec3 v[3], n;
	n = fvec3(0, 0, 1);
	int idx1 = 0;
	int idx2 = 0;
	int idx3 = 0;
	TFPoint* curPnt = NULL;

	vector<TFPoint*> pointList = curGeom->GetPointList();
	vector<TFTriangle*> triList = curGeom->GetTriangleList();
	vector<TFTriangle*>::iterator triIter;
	for (triIter = triList.begin();
		triIter != triList.end(); ++triIter)
	{
		TFTriangle* curTriangle = *triIter;
		curTriangle->GetPointIdx(idx1, idx2, idx3);
		curPnt = pointList[idx1];
		assert(curPnt != NULL);
		v[0] = fvec3(curPnt->GetX(), curPnt->GetY(), curPnt->GetZ());
		curPnt = pointList[idx2];
		assert(curPnt != NULL);
		v[1] = fvec3(curPnt->GetX(), curPnt->GetY(), curPnt->GetZ());
		curPnt = pointList[idx3];
		assert(curPnt != NULL);
		v[2] = fvec3(curPnt->GetX(), curPnt->GetY(), curPnt->GetZ());
		verts.push_back(v[0]);
		verts.push_back(v[1]);
		verts.push_back(v[2]);
		norms.push_back(n);
		norms.push_back(n);
		norms.push_back(n);
	}
}

void FillStripsGeometry(TFRep*& curGeom,
							 std::vector<ref<Geometry>> &stripsGeom)
{
	std::vector<fvec3> verts;
	std::vector<fvec3> norms;
	fvec3 v, n;
	n = fvec3(0, 0, 1);
	vector<int> stripsIdxs;
	vector<int>::iterator iter;
	TFPoint* curPnt = NULL;
	;
	vector<TFPoint*> pointList = curGeom->GetPointList();
	vector<TFStrips*> stripsList = curGeom->GetStripsList();
	vector<TFStrips*>::iterator triIter;
	for (triIter = stripsList.begin();
		triIter != stripsList.end(); ++triIter)
	{
		TFStrips* curStripts = *triIter;
		verts.clear();
		norms.clear();
		stripsIdxs = curStripts->GetStripIdxs();
		for (iter = stripsIdxs.begin();
			iter != stripsIdxs.end(); ++iter)
		{
			curPnt = pointList[*iter];
			assert(curPnt != NULL);
			v = fvec3(curPnt->GetX(), curPnt->GetY(), curPnt->GetZ());
			verts.push_back(v);
			norms.push_back(n);
		}
		ref<ArrayFloat3>  vertices   = new ArrayFloat3;
		ref<ArrayFloat3>  normals = new ArrayFloat3;
		vertices->resize(verts.size());
		normals->resize(verts.size());
		memcpy(normals ->ptr(), &norms[0], sizeof(norms[0])*norms.size());
		memcpy(vertices->ptr(), &verts[0], sizeof(verts[0])*verts.size());
		ref<DrawArrays> de = new DrawArrays(PT_TRIANGLE_STRIP,0,verts.size());
		ref<Geometry> geom = new Geometry;
		geom->drawCalls()->push_back(de.get());
		geom->setVertexArray(vertices.get());
		geom->setNormalArray(normals.get());
		stripsGeom.push_back(geom);
	}
}

void FillFanGeometry(TFRep*& curGeom,
						std::vector<ref<Geometry>> &fanGeom)
{
	std::vector<fvec3> verts;
	std::vector<fvec3> norms;
	fvec3 v, n;
	n = fvec3(0, 0, 1);
	vector<int> fansIdxs;
	vector<int>::iterator iter;
	TFPoint* curPnt = NULL;
	vector<TFPoint*> pointList = curGeom->GetPointList();
	vector<TFFan*> fanList = curGeom->GetFansList();
	vector<TFFan*>::iterator fanIter;
	for (fanIter = fanList.begin();
		fanIter != fanList.end(); ++fanIter)
	{
		TFFan* curFans = *fanIter;
		verts.clear();
		norms.clear();
		fansIdxs = curFans->GetFans();
		for (iter = fansIdxs.begin();
			iter != fansIdxs.end(); ++iter)
		{
			curPnt = pointList[*iter];
			assert(curPnt != NULL);
			v = fvec3(curPnt->GetX(), curPnt->GetY(), curPnt->GetZ());
			verts.push_back(v);
			norms.push_back(n);
		}
		ref<ArrayFloat3>  vertices   = new ArrayFloat3;
		ref<ArrayFloat3>  normals = new ArrayFloat3;
		vertices->resize(verts.size());
		normals->resize(verts.size());
		memcpy(normals ->ptr(), &norms[0], sizeof(norms[0])*norms.size());
		memcpy(vertices->ptr(), &verts[0], sizeof(verts[0])*verts.size());
		ref<DrawArrays> de = new DrawArrays(PT_TRIANGLE_FAN,0,verts.size());
		ref<Geometry> geom = new Geometry;
		geom->drawCalls()->push_back(de.get());
		geom->setVertexArray(vertices.get());
		geom->setNormalArray(normals.get());
		fanGeom.push_back(geom);
	}
}

void ClearAllTheRecode(CXTPReportRecord* curRecord)
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CModelPaneView& modelPane = m_pMainFrame->m_wndModelPaneView;

	CXTPReportRecords* rootChild = curRecord->GetChilds();
	int childNum = rootChild->GetCount();
	for (int i = 0; i < childNum; ++i)
	{
		CXTPReportRecord* curRecord = rootChild->GetAt(i);
		if (curRecord == NULL)
		{
			continue;
		}
		modelPane.m_wndModelTree.RemoveRecordEx(curRecord, TRUE);
		if (curRecord->GetChilds()->GetCount() > 0)
		{
			ClearAllTheRecode(curRecord);
		}	
	}
}

ref<ResourceDatabase> File3DXMLLoader::loadAscii(VirtualFile* file)
{
	std::vector<fvec3> verts;
	std::vector<fvec3> norms;
	std::vector<ref<Geometry>> stripGeomList;
	std::vector<ref<Geometry>> fanGeomList;
	std::vector<ref<Geometry>>::iterator stripIter;

	std::string fileStr = file->path().toStdString();
	CXMLParser xmlParser(fileStr);
	ReferenceTreeElement* rootElement = xmlParser.GetRootElement();

	//Update model tree
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CModelPaneView& modelPane = m_pMainFrame->m_wndModelPaneView;
	modelPane.m_pRootRecordNode->RemoveAll();

	ClearAllTheRecode(modelPane.m_pRootRecordNode);
	CString rootName;
	if (rootElement->instancename == "")
	{
		rootName = CString(rootElement->value.c_str());
	}
	else
	{
		rootName = CString(rootElement->instancename.c_str());
	}
	modelPane.m_pRootRecordNode->CreateItems(rootName);
	modelPane.m_wndModelTree.UpdateRecord(modelPane.m_pRootRecordNode, TRUE);

	/*CMessageRecord* pCurRecord = new CMessageRecord(_T("×°Åä1") ,0,TRUE,TRUE,FALSE, FALSE, FALSE); 
	modelPane.m_pRootRecordNode = pCurRecord;
	modelPane.m_wndModelTree.AddRecord(pCurRecord);*/

	//Display
	vector<TF3DRepFile*> repFileList = xmlParser.GetFileList();
	vector<TF3DRepFile*>::iterator fileIter;
	for (fileIter = repFileList.begin();
		fileIter != repFileList.end(); ++fileIter)
	{
		vector<TFRep*> geomList = (*fileIter)->GetRepList();
		vector<TFRep*>::iterator geomIter;

		vector<TFRep*> childRepList;
		vector<TFRep*>::iterator childRepIter;

		int i = 0;
		for (geomIter = geomList.begin();
			geomIter != geomList.end(); ++geomIter)
		{
			i++;
			TFRep* curGeom = *geomIter;
			childRepList = curGeom->GetChildRepList();
			for (childRepIter = childRepList.begin();
				childRepIter != childRepList.end(); ++childRepIter)
			{
				TFRep* curChildGeom = *childRepIter;
				FillTriangleVertAndNormal(curChildGeom, verts, norms);
				FillStripsGeometry(curChildGeom, stripGeomList);
				FillFanGeometry(curChildGeom, fanGeomList);
			}
			FillTriangleVertAndNormal(curGeom, verts, norms);
			FillStripsGeometry(curGeom, stripGeomList);
			FillFanGeometry(curGeom, fanGeomList);
		}
	}
	
	ref<ResourceDatabase> res_db = new ResourceDatabase;
	if (verts.empty())
	{
		return res_db;
	}
	ref<ArrayFloat3>  vertices   = new ArrayFloat3;
	ref<ArrayFloat3>  normals = new ArrayFloat3;
	vertices->resize(verts.size());
	normals->resize(verts.size());
	memcpy(normals ->ptr(), &norms[0], sizeof(norms[0])*norms.size());
	memcpy(vertices->ptr(), &verts[0], sizeof(verts[0])*verts.size());
	ref<DrawArrays> de = new DrawArrays(PT_TRIANGLES,0,verts.size());
	ref<Geometry> geom = new Geometry;
	geom->drawCalls()->push_back(de.get());
	geom->setVertexArray(vertices.get());
	geom->setNormalArray(normals.get());

	ref<Effect> effect = new Effect;
	res_db->resources().push_back( geom );
	res_db->resources().push_back( new Actor(geom.get(), effect.get(), NULL ) );
	res_db->resources().push_back( effect.get() );

	for (stripIter = stripGeomList.begin();
		stripIter != stripGeomList.end(); ++stripIter)
	{
		ref<Geometry> curGeom = *stripIter;
		res_db->resources().push_back( curGeom );
		res_db->resources().push_back( new Actor(curGeom.get(), effect.get(), NULL ) );
		res_db->resources().push_back( effect.get() );
	}

	for (stripIter = fanGeomList.begin();
		stripIter != fanGeomList.end(); ++stripIter)
	{
		ref<Geometry> curGeom = *stripIter;
		res_db->resources().push_back( curGeom );
		res_db->resources().push_back( new Actor(curGeom.get(), effect.get(), NULL ) );
		res_db->resources().push_back( effect.get() );
	}

	return res_db;
}

ref<ResourceDatabase> File3DXMLLoader::load3DXML(VirtualFile* file)
{
	ref<ResourceDatabase> res_db;
	res_db = loadAscii(file);
	return res_db;
}
//-----------------------------------------------------------------------------
