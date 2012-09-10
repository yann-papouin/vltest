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

//! \file texture.cpp implementation of the Texture class.


#include "texture.h"
//#include "../exception.h"
//#include "../global.h"

// Quazip library
#include "../quazip/quazip.h"
#include "../quazip/quazipfile.h"

//#include <QtDebug>

// The default maximum texture size
size_t Texture::m_MaxTextureSize(676, 676);

// The Minimum texture size
const size_t Texture::m_MinTextureSize(10, 10);

stdext::hash_map<GLuint, int> Texture::m_TextureIdUsage;

//////////////////////////////////////////////////////////////////////
// Constructor Destructor
//////////////////////////////////////////////////////////////////////

//! Default constructor
Texture::Texture()
: m_pQGLContext(NULL)
, m_FileName()
, m_GlTextureID(0)
, m_textureImage()
, m_TextureSize()
, m_HasAlphaChannel(false)
{

}

// Constructor with fileName
Texture::Texture(const string &Filename)
: m_pQGLContext(NULL)
, m_FileName(Filename)
, m_GlTextureID(0)
, m_textureImage(loadFromFile(m_FileName))
, m_TextureSize()
, m_HasAlphaChannel(m_textureImage.hasAlphaChannel())
{
	if (m_textureImage.isNull())
	{
		string ErrorMess("Texture::Texture open image : ");
		ErrorMess.append(m_FileName).append(" Failed");
		qDebug() << ErrorMess;
		Exception e(ErrorMess);
		throw(e);
	}
}
// Constructor with QFile
Texture::Texture(const QFile &file)
: m_pQGLContext(NULL)
, m_FileName(file.fileName())
, m_GlTextureID(0)
, m_textureImage()
, m_TextureSize()
, m_HasAlphaChannel(m_textureImage.hasAlphaChannel())
{
	m_textureImage.load(const_cast<QFile*>(&file), QFileInfo(m_FileName).suffix().toLocal8Bit());
	if (m_textureImage.isNull())
	{
		string ErrorMess("Texture::Texture open image : ");
		ErrorMess.append(m_FileName).append(" Failed");
		qDebug() << ErrorMess;
		Exception e(ErrorMess);
		throw(e);
	}
}

// Constructor with QImage
Texture::Texture(const QImage& image, const string& fileName)
: m_pQGLContext(NULL)
, m_FileName(fileName)
, m_GlTextureID(0)
, m_textureImage(image)
, m_TextureSize()
, m_HasAlphaChannel(m_textureImage.hasAlphaChannel())
{
	Q_ASSERT(!m_textureImage.isNull());
}

Texture::Texture(const Texture &TextureToCopy)
: m_pQGLContext(TextureToCopy.m_pQGLContext)
, m_FileName(TextureToCopy.m_FileName)
, m_GlTextureID(TextureToCopy.m_GlTextureID)
, m_textureImage(TextureToCopy.m_textureImage)
, m_TextureSize(TextureToCopy.m_TextureSize)
, m_HasAlphaChannel(m_textureImage.hasAlphaChannel())
{
	if (m_textureImage.isNull())
	{
		string ErrorMess("Texture::Texture open image : ");
		ErrorMess.append(m_FileName).append(" Failed");
		qDebug() << ErrorMess;
		Exception e(ErrorMess);
		throw(e);
	}
	addThisOpenGLTextureId();

}

// Overload "=" operator
Texture& Texture::operator=(const Texture& texture)
{
	if (!(*this == texture))
	{
		removeThisOpenGLTextureId();
		m_pQGLContext= texture.m_pQGLContext;
		m_FileName= texture.m_FileName;
		m_GlTextureID= texture.m_GlTextureID;
		addThisOpenGLTextureId();
		m_textureImage= texture.m_textureImage;
		m_TextureSize= texture.m_TextureSize;
		m_HasAlphaChannel= m_textureImage.hasAlphaChannel();
	}

	return *this;
}

Texture::~Texture()
{
	//qDebug() << "Texture::~Texture Texture ID : " << m_GlTextureID;
	removeThisOpenGLTextureId();
}
//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Return true if texture are the same
bool Texture::operator==(const Texture& texture) const
{
	bool result;
	if (this == &texture)
	{
		result= true;
	}
	else
	{
		result= (m_FileName == texture.m_FileName) && (m_textureImage == texture.m_textureImage);
	}
	return result;
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Set the maximum texture size
void Texture::setMaxTextureSize(const size_t& size)
{
	if ((size.height() > m_MinTextureSize.height()) && (size.width() > m_MinTextureSize.width()))
	{
		m_MaxTextureSize= size;
	}
	else
	{
		qDebug() << "Texture::setMaxTextureSize m_MaxTextureSize set to m_MinTextureSize";
		m_MaxTextureSize= m_MinTextureSize;
	}
}

//////////////////////////////////////////////////////////////////////
// Private OpenGL functions
//////////////////////////////////////////////////////////////////////
// Load the texture
void Texture::glLoadTexture(QGLContext* pContext)
{
	if (m_GlTextureID == 0)
	{
		if (NULL == pContext)
		{
			m_pQGLContext= const_cast<QGLContext*>(QGLContext::currentContext());
		}
		else
		{
			m_pQGLContext= pContext;
		}

		// Test image size
		if ((m_textureImage.height() > m_MaxTextureSize.height())
				|| (m_textureImage.width() > m_MaxTextureSize.width()))
		{
			QImage rescaledImage;
			if(m_textureImage.height() > m_textureImage.width())
			{
				rescaledImage= m_textureImage.scaledToHeight(m_MaxTextureSize.height(), Qt::SmoothTransformation);
			}
			else
			{
				rescaledImage= m_textureImage.scaledToWidth(m_MaxTextureSize.width(), Qt::SmoothTransformation);
			}
			m_textureImage= rescaledImage;
			m_TextureSize= m_textureImage.size();
		}
		else
		{
			m_TextureSize= m_textureImage.size();
		}
		m_GlTextureID= m_pQGLContext->bindTexture(m_textureImage);
		addThisOpenGLTextureId();

		//qDebug() << "Texture::glcBindTexture Texture ID = " << m_GlTextureID;
	}
}

// Bind texture in 2D mode
void Texture::glcBindTexture(void)
{
	if (m_GlTextureID == 0)
	{
		glLoadTexture();
	}
	glBindTexture(GL_TEXTURE_2D, m_GlTextureID);
}

QImage Texture::loadFromFile(const string& fileName)
{
	QImage resultImage;
	if (glc::isArchiveString(fileName))
	{

		// Load the image from a zip archive
		QuaZip* p3dxmlArchive= new QuaZip(glc::archiveFileName(fileName));
		// Trying to load archive
		if(!p3dxmlArchive->open(QuaZip::mdUnzip))
		{
		  delete p3dxmlArchive;
		  return QImage();
		}
		else
		{
			// Set the file Name Codec
			p3dxmlArchive->setFileNameCodec("IBM866");
		}
		string imageFileName= glc::archiveEntryFileName(fileName);

		// Create QuaZip File
		QuaZipFile* p3dxmlFile= new QuaZipFile(p3dxmlArchive);

		// Get the file of the 3dxml
		if (!p3dxmlArchive->setCurrentFile(imageFileName, QuaZip::csInsensitive))
		{
			delete p3dxmlFile;
			delete p3dxmlArchive;
			return QImage();
		}

		// Open the file of the 3dxml
		if(!p3dxmlFile->open(QIODevice::ReadOnly))
	    {
			delete p3dxmlFile;
			delete p3dxmlArchive;
			return QImage();
	    }
		resultImage.load(p3dxmlFile, QFileInfo(imageFileName).suffix().toLocal8Bit());
		p3dxmlFile->close();
		delete p3dxmlFile;
		delete p3dxmlArchive;
	}
	else
	{
		resultImage.load(fileName);
	}

	return resultImage;
}

void Texture::removeThisOpenGLTextureId()
{
	if ( 0 != m_GlTextureID)
	{
		Q_ASSERT(m_TextureIdUsage.contains(m_GlTextureID));
		--m_TextureIdUsage[m_GlTextureID];
		if (m_TextureIdUsage.value(m_GlTextureID) == 0)
		{
			m_pQGLContext->deleteTexture(m_GlTextureID);
			m_TextureIdUsage.remove(m_GlTextureID);
			m_GlTextureID= 0;
		}
	}
}

void Texture::addThisOpenGLTextureId()
{
	if (0 != m_GlTextureID)
	{
		if (m_TextureIdUsage.contains(m_GlTextureID))
		{
			++m_TextureIdUsage[m_GlTextureID];
		}
		else
		{
			m_TextureIdUsage.insert(m_GlTextureID, 1);
		}
	}
}

// Non-member stream operator
QDataStream &operator<<(QDataStream &stream, const Texture &texture)
{
	stream << texture.fileName();

	return stream;
}
QDataStream &operator>>(QDataStream &stream, Texture &texture)
{
	string fileName;
	stream >> fileName;
	texture= Texture(fileName);

	return stream;
}

