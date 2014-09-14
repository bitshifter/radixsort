#ifndef BITS_TIMER_H
#define BITS_TIMER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Structure must have enough space for all supported platforms */
typedef struct bitsTime
{
	union
	{
		uint64_t u64[2];
	} storage;
} bitsTime;

extern void bitsTimeInit(bitsTime * time);
extern double bitsTimeDeltaSeconds(const bitsTime * startTime);

#ifdef __cplusplus
}
#endif

#define BITS_TIMER_START(name) \
	bitsTime btsTimerStart##name; bitsTimeInit(&btsTimerStart##name)

#define BITS_TIMER_END(name) \
	bitsTimeDeltaSeconds(&btsTimerStart##name);

#endif // BITS_TIMER_H

