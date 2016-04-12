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

namespace bits
{

template <typename ValueType>
void radix8sort(uint32_t* __restrict keys_in_out,
    uint32_t* __restrict keys_temp, ValueType* __restrict values_in_out, ValueType* values_temp,
    uint32_t size);

template <typename ValueType>
void radix8sort(uint64_t* __restrict keys_in_out,
    uint64_t* __restrict keys_temp, ValueType* __restrict values_in_out, ValueType* values_temp,
    uint32_t size);

template <typename ValueType>
void radix8sort(float* __restrict keys_in_out, float* __restrict keys_temp,
    ValueType* __restrict values_in_out, ValueType* __restrict values_temp, uint32_t size);

template <typename ValueType>
void radix11sort(uint32_t* __restrict keys_in,
    uint32_t* __restrict keys_out, ValueType* __restrict values_in,
    ValueType* __restrict values_out, uint32_t size);

template <typename ValueType>
void radix11sort(uint64_t* __restrict keys_in,
    uint64_t* __restrict keys_out, ValueType* __restrict values_in,
    ValueType* __restrict values_out, uint32_t size);

template <typename ValueType>
void radix11sort(float* __restrict keys_in, float* __restrict keys_out,
    ValueType* __restrict values_in, ValueType* __restrict values_out, uint32_t size);

} // namespace bits

#include "radixsort.inl"

#endif // BITS_RADIXSORT_HPP
