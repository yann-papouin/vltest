///////////////////////////////////////////////////////////
//  LineAttributes.h
//  Implementation of the Class LineAttributes
//  Created on:      28-¾ÅÔÂ-2012 11:36:37
//  Original author: MWU
///////////////////////////////////////////////////////////

#if !defined(EA_43F0F0EB_9E4E_4336_B833_19D09B283AD6__INCLUDED_)
#define EA_43F0F0EB_9E4E_4336_B833_19D09B283AD6__INCLUDED_

class LineAttributes
{

public:
	LineAttributes();
	virtual ~LineAttributes();

	LineType GetLineType();
	float GetThickness();
	void SetLineType(LineType newVal);
	void SetThickness(float newVal);

private:
	LineType LineType;
	float thickness;

};
#endif // !defined(EA_43F0F0EB_9E4E_4336_B833_19D09B283AD6__INCLUDED_)
