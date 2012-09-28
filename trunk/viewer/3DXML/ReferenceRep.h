///////////////////////////////////////////////////////////
//  ReferenceRep.h
//  Implementation of the Class ReferenceRep
//  Created on:      28-九月-2012 11:36:38
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_8688BF18_3F41_44d0_AD44_991B82875068__INCLUDED_)
#define EA_8688BF18_3F41_44d0_AD44_991B82875068__INCLUDED_

#include "RepresentationLink.h"

/**
 * <ReferenceRep version="1.0" id="5" associatedFile="urn:3DXML:
 * fitting_2_UVR_1_b4dd00d4_2144_5056a3f4_1a32.3DRep" xsi:type="ReferenceRepType"
 * format="UVR" name="fitting_2_UVR">
 *      <PLM_ExternalID>fitting_2_UVR</PLM_ExternalID>
 *      <V_description>Representation</V_description>
 *      <V_discipline>Design</V_discipline>
 *    </ReferenceRep>
 * 
 * associatedFile关联到具体的3drep文件
 */
class ReferenceRep
{

public:
	ReferenceRep();
	virtual ~ReferenceRep();

	RepresentationLink GetAssociatedFile();
	RepresentationFormat GetFormat();
	unsigned int GetId();
	string GetName();
	float GetVersion();
	void SetAssociatedFile(RepresentationLink newVal);
	void SetFormat(RepresentationFormat newVal);
	void SetId(unsigned int newVal);
	void SetName(string newVal);
	void SetVersion(float newVal);

private:
	RepresentationLink associatedFile;
	string C_created;
	string C_modified;
	ReferenceRepExtension Extension;
	RepresentationFormat format;
	unsigned int id;
	string name;
	string PLM_ExtenrnalID;
	string V_description;
	string V_Name;
	float version;

};
#endif // !defined(EA_8688BF18_3F41_44d0_AD44_991B82875068__INCLUDED_)
