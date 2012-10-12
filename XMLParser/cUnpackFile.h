#pragma once
#pragma once
#ifndef UNICODE
#define UNICODE  
#define _UNICODE  
#endif

#include <iostream>
#include <fstream>
#include <io.h>
#include <atlbase.h>
#include <direct.h>

#include "zlib\unzip.h"

static inline bool IsFileExist(const char* pPath);

class cUnpackFile
{
public:
	cUnpackFile();
	~cUnpackFile();
	void CreateDirFromZip(const char * dirName, const char * zipFileName);//解压到文件夹dirName
	//void CreateFileFromZip(const char * fName, const char * zipFileName);//解压第一个文件到文件fName
};

