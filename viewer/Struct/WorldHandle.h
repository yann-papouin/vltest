#ifndef WorldHANDLE_H_
#define WorldHANDLE_H_

#include "struct/occurence.h"
#include "struct/SelectionSet.h"

#include <hash_map>

#include "vlGraphics/Viewport.hpp"

//////////////////////////////////////////////////////////////////////
//! \class WorldHandle
/*! \brief WorldHandle : Handle of shared World*/
//////////////////////////////////////////////////////////////////////
class WorldHandle
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! The default constructor
	WorldHandle();

	//! The default destructor
	~WorldHandle();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the collection
// 	inline 3DViewCollection* collection()
// 	{return &m_Collection;}

	//! Return the number of world associated with this handle
	inline int numberOfWorld() const
	{return m_NumberOfWorld;}

	//! Return true if there is only one world associated with this handle
	inline bool isOrphan() const
	{return m_NumberOfWorld == 0;}

	//! Return true if the specified occurence id is in this world
	bool containsOccurence(unsigned int id) const;

	//! Return the occurence specified by an id
	/*! Id must be a valid identifier*/
	const Occurence* getOccurence(unsigned int id) const;

	//! Return the list off occurences
	std::vector<Occurence*> occurences() const;

	//! Return the number of occurence
	int numberOfOccurence() const;

	//! Return the list of instance
	std::vector<Instance*> instances() const;

	//! Return the list of Reference
	std::vector<Reference*> references() const;

	//! Return the number of body
	int numberOfBody() const;

	//! Return the number of representation
	int representationCount() const;

	//! Return the world Up vector
	//inline Vector3d upVector() const
	//{return m_UpVector;}

	//! Return an handle to the selection set
	SelectionSet* selectionSetHandle();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Increment the number of world
	inline void increment()
	{++m_NumberOfWorld;}

	//! Decrement the number of world
	inline void decrement()
	{--m_NumberOfWorld;}

	//! An Occurence has been added
	void addOccurence(Occurence* pOccurence, bool isSelected= false, unsigned int shaderId= 0);

	//! An Occurence has been removed
	void removeOccurence(Occurence* pOccurence);

	//! All Occurence has been removed
	inline void removeAllOccurences()
	{
		m_OccurenceHash.clear();
	}

	//! Set the world Up Vector
// 	inline void setUpVector(const Vector3d& vect)
// 	{m_UpVector= vect;}

	//! Set the attached viewport of this world handle
	inline void setAttachedViewport(vl::Camera* pCamera)
	{/*m_Collection.setAttachedViewport(pCamera);*/}

	//! Select the given occurence id
	/*! The given occurence id must belong to this worldhandle*/
	void select(unsigned int occurenceId, bool propagate= true);

	//! Unselect the given occurence id
	/*! The given occurence id must belong to this worldhandle*/
	void unselect(unsigned int occurenceId, bool propagate= true);

	//! Select all occurence of this world handle
	void selectAllWith3DViewInstance(bool allShowState);

	//! Unselecte all occurence of this world handle
	void unselectAll();

	//! Show / Hide selected 3DViewInstance
	void showHideSelected3DViewInstance();

	//! Set selected 3DViewInstance visibility
	void setSelected3DViewInstanceVisibility(bool isVisible);


//@}

//////////////////////////////////////////////////////////////////////
// private members
//////////////////////////////////////////////////////////////////////
private:
	//! The Collection
//	ViewCollection m_Collection;

	//! Number of this world
	int m_NumberOfWorld;

	//! The hash table containing struct occurence
	stdext::hash_map<unsigned int, Occurence*> m_OccurenceHash;

	//! This world Up Vector
// 	Vector3d m_UpVector;

	//! This world selectionSet
	SelectionSet m_SelectionSet;
};

#endif /* WorldHANDLE_H_ */
