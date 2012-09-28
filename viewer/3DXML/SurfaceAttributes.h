///////////////////////////////////////////////////////////
//  SurfaceAttributes.h
//  Implementation of the Class SurfaceAttributes
//  Created on:      28-¾ÅÔÂ-2012 11:36:38
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_A4548F50_6B60_4a92_BD7A_45940543DC65__INCLUDED_)
#define EA_A4548F50_6B60_4a92_BD7A_45940543DC65__INCLUDED_

class SurfaceAttributes
{

public:
	SurfaceAttributes();
	virtual ~SurfaceAttributes();

	ColorType GetColor();
	MaterialApplication GetMaterialApplication();
	void SetColor(ColorType newVal);
	void SetMaterialApplication(MaterialApption newVal);

private:
	Color Color;
	MaterialApplication MaterialApplication;

};
#endif // !defined(EA_A4548F50_6B60_4a92_BD7A_45940543DC65__INCLUDED_)
