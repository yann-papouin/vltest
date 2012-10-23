#pragma once

#include <vector>
#include <string>
using namespace std;

class XMLDLL_EXPORT TFReference3D
{
public:
	TFReference3D(void);
	TFReference3D(string name,int id);
	TFReference3D(TFReference3D &p);
	~TFReference3D(void);
	void SetInstanceName(string InstanceName);
	void SetAggretedId(int id);
	int GetAggretedId() const;
	string GetName() const;
	int GetId() const;
	string GetInstaceName() const;
	void SetIstanceId(int id);
	int GetIstanceId() const;
	void SetFlag(bool bl);
	bool GetFlag() const;
	void SetIsMul(int f);
	int GetIsMul() const;
private:
	string name;
	int Id;
	string IstanceName;	//其实例的名字
	int IstanceId;	//实例的Id
	int AggretedId;	//根据实例的IsAggretedId得到，用来表名其作为哪一个结点的树枝
	bool flag;	//构建树时判断这个节点是否已经用到
	int IsMul;	//判断这个Reference是否包含多个Instance实例，用来判断复制结点
};
