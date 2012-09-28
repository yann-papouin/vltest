///////////////////////////////////////////////////////////
//  Reference3D.h
//  Implementation of the Class Reference3D
//  Created on:      28-¾ÅÔÂ-2012 11:36:38
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_F532FAC8_2A12_4501_AC35_801F64E24B0A__INCLUDED_)
#define EA_F532FAC8_2A12_4501_AC35_801F64E24B0A__INCLUDED_

#include "PLMCoreReference.h"
#include "ProductStructure.h"

/**
 * <Reference3D id="3" xsi:type="Reference3DType" name="fitting">
 *      <PLM_ExternalID>fitting</PLM_ExternalID>
 *      <V_description>fitting_Ref</V_description>
 *    </Reference3D>
 * 
 * 
 * <xs:complexType name="Reference3DExtensionType" abstract="true">
 *  <xs:annotation>
 *    <xs:documentation>Extension that can be used only to specialize 3D
 * References</xs:documentation>
 *  </xs:annotation>
 *  <xs:complexContent>
 *    <xs:extension base="ExtensionType"/>
 *  </xs:complexContent>
 * </xs:complexType>
 */
class Reference3D : public PLMCoreReference, public ProductStructure
{

public:
	Reference3D();
	virtual ~Reference3D();

private:
	Reference3DExtension Extension;

};
#endif // !defined(EA_F532FAC8_2A12_4501_AC35_801F64E24B0A__INCLUDED_)
