#include "catch.hpp"
#include "test_common.hpp"
#include "radixsort.h"

TEST_CASE("c/radix8sort uint32_t")
{
    bits::test_radixsort(radix8sort_u32);
}

TEST_CASE("c/radix8sort uint64_t")
{
    bits::test_radixsort(radix8sort_u64);
}

TEST_CASE("c/radix8sort float")
{
    bits::test_radixsort(radix8sort_f32);
}

TEST_CASE("c/radix11sort uint32_t")
{
    bits::test_radixsort(radix11sort_u32);
}

TEST_CASE("c/radix11sort uint64_t")
{
    bits::test_radixsort(radix11sort_u64);
}

TEST_CASE("c/radix11sort float")
{
    bits::test_radixsort(radix11sort_f32);
}

