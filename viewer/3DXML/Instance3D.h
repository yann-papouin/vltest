///////////////////////////////////////////////////////////
//  Instance3D.h
//  Implementation of the Class Instance3D
//  Created on:      28-九月-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_FB352009_0689_424c_A196_8B1515B3F8EB__INCLUDED_)
#define EA_FB352009_0689_424c_A196_8B1515B3F8EB__INCLUDED_

#include "Instance.h"

/**
 * 包含变换矩阵
 * 
 * <Instance3D id="6" xsi:type="Instance3DType" name="washer.prt">
 *      <PLM_ExternalID>washer.prt</PLM_ExternalID>
 *      <IsAggregatedBy>1</IsAggregatedBy>
 *      <IsInstanceOf>7</IsInstanceOf>
 *      <RelativeMatrix>-1 0 0 0 -1 0 0 0 1 0 9 0</RelativeMatrix>
 *    </Instance3D>
 * 
 * RelativeMatrix：相对矩阵
 */
class Instance3D : public Instance
{

public:
	Instance3D();
	virtual ~Instance3D();

private:
	Instance3DExtension Extension;
	Matrix4x3 RelativeMatrix;

};
#endif // !defined(EA_FB352009_0689_424c_A196_8B1515B3F8EB__INCLUDED_)
