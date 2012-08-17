#pragma once
#include "Hpt.h"
#include <vector>
#include <string>

using namespace std;

typedef const int TIMER_TYPE;
TIMER_TYPE TESSELATION_TIME = 0;
TIMER_TYPE GRAPH_BUILDING_TIME = 1;
TIMER_TYPE RENDERING_TIME = 2;
TIMER_TYPE UPDATE_VIEW_TIME = 3;
TIMER_TYPE DEBUG_TIME_1 = 4;
TIMER_TYPE DEBUG_TIME_2 = 5;
TIMER_TYPE DEBUG_TIME_3 = 6;
TIMER_TYPE TOTAL_TIME = 7;

#define START_TIMER( timer )\
{\
  PerformanceMetrics::Start( timer );\
}

#define STOP_TIMER( timer )\
{\
  PerformanceMetrics::Stop( timer );\
}

#define SWAP_TIMER( timer1, timer2 )\
{\
  PerformanceMetrics::Stop( timer1 );\
  PerformanceMetrics::Start( timer2 );\
}

/*
* Collect and expose rendering performance data
**/
class PerformanceMetrics
{
public:
	// Enabled by default
	static void Enable();
	// To disable and not consume resources
	static void Disable();

	static void Start(TIMER_TYPE timerType);
	static void Stop(TIMER_TYPE timerType);
	static double GetTime(TIMER_TYPE timerType);
	static void Reset(TIMER_TYPE timerType);
	static void ResetAll();

	static void ReportResults(const wchar_t* fileName);
	static void GetMemory(__int64* workingSet);
	static void NoteMemory( string title);
	static void AddComment(string comment);
};
