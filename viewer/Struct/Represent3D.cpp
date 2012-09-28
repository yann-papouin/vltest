#include <assert.h>
#include "vlGraphics/Actor.hpp"
#include "struct/Represent3D.h"

// Class chunk id
unsigned int Represent3D::m_ChunkId= 0xA702;

// Default constructor
Represent3D::Represent3D()
: Represent()
//, m_pGeomList(new QList<GLC_Geometry*>)
, m_pType(new int(Represent::VBOGEOM))
{

}

// Construct a 3DRep with a geometry
Represent3D::Represent3D(const ActorCollection& actors)
: Represent()
//, m_pGeomList(new QList<GLC_Geometry*>)
, m_pType(new int(Represent::VBOGEOM))
{
//	m_pGeomList->append(pGeom);
	mActorCollection = actors;
	*m_pIsLoaded= true;
//	setName(pGeom->name());
}

// Copy Constructor
Represent3D::Represent3D(const Represent3D& rep)
: Represent(rep)
//, m_pGeomList(rep.m_pGeomList)
, m_pType(rep.m_pType)
{

}

// Assignement operator
Represent3D& Represent3D::operator=(const Represent& rep)
{
	const Represent3D* p3DRep= dynamic_cast<const Represent3D*>(&rep);
	assert(NULL != p3DRep);
	if (this != &rep)
	{
		clear();
		Represent::operator=(rep);
//		m_pGeomList= p3DRep->m_pGeomList;
		m_pType= p3DRep->m_pType;
	}

	return *this;
}

// Clone the representation
Represent* Represent3D::clone() const
{
	return new Represent3D(*this);
}

// Make a deep copy of the 3DRep
Represent* Represent3D::deepCopy() const
{
	Represent3D* pCloneRep= new Represent3D;
	// Copy fields of the base class
	pCloneRep->setFileName(fileName());
	pCloneRep->setName(name());
	// Copy representation geometries
	//const int size= m_pGeomList->size();
	//for (int i= 0; i < size; ++i)
	//{
	//	pCloneRep->addGeom(m_pGeomList->at(i)->clone());
	//}

	const int size= mActorCollection.size();
	for (int i= 0; i < size; ++i)
	{
		pCloneRep->addActor(mActorCollection[i]);
	}

	return pCloneRep;
}

// Destructor
Represent3D::~Represent3D()
{
	clear();
}

//////////////////////////////////////////////////////////////////////
// Get functions
//////////////////////////////////////////////////////////////////////
// Return the class Chunk ID
unsigned int Represent3D::chunckID()
{
	return m_ChunkId;
}

// Return the type of representation
int Represent3D::type() const
{
	return (*m_pType);
}

//////////////////////////////////////////////////////////////////////
// Get functions
//////////////////////////////////////////////////////////////////////

// Return true if the rep bounding box is valid
bool Represent3D::boundsDirty() const
{
	bool result= !mActorCollection.empty();
	const int max= mActorCollection.size();
	int index= 0;
	while (result && (index < max))
	{
		result= result && mActorCollection.at(index)->boundsDirty();
		++index;
	}
	return result;
}

// Return the 3DRep bounding Box
AABB Represent3D::boundingBox() const
{
	AABB aabb;
	const int size= mActorCollection.size();
	for (int i= 0; i < size; ++i)
	{
		ref<Actor> actor = mActorCollection[i];
		aabb+=actor->boundingBox(); 
	//	resultBox.combine(m_pGeomList->at(i)->boundingBox());
	}
	return aabb;
}

// Get number of faces
unsigned int Represent3D::faceCount() const
{
	unsigned int result= 0;
	if (!mActorCollection.empty())
	{
		const int size= mActorCollection.size();
		for (int i= 0; i < size; ++i)
		{
// 			result+= mActorCollection.at(i)->faceCount();
		}
	}

	return result;
}

// Get number of vertex
unsigned int Represent3D::vertexCount() const
{
	unsigned int result= 0;
	if (!mActorCollection.empty())
 	{
 		const int size= mActorCollection.size();
 		for (int i= 0; i < size; ++i)
		{
//			result+= m_pGeomList->at(i)->VertexCount();
 		}
	}

	return result;
}

// Get number of materials
unsigned int Represent3D::materialCount() const
{
	unsigned int result= 0;
// 	if (!m_pGeomList->isEmpty())
// 	{
// 		const int size= m_pGeomList->size();
// 		for (int i= 0; i < size; ++i)
// 		{
// 			result+= m_pGeomList->at(i)->materialCount();
// 		}
// 	}

	return result;
}

// Remove empty geometries
void Represent3D::clean()
{
// 	QList<GLC_Geometry*>::iterator iGeomList= m_pGeomList->begin();
// 	while(iGeomList != m_pGeomList->constEnd())
// 	{
// 		if ((*iGeomList)->VertexCount() == 0)
// 		{
// 			qDebug() << "Delete empty geom--------------------";
// 			delete (*iGeomList);
// 			iGeomList= m_pGeomList->erase(iGeomList);
// 		}
// 		else
// 		{
// 			++iGeomList;
// 		}
		mActorCollection.clear();
//	}
}

// Load the representation
bool Represent3D::load()
{
	bool loadSucces= false;

	if(!(*m_pIsLoaded))
	{
// 		Q_ASSERT(m_pGeomList->isEmpty());
// 		if (fileName().isEmpty())
// 		{
// 			std::stringList stringList("Represent3D::load");
// 			stringList.append("Representation : " + Represent::name());
// 			stringList.append("Empty File Name");
// 			GLC_ErrorLog::addError(stringList);
// 		}
// 		else
// 		{
// 			Represent3D newRep= GLC_Factory::instance()->create3DRepFromFile(fileName());
// 			if (!newRep.isEmpty())
// 			{
// 				const int size= newRep.m_pGeomList->size();
// 				for (int i= 0; i < size; ++i)
// 				{
// 					m_pGeomList->append(newRep.m_pGeomList->at(i));
// 				}
// 				newRep.m_pGeomList->clear();
// 				(*m_pIsLoaded)= true;
// 				loadSucces= true;
// 			}
// 		}
	}

	return loadSucces;

}

//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////

// Clear the 3D representation
void Represent3D::clear()
{
	if (isTheLast())
	{
		//const int size= m_pGeomList->size();
		//for (int i= 0; i < size; ++i)
		//{
		//	delete (*m_pGeomList)[i];
		//}
		//delete m_pGeomList;
		//m_pGeomList= NULL;

		mActorCollection.clear();

		delete m_pType;
		m_pType= NULL;
	}
}

ref<Actor> Represent3D::actor( int index ) const
{
	return mActorCollection[index];
}

bool Represent3D::unload()
{
	return true;
}

void Represent3D::replace( Represent* )
{

}

