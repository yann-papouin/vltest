#ifndef VIEWCOLLECTION_H_
#define VIEWCOLLECTION_H_


#include <hash_map>
#include "viewinstance.h"
//#include "../viewport/glc_frustum.h"


//class GLC_SpacePartitioning;
// class GLC_Material;
//class GLC_Shader;
//class vl::Viewport;

//! ViewInstance Hash table
typedef stdext::hash_map< unsigned int, ViewInstance> ViewInstancesHash;

//! ViewInstance pointer Hash table
typedef stdext::hash_map<unsigned int, ViewInstance*> PointerViewInstanceHash;

//! Hash table of ViewInstance Hash table which use a shader
typedef stdext::hash_map<GLuint, PointerViewInstanceHash*> HashList;

//! Map Shader id to instances id (instances which use the shader)
typedef stdext::hash_map<unsigned int, GLuint> ShaderIdToInstancesId;

//////////////////////////////////////////////////////////////////////
//! \class ViewCollection
/*! \brief ViewCollection : ViewInstance flat collection */

/*! An ViewCollection contains  :
 * 		- A hash table containing ViewInstance Class
 * 		- A hash table use to associate shader with ViewInstance
 */
//////////////////////////////////////////////////////////////////////

class ViewCollection
{

//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default constructor
	ViewCollection();

	//! Destructor
	/*! Delete all Node in the Hash Table and clear the Hash Table*/
	virtual ~ViewCollection();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Return true if the collection is empty
	inline bool isEmpty() const
	{return m_ViewInstanceHash.size() == 0;}

	//! Return the number of Node in the collection
	inline int size(void) const
	{return m_ViewInstanceHash.size();}

	//! Return all ViewInstance from collection
	std::vector<ViewInstance*> instancesHandle();

	//! Return all visible ViewInstance from the collection
	std::vector<ViewInstance*> visibleInstancesHandle();

	//! Return all viewable ViewInstance from the collection
	std::vector<ViewInstance*> viewableInstancesHandle();

	//! Return a ViewInstance from collection
	/*! If the element is not found in collection a empty node is return*/
	ViewInstance* instanceHandle(unsigned int Key);

	//! Return the entire collection Bounding Box
	/*! If all object is set to true, visible and non visible object are used*/
	AABB boundingBox(bool allObject= false);

	//! Return the number of Node in the selection Hash
	inline int selectionSize(void) const
	{return m_SelectedInstances.size();}

	//! Get the Hash table of Selected Nodes
	inline PointerViewInstanceHash* selection()
	{return &m_SelectedInstances;}

	//! Return true if the Instance Id is in the collection
	inline bool contains(unsigned int key) const
	{return m_ViewInstanceHash.contains(key);}

	//! Return true if the element is selected
	inline bool isSelected(unsigned int key) const
	{return m_SelectedInstances.contains(key);}

	//! Return the showing state
	inline bool showState() const
	{return m_IsInShowSate;}

	//! Return the number of drawable objects
	int drawableObjectsSize() const;

	//! Return the element shading group
	inline GLuint shadingGroup(unsigned int key) const
	{ return m_ShaderGroup.value(key);}

	//! Return true if the element is in a shading group
	inline bool isInAShadingGroup(unsigned int key) const
	{ return m_ShaderGroup.contains(key);}

	//! Return instances name from the specified shading group
	std::vector<QString> instanceNamesFromShadingGroup(GLuint) const;

	//! Return the number of used shading group
	int numberOfUsedShadingGroup() const;

	////! Return true if the space partitioning is used
	//inline bool spacePartitioningIsUsed() const
	//{return m_UseSpacePartitioning;}

	////! Return an handle to  the space partitioning
	//inline GLC_SpacePartitioning* spacePartitioningHandle()
	//{return m_pSpacePartitioning;}


//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Bind the specified shader to the collection
	 /* return true if success false if shader is already bind*/
	bool bindShader(GLuint);

	//! Unbind the specified shader from the collection
	/* return true if success false otherwise*/
	bool unBindShader(GLuint);

	//! Unbind All shader
	bool unBindAllShader();

	//! Add a ViewInstance in the collection
	/*! return true if success false otherwise
	 * If shading group is specified, add instance in desire shading group*/
	bool add(const ViewInstance& ,GLuint shaderID=0);

	//! Change instance shading group
	/* Move the specified instances into
	 * the specified shading group
	 * Return true if success false otherwise*/
	void changeShadingGroup(unsigned int, GLuint);

	//! Remove a GLC_Geometry from the collection and delete it
	/*! 	- Find the GLC_Geometry in the collection
	 * 		- Delete the GLC_Geometry
	 * 		- Remove the Geometry container from collection
	 * 		- Delete the associated OpenGL Display list
	 * 		- Remove the Display list container from collection
	 * 		- Invalidate the collection
	 * return true if success false otherwise*/
	bool remove(unsigned int Key);

	//! Remove and delete all GLC_Geometry from the collection
	void clear(void);

	//! Select a Instance
	bool select(unsigned int, bool primitive= false);

	//! Select all instances in current show state or in all show state
	void selectAll(bool allShowState= false);

	//! unselect a Instance
	bool unselect(unsigned int);

	//! unselect all Instance
	void unselectAll();

	//! Set the polygon mode for all Instance
	void setPolygonModeForAll(GLenum, GLenum);

	//! Set Instance visibility
	void setVisibility(const unsigned int, const bool);

	//! Show all instances of the collection
	void showAll();

	//! Hide all instances of collection
	void hideAll();

	//! Set the Show or noShow state
	inline void swapShowState()
	{m_IsInShowSate= !m_IsInShowSate;}

	//! Set the LOD usage
	inline void setLodUsage(const bool usage, vl::Camera* pCamera)
	{
		m_UseLod= usage;
		m_pCamera= pCamera;
	}

	////! Bind the space partitioning
	//void bindSpacePartitioning(GLC_SpacePartitioning*);

	////! Unbind the space partitioning
	//void unbindSpacePartitioning();

	//! Use the space partitioning
	//inline void setSpacePartitionningUsage(bool use)
	//{m_UseSpacePartitioning= use;}

	//! Update the instance viewable state
	/*! Update the frustrum culling from the viewport
	 * If the specified matrix pointer is not null*/
	void updateInstanceViewableState(GLC_Matrix4x4* pMatrix= NULL);

	//! Update the instance viewable state with the specified frustum
	void updateInstanceViewableState(const vl::Frustum&);

	//! Set the attached viewport of this collection
	inline void setAttachedViewport(vl::Camera* pCamera)
	{m_pCamera= pCamera;}
//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Display the specified collection group
	/*! The main group is 0
	 * The selection group is 1
	 * User group are identified by user id
	 */
	void render(GLuint, glc::RenderFlag);

	//! Display all shader group
	void renderShaderGroup(glc::RenderFlag);

//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////

private:
	//! Display collection's member
// 	void glDraw(GLuint, glc::RenderFlag);
// 
// 	//! Draw instances of a PointerViewInstanceHash
// 	inline void glDrawInstancesOf(PointerViewInstanceHash*, glc::RenderFlag);

//@}

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! ViewInstance Hash Table
	ViewInstancesHash m_ViewInstanceHash;

	//! Selected Node Hash Table
	PointerViewInstanceHash m_SelectedInstances;

	//! List of other Node Hash Table
	HashList m_ShadedPointerViewInstanceHash;

	//! Shader groups hash
	ShaderIdToInstancesId m_ShaderGroup;

	//! Normal Node Hash Table
	PointerViewInstanceHash m_MainInstances;

	//! Show State
	bool m_IsInShowSate;

	//! Level of detail usage
	bool m_UseLod;

	//! The viewport associted to the collection for LOD Usage
	vl::Camera* m_pCamera;

	////! The space partitioning
	//GLC_SpacePartitioning* m_pSpacePartitioning;

	////! The space partition usage
	//bool m_UseSpacePartitioning;


};

// Draw instances of a PointerViewInstanceHash
void ViewCollection::glDrawInstancesOf(PointerViewInstanceHash* pHash, glc::RenderFlag renderFlag)
{
	bool forceDisplay= false;
	if (GLC_State::isInSelectionMode())
	{
		forceDisplay= true;
	}

	PointerViewInstanceHash::iterator iEntry= pHash->begin();
	// The current instance
	ViewInstance* pCurInstance;
	if (forceDisplay)
	{
		while (iEntry != pHash->end())
		{
			pCurInstance= iEntry.value();
			if ((pCurInstance->viewableFlag() != ViewInstance::NoViewable) && (pCurInstance->isVisible() == m_IsInShowSate))
			{
				pCurInstance->render(renderFlag, m_UseLod, m_pCamera);
			}
			++iEntry;
		}
	}
	else
	{
		if (!(renderFlag == glc::TransparentRenderFlag))
		{
			while (iEntry != pHash->end())
			{
				pCurInstance= iEntry.value();
				if ((pCurInstance->viewableFlag() != ViewInstance::NoViewable) && (pCurInstance->isVisible() == m_IsInShowSate))
				{
					if (!pCurInstance->isTransparent() || pCurInstance->renderPropertiesHandle()->isSelected() || (renderFlag == glc::WireRenderFlag))
					{
						pCurInstance->render(renderFlag, m_UseLod, m_pCamera);
					}
				}

				++iEntry;
			}

		}
		else
		{
			while (iEntry != pHash->end())
			{
				pCurInstance= iEntry.value();
				if ((pCurInstance->viewableFlag() != ViewInstance::NoViewable) && (pCurInstance->isVisible() == m_IsInShowSate))
				{
					if (pCurInstance->hasTransparentMaterials())
					{
						pCurInstance->render(renderFlag, m_UseLod, m_pCamera);
					}
				}

				++iEntry;
			}
	   }
	}
}

#endif //VIEWCOLLECTION_H_
