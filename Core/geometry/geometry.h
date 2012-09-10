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

//! \file geometry.h Interface for the Geometry class.

#ifndef GEOMETRY_H_
#define GEOMETRY_H_
//#include "../shading/material.h"
//#include "../shading/renderproperties.h"
//#include "wiredata.h"
// #include "../boundingbox.h"

#include "../config.h"

typedef stdext::hash_map<unsigned int, Material*> MaterialHash;
typedef stdext::hash_map<unsigned int, unsigned int> MaterialHashMap;

//////////////////////////////////////////////////////////////////////
//! \class Geometry
/*! \brief Geometry : parent class for all geometry*/

/*! Geometry is a abstract class. \n \n
 *  Main attributes of Geometry:
 *		- Materials Hash table : 	std::unordered_map<Material*>
 *
 * Geometry provides :
 * 		- Method to draw Geometry                                                             : Geometry::glExecute()
 * 		- Virtual method to overload for visual property                                      : Geometry::glPropGeom()
 * 		- Virtual method to load and generate Opengl textures for each materials              : Geometry::glLoadTexture()
 * 		- Virtual method to clear the content of the geometry and makes it empty			  : Geometry::clear()
 *
 * 		- Pure virtual method to overload for Object topology                                 : Geometry::glDraw()
 * 		- Pure virtual Clone method                                                           : Geometry::clone()
 * 		- Pure virtual method to get geometry bounding box                                    : Geometry::boundingBox()
 *
 * 		- Empty virtual method for reversing normals                                          : Geometry::reverseNormals()
 *      - Empty virtual method for setting the current level of detail (between 0 and 100)    : Geometry::setCurrentLod()
 *      - Empty virtual method to get the number of vertex                                    : Geometry::numberOfVertex()
 *      - Empty virtual method to get the number of faces                                     : Geoetry::numberOfFaces()
 *
 */
//////////////////////////////////////////////////////////////////////

class GVCORE_EXPORT Geometry
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default constructor
	/*!
	 * std::string Name
	 * const bool typeIsWire
	 */
	Geometry(const std::string &, const bool);
	//! Copy constructor
	/*!
	 * const VboGeom geometry to copy
	 */
	Geometry(const Geometry&);

	//! Overload "=" operator
	Geometry& operator=(const Geometry&);

	//! Destructor
	virtual ~Geometry();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Get Object ID
	inline unsigned int id() const
	{return m_Id;}

	//! Get Object Name
	inline std::string name() const
	{return m_Name;}

	//! Return true if the geometry is valid
	inline bool isValid(void) const
	{return m_GeometryIsValid;}

	//! Return true if the geometry has material
	inline bool hasMaterial() const
	{return !m_MaterialHash.isEmpty();}

	//! Return first material of geometry
	inline Material* firstMaterial(void) const
	{
		if (!m_MaterialHash.isEmpty())
		{
			return m_MaterialHash.begin().value();
		}
		else return NULL;
	}

	//! Return the number of materials
	inline int materialCount() const
	{return m_MaterialHash.size();}

	//! Return the specified mesh sub material
	inline Material* material(const unsigned int key) const
	{return m_MaterialHash[key];}

	//! Get materials Set
	inline std:;set<Material*> materialSet() const
	{return m_MaterialHash.values().toSet();}

	//! Get materials ID List
	inline std::list<unsigned int> materialIds() const
	{return m_MaterialHash.keys();}

	//! Return true if Material key is in the mesh
	inline bool containsMaterial(const unsigned int key) const
	{return m_MaterialHash.contains(key);}

	//! Return the geometry bounding box
	virtual const BoundingBox& boundingBox(void) = 0;

	//! Return true if the bounding box is valid
	inline bool boundingBoxIsValid() const
	{return NULL != m_pBoundingBox;}

	//! Clone the geometry
	virtual Geometry* clone() const = 0;

	//! Get the geometry transparency
	inline bool isTransparent() const
	{return (m_TransparentMaterialNumber >= m_MaterialHash.size()) && hasTransparentMaterials();}

	//! Return true if the geometry contains transparent materials
	inline bool hasTransparentMaterials() const
	{return m_TransparentMaterialNumber > 0;}

	//! Return true if color per vertex is used
	inline bool usedColorPerVertex() const
	{return m_UseColorPerVertex;}

	//! Return true if the geometry type is wireframe
	inline bool typeIsWire() const
	{return m_IsWire;}

	//! Get the number of faces
	virtual unsigned int faceCount(int lod=0) const;

	//! Get the number of vertex
	virtual unsigned int VertexCount() const;

	//! Return the line width
	GLfloat lineWidth() const
	{return m_LineWidth;}

	//! Return this geometry wire color
	inline fvec wireColor() const
	{return m_WireColor;}

	//! Return true if wire data is empty
	inline bool wireDataIsEmpty() const
	{return m_WireData.isEmpty();}

	//! Return the wire position vector
	inline GLfloatVector wirePositionVector() const
	{return m_WireData.positionVector();}

	//! Return the number of wire polylines
	inline int wirePolylineCount() const
	{return m_WireData.verticeGroupCount();}

	//! Return the polyline offset from the given index
	inline GLuint wirePolylineOffset(int index) const
	{return m_WireData.verticeGroupOffset(index);}

	//! Return the polyline size from the given index
	inline GLsizei wirePolylineSize(int index) const
	{return m_WireData.verticeGroupSize(index);}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Clear the content of the geometry and makes it empty
	virtual void clear();

	//! Replace the Master material
	virtual void replaceMasterMaterial(Material*);

	//! Add material to the geometry
	void addMaterial(Material *);

	//! Set the color per vertex usage
	inline void colorPerVertex(const bool colorPerVertex)
	{
		if (m_UseColorPerVertex != colorPerVertex)
		{
			m_UseColorPerVertex= colorPerVertex;
			m_GeometryIsValid= false;
		}
	}

	//! Update the transparent material number
	void updateTransparentMaterialNumber();

	//! Reverse normal
	virtual void reverseNormals() {}

	//! Set the lod Index
	/*! The value must be between 0 and 100*/
	virtual void setCurrentLod(const int) {}

	//! Set Geometry Id
	inline void setId(const unsigned int id)
	{m_Id= id;}

	//! Set geometry name
	inline void setName(const std::string name)
	{m_Name= name;}

	//! Add a vertice group to the geometry and returns its id
	inline unsigned int addVerticeGroup(const GLfloatVector& vector)
	{return m_WireData.addVerticeGroup(vector);}

	//! Set Line width
	inline void setLineWidth(GLfloat lineWidth)
	{m_LineWidth= lineWidth;}

	//! Set this geometry wire color
	void setWireColor(const fvec& color);

	//! Copy VBO to the Client Side
	virtual void copyVboToClientSide();

	//! Release client VBO
	virtual void releaseVboClientSide(bool update= false);

//@}
//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Load each textures of materials
	virtual void glLoadTexture(void);

	//! Virtual interface for OpenGL execution.
	virtual void render(const RenderProperties&);


protected:
	//! Virtual interface for OpenGL Geometry set up.
	/*! This Virtual function have to be implemented in concrete class.*/
	virtual void glDraw(const RenderProperties&) = 0;

	//! Virtual interface for OpenGL Geometry properties.
	virtual void glPropGeom(const RenderProperties&);

//@}
//////////////////////////////////////////////////////////////////////
/*! \name Protected services Functions*/
//@{
//////////////////////////////////////////////////////////////////////
protected:
	//! Remove the specified material from the geometry
	void removeMaterial(unsigned int);

	//! Clear the wire data and the bounding box of this geometry
	inline void clearWireAndBoundingBox()
	{
		delete m_pBoundingBox;
		m_pBoundingBox= NULL;
		m_WireData.clear();
		m_GeometryIsValid= false;
	}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Private services Functions*/
//@{
//////////////////////////////////////////////////////////////////////
private:
	//! Clear the content of this object and makes it empty
	void clearGeometry();

//@}

//////////////////////////////////////////////////////////////////////
// Protected members
//////////////////////////////////////////////////////////////////////
protected:

	//! Geometry validity
	bool m_GeometryIsValid;

	//! Bounding box
	BoundingBox* m_pBoundingBox;

	//! Material Hash table
	MaterialHash m_MaterialHash;

	//! Color per vertex usage
	bool m_UseColorPerVertex;

	//! Selection state
	bool m_IsSelected;

	//! Wire Data
	WireData m_WireData;

	//! The wire color
	fvec m_WireColor;

	//! The line width
	GLfloat m_LineWidth;


//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:

	//! Geometry type is wire
	bool m_IsWire;

	//! The number of transparent materials
	int m_TransparentMaterialNumber;

	//! The Unique id of an Geometry
	/*! Generated on creation*/
	unsigned int m_Id;

	//! Name of geometry
	std::string m_Name;
};

#endif /*GEOMETRY_H_*/
