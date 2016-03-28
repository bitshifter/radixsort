#include "helpers.hpp"
#include "radixsort.hpp"

#include <array>

using namespace bits;

template <typename KeyType, typename Rng>
void test_radix8sort(Rng & rng)
{
    static const uint32_t array_size = 8;
    std::array<KeyType, array_size> keys_in_out;
    std::array<KeyType, array_size> keys_temp;
    std::array<KeyType, array_size> keys_copy;
    std::array<uint32_t, array_size> values_in_out;
    std::array<uint32_t, array_size> values_temp;

    rand_keys(rng, keys_in_out.data(), values_in_out.data(), keys_copy.data(), array_size);
    print_array(keys_in_out.data(), array_size);
    radix8sort(keys_in_out.data(), keys_temp.data(), values_in_out.data(), values_temp.data(), array_size);
    print_array(keys_in_out.data(), array_size);
    check_sorted(keys_in_out.data(), values_in_out.data(), keys_copy.data(), array_size);
}


template <typename KeyType, typename Rng>
void test_radix11sort(Rng & rng)
{
    static const uint32_t array_size = 8;
    KeyType keys_in[array_size];
    KeyType keys_out[array_size];
    KeyType keys_copy[array_size];
    uint32_t values_in[array_size];
    uint32_t values_out[array_size];

    rand_keys(rng, keys_in, values_in, keys_copy, array_size);
    print_array(keys_in, array_size);
    auto result = radix11sort(keys_in, keys_out, values_in, values_out, array_size);
    print_array(result.first, array_size);
    check_sorted(result.first, result.second, keys_copy, array_size);
}

int main()
{
    // mersenne twister prng
    std::mt19937 rnd32;
    std::mt19937_64 rnd64;

    puts("test radix8 uint32_t");
    test_radix8sort<uint32_t>(rnd32);

    puts("test radix8 uint64_t");
    test_radix8sort<uint64_t>(rnd64);

    puts("test radix8 float");
    test_radix8sort<float>(rnd32);

    puts("test radix11 uint32_t");
    test_radix11sort<uint32_t>(rnd32);

    puts("test radix11 uint64_t");
    test_radix11sort<uint64_t>(rnd64);

    puts("test radix11 float");
    test_radix11sort<float>(rnd32);
}
