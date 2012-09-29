//! \file glc_instance.cpp implementation of the ViewInstance class.

#include "struct/viewinstance.h"
#include "vlCore/Vector3.hpp"

//! The global default LOD
int ViewInstance::m_GlobalDefaultLOD= 10;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Default constructor
ViewInstance::ViewInstance()
: GLC_Object()
, m_3DRep()
, m_pBoundingBox(NULL)
, m_AbsoluteMatrix()
, m_IsBoundingBoxValid(false)
, m_RenderProperties()
, m_IsVisible(true)
, m_DefaultLOD(m_GlobalDefaultLOD)
, m_ViewableFlag(ViewInstance::FullViewable)
, m_ViewableGeomFlag()
{
	// Encode Color Id
//	glc::encodeRgbId(m_Uid, m_colorId);
}

// Contruct instance with a geometry
ViewInstance::ViewInstance(GLC_Geometry* pGeom)
: GLC_Object()
, m_3DRep(pGeom)
, m_pBoundingBox(NULL)
, m_AbsoluteMatrix()
, m_IsBoundingBoxValid(false)
, m_RenderProperties()
, m_IsVisible(true)
, m_DefaultLOD(m_GlobalDefaultLOD)
, m_ViewableFlag(ViewInstance::FullViewable)
, m_ViewableGeomFlag()
{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

	setName(m_3DRep.name());

	//qDebug() << "ViewInstance::ViewInstance ID = " << m_Uid;
	//qDebug() << "Number of instance" << (*m_pNumberOfInstance);
}

// Contruct instance with a 3DRep
ViewInstance::ViewInstance(const Rep& rep)
: GLC_Object()
, m_3DRep(rep)
, m_pBoundingBox(NULL)
, m_AbsoluteMatrix()
, m_IsBoundingBoxValid(false)
, m_RenderProperties()
, m_IsVisible(true)
, m_DefaultLOD(m_GlobalDefaultLOD)
, m_ViewableFlag(ViewInstance::FullViewable)
, m_ViewableGeomFlag()
{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

	setName(m_3DRep.name());

	//qDebug() << "ViewInstance::ViewInstance ID = " << m_Uid;
	//qDebug() << "Number of instance" << (*m_pNumberOfInstance);
}

// Copy constructor
ViewInstance::ViewInstance(const ViewInstance& inputNode)
: GLC_Object(inputNode)
, m_3DRep(inputNode.m_3DRep)
, m_pBoundingBox(NULL)
, m_AbsoluteMatrix(inputNode.m_AbsoluteMatrix)
, m_IsBoundingBoxValid(inputNode.m_IsBoundingBoxValid)
, m_RenderProperties(inputNode.m_RenderProperties)
, m_IsVisible(inputNode.m_IsVisible)
, m_DefaultLOD(inputNode.m_DefaultLOD)
, m_ViewableFlag(inputNode.m_ViewableFlag)
, m_ViewableGeomFlag(inputNode.m_ViewableGeomFlag)
{
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

	if (NULL != inputNode.m_pBoundingBox)
	{
		m_pBoundingBox= new AABB(*inputNode.m_pBoundingBox);
	}
}


// Assignement operator
ViewInstance& ViewInstance::operator=(const ViewInstance& inputNode)
{
	if (this != &inputNode)
	{
		// Clear this instance
		clear();
		GLC_Object::operator=(inputNode);
		// Encode Color Id
		glc::encodeRgbId(m_Uid, m_colorId);

		m_3DRep= inputNode.m_3DRep;
		if (NULL != inputNode.m_pBoundingBox)
		{
			m_pBoundingBox= new AABB(*inputNode.m_pBoundingBox);
		}
		m_AbsoluteMatrix= inputNode.m_AbsoluteMatrix;
		m_IsBoundingBoxValid= inputNode.m_IsBoundingBoxValid;
		m_RenderProperties= inputNode.m_RenderProperties;
		m_IsVisible= inputNode.m_IsVisible;
		m_DefaultLOD= inputNode.m_DefaultLOD;
		m_ViewableFlag= inputNode.m_ViewableFlag;
		m_ViewableGeomFlag= inputNode.m_ViewableGeomFlag;

		//qDebug() << "ViewInstance::operator= :ID = " << m_Uid;
		//qDebug() << "Number of instance" << (*m_pNumberOfInstance);
	}

	return *this;
}

// Destructor
ViewInstance::~ViewInstance()
{
	clear();
}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Get the bounding box
AABB ViewInstance::boundingBox(void)
{
	AABB resultBox;
	if (boundingBoxValidity())
	{
		resultBox= *m_pBoundingBox;
	}
	else if (!m_3DRep.isEmpty())
	{
		computeBoundingBox();
		m_IsBoundingBoxValid= true;
		resultBox= *m_pBoundingBox;
	}

	return resultBox;
}

//! Set the global default LOD value
void ViewInstance::setGlobalDefaultLod(int lod)
{
	QMutexLocker locker(&m_Mutex);
	m_GlobalDefaultLOD= lod;
}

// Clone the instance
ViewInstance ViewInstance::deepCopy() const
{

	Rep* pRep= dynamic_cast<Rep*>(m_3DRep.deepCopy());
	Rep newRep(*pRep);
	delete pRep;
	ViewInstance cloneInstance(newRep);

	if (NULL != m_pBoundingBox)
	{
		cloneInstance.m_pBoundingBox= new AABB(*m_pBoundingBox);
	}

	cloneInstance.m_AbsoluteMatrix= m_AbsoluteMatrix;
	cloneInstance.m_IsBoundingBoxValid= m_IsBoundingBoxValid;
	cloneInstance.m_RenderProperties= m_RenderProperties;
	cloneInstance.m_IsVisible= m_IsVisible;
	cloneInstance.m_ViewableFlag= m_ViewableFlag;
	return cloneInstance;
}

// Instanciate the instance
ViewInstance ViewInstance::instanciate()
{
	ViewInstance instance(*this);
	instance.m_Uid= glc::GLC_GenID();
	// Encode Color Id
	glc::encodeRgbId(m_Uid, m_colorId);

	return instance;
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////


// Set the instance Geometry
bool ViewInstance::setGeometry(GLC_Geometry* pGeom)
{
	if (m_3DRep.contains(pGeom))
	{
		return false;
	}
	else
	{
		m_3DRep.addGeom(pGeom);
		return true;
	}
}

// Instance translation
ViewInstance& ViewInstance::translate(double Tx, double Ty, double Tz)
{
	multMatrix(GLC_Matrix4x4(Tx, Ty, Tz));

	return *this;
}


// Move instance with a 4x4Matrix
ViewInstance& ViewInstance::multMatrix(const GLC_Matrix4x4 &MultMat)
{
	m_AbsoluteMatrix= MultMat * m_AbsoluteMatrix;
	m_IsBoundingBoxValid= false;

	return *this;
}

// Replace the instance Matrix
ViewInstance& ViewInstance::setMatrix(const GLC_Matrix4x4 &SetMat)
{
	m_AbsoluteMatrix= SetMat;
	m_IsBoundingBoxValid= false;

	return *this;
}

// Reset the instance Matrix
ViewInstance& ViewInstance::resetMatrix(void)
{
	m_AbsoluteMatrix.setToIdentity();
	m_IsBoundingBoxValid= false;

	return *this;
}

//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////

// Display the instance
void ViewInstance::render(glc::RenderFlag renderFlag, bool useLod, vl::Camera* pCamera)
{
	//qDebug() << "ViewInstance::render render properties= " << m_RenderProperties.renderingMode();
	if (m_3DRep.isEmpty()) return;
	const int bodyCount= m_3DRep.numberOfBody();

	if (bodyCount != m_ViewableGeomFlag.size())
	{
		m_ViewableGeomFlag.fill(true, bodyCount);
	}

	m_RenderProperties.setRenderingFlag(renderFlag);

	// Save current OpenGL Matrix
	glPushMatrix();
	OpenglVisProperties();

	// Change front face orientation if this instance absolute matrix is indirect
	if (m_AbsoluteMatrix.type() == GLC_Matrix4x4::Indirect)
	{
		glFrontFace(GL_CW);
	}
	if(GLC_State::isInSelectionMode())
	{
		glColor3ubv(m_colorId); // D'ont use Alpha component
	}

	if (useLod && (NULL != pCamera))
	{
		for (int i= 0; i < bodyCount; ++i)
		{
			if (m_ViewableGeomFlag.at(i))
			{
				const int lodValue= choseLod(m_3DRep.geomAt(i)->boundingBox(), pCamera, useLod);
				if (lodValue <= 100)
				{
					m_3DRep.geomAt(i)->setCurrentLod(lodValue);
					m_RenderProperties.setCurrentBodyIndex(i);
					m_3DRep.geomAt(i)->render(m_RenderProperties);
				}
			}
		}
	}
	else
	{
		for (int i= 0; i < bodyCount; ++i)
		{
			if (m_ViewableGeomFlag.at(i))
			{
				int lodValue= 0;
				if (GLC_State::isPixelCullingActivated() && (NULL != pCamera))
				{
					lodValue= choseLod(m_3DRep.geomAt(i)->boundingBox(), pCamera, useLod);
				}

				if (lodValue <= 100)
				{
					m_3DRep.geomAt(i)->setCurrentLod(m_DefaultLOD);
					m_RenderProperties.setCurrentBodyIndex(i);
					m_3DRep.geomAt(i)->render(m_RenderProperties);
				}
			}
		}
	}
	// Restore OpenGL Matrix
	glPopMatrix();

	// Restore front face orientation if this instance absolute matrix is indirect
	if (m_AbsoluteMatrix.type() == GLC_Matrix4x4::Indirect)
	{
		glFrontFace(GL_CCW);
	}

}

// Display the instance in Body selection mode
void ViewInstance::renderForBodySelection()
{
	Q_ASSERT(GLC_State::isInSelectionMode());
	if (m_3DRep.isEmpty()) return;

	// Save previous rendering mode and set the rendering mode to BodySelection
	glc::RenderMode previousRenderMode= m_RenderProperties.renderingMode();
	m_RenderProperties.setRenderingMode(glc::BodySelection);

	// Save current OpenGL Matrix
	glPushMatrix();
	OpenglVisProperties();

	GLubyte colorId[4];
	const int size= m_3DRep.numberOfBody();
	for (int i= 0; i < size; ++i)
	{
		GLC_Geometry* pGeom= m_3DRep.geomAt(i);
		glc::encodeRgbId(pGeom->id(), colorId);
		glColor3ubv(colorId);
		pGeom->setCurrentLod(m_DefaultLOD);
		m_RenderProperties.setCurrentBodyIndex(i);
		pGeom->render(m_RenderProperties);
	}

	// Restore rendering mode
	m_RenderProperties.setRenderingMode(previousRenderMode);
	// Restore OpenGL Matrix
	glPopMatrix();
}

// Display the instance in Primitive selection mode and return the body index
int ViewInstance::renderForPrimitiveSelection(unsigned int bodyId)
{
	Q_ASSERT(GLC_State::isInSelectionMode());
	if (m_3DRep.isEmpty()) return -1;
	// Save previous rendering mode and set the rendering mode to BodySelection
	glc::RenderMode previousRenderMode= m_RenderProperties.renderingMode();
	m_RenderProperties.setRenderingMode(glc::PrimitiveSelection);

	// Save current OpenGL Matrix
	glPushMatrix();
	OpenglVisProperties();

	const int size= m_3DRep.numberOfBody();
	int i= 0;
	bool continu= true;
	while ((i < size) && continu)
	{
		GLC_Geometry* pGeom= m_3DRep.geomAt(i);
		if (pGeom->id() == bodyId)
		{
			pGeom->setCurrentLod(0);
			pGeom->render(m_RenderProperties);
			continu= false;
		}
		else ++i;
	}

	m_RenderProperties.setRenderingMode(previousRenderMode);

	// Restore OpenGL Matrix
	glPopMatrix();

	return i;
}



//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////


// compute the instance bounding box
// m_pGeomList should be not null
void ViewInstance::computeBoundingBox(void)
{
	if (m_3DRep.isEmpty()) return;

	if (m_pBoundingBox != NULL)
	{
		delete m_pBoundingBox;
		m_pBoundingBox= NULL;
	}
	m_pBoundingBox= new AABB();
	const int size= m_3DRep.numberOfBody();
	for (int i= 0; i < size; ++i)
	{
		m_pBoundingBox->combine(m_3DRep.geomAt(i)->boundingBox());
	}

	m_pBoundingBox->transform(m_AbsoluteMatrix);
}

// Clear current instance
void ViewInstance::clear()
{

	delete m_pBoundingBox;
	m_pBoundingBox= NULL;

	// invalidate the bounding box
	m_IsBoundingBoxValid= false;

}

// Compute LOD
int ViewInstance::choseLod(const AABB& boundingBox, vl::Camera* pCamera, bool useLod)
{
	if (NULL == pCamera) return 0;
	double pixelCullingRatio= 0.0;
	if (useLod)
	{
//		pixelCullingRatio= pView->minimumDynamicPixelCullingRatio();
	}
	else
	{
//		pixelCullingRatio= pView->minimumStaticPixelCullingRatio();
	}

	const double diameter= boundingBox.boundingSphereRadius() * 2.0 * m_AbsoluteMatrix.scalingX();
	vl::vec3 center(m_AbsoluteMatrix * boundingBox.center());

	vl::vec3 eye,at,up,right;
	pCamera->getViewMatrixAsLookAt(eye,at,up,right);

	const double dist= (center - eye).length();

	double viewTangent = tan(glc::toRadian(pCamera->fov()));
	const double cameraCover= dist * viewTangent;

	double ratio= diameter / cameraCover * 100.0;
	if (ratio > 100.0) ratio= 100.0;
	ratio= 100.0 - ratio;

	if ((ratio > (100.0 - pixelCullingRatio)) && GLC_State::isPixelCullingActivated()) ratio= 110.0;
	else if(useLod && (ratio > 50.0))
	{
		ratio= (ratio - 50.0) / 50.0 * 100.0;
		if (ratio < static_cast<double>(m_DefaultLOD)) ratio= static_cast<double>(m_DefaultLOD);
	}
	else
	{
		ratio= static_cast<double>(m_DefaultLOD);
	}

	return static_cast<int>(ratio);
}


