#include "stdafx.h"
#include "cUnpackFile.h"

static inline bool IsFileExist(const char* pPath)
{
	return GetFileAttributesA(pPath) != INVALID_FILE_ATTRIBUTES;
}

void cUnpackFile::CreateDirFromZip(const char * dirName, const char * zipFileName)
{
	bool slashFlag = true;
	unzFile unZipDir = unzOpen(zipFileName);//unzOpen(zipFileName);

	if (unZipDir == NULL)  
	{  
		//printf("无法解压zip文件!");  
		return;  
	}

	if (!IsFileExist(dirName))
		_mkdir(dirName);

	int nResult = unzGoToFirstFile(unZipDir);
	BYTE * buf;

	while(nResult == UNZ_OK)
	{
		char szCurrentFile[MAX_PATH];
		unz_file_info unZipFileInfo;
		unzGetCurrentFileInfo(unZipDir, &unZipFileInfo, szCurrentFile, sizeof(szCurrentFile), NULL, 0, NULL, 0);

		std::string filePath = std::string(szCurrentFile);
		std::string fileName;
		char fileRoot[MAX_PATH];
		unsigned last_slash_pos = filePath.find_last_of('\\');
		if (last_slash_pos == std::string::npos){
			last_slash_pos = filePath.find_last_of('/');
			slashFlag = false;
		}
		if (last_slash_pos != std::string::npos )
		{
			strcpy_s(fileRoot,dirName);
			if (slashFlag)
				strcat_s(fileRoot,"\\");
			else
				strcat_s(fileRoot,"/");
			std::string filetemp = filePath.substr(0,last_slash_pos);
			strcat_s(fileRoot,filetemp.c_str());

			if (!IsFileExist(fileRoot))
				_mkdir(fileRoot);
			if (last_slash_pos == filePath.length()-1)
			{
				nResult = unzGoToNextFile(unZipDir);
				continue;//文件夹项
			}
		}

		int size = unZipFileInfo.uncompressed_size;
		buf = new BYTE[size];
		if (!buf)
		{
		//	printf("No enough Memory!\n");
			return;
		}
		memset(buf, 0, size*sizeof(char));
		int nReadBytes;
		if (UNZ_OK == unzOpenCurrentFilePassword(unZipDir,NULL))
		{
			nReadBytes = unzReadCurrentFile(unZipDir, buf , size);
			unzCloseCurrentFile(unZipDir);
		}
		FILE * pFile = NULL;
		char filePos[MAX_PATH];
		strcpy_s(filePos,dirName);
		if (slashFlag)
			strcat_s(filePos,"\\");
		else
			strcat_s(filePos,"/");
		
		strcat_s(filePos,filePath.c_str());

		fopen_s(&pFile,filePos, "wb");
		if (pFile)
			fwrite(buf,nReadBytes,1,pFile);
		else
			printf("无法打开输出文件 %s \n",filePos);

		if (pFile)
		{
			fclose(pFile);
			delete buf;
		}

		nResult = unzGoToNextFile(unZipDir);
	}
	unzClose(unZipDir);
}

//void cUnpackFile::CreateFileFromZip(const char * fName, const char * zipFileName)
//{
//	bool slashFlag = true;
//	unzFile unZipDir = unzOpen(zipFileName);
//
//	if (unZipDir == NULL)  
//	{  
//		printf("无法解压zip文件!");  
//		return;  
//	}
//
//	int nResult = unzGoToFirstFile(unZipDir);
//	BYTE * buf;
//
//	if(nResult == UNZ_OK)
//	{
//		char szCurrentFile[MAX_PATH];
//		unz_file_info unZipFileInfo;
//		unzGetCurrentFileInfo(unZipDir, &unZipFileInfo, szCurrentFile, sizeof(szCurrentFile), NULL, 0, NULL, 0);
//
//		int size = unZipFileInfo.uncompressed_size;
//		buf = new BYTE[size];
//		if (!buf)
//		{
//			printf("No enough Memory!\n");
//			return;
//		}
//		memset(buf, 0, size*sizeof(char));
//		int nReadBytes;
//		if (UNZ_OK == unzOpenCurrentFilePassword(unZipDir,NULL))
//		{
//			nReadBytes = unzReadCurrentFile(unZipDir, buf , size);
//			unzCloseCurrentFile(unZipDir);
//		}
//		FILE * pFile;
//		fopen_s(&pFile,fName, "wb");
//		if (pFile)
//			fwrite(buf,nReadBytes,1,pFile);
//		else
//			printf("无法打开输出文件 %s \n",fName);
//
//		fclose(pFile);
//		delete buf;
//	}
//	unzClose(unZipDir);
//}

cUnpackFile::cUnpackFile()
{

}

cUnpackFile::~cUnpackFile()
{

}
