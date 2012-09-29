//! \file Reference.h interface for the Reference class.

#ifndef Represent3D_H_
#define Represent3D_H_

#include "Represent.h"

//////////////////////////////////////////////////////////////////////
//! \class Represent3D
/*! \brief Represent3D :  */
//////////////////////////////////////////////////////////////////////
class Represent3D : public Represent
{
public:
	Represent3D();
	~Represent3D();

public:
	unsigned int  numberOfBody() const{return 0;};
	unsigned int  faceCount() const{return 0;};
	unsigned int  vertexCount() const{return 0;};
	unsigned int  materialCount() const{return 0;};
};

#endif /* Represent3D_H_ */
