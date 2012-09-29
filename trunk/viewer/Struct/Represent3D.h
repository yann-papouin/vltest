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
//! \file Represent3D.h interface for the Represent3D class.

#ifndef Represent3D_H_
#define Represent3D_H_

//#include "geometry.h"
#include "struct/Represent.h"
#include "vlGraphics/Actor.hpp"
#include "vlCore/Collection.hpp"
using namespace vl;

//////////////////////////////////////////////////////////////////////
//! \class Represent3D
/*! \brief Represent3D : A referrence 3D Representation*/
//////////////////////////////////////////////////////////////////////
class Represent3D : public Represent
{
	//friend QDataStream &operator<<(QDataStream &, const Represent3D &);
	//friend QDataStream &operator>>(QDataStream &, Represent3D &);

//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default Constructor
	Represent3D();

	//! Construct a Represent3D with a geometry
	Represent3D(const ActorCollection&);

	//! Copy Constructor
	Represent3D(const Represent3D&);

	//! Assignement operator
	virtual Represent3D &operator=(const Represent&);

	//! Clone the representation
	virtual Represent* clone() const;

	//! Make a deep copy of the Represent3D
	virtual Represent* deepCopy() const;

	//! Destructor
	virtual ~Represent3D();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the class Chunk ID
	static unsigned int chunckID();

	//! Return the type of representation
	virtual int type() const;

	//! Get Geometry
	ref<Actor> actor(int index) const;

	//! Return the number of body
	inline int numberOfBody() const
	{
	//	assert(NULL != m_pGeomList);
		return mActorCollection.size();
	}

	//! Return true if the representation is empty
	inline virtual bool isEmpty() const
	{
	//	assert(NULL != m_pGeomList);
	//	return m_pGeomList->isEmpty();
		return mActorCollection.empty();
	}

	//! Return true if the rep bounding box is valid
	bool boundsDirty() const;

	//! Return the Represent3D bounding Box
	AABB boundingBox() const;

	//! Return true if the Represent3D contains the geometry
	inline bool contains(ref<Actor> actor)
	{return mActorCollection.find(actor.get()) > 0;}

	//! Return the number of faces of this Represent3D
	unsigned int faceCount() const;

	//! Return number of vertex of this Represent3D
	unsigned int vertexCount() const;

	//! Return number of materials of this Represent3D
	unsigned int materialCount() const;

	//! Return materials Set of this Represent3D
// 	std::set<Material*> materialSet() const;

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Add Geometry to the Represent3D
	inline void insertActor(ref<Actor> actor)
	{
		mActorCollection.push_back(actor.get());
		*m_pIsLoaded= true;
	}

	//! Remove empty geometries and factorise materials
	void clean();

	//! Reverse geometries normals
//	void reverseNormals();

	//! Load the representation and return true if success
	virtual bool load();

	//! UnLoad the representation and return true if success
	virtual bool unload();

	//! Replace the representation
	virtual void replace(Represent*);

	//! Replace the specified material by a new one
//	void replaceMaterial(unsigned int, Material*);

	//! Merge this Represent3D with another Represent3D

//@}

//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////
private:
	//! Clear current representation
	void clear();

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! Geometries of the 3D representation
//	std::vector<Geometry*>* m_pGeomList;
	ActorCollection mActorCollection;

	//! The Type of representation
	int* m_pType;

	//! Class chunk id
	static unsigned int m_ChunkId;

};

//! Non-member stream operator
//QDataStream &operator<<(QDataStream &, const Represent3D &);
//QDataStream &operator>>(QDataStream &, Represent3D &);

#endif /* Represent3D_H_ */
