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
#ifndef BITS_RADIXSORT_HPP
#define BITS_RADIXSORT_HPP

#include <cstdint>
#include <utility>

#define restrict __restrict

namespace bits
{

template <typename ValueType>
std::pair<uint32_t *, ValueType *> radixSort8(
		uint32_t * restrict keysInOut, uint32_t * restrict keysTemp,
		ValueType * restrict valuesInOut, ValueType * valuesTemp,
		uint32_t size);

template <typename ValueType>
std::pair<uint64_t *, ValueType *> radixSort8(
		uint64_t * restrict keysInOut, uint64_t * restrict keysTemp,
		ValueType * restrict valuesInOut, ValueType * valuesTemp,
		uint32_t size);

template <typename ValueType>
std::pair<float *, ValueType *> radixSort8(
		float * restrict keysInOut, float * restrict keysTemp,
		ValueType * restrict valuesInOut, ValueType * restrict valuesTemp,
		uint32_t size);

template <typename ValueType>
std::pair<uint32_t *, ValueType *> radixSort11(
		uint32_t * restrict keysIn, uint32_t * restrict keysOut,
		ValueType * restrict valuesIn, ValueType * restrict valuesOut,
		uint32_t size);

template <typename ValueType>
std::pair<uint64_t *, ValueType *> radixSort11(
		uint64_t * restrict keysInOut, uint64_t * restrict keysTemp,
		ValueType * restrict valuesInOut, ValueType * restrict valuesTemp,
		uint32_t size);

template <typename ValueType>
std::pair<float *, ValueType *> radixSort11(
		float * restrict keysIn, float * restrict keysOut,
		ValueType * restrict valuesIn, ValueType * restrict valuesOut,
		uint32_t size);

} // namespace bits

#include "radixsort.inl"

#undef restrict

#endif // BITS_RADIXSORT_HPP
