#ifndef BITS_RADIXSORT_H
#define BITS_RADIXSORT_H

#include <stdint.h>
#include <stdlib.h>

#if _MSC_VER <= 1700
#define restrict __restrict
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void radixSort8UInt32(
		uint32_t * restrict keysInOut, uint32_t * restrict keysTemp,
		uint32_t * restrict valuesInOut, uint32_t * valuesTemp,
		uint32_t size);

extern void radixSort8UInt64(
		uint64_t * restrict keysInOut, uint64_t * restrict keysTemp,
		uint32_t * restrict valuesInOut, uint32_t * valuesTemp,
		uint32_t size);

extern void radixSort8Float(
		float * restrict keysInOut, float * restrict keysTemp,
		uint32_t * restrict valuesInOut, uint32_t * restrict valuesTemp,
		uint32_t size);

extern void radixSort11UInt32(
		uint32_t * restrict keysIn, uint32_t * restrict keysOut,
		uint32_t * restrict valuesIn, uint32_t * restrict valuesOut,
		uint32_t size);

extern void radixSort11UInt64(
		uint64_t * restrict keysInOut, uint64_t * restrict keysTemp,
		uint32_t * restrict valuesInOut, uint32_t * valuesTemp,
		uint32_t size);

extern void radixSort11Float(
		float * restrict keysIn, float * restrict keysOut,
		uint32_t * restrict valuesIn, uint32_t * restrict valuesOut,
		uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // BITS_RADIXSORT_H

