///////////////////////////////////////////////////////////
//  ProductStructure.h
//  Implementation of the Class ProductStructure
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_DB13FEB5_8160_4d56_86EF_28F8A888B728__INCLUDED_)
#define EA_DB13FEB5_8160_4d56_86EF_28F8A888B728__INCLUDED_

#include "Instance3D.h"
#include "InstanceRep.h"
#include "Reference3D.h"
#include "ReferenceRep.h"

/**
 * <xs:complexType name="<b><u>ProductStructureType</u></b>">
 *  <xs:choice maxOccurs="unbounded">
 *    <xs:element name="<b>Reference3D</b>" type="Reference3DType" minOccurs="0"
 * maxOccurs="unbounded" />
 *     <xs:element name="<b>Instance3D</b>" type="Instance3DType" minOccurs="0"/>
 *    <xs:element name="<b>ReferenceRep</b>" type="ReferenceRepType"
 * minOccurs="0"/>
 *    <xs:element name="<b>InstanceRep</b>" type="InstanceRepType"
 * minOccurs="0"/>
 *  </xs:choice>
 *  <xs:attribute name="<b><i>root</i></b>" type="xs:unsignedInt"
 * use="optional"/>
 * </xs:complexType>
 */
class ProductStructure
{

public:
	ProductStructure();
	virtual ~ProductStructure();
	Instance3D *m_Instance3D;
	ReferenceRep *m_ReferenceRep;
	InstanceRep *m_InstanceRep;

	unsigned int GetRoot();
	void SetRoot(unsigned int newVal);

private:
	Instance3D Instance3D;
	InstanceRep InstanceRep;
	Reference3D Reference3D;
	ReferenceRep ReferenceRep;
	unsigned int root;

};
#endif // !defined(EA_DB13FEB5_8160_4d56_86EF_28F8A888B728__INCLUDED_)
