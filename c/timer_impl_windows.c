#ifdef WIN32
#include "timer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define TO_NANOSECS 1000000000

void bits_time_init(BitsTime * time)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER delta;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&delta);

    time->storage = (uint64_t)(TO_NANOSECS * delta.QuadPart / frequency.QuadPart);
}


double bits_time_delta_secs(const BitsTime * time)
{
    BitsTime now;
    bits_time_init(&now);
    return (double)(now.storage - time->storage) / TO_NANOSECS;
}

#endif // WIN32
