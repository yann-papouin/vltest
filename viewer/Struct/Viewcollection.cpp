
//! \file viewcollection.cpp implementation of the ViewCollection class.


#include "viewcollection.h"
#include "vlGraphics/Viewport.hpp"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

ViewCollection::ViewCollection()
: m_ViewInstanceHash()
, m_SelectedInstances()
, m_ShadedPointerViewInstanceHash()
, m_ShaderGroup()
, m_MainInstances()
, m_IsInShowSate(true)
, m_UseLod(false)
, m_pCamera(NULL)
//, m_pSpacePartitioning(NULL)
//, m_UseSpacePartitioning(false)
{
}

ViewCollection::~ViewCollection()
{
	// Delete all collection's elements and the collection bounding box
	clear();
}
//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

bool ViewCollection::bindShader(GLuint shaderId)
{
	if (m_ShadedPointerViewInstanceHash.contains(shaderId))
	{
		return false;
	}
	else
	{
		PointerViewInstanceHash* pNodeHash= new PointerViewInstanceHash;
		m_ShadedPointerViewInstanceHash.insert(shaderId, pNodeHash);
		return true;
	}
}

bool ViewCollection::unBindShader(GLuint shaderId)
{
	bool result= false;
	if (m_ShadedPointerViewInstanceHash.contains(shaderId))
	{
		// Find node which use the shader
		std::vector<unsigned int> nodeId(m_ShaderGroup.keys(shaderId));

		// Move these node in the standard hash and remove them from shader group
		PointerViewInstanceHash* pShaderNodeHash= m_ShadedPointerViewInstanceHash.take(shaderId);
		for (int i= 0; i < nodeId.size(); ++i)
		{
			const unsigned int id= nodeId[i];
			ViewInstance* pInstance= pShaderNodeHash->value(id);

			if (!pInstance->isSelected())
			{
				m_MainInstances.insert(id, pInstance);
			}
			else
			{
				m_SelectedInstances.insert(id, pInstance);
			}
			m_ShaderGroup.remove(id);
		}
		pShaderNodeHash->clear();
		delete pShaderNodeHash;
		result= true;
	}
	Q_ASSERT(!m_ShadedPointerViewInstanceHash.contains(shaderId));
	return result;
}

bool ViewCollection::unBindAllShader()
{
	bool result= true;
	HashList::iterator iEntry= m_ShadedPointerViewInstanceHash.begin();
	std::vector<GLuint> shaderList;
    while (iEntry != m_ShadedPointerViewInstanceHash.end())
    {
    	shaderList.append(iEntry.key());
    	++iEntry;
    }
    const int size= shaderList.size();
    for (int i=0; i < size; ++i)
    {
    	result= result && unBindShader(shaderList[i]);
    }
    return result;
}

bool ViewCollection::add(const ViewInstance& node, GLuint shaderID)
{
	bool result= false;
	const unsigned int key= node.id();
	if (m_ViewInstanceHash.contains(key))
	{
		return false;
	}

	m_ViewInstanceHash.insert(key, node);
	// Create an ViewInstance pointer of the inserted instance
	ViewInstancesHash::iterator iNode= m_ViewInstanceHash.find(key);
	ViewInstance* pInstance= &(iNode.value());
	// Chose the hash where instance is
	if(0 != shaderID)
	{
		// Test if shaderId group exist
		if (m_ShadedPointerViewInstanceHash.contains(shaderID))
		{
			m_ShaderGroup.insert(key, shaderID);

			if(pInstance->isSelected())
			{
				m_SelectedInstances.insert(key, pInstance);
			}
			else
			{
				m_ShadedPointerViewInstanceHash.value(shaderID)->insert(key, pInstance);
			}
			result=true;
		}
	}
	else if (!pInstance->isSelected())
	{
		m_MainInstances.insert(key, pInstance);
		result=true;
	}
	else
	{
		m_SelectedInstances.insert(key, pInstance);
		result=true;
	}

	return result;
}

void ViewCollection::changeShadingGroup(unsigned int instanceId, GLuint shaderId)
{
	// Test if the specified instance exist
	Q_ASSERT(m_ViewInstanceHash.contains(instanceId));
	// Get the instance shading group
	const GLuint instanceShadingGroup= shadingGroup(instanceId);
	// Get a pointer to the instance
	ViewInstance* pInstance= NULL;
	if (0 == instanceShadingGroup)
	{
		// The instance is not in a shading group
		if (m_MainInstances.contains(instanceId))
		{
			pInstance= m_MainInstances.take(instanceId);
		}
		else if (m_SelectedInstances.contains(instanceId))
		{
			// The instance is selected don't take it
			pInstance= m_SelectedInstances.value(instanceId);
		}
		else
		{
			Q_ASSERT(false);
		}
	}
	else
	{
		m_ShaderGroup.remove(instanceId);
		// The instance is in a shading group
		if (m_SelectedInstances.contains(instanceId))
		{
			// The instance is selected don't take it
			pInstance= m_SelectedInstances.value(instanceId);
		}
		else
		{
			pInstance= m_ShadedPointerViewInstanceHash.value(instanceShadingGroup)->take(instanceId);
		}

	}
	// Put the instance in specified shading group
	if (0 != shaderId)
	{
		m_ShaderGroup.insert(instanceId, shaderId);
		if (!pInstance->isSelected())
		{
			m_ShadedPointerViewInstanceHash.value(shaderId)->insert(instanceId, pInstance);
		}
	}
	else if (!pInstance->isSelected())
	{
		m_MainInstances.insert(instanceId, pInstance);
	}
}

bool ViewCollection::remove(unsigned int Key)
{
	ViewInstancesHash::iterator iNode= m_ViewInstanceHash.find(Key);

	if (iNode != m_ViewInstanceHash.end())
	{	// Ok, the key exist

		if (selectionSize() > 0)
		{
			// if the geometry is selected, unselect it
			unselect(Key);
		}

		m_MainInstances.remove(Key);

		m_ViewInstanceHash.remove(Key);		// Delete the conteneur

		//qDebug("ViewCollection::removeNode : Element succesfuly deleted");
		return true;

	}
	else
	{	// KO, key doesn't exist
		return false;
	}

}

void ViewCollection::clear(void)
{
	// Clear Selected node Hash Table
	m_SelectedInstances.clear();
	// Clear the not transparent Hash Table
	m_MainInstances.clear();
	// Clear Other Node Hash List
	HashList::iterator iEntry= m_ShadedPointerViewInstanceHash.begin();
    while (iEntry != m_ShadedPointerViewInstanceHash.end())
    {
    	iEntry.value()->clear();
    	delete iEntry.value();
    	iEntry= m_ShadedPointerViewInstanceHash.erase(iEntry);
    }

	m_ShadedPointerViewInstanceHash.clear();
	m_ShaderGroup.clear();

	// Clear main Hash table
    m_ViewInstanceHash.clear();

	// delete the space partitioning
//	delete m_pSpacePartitioning;
}

bool ViewCollection::select(unsigned int key, bool primitive)
{
	if (!m_ViewInstanceHash.contains(key)) return false;
	//qDebug() << "GLC_Collection::select " << key;

	ViewInstance* pSelectedInstance;
	ViewInstancesHash::iterator iNode= m_ViewInstanceHash.find(key);
	PointerViewInstanceHash::iterator iSelectedNode= m_SelectedInstances.find(key);

	if ((iNode != m_ViewInstanceHash.end()) && (iSelectedNode == m_SelectedInstances.end()))
	{	// Ok, the key exist and the node is not selected
		pSelectedInstance= &(iNode.value());
		m_SelectedInstances.insert(pSelectedInstance->id(), pSelectedInstance);

		// Remove Selected Node from is previous collection
		if (isInAShadingGroup(key))
		{
			m_ShadedPointerViewInstanceHash.value(shadingGroup(key))->remove(key);
			//qDebug() << "remove from shader list";
		}
		else
		{
			m_MainInstances.remove(key);
		}
		pSelectedInstance->select(primitive);

		//qDebug("ViewCollection::selectNode : Element succesfuly selected");
		return true;
	}
	else
	{	// KO, instance allready selected
		return false;
	}
}

void ViewCollection::selectAll(bool allShowState)
{
	unselectAll();
	ViewInstancesHash::iterator iNode= m_ViewInstanceHash.begin();
	while (iNode != m_ViewInstanceHash.end())
	{
		ViewInstance *pCurrentInstance= &(iNode.value());
		const unsigned int instanceId= pCurrentInstance->id();

		if (allShowState || (pCurrentInstance->isVisible() == m_IsInShowSate))
		{
			pCurrentInstance->select(false);
			m_SelectedInstances.insert(instanceId, pCurrentInstance);
			m_MainInstances.remove(instanceId);
			if(isInAShadingGroup(instanceId))
			{
				m_ShadedPointerViewInstanceHash.value(shadingGroup(instanceId))->remove(instanceId);
			}
		}
		iNode++;
	}
}

bool ViewCollection::unselect(unsigned int key)
{
	ViewInstance* pSelectedNode;

	PointerViewInstanceHash::iterator iSelectedNode= m_SelectedInstances.find(key);

	if (iSelectedNode != m_SelectedInstances.end())
	{	// Ok, the key exist and the node is selected
		iSelectedNode.value()->unselect();

		pSelectedNode= iSelectedNode.value();
		m_SelectedInstances.remove(key);

		// Insert Selected Node to the right collection
		if (isInAShadingGroup(key))
		{
			m_ShadedPointerViewInstanceHash.value(shadingGroup(key))->insert(key, pSelectedNode);
		}
		else
		{
			m_MainInstances.insert(key, pSelectedNode);
		}

		//qDebug("ViewCollection::unselectNode : Node succesfuly unselected");
		return true;

	}
	else
	{	// KO, key doesn't exist or node allready selected
		//qDebug("ViewCollection::unselectNode : Node not unselected");
		return false;
	}
}

void ViewCollection::unselectAll()
{
	PointerViewInstanceHash::iterator iSelectedNode= m_SelectedInstances.begin();

    while (iSelectedNode != m_SelectedInstances.end())
    {
    	ViewInstance* pInstance= iSelectedNode.value();
    	pInstance->unselect();
		if (isInAShadingGroup(pInstance->id()))
		{
			m_ShadedPointerViewInstanceHash.value(shadingGroup(pInstance->id()))->insert(pInstance->id(), pInstance);
		}
		else
		{
			m_MainInstances.insert(pInstance->id(), pInstance);
		}

        ++iSelectedNode;
    }
    // Clear selected node hash table
    m_SelectedInstances.clear();
}

void ViewCollection::setPolygonModeForAll(GLenum face, GLenum mode)
{
	ViewInstancesHash::iterator iEntry= m_ViewInstanceHash.begin();

    while (iEntry != m_ViewInstanceHash.end())
    {
    	// Update Instance Polygon Mode
    	iEntry.value().setPolygonMode(face, mode);
    	iEntry++;
    }

}

void ViewCollection::setVisibility(const unsigned int key, const bool visibility)
{
	ViewInstancesHash::iterator iNode= m_ViewInstanceHash.find(key);
	if (iNode != m_ViewInstanceHash.end())
	{	// Ok, the key exist
		iNode.value().setVisibility(visibility);
	}
}

void ViewCollection::showAll()
{
	ViewInstancesHash::iterator iEntry= m_ViewInstanceHash.begin();

    while (iEntry != m_ViewInstanceHash.end())
    {
    	// Update Instance Polygon Mode
    	iEntry.value().setVisibility(true);
    	iEntry++;
    }
}

void ViewCollection::hideAll()
{
	ViewInstancesHash::iterator iEntry= m_ViewInstanceHash.begin();

    while (iEntry != m_ViewInstanceHash.end())
    {
    	// Update Instance Polygon Mode
    	iEntry.value().setVisibility(false);
    	iEntry++;
    }

}

//void ViewCollection::bindSpacePartitioning(GLC_SpacePartitioning* pSpacePartitioning)
//{
//	Q_ASSERT(NULL != pSpacePartitioning);
//	Q_ASSERT(pSpacePartitioning->collectionHandle() == this);
//
//	delete m_pSpacePartitioning;
//	m_pSpacePartitioning= pSpacePartitioning;
//}
//
//void ViewCollection::unbindSpacePartitioning()
//{
//	delete m_pSpacePartitioning;
//	m_pSpacePartitioning= NULL;
//	m_UseSpacePartitioning= false;
//
//	ViewInstancesHash::iterator iEntry= m_3DViewInstanceHash.begin();
//    while (iEntry != m_3DViewInstanceHash.end())
//    {
//    	// Update Instance viewable flag
//    	iEntry.value().setViewable(ViewInstance::FullViewable);
//    	iEntry++;
//    }
//
//}

void ViewCollection::updateInstanceViewableState(GLC_Matrix4x4* pMatrix)
{
	if ((NULL != m_pCamera) /*&& m_UseSpacePartitioning && (NULL != m_pSpacePartitioning)*/)
	{
		//if (m_pCamera->updateFrustum(pMatrix))
		//	m_pSpacePartitioning->updateViewableInstances(m_pCamera->frustum());
	}
}

void ViewCollection::updateInstanceViewableState(const vl::Frustum& frustum)
{
//	m_pSpacePartitioning->updateViewableInstances(frustum);
}

std::vector<ViewInstance*> ViewCollection::instancesHandle()
{
	std::vector<ViewInstance*> instancesList;

	ViewInstancesHash::iterator iEntry= m_ViewInstanceHash.begin();

    while (iEntry != m_ViewInstanceHash.end())
    {
    	instancesList.append(&(iEntry.value()));
    	iEntry++;
    }
	return instancesList;
}

std::vector<ViewInstance*> ViewCollection::visibleInstancesHandle()
{
	std::vector<ViewInstance*> instancesList;

	ViewInstancesHash::iterator iEntry= m_ViewInstanceHash.begin();

    while (iEntry != m_ViewInstanceHash.end())
    {
    	if (iEntry.value().isVisible())
    	{
        	instancesList.append(&(iEntry.value()));
    	}
    	iEntry++;
    }
	return instancesList;

}

std::vector<ViewInstance*> ViewCollection::viewableInstancesHandle()
{
	std::vector<ViewInstance*> instancesList;

	ViewInstancesHash::iterator iEntry= m_ViewInstanceHash.begin();

    while (iEntry != m_ViewInstanceHash.end())
    {
    	if (iEntry.value().isVisible() == m_IsInShowSate)
    	{
        	instancesList.append(&(iEntry.value()));
    	}
    	iEntry++;
    }
	return instancesList;
}

ViewInstance* ViewCollection::instanceHandle(unsigned int Key)
{
	Q_ASSERT(m_ViewInstanceHash.contains(Key));
	return &(m_ViewInstanceHash[Key]);
}

AABB ViewCollection::boundingBox(bool allObject)
{
	AABB boundingBox;
	// Check if the bounding box have to be updated
	if (!m_ViewInstanceHash.isEmpty())
	{
		ViewInstancesHash::iterator iEntry= m_ViewInstanceHash.begin();
	    while (iEntry != m_ViewInstanceHash.end())
	    {
	        if(allObject || iEntry.value().isVisible() == m_IsInShowSate)
	        {
	        	// Combine Collection BoundingBox with element Bounding Box
	        	boundingBox.combine(iEntry.value().boundingBox());
	        }
	        ++iEntry;
	    }
	}
	return boundingBox;
}

int ViewCollection::drawableObjectsSize() const
{
	// The number of object to drw
	int numberOffDrawnHit= 0;

	// Count the number off instance to draw
	ViewInstancesHash::const_iterator i= m_ViewInstanceHash.begin();
	while (i != m_ViewInstanceHash.end())
	{
		//qDebug() << "transparent";
		if (i.value().isVisible() == m_IsInShowSate)
		{
			++numberOffDrawnHit;
		}
		++i;
	}
	return numberOffDrawnHit;
}

std::vector<QString> ViewCollection::instanceNamesFromShadingGroup(GLuint shaderId) const
{
	std::vector<QString> listOfInstanceName;
	std::vector<unsigned int> listOfInstanceNameId= m_ShaderGroup.keys(shaderId);
	if (!listOfInstanceNameId.isEmpty())
	{
		const int size= listOfInstanceNameId.size();
		for (int i= 0; i < size; ++i)
		{
			listOfInstanceName << m_ViewInstanceHash.value(listOfInstanceNameId[i]).name();
		}
	}
	return listOfInstanceName;
}

int ViewCollection::numberOfUsedShadingGroup() const
{
	return m_ShaderGroup.values().toSet().size();
}

//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////

void ViewCollection::render(GLuint groupId, glc::RenderFlag renderFlag)
{
	if (!isEmpty())
	{
		if (renderFlag == glc::WireRenderFlag)
		{
	        glEnable(GL_POLYGON_OFFSET_FILL);
	        glPolygonOffset (1.0, 1.0);
		}
		if (GLC_State::isInSelectionMode())
		{
			glDisable(GL_BLEND);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			glEnable(GL_LIGHTING);
		}
		glDraw(groupId, renderFlag);

		if (renderFlag == glc::WireRenderFlag)
		{
	        glDisable(GL_POLYGON_OFFSET_FILL);
		}
	}
}
void ViewCollection::renderShaderGroup(glc::RenderFlag renderFlag)
{
	if (!isEmpty())
	{
		if (GLC_State::isInSelectionMode())
		{
			glDisable(GL_BLEND);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
		}

		HashList::iterator iEntry= m_ShadedPointerViewInstanceHash.begin();
	    while (iEntry != m_ShadedPointerViewInstanceHash.end())
	    {
	    	glDraw(iEntry.key(), renderFlag);
	    	++iEntry;
	    }
	}
}

void ViewCollection::glDraw(GLuint groupId, glc::RenderFlag renderFlag)
{
	// Set render Mode and OpenGL state
	//if (!GLC_State::isInSelectionMode() && (groupId == 0))
	//{
	//	if (renderFlag == glc::TransparentRenderFlag)
	//	{
	//        glEnable(GL_BLEND);
	//        glDepthMask(GL_FALSE);
	//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	}
	//	else
	//	{
	//	    glDisable(GL_BLEND);
	//	    glDepthMask(GL_TRUE);
	//	    glEnable(GL_DEPTH_TEST);
	//	}
	//}

	//// Normal ViewInstance
	//if ((groupId == 0) && !m_MainInstances.isEmpty())
	//{
	//	glDrawInstancesOf(&m_MainInstances, renderFlag);

	//}
	//// Selected VIewInstance
	//else if ((groupId == 1) && !m_SelectedInstances.isEmpty())
	//{
	//	if (GLC_State::selectionShaderUsed()) GLC_SelectionMaterial::useShader();

	//	glDrawInstancesOf(&m_SelectedInstances, renderFlag);

	//	if (GLC_State::selectionShaderUsed()) GLC_SelectionMaterial::unUseShader();
	//}
	//// ViewInstance with shader
	//else if (!m_ShadedPointerViewInstanceHash.isEmpty())
	//{
	//    if(m_ShadedPointerViewInstanceHash.contains(groupId) && !m_ShadedPointerViewInstanceHash.value(groupId)->isEmpty())
	//    {
	//    	PointerViewInstanceHash* pNodeHash= m_ShadedPointerViewInstanceHash.value(groupId);

	//    	GLC_Shader::use(groupId);
	//    	glDrawInstancesOf(pNodeHash, renderFlag);
	//    	GLC_Shader::unuse();
	//    }
	//}

	//// Restore OpenGL state
	//if (renderFlag && !GLC_State::isInSelectionMode() && (groupId == 0))
	//{
	//	glDisable(GL_BLEND);
	//	glDepthMask(GL_TRUE);
	//	glEnable(GL_DEPTH_TEST);
	//}
}
