#pragma once
#ifndef UNICODE
#define UNICODE  
#define _UNICODE  
#endif

#include <iostream>
#include <fstream>
#include <io.h>
#include <AtlBase.h>

#include "zlib\zip.h"

class cPackFile
{
public:
	//将某个目录或者文件(dirName)压缩为zipFileName(.zip)文件
	void CreateZipFromDir(const char * dirName, const char * zipFileName); 
	cPackFile();
	~cPackFile();
private:
	//递归添加子目录到zip文件  
	void CollectFilesInDirToZip(zipFile zf, const char * strPath, const char * parentDir);

	void AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile);

};

