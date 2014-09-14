#ifdef WIN32
#include "timer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define TO_NANOSECS 1000000000

void bitsTimeInit(bitsTime * time)
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER delta;

	QueryPerformanceFrequency(&frequency); 
	QueryPerformanceCounter(&delta);

	time->storage = (uint64_t)(TO_NANOSECS * delta.QuadPart / frequency.QuadPart);
}


double bitsTimeDeltaSeconds(const bitsTime * time)
{
	bitsTime now;
	bitsTimeInit(&now);
	return (double)(now.storage - time->storage) / TO_NANOSECS;
}

#endif // WIN32
