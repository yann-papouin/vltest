#pragma once

#include <vector>
#include <string>
using namespace std;

class XMLDLL_EXPORT TFInstance3D
{
public:
	TFInstance3D(void);
	~TFInstance3D(void);
	TFInstance3D(string name,int id,int num_IsAggregatedBy,int num_IsInstanceOf);
	string GetName() const;
	int GetId() const;
	int GetAggregatedNum() const;
	int GetIsInstanceNum() const;
private:
	string name;
	int id;
	int num_IsAggregatedBy;
	int num_IsInstanceOf;
};
