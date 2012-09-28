///////////////////////////////////////////////////////////
//  PointAttributes.h
//  Implementation of the Class PointAttributes
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_CC67FA85_9435_4bf3_A5C3_D90F464A62D1__INCLUDED_)
#define EA_CC67FA85_9435_4bf3_A5C3_D90F464A62D1__INCLUDED_

class PointAttributes
{

public:
	PointAttributes();
	virtual ~PointAttributes();

	PointSymbol GetSymbolType();
	void SetSymbolType(PointSymbol newVal);

private:
	PointSymbol symbolType;

};
#endif // !defined(EA_CC67FA85_9435_4bf3_A5C3_D90F464A62D1__INCLUDED_)
