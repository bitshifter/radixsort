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
#include "radixsort.h"
#include <assert.h>
#include <string.h>


/**
 * Flip a float for sorting.
 *  finds SIGN of fp number.
 *  if it's 1 (negative float), it flips all bits
 *  if it's 0 (positive float), it flips the sign only
 */
static inline uint32_t floatFlip(uint32_t f)
{
	uint32_t mask = -((int32_t)(f >> 31)) | 0x80000000;
	return f ^ mask;
}

/**
 * Flip a float back (invert floatFlip)
 *  signed was flipped from above, so:
 *  if sign is 1 (negative), it flips the sign bit back
 *  if sign is 0 (positive), it flips all bits back
 */
static inline uint32_t invFloatFlip(uint32_t f)
{
	uint32_t mask = ((f >> 31) - 1) | 0x80000000;
	return f ^ mask;
}

/**
  * Initialise each histogram bucket with the key value
  */
static void initHistogramsUInt32(
		uint32_t kRadixBits,
		uint32_t kHistBuckets,
		uint32_t kHistSize,
		uint32_t * restrict hist,
		const uint32_t * restrict keysIn,
		uint32_t size)
{
	const uint32_t kHistMask = kHistSize - 1;
	for (uint32_t i = 0; i < size; ++i)
	{
		const uint32_t key = keysIn[i];
		for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
		{
			const uint32_t shift = bucket * kRadixBits;
			const uint32_t pos = (key >> shift) & kHistMask;
			uint32_t * offset = hist + (bucket * kHistSize);
			++offset[pos];
		}
	}
}


static void initHistogramsUInt64(
		uint32_t kRadixBits,
		uint32_t kHistBuckets,
		uint32_t kHistSize,
		uint32_t * restrict hist,
		const uint64_t * restrict keysIn,
		uint32_t size)
{
	const uint32_t kHistMask = kHistSize - 1;
	for (uint32_t i = 0; i < size; ++i)
	{
		const uint64_t key = keysIn[i];
		for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
		{
			const uint32_t shift = bucket * kRadixBits;
			const uint32_t pos = (key >> shift) & kHistMask;
			uint32_t * offset = hist + (bucket * kHistSize);
			++offset[pos];
		}
	}
}


static void initHistogramsFloat(
		uint32_t kRadixBits,
		uint32_t kHistBuckets,
		uint32_t kHistSize,
		uint32_t * restrict hist,
		const uint32_t * restrict keysIn,
		uint32_t size)
{
	const uint32_t kHistMask = kHistSize - 1;
	for (uint32_t i = 0; i < size; ++i)
	{
		const uint32_t key = floatFlip(keysIn[i]);
		for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
		{
			const uint32_t shift = bucket * kRadixBits;
			const uint32_t pos = (key >> shift) & kHistMask;
			uint32_t * offset = hist + (bucket * kHistSize);
			++offset[pos];
		}
	}
}


/**
 * Update the histogram data so each entry sums the previous entries.
 */
static void sumHistograms(
		uint32_t kHistBuckets,
		uint32_t kHistSize,
		uint32_t * restrict hist
		)
{
	uint32_t sum[kHistBuckets];
	for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
	{
		uint32_t * restrict offset = hist + (bucket * kHistSize);
		sum[bucket] = offset[0];
		offset[0] = 0;
	}

	uint32_t tsum;
	for (uint32_t i = 1; i < kHistSize; ++i)
	{
		for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
		{
			uint32_t * restrict offset = hist + (bucket * kHistSize);
			tsum = offset[i] + sum[bucket];
			offset[i] = sum[bucket];
			sum[bucket] = tsum;
		}
	}
}


/**
 * Perform a radix sort pass for the given bit shift and mask.
 */
static inline void radixPassUInt32(
		uint32_t * restrict hist, uint32_t shift, uint32_t mask,
		const uint32_t * restrict keysIn, uint32_t * restrict keysOut,
		const uint32_t * restrict valuesIn, uint32_t * restrict valuesOut,
		uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
	{
		const uint32_t key = keysIn[i];
		const uint32_t pos = (key >> shift) & mask;
		const uint32_t index = hist[pos]++;
		keysOut[index] = key;
		valuesOut[index] = valuesIn[i];
	}
}


static inline void radixPassUInt64(
		uint32_t * restrict hist, uint32_t shift, uint32_t mask,
		const uint64_t * restrict keysIn, uint64_t * restrict keysOut,
		const uint32_t * restrict valuesIn, uint32_t * restrict valuesOut,
		uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
	{
		const uint64_t key = keysIn[i];
		const uint32_t pos = (key >> shift) & mask;
		const uint32_t index = hist[pos]++;
		keysOut[index] = key;
		valuesOut[index] = valuesIn[i];
	}
}


static inline void radixSortUInt32(uint32_t kRadixBits,
		uint32_t * restrict keysIn, uint32_t * restrict keysTemp,
		uint32_t * restrict valuesIn, uint32_t * valuesTemp,
		uint32_t size
		)
{
	const uint32_t kHistBuckets = 1 + (((sizeof(uint32_t) * 8) - 1) / kRadixBits);
	const uint32_t kHistSize = 1 << kRadixBits;
	uint32_t hist[kHistBuckets * kHistSize];
	memset(hist, 0, sizeof(uint32_t) * kHistBuckets * kHistSize);

	initHistogramsUInt32(kRadixBits, kHistBuckets, kHistSize, hist,
		keysIn, size);

	sumHistograms(kHistBuckets, kHistSize, hist);

	// alternate input and output buffers on each radix pass
	uint32_t * restrict keys[2] = { keysIn, keysTemp };
	uint32_t * restrict values[2] = { valuesIn, valuesTemp };

	const uint32_t kHistMask = kHistSize - 1;
	for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
	{
		const uint32_t in = bucket & 1; const uint32_t out = !in;
		uint32_t * restrict offset = hist + (bucket * kHistSize);
		radixPassUInt32(offset, bucket * kRadixBits, kHistMask,
			keys[in], keys[out], values[in], values[out], size);
	}
}


void radixSort8UInt32(uint32_t * restrict keysInOut,
		uint32_t * restrict keysTemp, uint32_t * restrict valuesInOut,
		uint32_t * valuesTemp, uint32_t size)
{
	radixSortUInt32(8, keysInOut, keysTemp, valuesInOut, valuesTemp, size);
}


void radixSort11UInt32(uint32_t * restrict keysIn,
		uint32_t * restrict keysOut, uint32_t * restrict valuesIn,
		uint32_t * restrict valuesOut, uint32_t size)
{
	radixSortUInt32(11, keysIn, keysOut, valuesIn, valuesOut, size);
}


static inline void radixSortUInt64(uint32_t kRadixBits,
		uint64_t * restrict keysIn, uint64_t * restrict keysTemp,
		uint32_t * restrict valuesIn, uint32_t * valuesTemp,
		uint32_t size
		)
{
	const uint32_t kHistBuckets = 1 + (((sizeof(uint64_t) * 8) - 1) / kRadixBits);
	const uint32_t kHistSize = 1 << kRadixBits;
	uint32_t hist[kHistBuckets * kHistSize];
	memset(hist, 0, sizeof(uint32_t) * kHistBuckets * kHistSize);

	initHistogramsUInt64(kRadixBits, kHistBuckets, kHistSize, hist,
		keysIn, size);

	sumHistograms(kHistBuckets, kHistSize, hist);

	// alternate input and output buffers on each radix pass
	uint64_t * restrict keys[2] = { keysIn, keysTemp };
	uint32_t * restrict values[2] = { valuesIn, valuesTemp };

	const uint32_t kHistMask = kHistSize - 1;
	for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
	{
		const uint32_t in = bucket & 1; const uint32_t out = !in;
		uint32_t * restrict offset = hist + (bucket * kHistSize);
		radixPassUInt64(offset, bucket * kRadixBits, kHistMask,
			keys[in], keys[out], values[in], values[out], size);
	}
}


void radixSort8UInt64(
		uint64_t * restrict keysInOut, uint64_t * restrict keysTemp,
		uint32_t * restrict valuesInOut, uint32_t * valuesTemp,
		uint32_t size)
{
	radixSortUInt64(8, keysInOut, keysTemp, valuesInOut, valuesTemp, size);
}


void radixSort11UInt64(
		uint64_t * restrict keysInOut, uint64_t * restrict keysTemp,
		uint32_t * restrict valuesInOut, uint32_t * valuesTemp,
		uint32_t size)
{
	radixSortUInt64(11, keysInOut, keysTemp, valuesInOut, valuesTemp, size);
}


static inline void radixSortFloat(uint32_t kRadixBits,
		float * restrict keysInf, float * restrict keysTempf,
		uint32_t * restrict valuesIn, uint32_t * valuesTemp,
		uint32_t size
		)
{
	// create uint32_t pointers to inputs to avoid float to int casting
	uint32_t * restrict keysIn = (uint32_t*)keysInf;
	uint32_t * restrict keysTemp = (uint32_t*)keysTempf;

	const uint32_t kHistBuckets = 1 + (((sizeof(uint32_t) * 8) - 1) / kRadixBits);
	const uint32_t kHistSize = 1 << kRadixBits;
	uint32_t hist[kHistBuckets * kHistSize];
	memset(hist, 0, sizeof(uint32_t) * kHistBuckets * kHistSize);

	initHistogramsFloat(kRadixBits, kHistBuckets, kHistSize, hist,
		keysIn, size);

	sumHistograms(kHistBuckets, kHistSize, hist);

	// alternate input and output buffers on each radix pass
	uint32_t * restrict keys[2] = { keysIn, keysTemp };
	uint32_t * restrict values[2] = { valuesIn, valuesTemp };
	const uint32_t kHistMask = kHistSize - 1;

	{
		const uint32_t bucket = 0;
		const uint32_t in = bucket & 1; const uint32_t out = !in;
		uint32_t * restrict offset = hist + (bucket * kHistSize);
		for (uint32_t i = 0; i < size; ++i)
		{
			const uint32_t key = floatFlip(keys[in][i]);
			const uint32_t pos = key & kHistMask;
			const uint32_t index = offset[pos]++;
			keys[out][index] = key;
			values[out][index] = values[in][i];
		}
	}

	for (uint32_t bucket = 1; bucket < kHistBuckets - 1; ++bucket)
	{
		const uint32_t in = bucket & 1; const uint32_t out = !in;
		uint32_t * restrict offset = hist + (bucket * kHistSize);
		radixPassUInt32(offset, bucket * kRadixBits, kHistMask,
			keys[in], keys[out], values[in], values[out], size);
	}

	{
		const uint32_t bucket = kHistBuckets - 1;
		const uint32_t shift = bucket * kRadixBits;
		const uint32_t in = bucket & 1; const uint32_t out = !in;
		uint32_t * restrict offset = hist + (bucket * kHistSize);
		for (uint32_t i = 0; i < size; ++i)
		{
			const uint32_t key = keys[in][i];
			const uint32_t pos = (key >> shift) & kHistMask;
			const uint32_t index = offset[pos]++;
			keys[out][index] = invFloatFlip(key);
			values[out][index] = values[in][i];
		}
	}
}


void radixSort8Float(float * restrict keysInOutf,
		float * restrict keysTempf, uint32_t * restrict valuesInOut,
		uint32_t * restrict valuesTemp, uint32_t size)
{
	radixSortFloat(8, keysInOutf, keysTempf, valuesInOut, valuesTemp, size);
}


void radixSort11Float(float * restrict keysInf,
		float * restrict keysOutf, uint32_t * restrict valuesIn,
		uint32_t * restrict valuesOut, uint32_t size)
{
	radixSortFloat(11, keysInf, keysOutf, valuesIn, valuesOut, size);
}

