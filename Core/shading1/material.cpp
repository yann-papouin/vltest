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

//! \file material.cpp implementation of the Material class.

#include "material.h"
#include "../geometry/geometry.h"
#include "../factory.h"

#include <gl/GLU.h>

// Class chunk id
unsigned int Material::m_ChunkId= 0xA703;

//////////////////////////////////////////////////////////////////////
// Constructor Destructor
//////////////////////////////////////////////////////////////////////
// Default constructor
Material::Material()
:vl::Object("Material")
, m_AmbientColor()
, m_DiffuseColor()
, m_SpecularColor()
, m_EmissiveColor()
, m_Shininess(50.0)		// By default shininess 50
, m_WhereUsed()
, m_OtherUsage()
, m_pTexture(NULL)			// no texture
, m_Opacity(1.0)
{
	//qDebug() << "Material::Material" << id();
	// Diffuse Color
	initDiffuseColor();

	// Others
	initOtherColor();
}

Material::Material(const fvec &diffuseColor)
:vl::Object("Material")
, m_AmbientColor()
, m_DiffuseColor(diffuseColor)
, m_SpecularColor()
, m_EmissiveColor()
, m_Shininess(50.0)		// By default shininess 50
, m_WhereUsed()
, m_OtherUsage()
, m_pTexture(NULL)			// no texture
, m_Opacity(1.0)
{
	// Others
	initOtherColor();
}


Material::Material(const std::string& name ,const GLfloat *pDiffuseColor)
:vl::Object(name)
, m_AmbientColor()
, m_DiffuseColor()
, m_SpecularColor()
, m_EmissiveColor()
, m_Shininess(50.0)		// By default shininess 50
, m_WhereUsed()
, m_OtherUsage()
, m_pTexture(NULL)			// no texture
, m_Opacity(1.0)
{
	//qDebug() << "Material::Material" << id();
	// Init Diffuse Color
	if (pDiffuseColor != 0)
	{
		m_DiffuseColor.setRgbF(static_cast<double>(pDiffuseColor[0]),
								static_cast<double>(pDiffuseColor[1]),
								static_cast<double>(pDiffuseColor[2]),
								static_cast<double>(pDiffuseColor[3]));
	}
	else
	{
		initDiffuseColor();
	}
	// Others
	initOtherColor();
}
Material::Material(Texture* pTexture, const std::string& name)
:vl::Object(name)
, m_AmbientColor()
, m_DiffuseColor()
, m_SpecularColor()
, m_EmissiveColor()
, m_Shininess(50.0)		// By default shininess 50
, m_WhereUsed()
, m_OtherUsage()
, m_pTexture(pTexture)			// init texture
, m_Opacity(1.0)
{
	assert(NULL != m_pTexture);
	//qDebug() << "Material::Material" << id();

	// Diffuse Color
	initDiffuseColor();

	// Others
	initOtherColor();

	//if (m_pTexture->hasAlphaChannel()) m_Transparency= 0.99;
}

// Copy constructor
Material::Material(const Material &InitMaterial)
:vl::Object(InitMaterial)
, m_AmbientColor(InitMaterial.m_AmbientColor)
, m_DiffuseColor(InitMaterial.m_DiffuseColor)
, m_SpecularColor(InitMaterial.m_SpecularColor)
, m_EmissiveColor(InitMaterial.m_EmissiveColor)
, m_Shininess(InitMaterial.m_Shininess)
, m_WhereUsed()
, m_OtherUsage()
, m_pTexture(NULL)
, m_Opacity(InitMaterial.m_Opacity)
{
	//qDebug() << "Material::Material copy constructor" << id();
	if (NULL != InitMaterial.m_pTexture)
	{
		m_pTexture= new Texture(*(InitMaterial.m_pTexture));
		assert(m_pTexture != NULL);
	}

}

// Destructor
Material::~Material(void)
{
   	delete m_pTexture;
}


//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////
// Return the class Chunk ID
unsigned int Material::chunckID()
{
	return m_ChunkId;
}

// Get Ambiant color
dvec4 Material::ambientColor() const
{
	return m_AmbientColor;
}

// Get diffuse color
dvec4 Material::diffuseColor() const
{
	return m_DiffuseColor;
}

// Get specular color
dvec4 Material::specularColor() const
{
	return m_SpecularColor;
}

// Get the emissive color
dvec4 Material::emissiveColor() const
{
	return m_EmissiveColor;
}
// Get the texture File Name
std::string Material::textureFileName() const
{
	if (m_pTexture != NULL)
	{
		return m_pTexture->fileName();
	}
	else
	{
		return "";
	}
}

// Get Texture Id
GLuint Material::textureID() const
{
	if (m_pTexture != NULL)
	{
		return m_pTexture->GL_ID();
	}
	else
	{
		return 0;
	}

}

// return true if the texture is loaded
bool Material::textureIsLoaded() const
{
	if (m_pTexture != NULL)
	{
		return m_pTexture->isLoaded();
	}
	else
	{
		return false;
	}
}

// Return true if materials are equivalent
bool Material::operator==(const Material& mat) const
{
	bool result;
	if (this == &mat)
	{
		result= true;
	}
	else
	{
		result= m_AmbientColor == mat.m_AmbientColor;
		result= result && (m_DiffuseColor == mat.m_DiffuseColor);
		result= result && (m_SpecularColor == mat.m_SpecularColor);
		result= result && (m_EmissiveColor == mat.m_EmissiveColor);
		result= result && (m_Shininess == mat.m_Shininess);
		if ((NULL != m_pTexture) && (NULL != mat.m_pTexture))
		{
			result= result && ((*m_pTexture) == (*mat.m_pTexture));
		}
		else
		{
			result= result && (m_pTexture == mat.m_pTexture);
		}
		result= result && (m_Opacity == mat.m_Opacity);
	}
	return result;
}

// Return the material hash code
uint Material::hashCode() const
{
	std::string stringKey= std::string::number(m_AmbientColor.rgba());
	stringKey+= std::string::number(m_DiffuseColor.rgba());
	stringKey+= std::string::number(m_SpecularColor.rgba());
	stringKey+= std::string::number(m_EmissiveColor.rgba());
	stringKey+= std::string::number(m_Shininess);
	stringKey+= std::string::number(m_Opacity);
	if (NULL != m_pTexture)
	{
		stringKey+= m_pTexture->fileName();
	}

	return qHash(stringKey);
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////
// Set Material properties
 void Material::setMaterial(const Material* pMat)
 {
	if (NULL != pMat->m_pTexture)
	{
		Texture* pTexture= new Texture(*(pMat->m_pTexture));
		setTexture(pTexture);
	}
	else if (NULL != m_pTexture)
	{
		qDebug() << "Delete texture";
		delete m_pTexture;
		m_pTexture= NULL;
	}
	// Ambient Color
	m_AmbientColor= pMat->m_AmbientColor;
	// Diffuse Color
	m_DiffuseColor= pMat->m_DiffuseColor;
	// Specular Color
	m_SpecularColor= pMat->m_SpecularColor;
	// Lighting emit
	m_EmissiveColor= pMat->m_EmissiveColor;
 	// Shininess
 	m_Shininess= pMat->m_Shininess;
 	// Transparency
 	m_Opacity= pMat->m_Opacity;
	// Update geometry which use this material
	WhereUsed::const_iterator iGeom= m_WhereUsed.constBegin();
	while (iGeom != m_WhereUsed.constEnd())
	{
		iGeom.value()->updateTransparentMaterialNumber();
		++iGeom;
	}

 }

// Set Ambiant Color
void Material::setAmbientColor(const fvec& ambientColor)
{
	m_AmbientColor= ambientColor;
	m_AmbientColor.setAlphaF(m_Opacity);
}

// Set Diffuse color
void Material::setDiffuseColor(const fvec& diffuseColor)
{
	m_DiffuseColor= diffuseColor;
	m_DiffuseColor.setAlphaF(m_Opacity);
}

// Set Specular color
void Material::setSpecularColor(const fvec& specularColor)
{
	m_SpecularColor= specularColor;
	m_SpecularColor.setAlphaF(m_Opacity);
}

// Set Emissive
void Material::setEmissiveColor(const fvec& lightEmission)
{
	m_EmissiveColor= lightEmission;
	m_EmissiveColor.setAlphaF(m_Opacity);
}

// Set Texture
void Material::setTexture(Texture* pTexture)
{
	assert(NULL != pTexture);
	//qDebug() << "Material::SetTexture";
	if (m_pTexture != NULL)
	{
		delete m_pTexture;
		m_pTexture= pTexture;
		glLoadTexture();
	}
	else
	{
		// It is not sure that there is OpenGL context
		m_pTexture= pTexture;
	}

	//if (m_pTexture->hasAlphaChannel()) m_Transparency= 0.99;
}

// remove Material Texture
void Material::removeTexture()
{
	if (m_pTexture != NULL)
	{
		delete m_pTexture;
		m_pTexture= NULL;
	}
}

// Add Geometry to where used hash table
bool Material::addGeom(Geometry* pGeom)
{
	QMutexLocker mutexLocker(&m_Mutex);
	//qDebug() << "Material::addGeom" << pGeom->id();
	WhereUsed::iterator iGeom= m_WhereUsed.find(pGeom->id());

	if (iGeom == m_WhereUsed.end())
	{	// Ok, ID doesn't exist
		// Add Geometry to where used hash table
		m_WhereUsed.insert(pGeom->id(), pGeom);
		return true;
	}
	else
	{	// KO, ID exist
		qDebug("Material::addGeom : Geometry not added");
		return false;
	}
}

// Remove a geometry from the collection
bool Material::delGeom(uint Key)
{
	QMutexLocker mutexLocker(&m_Mutex);

	if (m_WhereUsed.contains(Key))
	{	// Ok, ID exist
		m_WhereUsed.remove(Key);	// Remove container

		return true;
	}
	else
	{	// KO doesn't exist
		qDebug("Material::delGeom : Geometry not remove");
		return false;
	}

}
// Add the id to the other used Set
bool Material::addUsage(uint id)
{
	QMutexLocker mutexLocker(&m_Mutex);
	if (!m_OtherUsage.contains(id))
	{
		m_OtherUsage << id;
		return true;
	}
	else
	{
		qDebug("Material::addUsage : id not added");
		return false;
	}
}

// Remove the id to the other used Set
bool Material::delUsage(uint id)
{
	QMutexLocker mutexLocker(&m_Mutex);
	if (m_OtherUsage.contains(id))
	{
		m_OtherUsage.remove(id);
		return true;
	}
	else
	{
		qDebug() << "Material::delUsage : id not removed " << m_Uid;
		return false;
	}
}


// Set the material opacity
void Material::setOpacity(const double alpha)
{
	m_Opacity= alpha;
	m_AmbientColor.setAlphaF(m_Opacity);
	m_DiffuseColor.setAlphaF(m_Opacity);
	m_SpecularColor.setAlphaF(m_Opacity);
	m_EmissiveColor.setAlphaF(m_Opacity);
	// Update geometry which use this material
	WhereUsed::const_iterator iGeom= m_WhereUsed.constBegin();
	while (iGeom != m_WhereUsed.constEnd())
	{
		iGeom.value()->updateTransparentMaterialNumber();
		++iGeom;
	}
}

//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////

// Load the texture
void Material::glLoadTexture(QGLContext* pContext)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->glLoadTexture(pContext);
	}
	else
	{
		qDebug() << "Material::glLoadTexture : Material without texture !";
	}
}

// Execute OpenGL Material
void Material::glExecute()
{

	GLfloat pAmbientColor[4]= {ambientColor().redF(),
								ambientColor().greenF(),
								ambientColor().blueF(),
								ambientColor().alphaF()};

	GLfloat pDiffuseColor[4]= {diffuseColor().redF(),
								diffuseColor().greenF(),
								diffuseColor().blueF(),
								diffuseColor().alphaF()};

	GLfloat pSpecularColor[4]= {specularColor().redF(),
								specularColor().greenF(),
								specularColor().blueF(),
								specularColor().alphaF()};

	GLfloat pLightEmission[4]= {emissiveColor().redF(),
								emissiveColor().greenF(),
								emissiveColor().blueF(),
								emissiveColor().alphaF()};

	if (m_pTexture != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		m_pTexture->glcBindTexture();
		if (State::glslUsed())
		{
			if (Shader::hasActiveShader())
			{
				Shader::currentShaderHandle()->programShaderHandle()->setUniformValue("tex", GLint(0));
				Shader::currentShaderHandle()->programShaderHandle()->setUniformValue("useTexture", true);
			}
		}

	}
	else
	{

		if (State::glslUsed() && Shader::hasActiveShader())
		{
				glEnable(GL_TEXTURE_2D);
				Shader::currentShaderHandle()->programShaderHandle()->setUniformValue("tex", GLint(0));
				Shader::currentShaderHandle()->programShaderHandle()->setUniformValue("useTexture", false);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}

	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pAmbientColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pDiffuseColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pSpecularColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pLightEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_Shininess);

	glColor4fv(pDiffuseColor);

	// OpenGL Error handler
	GLenum errCode;
	if ((errCode= glGetError()) != GL_NO_ERROR)
	{
		const GLubyte* errString;
		errString = gluErrorString(errCode);
		qDebug("Material::GlExecute OpenGL Error %s", errString);
	}
}

// Execute OpenGL Material
void Material::glExecute(float overwriteTransparency)
{
	GLfloat pAmbientColor[4]= {ambientColor().redF(),
								ambientColor().greenF(),
								ambientColor().blueF(),
								overwriteTransparency};

	GLfloat pDiffuseColor[4]= {diffuseColor().redF(),
								diffuseColor().greenF(),
								diffuseColor().blueF(),
								overwriteTransparency};

	GLfloat pSpecularColor[4]= {specularColor().redF(),
								specularColor().greenF(),
								specularColor().blueF(),
								overwriteTransparency};

	GLfloat pLightEmission[4]= {emissiveColor().redF(),
								emissiveColor().greenF(),
								emissiveColor().blueF(),
								overwriteTransparency};

	if (m_pTexture != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		m_pTexture->glcBindTexture();
		if (State::glslUsed())
		{
			if (Shader::hasActiveShader())
			{
				Shader::currentShaderHandle()->programShaderHandle()->setUniformValue("tex", GLint(0));
				Shader::currentShaderHandle()->programShaderHandle()->setUniformValue("useTexture", true);
			}
		}
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		if (State::glslUsed())
		{
			if (Shader::hasActiveShader())
			{
				Shader::currentShaderHandle()->programShaderHandle()->setUniformValue("tex", GLint(0));
				Shader::currentShaderHandle()->programShaderHandle()->setUniformValue("useTexture", false);
			}
		}
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pAmbientColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pDiffuseColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pSpecularColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pLightEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_Shininess);

	glColor4fv(pDiffuseColor);

	// OpenGL Error handler
	GLenum errCode;
	if ((errCode= glGetError()) != GL_NO_ERROR)
	{
		const GLubyte* errString;
		errString = gluErrorString(errCode);
		qDebug("Material::glExecute(float) OpenGL Error %s", errString);
	}
}

//////////////////////////////////////////////////////////////////////
// Private servicies Functions
//////////////////////////////////////////////////////////////////////

// Init Ambiant Color
void Material::initDiffuseColor(void)
{
	m_DiffuseColor.setRgbF(1.0, 1.0, 1.0, 1.0);
}

// Init default color
void Material::initOtherColor(void)
{
	//Ambiant Color
	m_AmbientColor.setRgbF(0.8, 0.8, 0.8, 1.0);

	// Specular Color
	m_SpecularColor.setRgbF(0.5, 0.5, 0.5, 1.0);

	// Lighting emit
	m_EmissiveColor.setRgbF(0.0, 0.0, 0.0, 1.0);
}

// Non Member methods
// Non-member stream operator
QDataStream &operator<<(QDataStream &stream, const Material &material)
{
	unsigned int chunckId= Material::m_ChunkId;
	stream << chunckId;

	// Store vl::Object class members
	stream << material.id() << material.name();

	// Store Material class members
	stream << material.ambientColor() << material.diffuseColor() << material.specularColor();
	stream << material.emissiveColor() << material.shininess() << material.opacity();

	// Test if the material has texture
	bool hasTexture= material.hasTexture();
	stream << hasTexture;
	if (hasTexture)
	{
		Texture texture(*(material.textureHandle()));
		stream << texture;
	}

	return stream;
}
QDataStream &operator>>(QDataStream &stream, Material &material)
{
	unsigned int chunckId;
	stream >> chunckId;

	assert(chunckId == Material::m_ChunkId);

	// Retrieve vl::Object members
	uint id;
	std::string name;
	stream >> id >> name;
	material.setId(id);
	material.setName(name);

	// Retrieve Material members
	fvec ambient, diffuse, specular, lightEmission;
	float shininess;
	double alpha;
	stream >> ambient >> diffuse >> specular >> lightEmission;
	stream >> shininess >> alpha;
	material.setAmbientColor(ambient);
	material.setDiffuseColor(diffuse);
	material.setSpecularColor(specular);
	material.setEmissiveColor(lightEmission);
	material.setShininess(shininess);
	material.setOpacity(alpha);

	// Test if material has texture
	bool hasTexture;
	stream >> hasTexture;
	if (hasTexture)
	{
		Texture texture;
		stream >> texture;
		material.setTexture(new Texture(texture));
	}
	return stream;
}
