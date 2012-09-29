//! \file glc_instance.h interface for the ViewInstance class.

#ifndef VIEWINSTANCE_H_
#define VIEWINSTANCE_H_


#include "vlGraphics/Camera.hpp"

class Camera;

//////////////////////////////////////////////////////////////////////
//! \class ViewInstance
/*! \brief ViewInstance : Rep + bounding box*/

/*! An ViewInstance contain  :
 * 		- Rep
 * 		- Geometry Bounding box in position
 * 		- Positionning 4 x 4 matrix
 */
//////////////////////////////////////////////////////////////////////

class ViewInstance : public GLC_Object
{
public:
	//! Viewable instance property
	enum Viewable
	{
		FullViewable= 120,
		PartialViewable= 121,
		NoViewable= 122
	};
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default constructor
	ViewInstance();

	//! Contruct instance with a geometry
	ViewInstance(ref<Geometry> pGeom);

	//! Contruct instance with a 3DRep
	ViewInstance(const Represent&);

	//! Copy constructor
	ViewInstance(const ViewInstance& );

	//! Assignement operator
	ViewInstance &operator=(const ViewInstance&);

	//! Destructor
	~ViewInstance();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return true if the all instance's mesh are transparent
	inline bool isTransparent() const;

	//! Return true if the instance contains mesh which contains transparent material
	inline bool hasTransparentMaterials() const;

	//! Return true if the instance as no geometry
	inline bool isEmpty() const
	{return m_3DRep.isEmpty();}

	//! Return true if the instance is selected
	inline bool isSelected(void) const
	{return m_RenderProperties.isSelected();}

	//! Return the number of geometry
	inline int numberOfGeometry() const
	{return m_3DRep.numberOfBody();}


	//! Return the geometry at the specified position
	inline GLC_Geometry* geomAt(int index) const
	{
		if (!m_3DRep.isEmpty()) return m_3DRep.geomAt(index);
		else return NULL;
	}

	//! Get the bounding box
	AABB boundingBox();

	//! Get the validity of the Bounding Box
	inline bool boundingBoxValidity() const
	{return (m_pBoundingBox != NULL) && m_IsBoundingBoxValid && m_3DRep.boundingBoxIsValid();}

	//! Return transfomation 4x4Matrix
	inline const GLC_Matrix4x4& matrix() const
	{return m_AbsoluteMatrix;}

	//! Make a deep copy of the instance
	ViewInstance deepCopy() const;

	//! Instanciate the instance
	ViewInstance instanciate();

	//! Get the Polygon mode off the instance
	/*! Polygon Mode can Be : GL_POINT, GL_LINE, or GL_FILL*/
	inline GLenum polygonMode() const
	{return m_RenderProperties.polygonMode();}

	//! Return an handle to the renderProperties
	inline GLC_RenderProperties* renderPropertiesHandle()
	{return &m_RenderProperties;}

	//! Get the visibility state of instance
	inline bool isVisible() const
	{return m_IsVisible;}

	//! Return true if the instance is viewable
	inline ViewInstance::Viewable viewableFlag() const
	{return m_ViewableFlag;}

	//! Return true if the geom at the index is viewable
	inline bool isGeomViewable(int index) const
	{return m_ViewableGeomFlag.at(index);}

	//! Get number of faces
	inline unsigned int numberOfFaces() const
	{return m_3DRep.faceCount();}

	//! Get number of vertex
	inline unsigned int numberOfVertex() const
	{return m_3DRep.vertexCount();}

	//! Get number of materials
	inline unsigned int numberOfMaterials() const
	{return m_3DRep.materialCount();}

	//! Get materials List
	inline QSet<GLC_Material*> materialSet() const
	{return m_3DRep.materialSet();}

	//! Return the default LOD Value
	inline int defaultLodValue() const
	{return m_DefaultLOD;}

	//! Return the instance representation
	inline Represent representation() const
	{return m_3DRep;}

	//! Return the number of body contains in the 3DRep
	inline int numberOfBody() const
	{return m_3DRep.numberOfBody();}

	//! Return the global default LOD value
	inline static int globalDefaultLod()
	{
		return m_GlobalDefaultLOD;
	}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Set the instance Geometry
	/*!
	 *  instance must be null
	 */
	bool setGeometry(GLC_Geometry* pGeom);

	//! Remove empty geometries
	inline void removeEmptyGeometry()
	{m_3DRep.clean();}

	//! Reverse geometry normals
	inline void reverseGeometriesNormals()
	{m_3DRep.reverseNormals();}

	//! Translate Instance
	ViewInstance& translate(double Tx, double Ty, double Tz);

	//! Translate Instance
	inline ViewInstance& translate(const GLC_Vector3d& v)
	{
		return translate(v.x(), v.y(), v.z());
	}

	//! Move instance with a 4x4Matrix
	ViewInstance& multMatrix(const GLC_Matrix4x4 &MultMat);

	//! Replace the instance Matrix
	ViewInstance& setMatrix(const GLC_Matrix4x4 &SetMat);

	//! Reset the instance Matrix
	ViewInstance& resetMatrix(void);

	//! Polygon's display style
	/*! Face Polygon Mode can be : GL_FRONT_AND_BACK, GL_FRONT, or GL_BACK
	 *  mode can be : GL_POINT, GL_LINE, or GL_FILL */
	inline void setPolygonMode(GLenum Face, GLenum Mode)
	{m_RenderProperties.setPolygonMode(Face, Mode);}

	//! Select the instance
	inline void select(bool primitive)
	{m_RenderProperties.select(primitive);}

	//! Unselect the instance
	inline void unselect(void)
	{m_RenderProperties.unselect();}

	//! Set instance visibility
	inline void setVisibility(bool visibility)
	{m_IsVisible= visibility;}

	//! Set Instance Id
// 	inline void setId(const unsigned int id)
// 	{
// 		GLC_Object::setId(id);
// 		glc::encodeRgbId(m_Uid, m_colorId);
// 	}

	//! Set the default LOD value
	inline void setDefaultLodValue(int lod)
	{
		m_DefaultLOD= lod;
	}

	//! Set the viewable flag
	inline bool setViewable(ViewInstance::Viewable flag);

	//! Set the viewable flag of a geometry
	inline void setGeomViewable(int index, bool flag)
	{m_ViewableGeomFlag[index]= flag;}


	//! Set the global default LOD value
	static void setGlobalDefaultLod(int);

	//! Set the renderProperties of this 3DView instance
	inline void setRenderProperties(const GLC_RenderProperties& renderProperties)
	{m_RenderProperties= renderProperties;}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Display the instance
	void render(glc::RenderFlag renderFlag= glc::ShadingFlag, bool useLoad= false, vl::Camera* pCamera= NULL);

	//! Display the instance in Body selection mode
	void renderForBodySelection();

	//! Display the instance in Primitive selection mode of the specified body id and return the body index
	int renderForPrimitiveSelection(unsigned int);


private:
	//! Set instance visualisation properties
	inline void OpenglVisProperties()
	{
		// Polygons display mode
		glPolygonMode(m_RenderProperties.polyFaceMode(), m_RenderProperties.polygonMode());
		// Change the current matrix
		glMultMatrixd(m_AbsoluteMatrix.getData());
	}


//@}

//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////
private:
	//! compute the instance bounding box
	void computeBoundingBox(void);

	//! Clear current instance
	void clear();

	//! Compute LOD
	int choseLod(const AABB&, vl::Camera*, bool);

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:

	//! The 3D rep of the instance
	Represent m_3DRep;

	//! BoundingBox of the instance
	AABB* m_pBoundingBox;

	//! Geometry matrix
	vl::dmat4 m_AbsoluteMatrix;

	//! Bounding box validity
	bool m_IsBoundingBoxValid;

	//! The 3DViewInstance rendering properties
	RenderProperties m_RenderProperties;

	//! Visibility
	bool m_IsVisible;

	//! The instance color ID
	GLubyte m_colorId[4];

	//! The Default LOD
	int m_DefaultLOD;

	//! Flag to know if the instance is viewable
	Viewable m_ViewableFlag;

	//! vector of Flag to know if geometies of this instance are viewable
	QVector<bool> m_ViewableGeomFlag;

	//! A Mutex
	static QMutex m_Mutex;

	//! The global default LOD
	static int m_GlobalDefaultLOD;
};

// Return true if the all instance's mesh are transparent
bool ViewInstance::isTransparent() const
{
	if (m_3DRep.isEmpty()) return false;
	if (m_RenderProperties.renderingMode() == glc::OverwriteTransparency) return true;
	if (m_RenderProperties.renderingMode() == glc::OverwriteMaterial)
	{
		return m_RenderProperties.overwriteMaterial()->isTransparent();
	}
	const int size= m_3DRep.numberOfBody();
	bool result= true;
	int i= 0;
	while((i < size) && result)
	{
		result= result && m_3DRep.geomAt(i)->isTransparent();
		++i;
	}
	return result && m_RenderProperties.needToRenderWithTransparency();
}

// Return true if the instance contains mesh which contains transparent material
bool ViewInstance::hasTransparentMaterials() const
{
	if (m_3DRep.isEmpty()) return false;
	if (m_RenderProperties.needToRenderWithTransparency()) return true;
	const int size= m_3DRep.numberOfBody();
	bool result= false;
	int i= 0;
	while ((i < size) && !result)
	{
		result= result || m_3DRep.geomAt(i)->hasTransparentMaterials();
		++i;
	}
	return result;
}
//! Set the viewable flag
bool ViewInstance::setViewable(ViewInstance::Viewable flag)
{
	const int bodyCount= m_3DRep.numberOfBody();
	if (bodyCount != m_ViewableGeomFlag.size())
	{
		m_ViewableGeomFlag.fill(true, bodyCount);
	}
	bool asChange= m_ViewableFlag != flag;
	if (asChange)
	{
		m_ViewableFlag= flag;
		if (flag != ViewInstance::PartialViewable)
		{
			bool viewable= (flag == ViewInstance::FullViewable);

			for (int i= 0; i < bodyCount; ++i)
			{
				m_ViewableGeomFlag[i]= viewable;
			}
		}
	}
	return asChange;
}


#endif /*VIEWINSTANCE_H_*/
