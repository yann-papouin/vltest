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

//! \file material.h interface for the Material class.

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../config.h"
#include "vlCore/Object.hpp"
#include "vlCore/Vector4.hpp"

#include "texture.h"
#include <hash_map>
#include <set>

using namespace vl;

class Geometry;

typedef stdext::hash_map< unsigned int, Geometry*> WhereUsed;

//////////////////////////////////////////////////////////////////////
//! \class Material
/*! \brief Material : OpenGL surface material properties */

/*! An Material specifies surface material properties */
//////////////////////////////////////////////////////////////////////


class GVCORE_EXPORT Material : public vl::Object
{
	//friend QDataStream &operator<<(QDataStream &, const Material &);
	//friend QDataStream &operator>>(QDataStream &, Material &);

//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Construct Colored Material
	//! Default constructor
	Material();

	/*! By default, Ambiant Color is dark grey*/
	Material(const dvec4 &);

	/*! By default, Ambiant Color is dark grey*/
	Material(const std::string& name, const GLfloat *);

	//! Construct textured Material
	Material(Texture* pTexture, const std::string& name= std::string());

	//! Copy constructor
	/*! Hast usage table are not copying
	 */
	Material(const Material &InitMaterial);

	//! Remove material where used geometry
	virtual ~Material(void);
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the class Chunk ID
	static unsigned int chunckID();

	//! Return true if the material is used
	inline bool isUnused() const
	{return m_WhereUsed.empty() && m_OtherUsage.empty();}

	//! Return true is material has attached texture
	inline bool hasTexture() const
	{return m_pTexture != NULL;}

	//! Get Ambiant color
	dvec4 ambientColor() const;

	//! Get diffuse color
	dvec4 diffuseColor() const;

	//! Get specular color
	dvec4 specularColor() const;

	//! Get the emissive color
	dvec4 emissiveColor() const;

	//! Get Shininess
	inline GLfloat shininess() const
	{return m_Shininess;}

	//! Get the texture File Name
	std::string textureFileName() const;

	//! Get Texture Id
	GLuint textureID() const;

	//! return true if the texture is loaded
	bool textureIsLoaded() const;

	//! Return true if the material is transparent
	inline bool isTransparent() const
	{return  m_Opacity < 1.0;}

	//! Return true if materials are equivalent
	bool operator==(const Material&) const;

	//! Return the material opacity
	inline double opacity() const
	{return m_DiffuseColor().w();}

	//! Return the number of this material usage
	inline int numberOfUsage() const
	{return m_WhereUsed.size() + m_OtherUsage.size();}

	//! Return the texture handle
	inline Texture* textureHandle() const
	{return m_pTexture;}

	//! Return the material hash code
	unsigned int hashCode() const;

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Assignement operator
	/*! The Hash Table WhereUse
	 *  is not modified
	 */
	inline Material &operator=(const Material& mat)
	{
		setMaterial(&mat);
		return *this;
	}

	//! Set Material properties
	/*! The Hash Table WhereUse
	 *  is not modified
	 */
	 void setMaterial(const Material*);

	//! Set Ambiant Color
	void setAmbientColor(const dvec4& ambientColor);

	//! Set Diffuse color
	void setDiffuseColor(const dvec4& diffuseColor);

	//! Set Specular color
	void setSpecularColor(const dvec4& specularColor);

	//! Set Emissive
	void setEmissiveColor(const dvec4& lightEmission);

	//! Set Shininess
	inline void setShininess(GLfloat Shininess)
	{ m_Shininess= Shininess;}

	//! Set Texture
	void setTexture(Texture* pTexture);

	//! remove Material Texture
	void removeTexture();

	//! Add Geometry to the "where used" hash table
	/*! This method is thread safe*/
	bool addGeom(Geometry* pGeom);

	//! Remove Geometry to the "where used" hash table
	/*! This method is thread safe*/
	bool delGeom(unsigned int Key);

	//! Add the id to the other used Set
	/*! This method is thread safe*/
	bool addUsage(unsigned int);

	//! Remove the id to the other used Set
	/*! This method is thread safe*/
	bool delUsage(unsigned int);

	//! Set the material opacity
	void setOpacity(const double);

//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Load the texture
// 	void glLoadTexture(QGLContext* pContext= NULL);

	//! Execute OpenGL Material
	virtual void glExecute();

	//! Execute OpenGL Material with overWrite transparency
	virtual void glExecute(float);

//@}

//////////////////////////////////////////////////////////////////////
// Private services Functions
//////////////////////////////////////////////////////////////////////
private:
	//! Init Ambiant Color
	void initDiffuseColor(void);

	//! Init other color
	void initOtherColor(void);


//////////////////////////////////////////////////////////////////////
// Private Member
//////////////////////////////////////////////////////////////////////

private:

	//! Ambiant Color
	dvec4 m_AmbientColor;
	//! Diffuse Color
	dvec4 m_DiffuseColor;
	//! Specular Color
	dvec4 m_SpecularColor;

	//! emmisive lighting
	dvec4 m_EmissiveColor;

	//! Shiness
	GLfloat m_Shininess;

	//! Hash table of geomtries which used this material
	WhereUsed m_WhereUsed;

	//! Set of id of other objects that uses this material
	std::set<unsigned int> m_OtherUsage;

	//! Material's texture
	Texture* m_pTexture;

	//! Material opacity
	double m_Opacity;

	//! Class chunk id
	static unsigned int m_ChunkId;

};

//! Non-member stream operator
//QDataStream &operator<<(QDataStream &, const Material &);
//QDataStream &operator>>(QDataStream &, Material &);

#endif //MATERIAL_H_
