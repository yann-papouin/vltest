#include "stdafx.h"
#include "cPackFile.h"    


//���ļ���ӵ�zip�ļ��У�ע�����Դ�ļ�srcFileΪ������ӿ�Ŀ¼    
//fileNameInZip: ��zip�ļ��е��ļ������������·��    
void cPackFile::AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile)
{    
	FILE* srcfp = NULL;        

	//��ʼ��д��zip���ļ���Ϣ
	zip_fileinfo zi;        
	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =    
		zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;    
	zi.dosDate = 0;    
	zi.internal_fa = 0;    
	zi.external_fa = 0;    

	//���srcFileΪ�գ������Ŀ¼    
	char new_file_name[MAX_PATH];    
	memset(new_file_name, 0, sizeof(new_file_name));    
	strcat_s(new_file_name, fileNameInZip);    
	if (srcFile == NULL)    
	{    
		strcat_s(new_file_name, "/");    
	}    

	//��zip�ļ��д������ļ�    
	zipOpenNewFileInZip(zf, new_file_name, &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);    

	if (srcFile != NULL)    
	{    
		//��Դ�ļ�    
		//srcfp = fopen(srcFile, "rb");
		fopen_s(&srcfp,srcFile,"rb");
		if (srcfp == NULL)    
		{    
			printf("�޷�����ļ�: %s !\n",srcFile);    
			zipCloseFileInZip(zf); //�ر�zip�ļ�    
			return;    
		}    

		//����Դ�ļ���д��zip�ļ�    
		char buf[100*1024]; //buffer    
		int numBytes = 0;    
		while( !feof(srcfp) )    
		{    
			numBytes = fread(buf, 1, sizeof(buf), srcfp);    
			zipWriteInFileInZip(zf, buf, numBytes);    
			if( ferror(srcfp) )    
				break;    
		}    

		//�ر�Դ�ļ�    
		fclose(srcfp);    
	}    

	//�ر�zip�ļ�    
	zipCloseFileInZip(zf);    
}    


//�ݹ������Ŀ¼��zip�ļ�    
void cPackFile::CollectFilesInDirToZip(zipFile zf, const char * strPath, const char * parentDir) 
{
	//AddFileToZip(zf, fullPath, NULL); //��zip�ļ������ɸ�Ŀ¼
	//USES_CONVERSION; 
	char allfilestr[MAX_PATH];

	strcpy_s(allfilestr,strPath);
	strcat_s(allfilestr,"//*.*");//find all files

	long Handle;
	struct _finddata_t FileInfo;

	if((Handle=_findfirst(allfilestr,&FileInfo))==-1L)//Ŀ¼������
		return;

	char currentPath[MAX_PATH];
	char fullPath[MAX_PATH];

	do 
	{
		if(strlen(FileInfo.name)==1&&FileInfo.name[0]=='.'
			||strlen(FileInfo.name)==2&&FileInfo.name[0]=='.'&&FileInfo.name[1]=='.')
			continue;//��ǰ�ļ��� ��һ��Ŀ¼

		if (strlen(parentDir)!=0)
		{
			strcpy_s(fullPath,parentDir);
			strcat_s(fullPath,"/");
			strcat_s(fullPath,FileInfo.name);//������zip�ļ��е����·��
		}
		else
			strcpy_s(fullPath,FileInfo.name);

		strcpy_s(currentPath,strPath);
		strcat_s(currentPath,"//");
		strcat_s(currentPath,FileInfo.name);

		if(FileInfo.attrib & _A_SUBDIR)// is a directory
		{
			AddFileToZip(zf, fullPath, NULL); //��zip�ļ�������Ŀ¼�ṹ
			CollectFilesInDirToZip(zf, currentPath, fullPath);//�ݹ��ռ���Ŀ¼�ļ�    
			continue;    
		} 
		AddFileToZip(zf, fullPath, currentPath); //���ļ���ӵ�zip�ļ���
	} while ( _findnext(Handle,&FileInfo)==0 );//find next file
}

//��ĳ��Ŀ¼�����ļ�(dirName)ѹ��ΪzipFileName(.zip)�ļ�
void cPackFile::CreateZipFromDir(const char * dirName, const char * zipFileName)
{    
	long Handle;
	struct _finddata_t FileInfo;

	if((Handle=_findfirst(dirName,&FileInfo))==-1L)//Ŀ¼������
		return;
    
	zipFile newZipFile = zipOpen(zipFileName, APPEND_STATUS_CREATE); //����zip�ļ�    
	if (newZipFile == NULL)    
	{    
		printf("�޷�����zip�ļ�!");    
		return;    
	}

	if(FileInfo.attrib & _A_SUBDIR)// is a directory
	{
		AddFileToZip(newZipFile,FileInfo.name,NULL);
		CollectFilesInDirToZip(newZipFile, dirName, FileInfo.name);    
	}
	else// is a file
		AddFileToZip(newZipFile, FileInfo.name, dirName); 
	zipClose(newZipFile, NULL); //�ر�zip�ļ�    
}    

cPackFile::cPackFile()
{

}

cPackFile::~cPackFile()
{

}
