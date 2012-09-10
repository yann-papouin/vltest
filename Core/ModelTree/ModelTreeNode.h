#if !defined(MODEL_TREE_NODE_H_)
#define MODEL_TREE_NODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\config.h"
#include <stdlib.h>
#include <string.h>

#ifdef XERCES
namespace gv
{
/**
 * Class ModelTreeNode.
 *<b>Role</b>: Describe Role of Class.
 *
 */
class GVCORE_EXPORT ModelTreeNode
{
public:
/**
 * Constructs a ModelTreeNode.
 */
	ModelTreeNode();
/**
 * Constructs a ModelTreeNode.
 */
	ModelTreeNode(const int numOfChild,const wchar_t* nodeName,
		const wchar_t* fileName,const wchar_t* srcfileName = NULL,const wchar_t* referenceId = NULL,const float* positionMatrix = NULL);
	virtual ~ModelTreeNode();

/**
 * Constructs a ModelTreeNode.
 */
	ModelTreeNode(const ModelTreeNode&);	
	ModelTreeNode& operator =(const ModelTreeNode&);

public:
/**
 * Returns const int.
 */
	const int GetSize() const
	{
		return m_sizeOfChild;
	};

/**
 * Returns const wchar_t*.
 */
	const wchar_t* GetNodeName() const
	{
		return m_nodeName;
	};

/**
 * Returns const float*.
 */
	const double* GetPositionMatrix() const
	{
		return m_mat4x4;
	};

/**
 * Returns const wchar_t*.
 */
	const wchar_t* GetFileName() const
	{
		return m_fileName;
	};

/**
 * Returns const wchar_t*.
 */
	const wchar_t* GetSrcFileName() const
	{
		return m_srcfileName;
	};

/**
 * Returns ModelTreeNode**.
 */
	ModelTreeNode** GetChilds() const
	{
		return m_ppChildList;
	};

/**
 * .
 * <br><br>
 * @param key
 * .
 */
	void SetKey(long key)
	{
		m_key = key;
	}

/**
 * Returns long.
 */
	long GetKey() const
	{
		return m_key;
	}

	// Bashi Add
/**
 * Returns const wchar_t*.
 */
	const wchar_t* GetReferenceId() const
	{
		return m_referenceId;
	};

private:
	wchar_t* m_nodeName;
	wchar_t* m_fileName;//including full path
	wchar_t* m_srcfileName;//including full path 
	wchar_t* m_referenceId; // Bashi Add
	double* m_mat4x4;

	long m_key;//record the key of the corresponding segment

	int m_sizeOfChild;
	ModelTreeNode** m_ppChildList;
};
}

#endif // ifdef XERCES
#endif // !defined(MODEL_TREE_NODE_H_)
