#include "stdafx.h"

#include <Windows.h>

#include "CStopWatch.h"

CStopWatch::CStopWatch()
{
	timer.start.QuadPart = 0;
	timer.stop.QuadPart = 0;
	QueryPerformanceFrequency(&frequency);
}

CStopWatch::~CStopWatch()
{
}

double CStopWatch::LItoSecs(LARGE_INTEGER &L)
{
	return ((double)L.QuadPart / (double)frequency.QuadPart);
}

void CStopWatch::StartTimer()
{
	QueryPerformanceCounter(&timer.start);
}

void CStopWatch::StopTimer()
{
	QueryPerformanceCounter(&timer.stop);
}

double CStopWatch::getElapsedTime()
{
	LARGE_INTEGER time;
	time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart;
	return LItoSecs(time);
}