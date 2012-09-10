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
//! \file Rep3D.h interface for the Rep3D class.

#ifndef Rep3D_H_
#define Rep3D_H_

#include "rep.h"
#include "vlGraphics/Geometry.hpp"
using namespace vl;

#include "../config.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
//! \class Rep3D
/*! \brief Rep3D : A referrence 3D Representation*/
//////////////////////////////////////////////////////////////////////
class GVCORE_EXPORT Rep3D : public Rep
{
	//friend GVCORE_EXPORT QDataStream &operator<<(QDataStream &, const Rep3D &);
	//friend GVCORE_EXPORT QDataStream &operator>>(QDataStream &, Rep3D &);

//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default Constructor
	Rep3D();

	//! Construct a Rep3D with a geometry
	Rep3D(vl::Geometry*);

	//! Copy Constructor
	Rep3D(const Rep3D&);

	//! Assignement operator
	virtual Rep3D &operator=(const Rep&);

	//! Clone the representation
	virtual Rep* clone() const;

	//! Make a deep copy of the Rep3D
	virtual Rep* deepCopy() const;

	//! Destructor
	virtual ~Rep3D();

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
	inline vl::Geometry* geomAt(int index) const
	{
		assert(NULL != m_pGeomList);
		assert(m_pGeomList->size() > index);
		return m_pGeomList->at(index);
	}

	//! Return the number of body
	inline int numberOfBody() const
	{
		assert(NULL != m_pGeomList);
		return m_pGeomList->size();
	}

	//! Return true if the representation is empty
	inline virtual bool isEmpty() const
	{
		assert(NULL != m_pGeomList);
		return m_pGeomList->empty();
	}

	//! Return true if the rep bounding box is valid
	bool boundingBoxIsValid() const;

	//! Return the Rep3D bounding Box
	AABB boundingBox() const;

	//! Return true if the Rep3D contains the geometry
	inline bool contains(vl::Geometry* pGeom)
	{
		bool bContained = false;

		std::vector<vl::Geometry*>::const_iterator itor;
		for (itor = m_pGeomList->begin();itor!=m_pGeomList->end();itor++)
		{
			if (*itor == pGeom)
			{
				bContained = true;
				break;
			}
		}

		return bContained;
	}

	//! Return the number of faces of this Rep3D
	unsigned int faceCount() const;

	//! Return number of vertex of this Rep3D
	unsigned int vertexCount() const;

	//! Return number of materials of this Rep3D
//	unsigned int materialCount() const;

	//! Return materials Set of this Rep3D
//	std::set<Material*> materialSet() const;

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Add Geometry to the Rep3D
	inline void addGeom(vl::Geometry* pGeom)
	{
		m_pGeomList->push_back(pGeom);
		*m_pIsLoaded= true;
	}

	//! Remove empty geometries and factorise materials
	void clean();

	//! Reverse geometries normals
	void reverseNormals();

	//! Load the representation and return true if success
	virtual bool load();

	//! UnLoad the representation and return true if success
	virtual bool unload();

	//! Replace the representation
	virtual void replace(Rep*);

	//! Replace the specified material by a new one
//	void replaceMaterial(unsigned int, Material*);

	//! Merge this Rep3D with another Rep3D
	void merge(const Rep3D*);

	//! Take the geometry of another Rep3D
	void take(Rep3D* pSource);

	//! Copy VBO to the Client Side
// 	void copyVboToClientSide();

	//! Release client VBO
	//void releaseVboClientSide(bool update= false);

	//! Transform Rep3D sub mesh vertice with the given matrix
	void transformSubGeometries(const vl::dmat4& matrix);


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
	std::vector<vl::Geometry*>* m_pGeomList;

	//! The Type of representation
	int* m_pType;

	//! Class chunk id
	static unsigned int m_ChunkId;

};

//! Non-member stream operator
//GVCORE_EXPORT QDataStream &operator<<(QDataStream &, const Rep3D &);
//GVCORE_EXPORT QDataStream &operator>>(QDataStream &, Rep3D &);

#endif /* Rep3D_H_ */
