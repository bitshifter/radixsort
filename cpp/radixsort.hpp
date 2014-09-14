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
