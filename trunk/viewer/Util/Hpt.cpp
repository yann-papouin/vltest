#include "stdafx.h"
#include "Hpt.h"

//////////////////////////////////////////////////////////////////////////////////////////////
Hpt::Hpt()
{ 
}
Hpt::~Hpt()
{ 
}
//////////////////////////////////////////////////////////////////////////////////////////////

void Hpt::Start()
{
	if (_isStarted)
		return;
	_isStarted = true;
	QueryPerformanceCounter( &startTime ); 
}

void Hpt::Stop()
{
	if ( !_isStarted)
		return;
	_isStarted = false;

	LARGE_INTEGER stopTime;
	QueryPerformanceCounter( &stopTime ); 

	LARGE_INTEGER totalTime;
	totalTime.QuadPart = stopTime.QuadPart - startTime.QuadPart;

	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );

	double time = (double)totalTime.QuadPart / (double)freq.QuadPart;
	_accumulatedTime += time;
}

double Hpt::GetTime()
{
	return _accumulatedTime;
}

void Hpt::Reset()
{
	_isStarted = false;
	_accumulatedTime = 0.0; 
}
