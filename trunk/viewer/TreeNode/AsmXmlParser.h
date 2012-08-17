/*******************************************************************/
/*    Copyright (c) 2003-2008 by Spatial Corp.                     */
/*******************************************************************/

#if !defined(AH_ASM_XML_PARSER_H_)
#define AH_ASM_XML_PARSER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------- ACIS Kernel ----------

//---------- AGM_HUSK ----------

//---------- AGM_ACIS ----------

//---------- AH_MFC ----------
#include "ModelTreeNode.h"

#ifdef XERCES

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>

#define SIZE 1024

using xercesc::DOMNode;

/**
 * Class AsmXmlParser.
 *<b>Role</b>: Describe Role of Class.
 *
 */
class AsmXmlParser
{
public:
/**
 * Constructs a AsmXmlParser.
 */
	AsmXmlParser();
	virtual ~AsmXmlParser();

/**
 * .
 * <br><br>
 * @param xmlFileName
 * .
 * @param locale
 * .
 */
	void ParserXMLFile(const char* xmlFileName, const char* locale = "america");

/**
 * .
 * <br><br>
 * @param xmlFileName
 * .
 * @param locale
 * .
 */
	void ParserXMLFile(const wchar_t* xmlFileName, const char* locale = "america");

/**
 * .
 * <br><br>
 * @param pAsmNode
 * .
 */
	void DeleteAsmTreeNode(ModelTreeNode*& pAsmNode);

/**
 * Returns ModelTreeNode*.
 */
	ModelTreeNode* GetAsmTreeNode()
	{
		return m_TreeNode;
	};

/**
 * Returns const wchar_t*.
 */
	const wchar_t* GetRootName();

private:
/**
 * .
 * <br><br>
 * @param pNode
 * .
 * @param numOfChild
 * .
 * @param nodeName
 * .
 * @param fileName
 * .
 * @param srcfileName
 * .
 * @param referenceId
 * .
 * @param positionMatrix
 * .
 */
	void GetNodeAttributes(DOMNode* pNode, int& numOfChild, XMLCh* nodeName,
		XMLCh* fileName, XMLCh* srcfileName, XMLCh* referenceId,
		float*& positionMatrix);

/**
 * .
 * <br><br>
 * @param pNode
 * .
 * @param pAsmNode
 * .
 */
	void CreateAsmTree(DOMNode* pNode, ModelTreeNode*& pAsmNode);

	//wumin 2004.12.30
/**
 * Returns bool.
 * <br><br>
 * @param xmlFileName
 * .
 * <br><br>
 * @param locale
 * .
 */
	bool ModifyXMLEncoding(const wchar_t* xmlFileName,const char* locale = "america");


private:

	struct AHAsmReference
	{
		wchar_t refName[SIZE];
		wchar_t refIdentifier[SIZE];
		wchar_t refRepShapeId[SIZE];
	};

	struct AHAsmDocument
	{
		wchar_t docName[SIZE];
		wchar_t docIdentifier[SIZE*4];
		wchar_t docFilePath[SIZE*4];
		wchar_t docSrcFilePath[SIZE*4];
	};

	int m_numOfDoc;
	AHAsmDocument* m_DocList;
	int m_numOfRef;
	AHAsmReference* m_RefList;

	wchar_t m_rootName[SIZE];
	ModelTreeNode* m_TreeNode;
};

#endif // ifdef XERCES
#endif // !defined(AH_ASM_XML_PARSER_H_)