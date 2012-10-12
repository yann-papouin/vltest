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
#include "TFRep.h"
#include "assert.h"
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
void FillTheVertAndNormal(TFRep*& curGeom,
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

ref<ResourceDatabase> File3DXMLLoader::loadAscii(VirtualFile* file)
{
	//GLC_3DRep newRep= GLC_Factory::instance()->createWorldStructureFrom3dxml("E:\\Sample Files\\3DXML_Test_File\\US Coast Guard RB-HS Fast Response Boat.3dxml");

	std::vector<fvec3> verts;
	std::vector<fvec3> norms;

	std::string fileStr = file->path().toStdString();
	CXMLParser xmlParser(fileStr);
	vector<TFRep*> geomList = xmlParser.GetGeometryList();
	vector<TFRep*>::iterator geomIter;
	
	vector<TFRep*> childRepList;
	vector<TFRep*>::iterator childRepIter;

	for (geomIter = geomList.begin();
		geomIter != geomList.end(); ++geomIter)
	{
		TFRep* curGeom = *geomIter;
		childRepList = curGeom->GetChildRepList();
		for (childRepIter = childRepList.begin();
			childRepIter != childRepList.end(); ++childRepIter)
		{
			TFRep* curChildGeom = *childRepIter;
			FillTheVertAndNormal(curChildGeom, verts, norms);
		}
		FillTheVertAndNormal(curGeom, verts, norms);
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
	return res_db;
}

ref<ResourceDatabase> File3DXMLLoader::load3DXML(VirtualFile* file)
{
	ref<ResourceDatabase> res_db;
	res_db = loadAscii(file);
	return res_db;
}
//-----------------------------------------------------------------------------
