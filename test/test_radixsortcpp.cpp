#include <catch2/catch_test_macros.hpp>

#include "test_common.hpp"
#include "radixsort.hpp"

uint32_t radix8sort_u32(uint32_t* keys_in_out, uint32_t* keys_temp, uint32_t* values_in_out,
    uint32_t* values_temp, uint32_t size)
{
    return bits::radix8sort(keys_in_out, keys_temp, values_in_out, values_temp, size);
}

uint32_t radix8sort_u64(uint64_t*  keys_in_out, uint64_t*  keys_temp,
    uint32_t*  values_in_out, uint32_t* values_temp, uint32_t size)
{
    return bits::radix8sort(keys_in_out, keys_temp, values_in_out, values_temp, size);
}

uint32_t radix8sort_f32(float*  keys_in_out, float*  keys_temp,
    uint32_t*  values_in_out, uint32_t*  values_temp, uint32_t size)
{
    return bits::radix8sort(keys_in_out, keys_temp, values_in_out, values_temp, size);
}

uint32_t radix11sort_u32(uint32_t*  keys_in, uint32_t*  keys_out,
    uint32_t*  values_in, uint32_t*  values_out, uint32_t size)
{
    return bits::radix11sort(keys_in, keys_out, values_in, values_out, size);
}

uint32_t radix11sort_u64(uint64_t*  keys_in, uint64_t*  keys_out,
    uint32_t*  values_in, uint32_t*  values_out, uint32_t size)
{
    return bits::radix11sort(keys_in, keys_out, values_in, values_out, size);
}

uint32_t radix11sort_f32(float*  keys_in, float*  keys_out,
    uint32_t*  values_in, uint32_t*  values_out, uint32_t size)
{
    return bits::radix11sort(keys_in, keys_out, values_in, values_out, size);
}

TEST_CASE("cpp/radix8sort uint32_t")
{
    bits::test_radixsort(radix8sort_u32);
}

TEST_CASE("cpp/radix8sort uint64_t")
{
    bits::test_radixsort(radix8sort_u64);
}

TEST_CASE("cpp/radix8sort float")
{
    bits::test_radixsort(radix8sort_f32);
}

TEST_CASE("cpp/radix11sort uint32_t")
{
    bits::test_radixsort(radix11sort_u32);
}

TEST_CASE("cpp/radix11sort uint64_t")
{
    bits::test_radixsort(radix11sort_u64);
}

TEST_CASE("cpp/radix11sort float")
{
    bits::test_radixsort(radix11sort_f32);
}
