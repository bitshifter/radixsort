/*
 * Copyright (c) 2014 Cameron Hart
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
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

