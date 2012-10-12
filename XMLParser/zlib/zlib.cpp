#include "zlib.h"
#include <wtypes.h>
#define DLL_FILE_STR "zlib1.dll"

HMODULE hLib=NULL;

uLong ZEXPORT crc32   OF((uLong crc, const Bytef *buf, uInt len))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	}  //����a.dll,�����Ǽ���a.dll��������ͬһĿ¼
	typedef uLong ZEXPORT (*funcSum)OF((uLong crc, const Bytef *buf, uInt len)); // ���庯��ָ��
	funcSum crc32_real = (funcSum) GetProcAddress(hLib, "crc32"); //��ȡ������ַ
	uLong ZEXPORT ret = crc32_real(crc,buf,len); //����
	//FreeLibrary(hLib); //�ͷ�
	return ret;
}

int ZEXPORT inflateInit2_ OF((z_streamp strm, int  windowBits,
							 const char *version, int stream_size))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	} 
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm, int  windowBits,
		const char *version, int stream_size)); // ���庯��ָ��
	funcSum inflateInit2_real = (funcSum) GetProcAddress(hLib, "inflateInit2_"); //��ȡ������ַ
	int ret = inflateInit2_real(strm,windowBits,version,stream_size); //����
	//FreeLibrary(hLib); //�ͷ�
	return ret;
}

int ZEXPORT inflateEnd OF((z_streamp strm))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	} //����a.dll,�����Ǽ���a.dll��������ͬһĿ¼
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm)); // ���庯��ָ��
	funcSum inflateEnd_real = (funcSum) GetProcAddress(hLib, "inflateEnd"); //��ȡ������ַ
	int ret = inflateEnd_real(strm); //����
	//FreeLibrary(hLib); //�ͷ�
	return ret;
}

int ZEXPORT inflate OF((z_streamp strm, int flush))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	}  //����a.dll,�����Ǽ���a.dll��������ͬһĿ¼
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm, int flush)); // ���庯��ָ��
	funcSum inflate_real = (funcSum) GetProcAddress(hLib, "inflate"); //��ȡ������ַ
	int ret = inflate_real(strm,flush); //����
	//FreeLibrary(hLib); //�ͷ�
	return ret;
}

const uLongf * ZEXPORT get_crc_table    OF((void))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	}  //����a.dll,�����Ǽ���a.dll��������ͬһĿ¼
	typedef uLongf * ZEXPORT (*funcSum) OF((void)); // ���庯��ָ��
	funcSum get_crc_table_real = (funcSum) GetProcAddress(hLib, "get_crc_table"); //��ȡ������ַ
	uLongf * ret = get_crc_table_real(); //����
	//FreeLibrary(hLib); //�ͷ�
	return ret;
}

int ZEXPORT deflateInit2_ OF((z_streamp strm, int  level, int  method,
							 int windowBits, int memLevel,
							 int strategy, const char *version,
							 int stream_size))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	}  //����a.dll,�����Ǽ���a.dll��������ͬһĿ¼
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm, int  level, int  method,
		int windowBits, int memLevel,
		int strategy, const char *version,
		int stream_size)); // ���庯��ָ��
	funcSum deflateInit2_real = (funcSum) GetProcAddress(hLib, "deflateInit2_"); //��ȡ������ַ
	int ret = deflateInit2_real(strm,level,method,windowBits,memLevel,strategy,version,stream_size); //����
	//FreeLibrary(hLib); //�ͷ�
	return ret;
}

int ZEXPORT deflate OF((z_streamp strm, int flush))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	} //����a.dll,�����Ǽ���a.dll��������ͬһĿ¼
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm, int flush)); // ���庯��ָ��
	funcSum deflate_real = (funcSum) GetProcAddress(hLib, "deflate"); //��ȡ������ַ
	int ret = deflate_real(strm,flush); //����
	//FreeLibrary(hLib); //�ͷ�
	return ret;
}

int ZEXPORT deflateEnd OF((z_streamp strm))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	} //����a.dll,�����Ǽ���a.dll��������ͬһĿ¼
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm)); // ���庯��ָ��
	funcSum deflateEnd_real = (funcSum) GetProcAddress(hLib, "deflateEnd"); //��ȡ������ַ
	int ret = deflateEnd_real(strm); //����
	//FreeLibrary(hLib); //�ͷ�
	return ret;
}