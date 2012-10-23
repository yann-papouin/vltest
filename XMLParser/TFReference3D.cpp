#include "StdAfx.h"
#include "TFReference3D.h"

TFReference3D::TFReference3D(void)
{
}

TFReference3D::~TFReference3D(void)
{
}

TFReference3D::TFReference3D(string name,int id)
{
	this->Id=id;
	this->name=name;
	this->AggretedId=-1;
	this->IstanceId=-1;
	this->IstanceName="";
	this->flag=false;
	this->IsMul=0;
}

TFReference3D::TFReference3D(TFReference3D& p)
{
	this->name=p.name;
	this->Id=p.Id;
	this->AggretedId=-1;
	this->IstanceId=-1;
	this->IstanceName="";
	this->flag=false;
	this->IsMul=0;
}

string TFReference3D::GetName() const
{
	return name;
}

int TFReference3D::GetId() const
{
	return Id;
}

string TFReference3D::GetInstaceName() const
{
	return IstanceName;
}

void TFReference3D::SetInstanceName(string InstanceName)
{
	this->IstanceName=InstanceName;
}

void TFReference3D::SetAggretedId(int id)
{
	this->AggretedId=id;
}

int TFReference3D::GetAggretedId() const
{
	return this->AggretedId;
}

void TFReference3D::SetIstanceId(int id)
{
	this->IstanceId=id;
}

int TFReference3D::GetIstanceId() const
{
	return this->IstanceId;
}

void TFReference3D::SetFlag(bool bl)
{
	this->flag=bl;
}

bool TFReference3D::GetFlag() const
{
	return this->flag;
}

void TFReference3D::SetIsMul(int f)
{
	this->IsMul=f;
}

int TFReference3D::GetIsMul() const
{
	return IsMul;
}
