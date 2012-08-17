/*******************************************************************/
/*    Copyright (c) 2003-2008 by Spatial Corp.                     */
/*******************************************************************/
// ModelTreeNode.cpp: implementation of the ModelTreeNode class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ModelTreeNode.h"

#ifdef XERCES
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModelTreeNode::ModelTreeNode()
{
	m_key = -1L;//INVALID_KEY
	m_sizeOfChild = 0;
	m_nodeName = NULL;
	m_fileName = NULL;
	m_srcfileName = NULL;
	m_positionMatrix = new float[16];
	m_positionMatrix[0]=1.0f;
	m_positionMatrix[1]=0.0f;
	m_positionMatrix[2]=0.0f;
	m_positionMatrix[3]=0.0f;
	m_positionMatrix[4]=0.0f;
	m_positionMatrix[5]=1.0f;
	m_positionMatrix[6]=0.0f;
	m_positionMatrix[7]=0.0f;
	m_positionMatrix[8]=0.0f;
	m_positionMatrix[9]=0.0f;
	m_positionMatrix[10]=1.0f;
	m_positionMatrix[11]=0.0f;
	m_positionMatrix[12]=0.0f;
	m_positionMatrix[13]=0.0f;
	m_positionMatrix[14]=0.0f;
	m_positionMatrix[15]=1.0f;

	m_ppChildList = NULL;
}

ModelTreeNode::ModelTreeNode(const int sizeOfChild,
							 const wchar_t* nodeName/* = NULL*/,
							 const wchar_t* fileName/* = NULL*/,
							 const wchar_t* srcfileName/* = NULL*/,
							 const wchar_t* referenceId /* = NULL*/,
							 const float* positionMatrix/* = NULL*/
							 ):m_sizeOfChild(sizeOfChild),m_ppChildList(NULL),m_nodeName(NULL),m_fileName(NULL),m_srcfileName(NULL),m_referenceId(NULL),m_key(-1L)
{
	m_positionMatrix = new float[16];
	if(positionMatrix != NULL)
	{
		for(int i=0;i<16;i++)
		{
			m_positionMatrix[i]=positionMatrix[i];
		}
	}
	else
	{
		m_positionMatrix[0]=1.0f;
		m_positionMatrix[1]=0.0f;
		m_positionMatrix[2]=0.0f;
		m_positionMatrix[3]=0.0f;
		m_positionMatrix[4]=0.0f;
		m_positionMatrix[5]=1.0f;
		m_positionMatrix[6]=0.0f;
		m_positionMatrix[7]=0.0f;
		m_positionMatrix[8]=0.0f;
		m_positionMatrix[9]=0.0f;
		m_positionMatrix[10]=1.0f;
		m_positionMatrix[11]=0.0f;
		m_positionMatrix[12]=0.0f;
		m_positionMatrix[13]=0.0f;
		m_positionMatrix[14]=0.0f;
		m_positionMatrix[15]=1.0f;
	}

	if(NULL == nodeName)
	{
		m_nodeName = new wchar_t[1];
		m_nodeName = L'\0';
	}
	else
	{
		int length = wcslen(nodeName);
		m_nodeName = new wchar_t[length+1];
		wcscpy(m_nodeName,nodeName);
	}

	if(NULL == fileName)
	{
		m_fileName = new wchar_t[1];
		m_fileName = L'\0';
	}
	else
	{
		int length = wcslen(fileName);
		m_fileName = new wchar_t[length+1];
		wcscpy(m_fileName,fileName);
	}

	if(NULL == srcfileName)
	{
		m_srcfileName = new wchar_t[1];
		m_srcfileName = L'\0';
	}
	else
	{
		int length = wcslen(srcfileName);
		m_srcfileName = new wchar_t[length+1];
		wcscpy(m_srcfileName,srcfileName);
	}

	if (NULL == referenceId)
	{
		m_referenceId = new wchar_t[1];
		m_referenceId = L'\0';
	}
	else
	{
		int length = wcslen(referenceId);
		m_referenceId = new wchar_t[length+1];
		wcscpy(m_referenceId,referenceId);
	}
		
	if(m_sizeOfChild >0 )
	{
		m_ppChildList = new ModelTreeNode* [m_sizeOfChild];
	}
}

ModelTreeNode::~ModelTreeNode()
{
	if(NULL != m_nodeName)
	{
		delete[] m_nodeName;
		m_nodeName = NULL;
	}

	if(NULL != m_fileName)
	{
		delete[] m_fileName;
		m_fileName = NULL;
	}
	
	if(NULL != m_srcfileName)
	{
		delete[] m_srcfileName;
		m_srcfileName = NULL;
	}

	if(NULL != m_positionMatrix)
	{
		delete[] m_positionMatrix;
		m_positionMatrix = NULL;
	}

	if(NULL != m_referenceId)
	{
		delete[] m_referenceId;
		m_referenceId = NULL;
	}

	if(	m_sizeOfChild >0 && NULL != m_ppChildList)
	{
	    delete[] m_ppChildList;
	    m_ppChildList = NULL;
	}
}

ModelTreeNode::ModelTreeNode(const ModelTreeNode& pNode)
{
	int length = wcslen(pNode.m_nodeName);
	m_nodeName = new wchar_t[length+1];
	wcscpy(m_nodeName,pNode.m_nodeName);
	
	length = wcslen(pNode.m_fileName);
	m_fileName = new wchar_t[length+1];
	wcscpy(m_fileName,pNode.m_fileName);

	length = wcslen(pNode.m_srcfileName);
	m_srcfileName = new wchar_t[length+1];
	wcscpy(m_srcfileName,pNode.m_srcfileName);

	length = wcslen(pNode.m_referenceId);
	m_referenceId = new wchar_t[length+1];
	wcscpy(m_referenceId,pNode.m_referenceId);

	m_key = pNode.m_key;
	
	m_sizeOfChild = pNode.m_sizeOfChild;
	m_ppChildList = new ModelTreeNode* [m_sizeOfChild];
}

ModelTreeNode& ModelTreeNode::operator= (const ModelTreeNode& pNode)
{
	if(this == &pNode)
		return *this;

	if(NULL != m_positionMatrix)
	{
		delete[] m_positionMatrix;
		m_positionMatrix = NULL;
	}

	if(NULL != m_nodeName)
	{
		delete[] m_nodeName;
		m_nodeName = NULL;
	}

	if(NULL != m_fileName)
	{
		delete[] m_fileName;
		m_fileName = NULL;
	}

	if(NULL != m_srcfileName)
	{
		delete[] m_srcfileName;
		m_srcfileName = NULL;
	}
	
	if(NULL != m_referenceId)
	{
		delete[] m_referenceId;
		m_referenceId = NULL;
	}

	if(NULL != m_ppChildList)
	{
	    delete[] m_ppChildList;
	    m_ppChildList = NULL;
	}

	int length = wcslen(pNode.m_nodeName);
	m_nodeName = new wchar_t[length+1];
	wcscpy(m_nodeName,pNode.m_nodeName);

	length = wcslen(pNode.m_fileName);
	m_fileName = new wchar_t[length+1];
	wcscpy(m_fileName,pNode.m_fileName);
	
	length = wcslen(pNode.m_srcfileName);
	m_srcfileName = new wchar_t[length+1];
	wcscpy(m_srcfileName,pNode.m_srcfileName);

	length = wcslen(pNode.m_referenceId);
	m_referenceId = new wchar_t[length+1];
	wcscpy(m_referenceId,pNode.m_referenceId);

	m_key = pNode.m_key;
	m_sizeOfChild = pNode.m_sizeOfChild;
	m_ppChildList = new ModelTreeNode* [m_sizeOfChild];
	
	return *this;
}

#endif