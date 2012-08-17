#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <psapi.h>
#include "PerformanceMetrics.h"

using namespace std;

static Hpt timers[TOTAL_TIME + 1];
static bool isEnable = true;
static std::vector<std::pair<string, __int64>> memorySizes;
static std::vector<string> comments;

///////////////////////////////////
void PerformanceMetrics::Enable()
{
	ResetAll();
	isEnable = true;
}
void PerformanceMetrics::Disable()
{
	isEnable = false;
}
///////////////////////////////////
void PerformanceMetrics::Start(TIMER_TYPE timerType)
{
	if (!isEnable)
		return;
	timers[timerType].Start();
}
void PerformanceMetrics::Stop(TIMER_TYPE timerType)
{
	if (!isEnable)
		return;
	timers[timerType].Stop();
}
double PerformanceMetrics::GetTime(TIMER_TYPE timerType)
{
	if (!isEnable)
		return 0.0;
	return timers[timerType].GetTime();
}
void PerformanceMetrics::Reset(TIMER_TYPE timerType)
{
	if (!isEnable)
		return;
	timers[timerType].Reset();
}
void PerformanceMetrics::ResetAll()
{
	if (!isEnable)
		return;
	for (int i = 0; i <= TOTAL_TIME; i++)
	{
		timers[i].Reset();
	}
	memorySizes.clear();
}
void PerformanceMetrics::ReportResults(const wchar_t* fileName)
{
	if (!isEnable)
		return;
	double totalTesselationTime = timers[TESSELATION_TIME].GetTime();
	double graphBuildingTime	= timers[GRAPH_BUILDING_TIME].GetTime();
	double renderingTime		= timers[RENDERING_TIME].GetTime();
	double updateViewTime		= timers[UPDATE_VIEW_TIME].GetTime();
	double totalTime			= timers[TOTAL_TIME].GetTime();

	double pureTesselationTime	= totalTesselationTime - graphBuildingTime - renderingTime;
	double otherTime			= totalTime - totalTesselationTime - updateViewTime;

	#ifdef HOOPSRENDERER
	wchar_t* fName = L"C:\\CGMViewerHOOPS.report";
	#elif _USE_SCENIX_RENDERER
	wchar_t* fName = L"C:\\CGMViewerSceniX.report";
	#elif _USE_RED_RENDERER
	wchar_t* fName = L"C:\\CGMViewerRED.report";
	#else
	wchar_t* fName = L"C:\\CGMViewerOpenGL.report";
	#endif 
	
	ofstream myfile( fName );
	if (myfile.is_open())
	{
		myfile << "Tesselation only time   = " << pureTesselationTime << endl;
		myfile << "Graph building time     = " << graphBuildingTime << endl;
		myfile << "Rendering time          = " << renderingTime << endl;
		myfile << "Total time in tesselate = " << totalTesselationTime << endl;
		myfile << "Update view time        = " << updateViewTime << endl;
		myfile << "Total time              = " << totalTime << endl;
		myfile << "Other time              = " << otherTime << endl;
		myfile << endl;
		myfile << "Debug time 1            = " << timers[DEBUG_TIME_1].GetTime() << endl;
		myfile << "Debug time 2            = " << timers[DEBUG_TIME_2].GetTime() << endl;
		myfile << "Debug time 3            = " << timers[DEBUG_TIME_3].GetTime() << endl;
		myfile << endl;

		std::vector<pair<string, __int64>>::iterator pos;
		int length = 0;
		for (pos = memorySizes.begin(); pos != memorySizes.end(); ++pos)
		{
			string title = pos->first;
			if ((int)title.length() > length)
				length = title.length();
		}
	    char buffer[16];
		for (pos = memorySizes.begin(); pos != memorySizes.end(); ++pos)
		{
			string title = pos->first;
			title.resize(length, ' ');
			__int64 workingSet = pos->second;
			memset(buffer, ' ', 16);
			sprintf_s( buffer, 16, "%7d", workingSet );

			myfile << "Working set (kB) at '" << title << "' = " << string(buffer) << endl;
		}
		myfile << endl;

		std::vector<string>::iterator iter;
		for ( iter = comments.begin(); iter != comments.end(); ++iter)
		{
			string comment = iter->data();
			myfile << comment << endl;
		}
		myfile.close();
	}
	else 
		cout << "Unable to open file";
}

void PerformanceMetrics::GetMemory( __int64* workingSet)
{
	if (!isEnable)
		return;
   PROCESS_MEMORY_COUNTERS_EX pmc = { sizeof(PROCESS_MEMORY_COUNTERS_EX) };
   GetProcessMemoryInfo(GetCurrentProcess(), (PPROCESS_MEMORY_COUNTERS)&pmc, sizeof(pmc));

   *workingSet = (__int64) pmc.WorkingSetSize / 1024;
}
void PerformanceMetrics::NoteMemory(string title)
{
	if (!isEnable)
		return;
	__int64 workingSet;
	GetMemory( &workingSet);

	memorySizes.push_back(pair<string, __int64>( title, workingSet));
	//memorySizes.insert( std::map<string, __int64>::value_type( title, workingSet));
}


void PerformanceMetrics::AddComment(string comment)
{
	comments.push_back(comment);
}
