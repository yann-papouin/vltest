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
	//��ĳ��Ŀ¼�����ļ�(dirName)ѹ��ΪzipFileName(.zip)�ļ�
	void CreateZipFromDir(const char * dirName, const char * zipFileName); 
	cPackFile();
	~cPackFile();
private:
	//�ݹ������Ŀ¼��zip�ļ�  
	void CollectFilesInDirToZip(zipFile zf, const char * strPath, const char * parentDir);

	void AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile);

};

