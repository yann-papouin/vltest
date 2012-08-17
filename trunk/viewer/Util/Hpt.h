#pragma once

#include <windows.h>

class Hpt 
{
public:
	Hpt();
	~Hpt();

	void Start();
	void Stop();
	double GetTime();
	void Reset();
private:
	LARGE_INTEGER startTime;
	double _accumulatedTime;
	bool _isStarted;
};
