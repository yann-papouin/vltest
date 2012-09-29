//! \file World.h interface for the World class.

#ifndef World_H_
#define World_H_

#include "struct/occurence.h"
#include "struct/reference.h"
#include "struct/instance.h"
#include "struct/Worldhandle.h"

//////////////////////////////////////////////////////////////////////
//! \class World
/*! \brief World : The Root of Lib Scene Graph*/
/*!
 *  World contain :
 * 		- The Scene root Product.
 * 		- a 3DViewCollection which manage all scene shapes (3DViewInstance)
 *
 */
//////////////////////////////////////////////////////////////////////
class World
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default constructor
	World();

	//! Copy constructor
	World(const World&);

	//! Destructor
	virtual ~World();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the entire world Bounding Box
	//inline AABB boundingBox()
	//{ return m_pWorldHandle->collection()->boundingBox();}

	//! Return the root of the world
	inline Occurence* rootOccurence() const
	{return m_pRoot;}

	//! Return the world collection
// 	inline 3DViewCollection* collection()
// 	{return m_pWorldHandle->collection();}

	//! Return the size of the world
	//inline int size() const
	//{return m_pWorldHandle->collection()->size();}

	////! Return true if the world is empty
	//inline bool isEmpty() const
	//{return  m_pWorldHandle->collection()->empty() && !m_pRoot->hasChild();}

	//! Return number of faces
	inline int numberOfFaces() const
	{return m_pRoot->numberOfFaces();}

	//! Return number of vertex
	inline int numberOfVertex() const
	{return m_pRoot->numberOfVertex();}

	//! Return the number of materials
	inline int numberOfMaterials() const
	{return m_pRoot->numberOfMaterials();}

	//! Return the list of material
	//inline std::vector<Material*> listOfMaterials() const
	//{return m_pRoot->materialSet().toList();}

	//! Return list of world's instances
	//inline std::vector<3DViewInstance*> instancesHandle() const
	//{return m_pWorldHandle->collection()->instancesHandle();}

	////! Return all visible 3DViewInstance from the world
	//inline std::vector<3DViewInstance*> visibleInstancesHandle() const
	//{return m_pWorldHandle->collection()->visibleInstancesHandle();}

	//! Return instances name from the specified shading group
	//inline std::vector<std::string> instanceNamesFromShadingGroup(unsigned int id) const
	//{return m_pWorldHandle->collection()->instanceNamesFromShadingGroup(id);}

	////! Return the number of used shading group
	//inline int numberOfUsedShadingGroup() const
	//{return m_pWorldHandle->collection()->numberOfUsedShadingGroup();}

	//! Return the worldHandle of this world
	inline WorldHandle* worldHandle()
	{return m_pWorldHandle;}

	//! Return the occurence specified by an id
	/*! Id must be a valid identifier*/
	inline const Occurence* occurence(unsigned int id) const
	{return m_pWorldHandle->getOccurence(id);}

	//! Return the list off occurences
	inline std::vector<Occurence*> listOfOccurence() const
	{return m_pWorldHandle->occurences();}

	//! Return the number of occurence
	inline int numberOfOccurence() const
	{return m_pWorldHandle->numberOfOccurence();}

	//! Return true if the world contians specified id
	inline int containsOccurence(unsigned int id) const
	{return m_pWorldHandle->containsOccurence(id);}

	//! Return the list of instance
	inline std::vector<Instance*> instances() const
	{return m_pWorldHandle->instances();}

	//! Return the list of Reference
	inline std::vector<Reference*> references() const
	{return m_pWorldHandle->references();}

	//! Return the number of body
	inline int numberOfBody() const
	{return m_pWorldHandle->numberOfBody();}

	//! Return the number of representation
	inline int representationCount() const
	{return m_pWorldHandle->representationCount();}

	//! Return the world Up vector
	//inline Vector3d upVector() const
	//{return m_pWorldHandle->upVector();}

	//! Return the number of selected occurence
	int selectionSize() const
	{return m_pWorldHandle->selectionSetHandle()->size();}

	//! Return true if the given occurence is selected
	inline bool isSelected(const Occurence* pOccurence) const
	{return m_pWorldHandle->selectionSetHandle()->contains(pOccurence);}

	//! Return true if the given occurence id is selected
	inline bool isSelected(unsigned int selectionId) const
	{return m_pWorldHandle->selectionSetHandle()->contains(selectionId);}

	//! Return the list of selected occurences
	inline std::vector<Occurence*> selectedOccurenceList() const
	{return m_pWorldHandle->selectionSetHandle()->occurencesList();}

	//! Return the list of selected occurences
	inline std::vector<Occurence*> selectedPrimitiveOccurenceList() const
	{return m_pWorldHandle->selectionSetHandle()->occurencesListWithSelectedPrimitive();}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Merge this world with another world
	void mergeWithAnotherWorld(World &);

	//! Reverse worlds part normal
	inline void reversePartNormal() {m_pRoot->reverseNormals();}

	//! Clear this world
	World& clear() {return *this= World();}

	//! Set the World root Name
	inline void setRootName(const std::string& name)
	{m_pRoot->setName(name);}

	//! Set the world Up Vector
	/*inline void setUpVector(const Vector3d& vect)
	{m_pWorldHandle->setUpVector(vect);}*/

	//! Set the attached viewport of this world
	inline void setAttachedViewport(vl::Camera* pCamera)
	{m_pWorldHandle->setAttachedViewport(pCamera);}

	//! Select the given occurence
	/*! The given occurence must belong to the world handle of this world*/
	inline void select(const Occurence* pOccurence)
	{m_pWorldHandle->select(pOccurence->id());}

	//! Select the given occurence id
	/*! The given occurence id must belong to the world handle of this world*/
	inline void select(unsigned int occurenceId)
	{m_pWorldHandle->select(occurenceId);}

	//! Unselect the given occurence id
	/*! The given occurence id must belong to the world handle of this world*/
	inline void unselect(unsigned int occurenceId)
	{m_pWorldHandle->unselect(occurenceId);}

	//! Select all occurence of this world with a 3DViewInstance
	inline void selectAllWith3DViewInstance()
	{m_pWorldHandle->selectAllWith3DViewInstance(true);}

	//! Select all occurence of this world with a 3DViewInstance in the current show state
	inline void selectAllWith3DViewInstanceInCurrentShowState()
	{m_pWorldHandle->selectAllWith3DViewInstance(false);}

	//! Unselect all occurence of this world
	inline void unselectAll()
	{m_pWorldHandle->unselectAll();}

	//! Show / Hide selected 3DViewInstance
	inline void showHideSelected3DViewInstance()
	{m_pWorldHandle->showHideSelected3DViewInstance();}

	//! Show selected 3DViewInstance
	inline void showSelected3DViewInstance()
	{m_pWorldHandle->setSelected3DViewInstanceVisibility(true);}

	//! Hide selected 3DViewInstance
	inline void hideSelected3DViewInstance()
	{m_pWorldHandle->setSelected3DViewInstanceVisibility(false);}


//@}

//////////////////////////////////////////////////////////////////////
/*! @name Operator Overload */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Assignement operator
	World& operator=(const World&);
//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Display the world
	//inline void render(unsigned int groupId, glc::RenderFlag renderFlag= glc::ShadingFlag)
	//{m_pWorldHandle->collection()->render(groupId, renderFlag);}

	////! Display the world's shader group
	//inline void renderShaderGroup(glc::RenderFlag renderFlag= glc::ShadingFlag)
	//{m_pWorldHandle->collection()->renderShaderGroup(renderFlag);}

//@}
//////////////////////////////////////////////////////////////////////
// private members
//////////////////////////////////////////////////////////////////////
private:
	//! The World Handle
	WorldHandle* m_pWorldHandle;

	//! The root of the structure
	Occurence* m_pRoot;
};

#endif /*World_H_*/
