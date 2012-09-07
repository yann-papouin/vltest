#include "stdafx.h"

#include "InterOp/IOManager.hpp"

#ifdef VL
#include "vlCore/LoadWriterManager.hpp"
#endif

IOManager* IOManager::Instance()
{
	if (_ioManager == 0)
	{
		_ioManager =  new IOManager;
	}
	return _ioManager;
}

const char* IOManager::GetInputHandlerTypes()
{
//#ifdef VL
//	vl::defLoadWriterManager();
//#endif

	return NULL;
}

IOManager::IOManager()
{
}

IOManager* IOManager::_ioManager = 0;
