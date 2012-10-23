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
	string IstanceName;	//��ʵ��������
	int IstanceId;	//ʵ����Id
	int AggretedId;	//����ʵ����IsAggretedId�õ���������������Ϊ��һ��������֦
	bool flag;	//������ʱ�ж�����ڵ��Ƿ��Ѿ��õ�
	int IsMul;	//�ж����Reference�Ƿ�������Instanceʵ���������жϸ��ƽ��
};
