/*******************************************************************/
/*    Copyright (c) 2003-2008 by Spatial Corp.                     */
/*******************************************************************/
#include "stdafx.h"
#include <afx.h>

#include <locale.h>

#ifndef __GNUC__    // 06/03/2009 for gcc. spatial
#include <TCHAR.H>
#endif

#include "acis.hxx"

#include "AsmXmlParser.h"

#ifdef XERCES

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif

XERCES_CPP_NAMESPACE_USE

AsmXmlParser::AsmXmlParser()
{
	m_numOfDoc = 0;
	m_DocList = NULL;
	m_numOfRef = 0;
	m_RefList = NULL;

	m_TreeNode = NULL;

	// Initialize 06/16/2006 Bashi Add 
	XMLPlatformUtils::Initialize();
}

AsmXmlParser::~AsmXmlParser()
{
	if (NULL != m_DocList)
	{
		delete[] m_DocList;
		m_DocList = NULL;
	}

	if (NULL != m_RefList)
	{
		delete[] m_RefList;
		m_RefList = NULL;
	}

	if (NULL != m_TreeNode)
	{
		DeleteAsmTreeNode(m_TreeNode);
		m_TreeNode = NULL;
	}

	// Terminate 06/16/2006 Bashi Add 
	XMLPlatformUtils::Terminate();

}

void AsmXmlParser::GetNodeAttributes(DOMNode* pNode, int& numOfChild,
	XMLCh* nodeName, XMLCh* fileName, XMLCh* srcfileName, XMLCh* referenceId,
	float*& positionMatrix)
{
	//Get the name of the node
	DOMNamedNodeMap* pAttrMap = pNode->getAttributes();
	XMLCh* pNameCh = XMLString::transcode("Name");
	DOMNode* nameAttr = pAttrMap->getNamedItem(pNameCh);
	XMLString::release(&pNameCh);
	if (nameAttr != NULL)
	{
		XMLString::copyString(nodeName, nameAttr->getNodeValue());
	}

	//Get the number of child
	DOMNodeList* nodeList = pNode->getChildNodes();
	unsigned int idx = 0;
	while (idx < nodeList->getLength())
	{
		const XMLCh* pNodeName = nodeList->item(idx)->getNodeName();
		if (XMLString::compareString(pNodeName, L"Child") == 0)
		{
			numOfChild++;
		}
		idx++;
		//##		XMLString::release(&pNodeName);
	}

	//Get filename and position matrix if this node has reflink and position 

	DOMNode* firstChild = pNode->getFirstChild();
	DOMNode* lastChild = pNode->getLastChild();
	for (pNode = firstChild;
		pNode != lastChild;
		pNode = pNode->getNextSibling())
	{
		const XMLCh* pNodeName = pNode->getNodeName();

		// Bashi 11/29/2004 Add
		if (XMLString::compareString(pNodeName, L"#text") == 0)
		{
			continue;
		}

		if (XMLString::compareString(pNodeName, L"RefLink") == 0)
		{
			//Get filename (include full path)
			DOMNode* refLink = pNode;
			pAttrMap = refLink->getAttributes();
			XMLCh* idStr = XMLString::transcode("Id");
			DOMNode* idAttr = pAttrMap->getNamedItem(idStr);
			XMLString::release(&idStr);

			if (idAttr != NULL)
			{
				const XMLCh* RefLinkId = idAttr->getNodeValue();
				XMLString::copyString(referenceId, RefLinkId); // Bashi

				for (int i = 0; i < m_numOfRef; i++)
				{
					if (XMLString::compareString(RefLinkId,m_RefList[i].refIdentifier) == 0)
					{
						for (int j = 0; j < m_numOfDoc; j++)
						{
							if (XMLString::compareString(m_RefList[i].refRepShapeId,m_DocList[j].docIdentifier) == 0)
							{
								XMLString::copyString(fileName,m_DocList[j].docFilePath);
								XMLString::copyString(srcfileName,m_DocList[j].docSrcFilePath);
								break;
							}	
							//==========for r15 subasm================
							//no rep shape id can be found in r15 xml file
							else if(XMLString::compareString(m_RefList[i].refRepShapeId,L"") == 0
								&& XMLString::compareString(m_RefList[i].refName,m_DocList[j].docName) == 0)
							{
								XMLString::copyString(fileName,L"");
								XMLString::copyString(srcfileName,m_DocList[j].docFilePath);
								break;
							}
							//==========for r15 subasm================
						}
						break;
					}
				}
			}
		}
		else if (XMLString::compareString(pNodeName, L"Position") == 0)
		{
			positionMatrix = new float[16];
			//Get child position
			DOMNodeList* matrixList = pNode->getChildNodes();
			for (unsigned int idxChild = 0;
				idxChild < matrixList->getLength();
				idxChild++)
			{
				const XMLCh* childNodeName = matrixList->item(idxChild)->getNodeName();
				const XMLCh* textContent = matrixList->item(idxChild)->getTextContent();
				float value = (float) wcstod(textContent, NULL);

				// Bashi 11/29/2004 Add
				if (XMLString::compareString(childNodeName, L"#text") == 0)
				{
					continue;
				}

				if (XMLString::compareString(childNodeName, L"V1") == 0)
				{
					positionMatrix[0] = value;
				}
				else if (XMLString::compareString(childNodeName, L"V2") == 0)
				{
					positionMatrix[1] = value;
				}
				else if (XMLString::compareString(childNodeName, L"V3") == 0)
				{
					positionMatrix[2] = value;
				}
				else if (XMLString::compareString(childNodeName, L"V4") == 0)
				{
					positionMatrix[4] = value;
				}
				else if (XMLString::compareString(childNodeName, L"V5") == 0)
				{
					positionMatrix[5] = value;
				}
				else if (XMLString::compareString(childNodeName, L"V6") == 0)
				{
					positionMatrix[6] = value;
				}
				else if (XMLString::compareString(childNodeName, L"V7") == 0)
				{
					positionMatrix[8] = value;
				}
				else if (XMLString::compareString(childNodeName, L"V8") == 0)
				{
					positionMatrix[9] = value;
				}
				else if (XMLString::compareString(childNodeName, L"V9") == 0)
				{
					positionMatrix[10] = value;
				}
				else if (XMLString::compareString(childNodeName, L"T1") == 0)
				{
					positionMatrix[12] = value;
				}
				else if (XMLString::compareString(childNodeName, L"T2") == 0)
				{
					positionMatrix[13] = value;
				}
				else if (XMLString::compareString(childNodeName, L"T3") == 0)
				{
					positionMatrix[14] = value;
				}
			}
			positionMatrix[3] = 0.0f;
			positionMatrix[7] = 0.0f;
			positionMatrix[11] = 0.0f;
			positionMatrix[15] = 1.0f;
		}
	}
}

void AsmXmlParser::CreateAsmTree(DOMNode* pNode, ModelTreeNode*& pAsmNode)//Need recursive
{
	if (pNode != NULL)
	{
		//Get information from pNode
		int numOfChild = 0;
		XMLCh nodeName[SIZE];
		memset(nodeName, 0, sizeof(nodeName));
		XMLCh fileName[SIZE*4];
		memset(fileName, 0, sizeof(fileName));
		XMLCh srcfileName[SIZE*4];
		memset(srcfileName, 0, sizeof(srcfileName));
		XMLCh referenceId[SIZE*4];
		memset(referenceId, 0, sizeof(referenceId));

		float* positionMatrix = NULL;

		GetNodeAttributes(pNode, numOfChild, nodeName, fileName, srcfileName,
			referenceId, positionMatrix);

		//new a ModelTreeNode node

		pAsmNode = new ModelTreeNode(numOfChild, nodeName, fileName,
						srcfileName, referenceId, positionMatrix);

		if (positionMatrix)
		{
			delete[] positionMatrix;
			positionMatrix = NULL;
		}

		//Recursive
		DOMNodeList* childList = pNode->getChildNodes();
		int curSize = 0;
		for (unsigned int i = 0; i < childList->getLength(); i++)
		{
			DOMNode* node = childList->item(i);
			const XMLCh* pNodeName = node->getNodeName();
			if (XMLString::compareString(pNodeName, L"Child") == 0)
			{
				ModelTreeNode** ppChildList = pAsmNode->GetChilds();
				if (NULL != ppChildList)
				{
					CreateAsmTree(node, ppChildList[curSize++]);
				}
			}
		}
	}
}

void AsmXmlParser::ParserXMLFile(const wchar_t* xmlFileName,
	const char* locale)
{
	const XMLCh* xmlFile = xmlFileName;

	bool doNamespaces = true;
	bool doSchema = true;
	bool schemaFullChecking = false;

	setlocale(LC_ALL, locale);

	try
	{
		if (strlen(locale))
		{
			XMLPlatformUtils::Initialize(locale);
		}
		else
		{
			XMLPlatformUtils::Initialize();
		}
	}
	catch (const XMLException& toCatch)
	{
		XERCES_STD_QUALIFIER cerr << "Error during initialization! Message:\n" << toCatch. getMessage() <<
			XERCES_STD_QUALIFIER endl;
		return;
	}

	// Instantiate the DOM parser.

	XercesDOMParser* parser = new XercesDOMParser;
	parser->setDoSchema(doSchema);
	parser->setDoNamespaces(doNamespaces);	

	//Parser the xml file

	// wumin 2004.12.30
	//Modify the Encoding according to locale settings
	ModifyXMLEncoding(xmlFile,"chs");

	try
	{
		parser->parse(xmlFile);//doesn't support Unicode filename/pathname?????
	}
	catch (const XMLException& e)
	{
		XERCES_STD_QUALIFIER cerr << "An error occurred during parsing\n   Message: " << e. getMessage() <<
			XERCES_STD_QUALIFIER endl;
	}
	catch (const DOMException& e)
	{
		const unsigned int maxChars = 2047;
		XMLCh errText[maxChars + 1];

		XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << xmlFile << "'\n" << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;

		if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
			XERCES_STD_QUALIFIER cerr << "Message is: " << errText << XERCES_STD_QUALIFIER endl;
	}
	catch (...)
	{
		XERCES_STD_QUALIFIER cerr << "An error occurred during parsing\n " << XERCES_STD_QUALIFIER endl;
	}

	//Read tree data

//	xercesc_2_7::DOMDocument* doc = parser->getDocument();
	XERCES_CPP_NAMESPACE::DOMDocument* doc = parser->getDocument();
	DOMNode* root = parser->getDocument();

	if (NULL == doc)
		return;

	//Get document
	{
		XMLCh fileFullPath[512];//just full path
		memset(fileFullPath, 0, sizeof(fileFullPath));
		BOOL bHasFileFullPath = FALSE;

		//Get document number
		DOMTreeWalker* walker1 = doc->createTreeWalker(root,DOMNodeFilter::SHOW_ALL, NULL, true);
		DOMNode* current = NULL;
		for ( current = walker1->nextNode();
			current != 0;
			current = walker1->nextNode())
		{
			const XMLCh* pNodeName = current->getNodeName();

			if (XMLString::compareString(pNodeName, L"Document") == 0)
			{
				// Bashi - 11/29/2004 
				DOMNamedNodeMap* pAttrib = current->getAttributes();
				XMLCh* pTypeCh = XMLString::transcode("Type");

				// 12/22/2005 Bashi
				if ( pTypeCh != NULL )
				{
					DOMNode* pTypeAttr = pAttrib->getNamedItem(pTypeCh);
					XMLString::release(&pTypeCh);

					// 12/22/2005 Bashi
					if ( pTypeAttr != NULL )
					{
						const XMLCh* pNodeValueCh = pTypeAttr->getNodeValue();
						char* spTypeValue = XMLString::transcode(pNodeValueCh);

						//				if (	(strcmp( spTypeValue, "Assembly" ) == 0 ) ||
						//						(strcmp( spTypeValue, "CATPart" ) == 0 ) ||
						//						(strcmp( spTypeValue, "CATProduct" ) == 0 ))
						//				{
						if (strcmp(spTypeValue, "sat") != 0 )
						{
							// Bashi - 11/29/2004 

							m_numOfDoc++;
							if (!bHasFileFullPath)
							{
								//Get full path of SAT file
								DOMNamedNodeMap* pAttrMap = current->getAttributes();

								XMLCh* pFilePathCh = XMLString::transcode("FilePath");
								DOMNode* filePathAttr = pAttrMap->getNamedItem(pFilePathCh);
								XMLString::release(&pFilePathCh);

								if (filePathAttr != NULL)
								{
									const XMLCh* pNodeValue = filePathAttr->getNodeValue();

									//replace '/' with '\'  wumin 2004.12.30
									wchar_t* temp = const_cast<wchar_t*>(pNodeValue);
									while (*temp != L'\0')
									{
										if (*temp == L'/')
										{
											*temp = L'\\';
										}
										temp++;
									}	
								
									if (wcsstr(pNodeValue, L":\\"))
									{
										const wchar_t* pDest = wcsrchr(pNodeValue, L'\\');
										if (pDest)
										{
											int index = pDest - pNodeValue + 1;
											wcsncpy(fileFullPath, pNodeValue,index - 1);
										}
										bHasFileFullPath = TRUE;
									}
								}
							}
						}// Bashi - 11/29/2004 
						XMLString::release(&spTypeValue);
					}// Bashi - 12/22/2005
				}// Bashi - 12/22/2005
			}
		}
		walker1->release();

		m_DocList = new AHAsmDocument[m_numOfDoc];
		for(int k=0;k<m_numOfDoc;k++)//initialize
		{
			XMLString::copyString(m_DocList[k].docName,L"");
			XMLString::copyString(m_DocList[k].docIdentifier,L"");
			XMLString::copyString(m_DocList[k].docFilePath,L"");
			XMLString::copyString(m_DocList[k].docSrcFilePath,L"");
		}// 2005/07/06 wumin

		//Get document attribute
		DOMTreeWalker* walker2 = doc->createTreeWalker(root,DOMNodeFilter::SHOW_ALL, NULL, true);
		int i = 0;
		for (current = walker2->nextNode();
			current != 0 && i<m_numOfDoc;
			current = walker2->nextNode())
		{
			const XMLCh* pNodeName = current->getNodeName();
			if (XMLString::compareString(pNodeName, L"Document") == 0)
			{
				// Bashi - 11/29/2004 - Check if Document is Assembly/CATPart/CATProduct
				DOMNamedNodeMap* pAttrib = current->getAttributes();
				XMLCh* pTypeCh = XMLString::transcode("Type");
				DOMNode* pTypeAttr = pAttrib->getNamedItem(pTypeCh);
				XMLString::release(&pTypeCh);

				// 12/22/2005 Bashi
				if ( pTypeAttr != NULL )
				{
					const XMLCh* pNodeValueCh = pTypeAttr->getNodeValue();
					char* spTypeValue = XMLString::transcode(pNodeValueCh);

					//				if (  !((strcmp( spTypeValue, "Assembly" ) == 0 ) ||
					//						(strcmp( spTypeValue, "CATPart" ) == 0 ) ||
					//						(strcmp( spTypeValue, "CATProduct" ) == 0 )))
					//				{
	//				if ((strcmp(spTypeValue, "sat") != 0))
	//				{
	//					XMLString::release(&spTypeValue);
	//					continue;
	//				}
	//				else
	//				{
	//					XMLString::release(&spTypeValue);
	//				}
					// Bashi - 11/29/2004 
					if( strcmp(spTypeValue,"sat") == 0 || strcmp(spTypeValue,"CATProduct") == 0)
					{
						XMLString::release(&spTypeValue);
					}
					else
					{
						XMLString::release(&spTypeValue);
						continue;
					}
					// 2005/07/06 wumin

					//Get attributes
					DOMNamedNodeMap* pAttrMap = current->getAttributes();
					XMLCh* pNameCh = XMLString::transcode("Name");
					DOMNode* nameAttr = pAttrMap->getNamedItem(pNameCh);
					XMLString::release(&pNameCh);
					if (nameAttr != NULL)
					{
						XMLString::copyString(m_DocList[i].docName,nameAttr->getNodeValue());
					}
					XMLCh* pFilePathCh = XMLString::transcode("FilePath");
					DOMNode* filePathAttr = pAttrMap->getNamedItem(pFilePathCh);
					XMLString::release(&pFilePathCh);
					if (filePathAttr != NULL)
					{
						// add full path
						const XMLCh* pNodeValue = filePathAttr->getNodeValue();

						//replace '/' with '\'  wumin 2004.12.30
						wchar_t* temp = const_cast<wchar_t*>(pNodeValue);
						
						while (*temp != L'\0')
						{
							if (*temp == L'/')
							{
								*temp = L'\\';
							}
							temp++;
						}	
						
						if (!wcsstr(pNodeValue, L":\\"))//relative path
						{
							const wchar_t* pDest = wcsrchr(pNodeValue, L'\\');
							if (NULL != pDest)
							{
								XMLString::copyString(m_DocList[i].docFilePath,fileFullPath);
								XMLString::catString(m_DocList[i].docFilePath,L"\\");
								XMLString::catString(m_DocList[i].docFilePath,pDest + 1);
							}
						}
						else
						{
							XMLString::copyString(m_DocList[i].docFilePath,filePathAttr->getNodeValue());
						}
					}

					XMLCh* pIdentifierCh = XMLString::transcode("Identifier");
					DOMNode* identifierAttr = pAttrMap->getNamedItem(pIdentifierCh);
					XMLString::release(&pIdentifierCh);
					if (identifierAttr != NULL)
					{
	//??wumin			XMLString::copyString(m_DocList[i].docIdentifier,filePathAttr->getNodeValue());
						XMLString::copyString(m_DocList[i].docIdentifier,identifierAttr->getNodeValue());
					}

					//Get DocLink Id

					DOMNodeList* DocumentChild = current->getChildNodes();
					for (unsigned int iChild = 0;
						iChild < DocumentChild->getLength();
						iChild++)
					{
						const XMLCh* pDocNodeName = DocumentChild->item(iChild)->getNodeName();
						if (XMLString::compareString(pDocNodeName, L"IssuedFrom") == 0)
						{
							DOMNode* issuedFrom = DocumentChild->item(iChild);

							DOMNodeList* DocChild = issuedFrom->getChildNodes();
							for (unsigned int idxChild = 0;
								idxChild < DocChild->getLength();
								idxChild++)
							{
								const XMLCh* nodeName = DocChild->item(idxChild)->getNodeName();
								if (XMLString::compareString(nodeName, L"DocLink") == 0)
								{
									DOMNode* docLink = DocChild->item(iChild);
									pAttrMap = docLink->getAttributes();
									XMLCh* pIdCh = XMLString::transcode("Id");
									DOMNode* idAttr = pAttrMap->getNamedItem(pIdCh);
									XMLString::release(&pIdCh);
									if (idAttr != NULL)
									{
										XMLString::copyString(m_DocList[i].docSrcFilePath,idAttr->getNodeValue());
									}
									break;
								}
							}
							break;
						}
					}
				} // 12/22/2005 Bashi
				i++;
			}
		}
		walker2->release();
	}
	//Get ref
	{
		//Get Reference Number
		DOMTreeWalker* walker1 = doc->createTreeWalker(root,DOMNodeFilter::SHOW_ALL, NULL, true);
		DOMNode* current = NULL;
		for (current = walker1->nextNode();
			current != 0;
			current = walker1->nextNode())
		{
			const XMLCh* pNodeName = current->getNodeName();
			if (XMLString::compareString(pNodeName, L"Reference") == 0)
			{
				m_numOfRef++;
			}
		}
		m_RefList = new AHAsmReference[m_numOfRef];
		int k = 0;
		for( k=0;k<m_numOfRef;k++)//initialize
		{
			XMLString::copyString(m_RefList[k].refName,L"");
			XMLString::copyString(m_RefList[k].refIdentifier,L"");
			XMLString::copyString(m_RefList[k].refRepShapeId,L"");
		}// 2005/07/06 wumin

		walker1->release();

		DOMTreeWalker* walker2 = doc->createTreeWalker(root,DOMNodeFilter::SHOW_ALL, NULL, true);
		int i = 0;
		for (current = walker2->nextNode();
			current != 0;
			current = walker2->nextNode())
		{
			const XMLCh* pNodeName = current->getNodeName();
			if (XMLString::compareString(pNodeName, L"Reference") == 0)
			{
				//Get attributes
				DOMNamedNodeMap* pAttrMap = current->getAttributes();

				XMLCh* pNameCh = XMLString::transcode("Name");
				DOMNode* nameAttr = pAttrMap->getNamedItem(pNameCh);
				XMLString::release(&pNameCh);
				if (nameAttr != NULL)
				{
					XMLString::copyString(m_RefList[i].refName,nameAttr->getNodeValue());
				}
				XMLCh* pIdentifierCh = XMLString::transcode("Identifier");
				DOMNode* identifierAttr = pAttrMap->getNamedItem(pIdentifierCh);
				XMLString::release(&pIdentifierCh);
				if (identifierAttr != NULL)
				{
					XMLString::copyString(m_RefList[i].refIdentifier,identifierAttr->getNodeValue());
				}

				//Get shape id

				DOMNodeList* ReferenceChild = current->getChildNodes();
				unsigned int iChild = 0;
				for (iChild = 0;iChild < ReferenceChild->getLength();iChild++)
				{
					const XMLCh* pRepNodeName = ReferenceChild->item(iChild)->getNodeName();
					if (XMLString::compareString(pRepNodeName, L"Rep") == 0)
					{
						DOMNode* rep = ReferenceChild->item(iChild);

						DOMNodeList* RepChild = rep->getChildNodes();
						for (unsigned int idxChild = 0;idxChild < RepChild->getLength();idxChild++)
						{
							const XMLCh* nodeName = RepChild->item(idxChild)->getNodeName();
							if (XMLString::compareString(nodeName, L"Shape") == 0)
							{
								DOMNode* shape = RepChild->item(iChild);
								pAttrMap = shape->getAttributes();
								XMLCh* pIdCh = XMLString::transcode("Id");
								DOMNode* idAttr = pAttrMap->getNamedItem(pIdCh);
								XMLString::release(&pIdCh);
								if (idAttr != NULL)
								{
									XMLString::copyString(m_RefList[i].refRepShapeId,idAttr->getNodeValue());
								}
								break;
							}
						}
						break;
					}
					// 2005/07/05 Add by wumin - for R15
					else// cannot find rep ,and then shape id ,we set shape id = "";
					{
						XMLString::copyString(m_RefList[i].refRepShapeId,L"");
					}
					// 2005/07/05 Add by wumin - for R15
				}
				i++;
			}
		}
		walker2->release();
	}

	//Create assembly tree structure
	{
		DOMTreeWalker* walker = doc->createTreeWalker(root,DOMNodeFilter::SHOW_ALL, NULL, true);
		root = walker->getRoot();

		DOMNode* current = NULL;
		for ( current = walker->nextNode();
			current != 0;
			current = walker->nextNode())
		{
			const XMLCh* pRootNodeName = current->getNodeName();
			if (XMLString::compareString(pRootNodeName, L"Root") == 0)
			{
				//Get Root Name
				DOMNamedNodeMap* pAttrMap = current->getAttributes();

				XMLCh* pNameCh = XMLString::transcode("Name");
				DOMNode* nameAttr = pAttrMap->getNamedItem(pNameCh);
				XMLString::release(&pNameCh);
				if (nameAttr != NULL)
				{
					XMLString::copyString(m_rootName, nameAttr->getNodeValue());
				}

				//Create Assembly Tree
				CreateAsmTree(current, m_TreeNode);//Create assembly tree structure
				break;
			}
		}
		walker->release();
	}

	if (NULL != m_DocList)
	{
		delete[] m_DocList;
		m_DocList = NULL;
	}
	if (NULL != m_RefList)
	{
		delete[] m_RefList;
		m_RefList = NULL;
	}

	delete parser;
	XMLPlatformUtils::Terminate();
}

void AsmXmlParser::ParserXMLFile(const char* xmlFileName, const char* locale)
{
	wchar_t fileName[SIZE*4];
	memset(fileName, 0, sizeof(fileName));
	mbstowcs(fileName, xmlFileName, sizeof(fileName));
	ParserXMLFile(fileName, locale);
}

void AsmXmlParser::DeleteAsmTreeNode(ModelTreeNode*& pAsmNode)
{
	ModelTreeNode** ppChildList = pAsmNode->GetChilds();
	int i = 0;
	for ( i = 0; i < pAsmNode->GetSize(); i++)
	{
		if (ppChildList)
		{
			DeleteAsmTreeNode(ppChildList[i]);
		}
	}
	delete pAsmNode;
	pAsmNode = NULL;
}

const wchar_t* AsmXmlParser::GetRootName()
{
	return (m_rootName);
}

bool AsmXmlParser::ModifyXMLEncoding(const wchar_t* xmlFileName,const char* locale)
{
	char xmlFile[1024];
	memset(xmlFile,0,1024);
	wcstombs(xmlFile,xmlFileName,1024);

	const char* oldEncoding = "ISO-8859-1";
	char newEncoding[20];

	if((strcmp(locale,"chs") == 0) || (strcmp(locale,"simplified-chinese") == 0))
	{
		sprintf(newEncoding,"GB2312");
	}
	else if((strcmp(locale,"jpn") == 0) || (strcmp(locale,"japanese") == 0))
	{
		sprintf(newEncoding,"ISO-2022-JP");
	}
//	else
//	{
//		sprintf(newEncoding,"ISO-8859-1");
//	}

	bool retval = false;

	int diffLength = strlen(newEncoding)-strlen(oldEncoding);

	CFile file(xmlFile, CFile::modeRead);
	
	LONG lenOfFile = (LONG)file.GetLength();
	char* buffer = NULL;
	buffer = new char[lenOfFile+1];
	memset(buffer, 0, lenOfFile);
	file.Read(buffer, lenOfFile);
	file.Close();


	CString str(buffer);

	if(str.Find(oldEncoding,0)!=-1 /*found*/ && diffLength != 0/*is different*/)
	{
		retval = true;

		str.Replace(oldEncoding,newEncoding);

		CFile newfile(xmlFile, CFile::modeCreate|CFile::modeWrite);

		newfile.Write(str, lenOfFile+diffLength);
		newfile.Close();
	}

	if (NULL != buffer)
	{
		delete[] buffer;
		buffer = NULL;
	}

	return retval;
}

#endif