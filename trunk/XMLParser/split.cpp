#include "stdafx.h"
#include "split.h"
#include <vector>
#include <string>
void split(const std::string& s,
		   const std::string& delim,
		   std::vector<std::string>* ret)
{
	size_t last = 0;
	size_t index=s.find_first_of(delim,last);
	while (index!=std::string::npos)
	{
		ret->push_back(s.substr(last,index-last));
		last=index+1;
		index=s.find_first_of(delim,last);
	}
	if (index-last>0)
	{
		ret->push_back(s.substr(last,index-last));
	}

	//std::string::size_type pos;
	//std::vector<std::string> result;
	//std::string tempStr = str + pattern;
	////str+=pattern;
	//int size=tempStr.size();
	//
	//for(int i=0;i<size;i++)
	//{
	//	pos=tempStr.find(pattern,i);
	//	if(pos<size)
	//	{
	//		std::string s=tempStr.substr(i,pos-i);
	//		result.push_back(s);
	//		i=pos+pattern.size()-1;
	//	}
	//}
	//return result;
}