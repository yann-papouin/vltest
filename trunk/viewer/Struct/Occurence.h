
//! \file Occurence.h interface for the Occurence class.

#ifndef Occurence_H_
#define Occurence_H_

#include "vlCore/Matrix4.hpp"
#include "vlCore/AABB.hpp"


#include <set>
#include <vector>

// #include "struct/Instance.h"
#include "renderer/vl/VLBaseView.hpp"


//class Material;
//class RenderProperties;
class Represent3D;
class Instance;
class Reference;

//////////////////////////////////////////////////////////////////////
//! \class Occurence
/*! \brief Occurence : A scene graph occurence node */
//////////////////////////////////////////////////////////////////////
class Occurence
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////

public:
	//! Default constructor
	Occurence();

	//! Create Occurence of the specified instance
	Occurence(Instance*, VLBaseView* pVLBaseView= NULL, GLuint shaderId=0);

	//! Construct Occurence withe the specified Represent3D
	Occurence(Represent3D*);

	//! Copy constructor
	Occurence(VLBaseView*, const Occurence&, bool shareInstance);

	//! Destructor
	virtual ~Occurence();
//@}
//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return this Occurence id
	inline unsigned int id() const
	{return m_Uid;}

	//! Return the instance name of this occurence
	const std::string name() const;

	//! Return the absolute matrix of this occurence
	inline vl::dmat4 absoluteMatrix() const
	{ return m_AbsoluteMatrix;}

	//! Return true if this occurence is orphan
	inline bool isOrphan() const
	{ return NULL == m_pParent;}

	//! Return true if this occurence has parent
	inline bool hasParent() const
	{ return NULL != m_pParent;}

	//! Return true if this occurence has a representation
	bool hasRepresentation() const;

	//! Return true if this occurence has 3DViewInstance
	bool has3DViewInstance() const;

	//! Return the instance of this occurence
	Instance* instance() const;

	//! Return the reference of this occurence
	Reference* reference() const;

	//! Return the number of childs
	inline int childCount() const
	{ return m_Childs.size();}

	//! Return true if the occurence has child
	inline bool hasChild() const
	{return childCount() > 0;}

	//! Return true if the given occurence can be added to this occurence children
	bool canBeAddedToChildren(Occurence* pOccurence) const;

	//! Return The parent of this occurence
	inline Occurence* parent() const
	{return m_pParent;}

	//! Return a child of this occurence
	/*! The index must exist*/
	Occurence* child(const int index) const;

	//! Return the list of children of this occurence
	inline std::vector<Occurence*> children() const
	{return m_Childs;}

	//! Return the list of all accurence under this occurence
	std::vector<Occurence*> subOccurenceList() const;

	//! Return the number of faces of the representation of this occurence
	unsigned int numberOfFaces() const;

	//! Return the number of vertex of the representation of this occurence
	unsigned int numberOfVertex() const;

	//! Return the number of materials of the representation of this occurence
	unsigned int numberOfMaterials() const;

	//! Return the materials List of the representation of this occurence
//	std::set<Material*> materialSet() const;

	//! Return a clone this occurence
	Occurence* clone(VLBaseView*, bool shareInstance) const;

	//! Return true if this occurence is visible
	bool isVisible() const;

	//! Return the occurence Bounding Box
	AABB boundingBox() const;

	//! Return the occurence number of this occurence
	inline unsigned int occurenceNumber() const
	{return m_OccurenceNumber;}

	//! Return an handle of the renderProperties of this occurence
//	RenderProperties* renderPropertiesHandle() const
//	{return m_pRenderProperties;}

	//! Return the number of node of this branch
	unsigned int nodeCount() const;

	//! Return the world handle of this occurence
	inline VLBaseView* getVLBaseView() const
	{return m_pVLBaseView;}

	//! Return the Set of children references of this occurence
	std::set<Reference*> childrenReferences() const;

	//! Return the set of parents references of the given occurence
	static std::set<Reference*> parentsReferences(const Occurence* pOccurence);

	//! Return true if the automatic creation of 3DViewInstance is used
	inline bool useAutomatic3DViewInstanceCreation() const
	{return m_AutomaticCreationOf3DViewInstance;}


//@}
//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Set Occurence instance Name
	void setName(const std::string name);

	//! Update the absolute matrix
	Occurence* updateAbsoluteMatrix();

	//! Update children obsolute Matrix
	Occurence* updateChildrenAbsoluteMatrix();

	//! Add Child
	/*! The new child must be orphan*/
	void addChild(Occurence*);

	//! Add Child instance and returns the newly created occurence
	Occurence* addChild(Instance*);

	//! make the occurence orphan
	void makeOrphan();

	//! Remove the specified child
	/*! The removed child will not be deleted*/
	bool removeChild(Occurence* pChild);

	//! Reverse Normals of this Occurence and childs
	void reverseNormals();

	//! Create the 3DViewInstance of this occurence if there is a valid Represent3D
	bool create3DViewInstance();

	//! Remove the 3DViewInstance of this occurence
	bool remove3DViewInstance();

	//! Set this occurence world Handle
	void setVLBaseView(VLBaseView*);

	//! Load the representation and return true if success
	bool loadRepresentation();

	//! UnLoad the representation and return true if success
	bool unloadRepresentation();

	//! Set the occurence number of this occurence
	inline void setOccurenceNumber(unsigned int n)
	{m_OccurenceNumber= n;}

	//! Update the occurence number of this occurence branch
	unsigned int updateOccurenceNumber(unsigned int n);

	//! Set this occurence visibility
	void setVisibility(bool visibility);

	//! set the renderProperties of this occurence
	//void setRenderProperties(const RenderProperties& renderProperties);

	//! Remove empty children
	void removeEmptyChildren();

	//! Set the given reference to this occurence
	void setReference(Reference* pRef);

	//! Set the automatic creation of 3DViewInstance usage
	inline void setAutomatic3DViewInstanceCreationUsage(bool usage)
	{m_AutomaticCreationOf3DViewInstance= usage;}


//@}

//////////////////////////////////////////////////////////////////////
// Private services function
//////////////////////////////////////////////////////////////////////
private:
	//! Detach the occurence from the World
	void detach();

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! Occurence Unique ID
	unsigned int m_Uid;

	//! the occurence's World Handle
	VLBaseView* m_pVLBaseView;

	//! Number of this Occurence
	int* m_pNumberOfOccurence;

	//! The struct instance of this occurence
	Instance* m_pInstance;

	//! The parent of this occurence
	Occurence* m_pParent;

	//! The Child of this occurence
	std::vector<Occurence*> m_Childs;

	//! The absolute matrix of the occurence
	vl::dmat4 m_AbsoluteMatrix;

	//! The occurence number
	unsigned int m_OccurenceNumber;

	//! Flag to know if a occurence without instance is visible
	bool m_IsVisible;

	//! The occurence rendering properties
//	RenderProperties* m_pRenderProperties;

	//! Automatique creation of 3DViewInstance
	bool m_AutomaticCreationOf3DViewInstance;

};

#endif /* Occurence_H_ */
