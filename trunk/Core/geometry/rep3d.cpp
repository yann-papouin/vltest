/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 *****************************************************************************/

#include "Rep3D.h"
#include "../factory.h"
//#include "mesh.h"
//#include "errorlog.h"

// Class chunk id
unsigned int Rep3D::m_ChunkId= 0xA702;

// Default constructor
Rep3D::Rep3D()
: Rep()
, m_pGeomList(new std::vector<vl::Geometry*>)
, m_pType(new int(Rep::VBOGEOM))
{

}

// Construct a Rep3D with a geometry
Rep3D::Rep3D(Geometry* pGeom)
: Rep()
, m_pGeomList(new std::vector<vl::Geometry*>)
, m_pType(new int(Rep::VBOGEOM))
{
	m_pGeomList->push_back(pGeom);
	*m_pIsLoaded= true;
	setName(pGeom->objectName());
}

// Copy Constructor
Rep3D::Rep3D(const Rep3D& rep)
: Rep(rep)
, m_pGeomList(rep.m_pGeomList)
, m_pType(rep.m_pType)
{

}

// Assignement operator
Rep3D& Rep3D::operator=(const Rep& rep)
{
	const Rep3D* pRep3D= dynamic_cast<const Rep3D*>(&rep);
	assert(NULL != pRep3D);
	if (this != &rep)
	{
		clear();
		Rep::operator=(rep);
		m_pGeomList= pRep3D->m_pGeomList;
		m_pType= pRep3D->m_pType;
	}

	return *this;
}

// Clone the representation
Rep* Rep3D::clone() const
{
	return new Rep3D(*this);
}

// Make a deep copy of the Rep3D
Rep* Rep3D::deepCopy() const
{
	Rep3D* pCloneRep= new Rep3D;
	// Copy fields of the base class
	pCloneRep->setFileName(fileName());
	pCloneRep->setName(name());
	// Copy representation geometries
	const int size= m_pGeomList->size();
	for (int i= 0; i < size; ++i)
	{
		pCloneRep->addGeom(m_pGeomList->operator [](i)->deepCopy().get());
	}
	return pCloneRep;
}

// Destructor
Rep3D::~Rep3D()
{
	clear();
}

//////////////////////////////////////////////////////////////////////
// Get functions
//////////////////////////////////////////////////////////////////////
// Return the class Chunk ID
unsigned int Rep3D::chunckID()
{
	return m_ChunkId;
}

// Return the type of representation
int Rep3D::type() const
{
	return (*m_pType);
}

//////////////////////////////////////////////////////////////////////
// Get functions
//////////////////////////////////////////////////////////////////////

// Return true if the rep bounding box is valid
bool Rep3D::boundingBoxIsValid() const
{
	bool result= !m_pGeomList->empty();
	const int max= m_pGeomList->size();
	int index= 0;
	while (result && (index < max))
	{
//		result= result && m_pGeomList->operator [](index)->boundingBoxIsValid();
		++index;
	}
	return result;
}

// Return the Rep3D bounding Box
AABB Rep3D::boundingBox() const
{
	AABB resultBox;
	const int size= m_pGeomList->size();
	for (int i= 0; i < size; ++i)
	{
		resultBox+=m_pGeomList->operator [](i)->boundingBox();
	}
	return resultBox;
}

// Get number of faces
unsigned int Rep3D::faceCount() const
{
	unsigned int result= 0;
	if (!m_pGeomList->empty())
	{
		const int size= m_pGeomList->size();
		for (int i= 0; i < size; ++i)
		{
		//	result+= m_pGeomList->operator [](i)->faceCount();
		}
	}

	return result;
}

// Get number of vertex
unsigned int Rep3D::vertexCount() const
{
	unsigned int result= 0;
	if (!m_pGeomList->empty())
	{
		const int size= m_pGeomList->size();
		for (int i= 0; i < size; ++i)
		{
//			result+= m_pGeomList->operator[](i)->VertexCount();
		}
	}

	return result;
}

// Get number of materials
//unsigned int Rep3D::materialCount() const
//{
//	unsigned int result= 0;
//	if (!m_pGeomList->isEmpty())
//	{
//		const int size= m_pGeomList->size();
//		for (int i= 0; i < size; ++i)
//		{
//			result+= m_pGeomList->operator[](i)->materialCount();
//		}
//	}
//
//	return result;
//}

// Get materials List
//std::set<Material*> Rep3D::materialSet() const
//{
//	std::set<Material*> result;
//	if (!m_pGeomList->isEmpty())
//	{
//		const int size= m_pGeomList->size();
//		for (int i= 0; i < size; ++i)
//		{
//			result.unite(m_pGeomList->operator[](i)->materialSet());
//		}
//	}
//
//	return result;
//}

// Remove empty geometries
void Rep3D::clean()
{
	std::vector<Geometry*>::iterator iGeomList= m_pGeomList->begin();
	while(iGeomList != m_pGeomList->end())
	{
		//if ((*iGeomList)->VertexCount() == 0)
		//{
		//	cout<< "Delete empty geom--------------------";
		//	delete (*iGeomList);
		//	iGeomList= m_pGeomList->erase(iGeomList);
		//}
		//else
		//{
		//	++iGeomList;
		//}
	}
}

// Reverse geometries normals
void Rep3D::reverseNormals()
{
	const int size= m_pGeomList->size();
	for (int i= 0; i < size; ++i)
	{
//		(*m_pGeomList)[i]->reverseNormals();
	}
}

// Load the representation
bool Rep3D::load()
{
	bool loadSucces= false;

	if(!(*m_pIsLoaded))
	{
		assert(m_pGeomList->empty());
		if (fileName().empty())
		{
			//std::stringList stringList("Rep3D::load");
			//stringList.append("Representation : " + Rep::name());
			//stringList.append("Empty File Name");
			//ErrorLog::addError(stringList);
		}
		else
		{
			Rep3D newRep= Factory::instance()->createRep3DFromFile(fileName());
			if (!newRep.isEmpty())
			{
				const int size= newRep.m_pGeomList->size();
				for (int i= 0; i < size; ++i)
				{
					m_pGeomList->push_back(newRep.m_pGeomList->operator[](i));
				}
				newRep.m_pGeomList->clear();
				(*m_pIsLoaded)= true;
				loadSucces= true;
			}
		}
	}

	return loadSucces;

}
// Replace the representation
void Rep3D::replace(Rep* pRep)
{
	Rep3D* pRep3D= dynamic_cast<Rep3D*>(pRep);
	assert(NULL != pRep3D);

	setFileName(pRep3D->fileName());
	setName(pRep3D->name());

	if (!pRep3D->isEmpty())
	{
		const int size= pRep3D->m_pGeomList->size();
		for (int i= 0; i < size; ++i)
		{
			m_pGeomList->push_back(pRep3D->m_pGeomList->operator[](i));
		}
		pRep3D->m_pGeomList->clear();
		(*m_pIsLoaded)= true;
	}
}

// Replace the specified material by a new one
//void Rep3D::replaceMaterial(unsigned int oldId, Material* pNewMaterial)
//{
//	//cout<< "Rep3D::replaceMaterial";
//	const int size= m_pGeomList->size();
//	for (int i= 0; i < size; ++i)
//	{
//		Geometry* pGeom= m_pGeomList->operator[](i);
//		if (pGeom->containsMaterial(oldId))
//		{
//			assert(!pGeom->containsMaterial(pNewMaterial->id()));
//			Mesh* pMesh= dynamic_cast<Mesh*>(m_pGeomList->operator[](i));
//			if (NULL != pMesh)
//			{
//				pMesh->replaceMaterial(oldId, pNewMaterial);
//			}
//		}
//	}
//}

// Merge this Rep3D with another Rep3D
void Rep3D::merge(const Rep3D* pRep)
{
	// Get the number of geometry of pRep
	const int pRepSize= pRep->m_pGeomList->size();
	for (int i= 0; i < pRepSize; ++i)
	{
		addGeom(pRep->geomAt(i)->deepCopy().get());
	}
}

void Rep3D::take(Rep3D* pSource)
{
	// Get the number of geometry of pRep
	const int pRepSize= pSource->m_pGeomList->size();
	for (int i= 0; i < pRepSize; ++i)
	{
		addGeom(pSource->geomAt(i));
	}
	pSource->m_pGeomList->clear();
}

// void Rep3D::copyVboToClientSide()
// {
// 	// Get the number of geometry of pRep
// 	const int pRepSize= m_pGeomList->size();
// 	for (int i= 0; i < pRepSize; ++i)
// 	{
// 		geomAt(i)->copyVboToClientSide();
// 	}
// }

//void Rep3D::releaseVboClientSide(bool update)
//{
//	// Get the number of geometry of pRep
//	const int pRepSize= m_pGeomList->size();
//	for (int i= 0; i < pRepSize; ++i)
//	{
//		geomAt(i)->releaseVboClientSide(update);
//	}
//}

void Rep3D::transformSubGeometries(const vl::dmat4& matrix)
{
	// Get the number of geometry of pRep
	const int repCount= m_pGeomList->size();
	//cout<< "repCount " << repCount;
	for (int i= 0; i < repCount; ++i)
	{
		//Mesh* pCurrentMesh= dynamic_cast<Mesh*>(geomAt(i));
		//if (NULL != pCurrentMesh)
		//{
		//	pCurrentMesh->transformVertice(matrix);
		//}
	}
}

// UnLoad the representation
bool Rep3D::unload()
{
	bool unloadSucess= false;
	if ((NULL != m_pGeomList) && !m_pGeomList->empty())
	{
		if (fileName().empty())
		{
			//std::stringList stringList("Rep3D::unload()");
			//stringList.append("Cannot unload rep without filename");
	//		ErrorLog::addError(stringList);
		}
		else
		{
			const int size= m_pGeomList->size();
			for (int i= 0; i < size; ++i)
			{
				delete (*m_pGeomList)[i];
			}
			m_pGeomList->clear();

			(*m_pIsLoaded)= false;
			unloadSucess= true;
		}
	}
	return unloadSucess;
}

//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////

// Clear the 3D representation
void Rep3D::clear()
{
	if (isTheLast())
	{
		const int size= m_pGeomList->size();
		for (int i= 0; i < size; ++i)
		{
			delete (*m_pGeomList)[i];
		}
		delete m_pGeomList;
		m_pGeomList= NULL;

		delete m_pType;
		m_pType= NULL;
	}
}
// Non Member methods
// Non-member stream operator
//QDataStream &operator<<(QDataStream & stream, const Rep3D & rep)
//{
//	unsigned int chunckId= Rep3D::m_ChunkId;
//	stream << chunckId;
//
//	// The representation name
//	stream << rep.name();
//
//	// Save the list of Rep3D materials
//	std::vector<Material> materialsList;
//	std::vector<Material*> sourceMaterialsList= rep.materialSet().toList();
//	const int materialNumber= sourceMaterialsList.size();
//	for (int i= 0; i < materialNumber; ++i)
//	{
//		materialsList.append(*(sourceMaterialsList.at(i)));
//		materialsList[i].setId(sourceMaterialsList.at(i)->id());
//	}
//	// Save the list of materials
//	stream << materialsList;
//
//	// Save the list of mesh
//	const int meshNumber= rep.m_pGeomList->size();
//	stream << meshNumber;
//	for (int i= 0; i < meshNumber; ++i)
//	{
//		Mesh* pMesh= dynamic_cast<Mesh*>(rep.m_pGeomList->operator[](i));
//		if (NULL != pMesh)
//		{
//			pMesh->saveToDataStream(stream);
//		}
//	}
//
//	return stream;
//}
//QDataStream &operator>>(QDataStream & stream, Rep3D & rep)
//{
//	assert(rep.isEmpty());
//
//	unsigned int chunckId;
//	stream >> chunckId;
//	assert(chunckId == Rep3D::m_ChunkId);
//
//	// The rep name
//	std::string name;
//	stream >> name;
//	rep.setName(name);
//
//	// Retrieve the list of rep materials
//	std::vector<Material> materialsList;
//	stream >> materialsList;
//	MaterialHash materialHash;
//	// Update mesh materials hash table
//	tr1::unordered_map<unsigned int, unsigned int> materialIdMap;
//	const int materialsCount= materialsList.size();
//	for (int i= 0; i < materialsCount; ++i)
//	{
//		Material* pMaterial= new Material(materialsList.at(i));
//		pMaterial->setId(glc::GenID());
//		materialIdMap.insert(materialsList.at(i).id(), pMaterial->id());
//		materialHash.insert(pMaterial->id(), pMaterial);
//	}
//
//	int meshNumber;
//	stream >> meshNumber;
//	for (int i= 0; i < meshNumber; ++i)
//	{
//		Mesh* pMesh= new Mesh();
//		pMesh->loadFromDataStream(stream, materialHash, materialIdMap);
//
//		rep.addGeom(pMesh);
//	}
//
//	return stream;
//}
