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

//! \file world.h interface for the World class.

#ifndef WORLD_H_
#define WORLD_H_

//#include "3dviewcollection.h"
#include "structoccurence.h"
#include "structreference.h"
#include "structinstance.h"
#include "worldhandle.h"

#include "../config.h"

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
class GVCORE_EXPORT World
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
	inline AABB boundingBox()
	{ return m_pWorldHandle->collection()->boundingBox();}

	//! Return the root of the world
	inline StructOccurence* rootOccurence() const
	{return m_pRoot;}

	//! Return the world collection
	//inline 3DViewCollection* collection()
	//{return m_pWorldHandle->collection();}

	//! Return the size of the world
	//inline int size() const
	//{return m_pWorldHandle->collection()->size();}

	//! Return true if the world is empty
	//inline bool isEmpty() const
	//{return  m_pWorldHandle->collection()->isEmpty() && !m_pRoot->hasChild();}

	//! Return number of faces
	inline int numberOfFaces() const
	{return m_pRoot->numberOfFaces();}

	//! Return number of vertex
	inline int numberOfVertex() const
	{return m_pRoot->numberOfVertex();}

	//! Return the number of materials
	//inline int numberOfMaterials() const
	//{return m_pRoot->numberOfMaterials();}

	////! Return the list of material
	//inline std::vector<Material*> listOfMaterials() const
	//{return m_pRoot->materialSet().toList();}

	//! Return list of world's instances
	//inline std::vector<3DViewInstance*> instancesHandle() const
	//{return m_pWorldHandle->collection()->instancesHandle();}

	////! Return all visible 3DViewInstance from the world
	//inline std::vector<3DViewInstance*> visibleInstancesHandle() const
	//{return m_pWorldHandle->collection()->visibleInstancesHandle();}

	//! Return instances name from the specified shading group
	inline std::vector<std::string> instanceNamesFromShadingGroup(GLuint id) const
	{return m_pWorldHandle->collection()->instanceNamesFromShadingGroup(id);}

	//! Return the number of used shading group
	inline int numberOfUsedShadingGroup() const
	{return m_pWorldHandle->collection()->numberOfUsedShadingGroup();}

	//! Return the worldHandle of this world
	inline WorldHandle* worldHandle()
	{return m_pWorldHandle;}

	//! Return the occurence specified by an id
	/*! Id must be a valid identifier*/
	inline StructOccurence* occurence(unsigned int id) const
	{return m_pWorldHandle->getOccurence(id);}

	//! Return the list off occurences
	inline std::vector<StructOccurence*> listOfOccurence() const
	{return m_pWorldHandle->occurences();}

	//! Return the number of occurence
	inline int numberOfOccurence() const
	{return m_pWorldHandle->numberOfOccurence();}

	//! Return true if the world contians specified id
	inline int containsOccurence(unsigned int id) const
	{return m_pWorldHandle->containsOccurence(id);}

	//! Return the list of instance
	inline std::vector<StructInstance*> instances() const
	{return m_pWorldHandle->instances();}

	//! Return the list of Reference
	inline std::vector<StructReference*> references() const
	{return m_pWorldHandle->references();}

	//! Return the number of body
	inline int numberOfBody() const
	{return m_pWorldHandle->numberOfBody();}

	//! Return the number of representation
	inline int representationCount() const
	{return m_pWorldHandle->representationCount();}

	//! Return the world Up vector
	inline vl::dvec3 upVector() const
	{return m_pWorldHandle->upVector();}

	//! Return the number of selected occurence
	int selectionSize() const
	{return m_pWorldHandle->selectionSetHandle()->size();}

	//! Return true if the given occurence is selected
	inline bool isSelected(const StructOccurence* pOccurence) const
	{return m_pWorldHandle->selectionSetHandle()->contains(pOccurence);}

	//! Return true if the given occurence id is selected
	inline bool isSelected(unsigned int selectionId) const
	{return m_pWorldHandle->selectionSetHandle()->contains(selectionId);}

	//! Return the list of selected occurences
	inline std::vector<StructOccurence*> selectedOccurenceList() const
	{return m_pWorldHandle->selectionSetHandle()->occurencesList();}

	//! Return the list of selected occurences
	inline std::vector<StructOccurence*> selectedPrimitiveOccurenceList() const
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
	inline void setUpVector(const vl::dvec3& vect)
	{m_pWorldHandle->setUpVector(vect);}

	////! Set the attached viewport of this world
	//inline void setAttachedViewport(Viewport* pViewport)
	//{m_pWorldHandle->setAttachedViewport(pViewport);}

	//! Select the given occurence
	/*! The given occurence must belong to the world handle of this world*/
	inline void select(const StructOccurence* pOccurence)
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
	//inline void render(GLuint groupId, glc::RenderFlag renderFlag= glc::ShadingFlag)
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
	StructOccurence* m_pRoot;
};

#endif /*WORLD_H_*/
