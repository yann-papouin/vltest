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

//! \file texture.h interface for the Texture class.

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../config.h"
#include <string>
#include <hash_map>

/////////////////////////////////////////////////////////////////////
//! \class Texture
/*! \brief Texture : Image texture */

/*! Image texture define a texture map in 2 D coordinate system*/
//////////////////////////////////////////////////////////////////////


class GVCORE_EXPORT Texture
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////

public:
	//! Default constructor
	void Texture();

	//! Constructor with fileName
	void Texture(const std::string&);

	//! Constructor with QFile
	//Texture(const QFile&);

	////! Constructor with QImage
	//Texture(const QImage&, const string& fileName= string());

	//! Copy constructor
	void Texture(const Texture& TextureToCopy);

	//! Overload "=" operator
	Texture& operator=(const Texture&);

	//! Default Destructor
	virtual ~Texture();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the QGLContext of the texture
	//inline QGLContext* context() const
	//{return m_pQGLContext;}

	//! Return the texture File Name
	inline string fileName() const
	{return m_FileName;}

	//! Return OpenGL Texture Id
	inline GLuint GL_ID() const
	{return m_GlTextureID;}

	//! Return true if the texture is loaded
	inline bool isLoaded() const
	{return (m_GlTextureID != 0);}

	//! Return the texture size
	inline size_t size() const
	{return m_TextureSize;}

	//! Return the maximum texture size
	static size_t maxSize()
	{return m_MaxTextureSize;}

	//! Return true if texture are the same
	bool operator==(const Texture&) const;

	//! Return true if the texture has alpha channel
	inline bool hasAlphaChannel() const
	{ return m_HasAlphaChannel;}

	//! Return the an image of the texture
	//inline QImage imageOfTexture() const
	//{ return m_textureImage;}


//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	// Set the maximum texture size
	static void setMaxTextureSize(const size_t&);

//@}
//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Load the texture
	//void glLoadTexture(QGLContext* pContext= NULL);
	//! Bind texture in 2D mode
	void glcBindTexture(void);


//////////////////////////////////////////////////////////////////////
/*! @name Private services functions */
//@{
//////////////////////////////////////////////////////////////////////
private:
	//! Load the image of this texture from the given fileName and return resutling image
	//QImage loadFromFile(const string& fileName);

	//! Remove this Opengl texture id
	void removeThisOpenGLTextureId();

	//! Add this Opengl texture id
	void addThisOpenGLTextureId();

//@}

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////

private:
	//! OpenGL Context
	//QGLContext *m_pQGLContext;

	//! Texture Name
	std::string m_FileName;

	//! OpenGL Texture ID
	GLuint	m_GlTextureID;

	//! QImage off the texture
	//QImage m_textureImage;

	//! Size of the texture
	size_t m_TextureSize;

	//! Flag to know if the texture has alpha channel
	bool m_HasAlphaChannel;

	//! Static member used to check texture size
	static size_t m_MaxTextureSize;
	static const size_t m_MinTextureSize;

	//! Static hash table to manage OpenGL testure ID
	static stdext::hash_map<GLuint, int> m_TextureIdUsage;
};

//! Non-member stream operator
//QDataStream &operator<<(QDataStream &, const Texture &);
//QDataStream &operator>>(QDataStream &, Texture &);


#endif //TEXTURE_H_
