#ifndef _CSTOP_WATCH_H_
#define _CSTOP_WATCH_H_

#ifndef __linux


#include <Windows.h>

typedef struct
{
	LARGE_INTEGER start;
	LARGE_INTEGER stop;
} stopWatch;

class CStopWatch
{
public:
	CStopWatch();
	~CStopWatch();

	void StartTimer();
	void StopTimer();
	double getElapsedTime();

private:
	stopWatch timer;
	LARGE_INTEGER frequency;
	double LItoSecs(LARGE_INTEGER &L);
};

#endif
#endif _CSTOP_WATCH_H_