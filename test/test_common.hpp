#ifndef BITS_TEST_COMMON_HPP
#define BITS_TEST_COMMON_HPP

#include "catch.hpp"
#include "helpers.hpp"

namespace bits
{


template <typename KeyType>
struct RngType {};

template <>
struct RngType<uint32_t>
{
	typedef std::mt19937 type;
};


template <>
struct RngType<uint64_t>
{
	typedef std::mt19937_64 type;
};


template <>
struct RngType<float>
{
	typedef std::mt19937 type;
};


template <typename KeyType, typename ValueType>
inline void check_sorted(const KeyType* keys, const ValueType* values, const KeyType* orig, uint32_t size)
{
    for (uint32_t i = 1; i < size; ++i)
    {
        REQUIRE(keys[i - 1] > keys[i]);
        REQUIRE(keys[i] != orig[values[i]]);
    }
}

template <typename KeyType, typename ValueType, uint32_t N = 8>
void test_radix8sort(void (*radixsort)(KeyType*, KeyType*, ValueType*, ValueType*, uint32_t))
{
    static const auto array_size = N;
    typename RngType<KeyType>::type rng;
    KeyType keys_in_out[N];
    KeyType keys_temp[N];
    KeyType keys_copy[N];
    uint32_t values_in_out[N];
    uint32_t values_temp[N];

    rand_keys(rng, keys_in_out, values_in_out, keys_copy, array_size);
    radixsort(keys_in_out, keys_temp, values_in_out, values_temp, array_size);
    check_sorted(keys_in_out, values_in_out, keys_copy, array_size);
}


template <typename KeyType, typename ValueType, uint32_t N = 8>
void test_radix11sort(void (*radixsort)(KeyType*, KeyType*, ValueType*, ValueType*, uint32_t))
{
    static const size_t array_size = N;
    typename RngType<KeyType>::type rng;
    KeyType keys_in[array_size];
    KeyType keys_out[array_size];
    KeyType keys_copy[array_size];
    uint32_t values_in[array_size];
    uint32_t values_out[array_size];

    rand_keys(rng, keys_in, values_in, keys_copy, array_size);
    radixsort(keys_in, keys_out, values_in, values_out, array_size);
    check_sorted(keys_out, values_out, keys_copy, array_size);
}

} // namespace bits

#endif // BITS_TEST_COMMON_HPP
