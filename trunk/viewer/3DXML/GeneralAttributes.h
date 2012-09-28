///////////////////////////////////////////////////////////
//  GeneralAttributes.h
//  Implementation of the Class GeneralAttributes
//  Created on:      28-¾ÅÔÂ-2012 11:36:36
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_EBDECA13_0967_45bb_A22D_29B978D1D021__INCLUDED_)
#define EA_EBDECA13_0967_45bb_A22D_29B978D1D021__INCLUDED_

class GeneralAttributes
{

public:
	GeneralAttributes();
	virtual ~GeneralAttributes();

	bool IsSelectable();
	bool IsVisible();
	void SetSelectable(bool newVal);
	void SetVisible(bool newVal);

private:
	bool selectable;
	bool visible;

};
#endif // !defined(EA_EBDECA13_0967_45bb_A22D_29B978D1D021__INCLUDED_)
