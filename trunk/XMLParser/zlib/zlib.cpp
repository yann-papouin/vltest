#include "zlib.h"
#include <wtypes.h>
#define DLL_FILE_STR "zlib1.dll"

HMODULE hLib=NULL;

uLong ZEXPORT crc32   OF((uLong crc, const Bytef *buf, uInt len))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	}  //加载a.dll,这里是假设a.dll与程序放在同一目录
	typedef uLong ZEXPORT (*funcSum)OF((uLong crc, const Bytef *buf, uInt len)); // 定义函数指针
	funcSum crc32_real = (funcSum) GetProcAddress(hLib, "crc32"); //获取函数地址
	uLong ZEXPORT ret = crc32_real(crc,buf,len); //调用
	//FreeLibrary(hLib); //释放
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
		const char *version, int stream_size)); // 定义函数指针
	funcSum inflateInit2_real = (funcSum) GetProcAddress(hLib, "inflateInit2_"); //获取函数地址
	int ret = inflateInit2_real(strm,windowBits,version,stream_size); //调用
	//FreeLibrary(hLib); //释放
	return ret;
}

int ZEXPORT inflateEnd OF((z_streamp strm))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	} //加载a.dll,这里是假设a.dll与程序放在同一目录
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm)); // 定义函数指针
	funcSum inflateEnd_real = (funcSum) GetProcAddress(hLib, "inflateEnd"); //获取函数地址
	int ret = inflateEnd_real(strm); //调用
	//FreeLibrary(hLib); //释放
	return ret;
}

int ZEXPORT inflate OF((z_streamp strm, int flush))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	}  //加载a.dll,这里是假设a.dll与程序放在同一目录
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm, int flush)); // 定义函数指针
	funcSum inflate_real = (funcSum) GetProcAddress(hLib, "inflate"); //获取函数地址
	int ret = inflate_real(strm,flush); //调用
	//FreeLibrary(hLib); //释放
	return ret;
}

const uLongf * ZEXPORT get_crc_table    OF((void))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	}  //加载a.dll,这里是假设a.dll与程序放在同一目录
	typedef uLongf * ZEXPORT (*funcSum) OF((void)); // 定义函数指针
	funcSum get_crc_table_real = (funcSum) GetProcAddress(hLib, "get_crc_table"); //获取函数地址
	uLongf * ret = get_crc_table_real(); //调用
	//FreeLibrary(hLib); //释放
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
	}  //加载a.dll,这里是假设a.dll与程序放在同一目录
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm, int  level, int  method,
		int windowBits, int memLevel,
		int strategy, const char *version,
		int stream_size)); // 定义函数指针
	funcSum deflateInit2_real = (funcSum) GetProcAddress(hLib, "deflateInit2_"); //获取函数地址
	int ret = deflateInit2_real(strm,level,method,windowBits,memLevel,strategy,version,stream_size); //调用
	//FreeLibrary(hLib); //释放
	return ret;
}

int ZEXPORT deflate OF((z_streamp strm, int flush))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	} //加载a.dll,这里是假设a.dll与程序放在同一目录
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm, int flush)); // 定义函数指针
	funcSum deflate_real = (funcSum) GetProcAddress(hLib, "deflate"); //获取函数地址
	int ret = deflate_real(strm,flush); //调用
	//FreeLibrary(hLib); //释放
	return ret;
}

int ZEXPORT deflateEnd OF((z_streamp strm))
{
	if (!hLib)
	{
		hLib = LoadLibraryA(DLL_FILE_STR);
	} //加载a.dll,这里是假设a.dll与程序放在同一目录
	typedef int ZEXPORT (*funcSum) OF((z_streamp strm)); // 定义函数指针
	funcSum deflateEnd_real = (funcSum) GetProcAddress(hLib, "deflateEnd"); //获取函数地址
	int ret = deflateEnd_real(strm); //调用
	//FreeLibrary(hLib); //释放
	return ret;
}