#ifndef BITS_TIMER_H
#define BITS_TIMER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Structure must have enough space for all supported platforms */
typedef struct BitsTime
{
    union
    {
        uint64_t u64[2];
    } storage;
} BitsTime;

extern void bits_time_init(BitsTime* time);
extern double bits_time_delta_secs(const BitsTime* start_time);

#ifdef __cplusplus
}
#endif

#define BITS_TIMER_START(name)                                                                     \
    BitsTime btsTimerStart##name;                                                                  \
    bits_time_init(&btsTimerStart##name)

#define BITS_TIMER_END(name) bits_time_delta_secs(&btsTimerStart##name);

#endif // BITS_TIMER_H
