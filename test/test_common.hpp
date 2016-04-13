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


template <typename KeyType, typename ValueType, uint32_t N = 8>
void test_radixsort(uint32_t (*radixsort)(KeyType*, KeyType*, ValueType*, ValueType*, uint32_t))
{
    static const size_t array_size = N;
    typename RngType<KeyType>::type rng;
    KeyType keys[2][array_size];
    KeyType keys_copy[array_size];
    uint32_t indices[2][array_size];

    rand_keys(rng, keys[0], indices[0], keys_copy, array_size);
    auto out = radixsort(keys[0], keys[1], indices[0], indices[1], array_size);

    REQUIRE(out < 2);

    const KeyType* keys_out = keys[out];
    const uint32_t* indices_out = indices[out];

    for (uint32_t i = 1; i < array_size; ++i)
    {
        REQUIRE(keys_out[i - 1] <= keys_out[i]);
        REQUIRE(keys_out[i] == keys_copy[indices_out[i]]);
    }
}

} // namespace bits

#endif // BITS_TEST_COMMON_HPP
