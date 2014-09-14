#ifdef __linux__

#define _POSIX_C_SOURCE 200809L

#include "timer.h"

#include <assert.h>
#include <inttypes.h>
#include <time.h>

#define TO_NANOSECS 1000000000


void bitsTimeInit(bitsTime * time)
{
	static_assert(sizeof(struct timespec) <= sizeof(bitsTime),
		"Not enough space for timespec");
	struct timespec * ts = (struct timespec *)time;
	clock_gettime(CLOCK_MONOTONIC, ts);
}


double bitsTimeDeltaSeconds(const bitsTime * time)
{
	struct timespec now, result;
	const struct timespec * start = (struct timespec *)time;
	clock_gettime(CLOCK_MONOTONIC, &now);
	result.tv_sec = now.tv_sec - start->tv_sec;
	result.tv_nsec = now.tv_nsec - start->tv_nsec;
	return (double)((result.tv_sec * TO_NANOSECS) + result.tv_nsec) / TO_NANOSECS;
}

#endif // __linux__
