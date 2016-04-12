#include "helpers.hpp"
#include "radixsort.hpp"

#include <algorithm>
#include <chrono>
#include <vector>

//#define PRINT_ARRAY(keys, size) print_array((keys), (size))
#define PRINT_ARRAY(...)

template <typename KeyType, typename Rand>
void perf_test(Rand rng, uint32_t array_size, uint32_t iterations)
{
    using namespace bits;

    using std::chrono::duration;
    using std::chrono::high_resolution_clock;
    using std::chrono::seconds;
    using std::vector;

    duration<double, seconds::period> radix8_total_s, radix11_total_s, std_sort_total_s,
        std_stable_sort_total_s;
    double inv_iter = 1.0 / (double)iterations;

    vector<KeyType> keys_orig(array_size);
    vector<KeyType> keys0(array_size);
    vector<KeyType> keys1(array_size);
    vector<uint32_t> values_orig(array_size);
    vector<uint32_t> values0(array_size);
    vector<uint32_t> values1(array_size);

    for (uint32_t i = 0; i < iterations; ++i)
    {
        rand_keys(rng, keys_orig.data(), values_orig.data(), keys_orig.data(), array_size);

        {
            keys0 = keys_orig;
            values0 = values_orig;
            PRINT_ARRAY(keys0, array_size);
            auto start = high_resolution_clock::now();
            auto result =
                radix8sort(keys0.data(), keys1.data(), values0.data(), values1.data(), array_size);
            radix8_total_s += high_resolution_clock::now() - start;
            PRINT_ARRAY(result.first, array_size);
            check_sorted(result.first, result.second, keys_orig.data(), array_size);
        }

        {
            keys0 = keys_orig;
            values0 = values_orig;
            PRINT_ARRAY(keys0, array_size);
            auto start = high_resolution_clock::now();
            auto result =
                radix11sort(keys0.data(), keys1.data(), values0.data(), values1.data(), array_size);
            radix11_total_s += high_resolution_clock::now() - start;
            PRINT_ARRAY(result.first, array_size);
            check_sorted(result.first, result.second, keys_orig.data(), array_size);
        }

        {
            keys0 = keys_orig;
            PRINT_ARRAY(keys0, array_size);
            auto start = high_resolution_clock::now();
            std::sort(std::begin(keys0), std::end(keys0));
            std_sort_total_s += high_resolution_clock::now() - start;
            PRINT_ARRAY(keys1, array_size);
        }

        {
            keys0 = keys_orig;
            PRINT_ARRAY(keys0, array_size);
            auto start = high_resolution_clock::now();
            std::stable_sort(std::begin(keys0), std::end(keys0));
            std_stable_sort_total_s += high_resolution_clock::now() - start;
            PRINT_ARRAY(keys1, array_size);
        }
    }

    printf("%6" PRIu32 "  %5f  %5f  %5f  %5f\n", array_size, radix8_total_s.count() * inv_iter,
        radix11_total_s.count() * inv_iter, std_sort_total_s.count() * inv_iter,
        std_stable_sort_total_s.count() * inv_iter);
}

int main()
{
    uint32_t start = 32, end = 65536, iterations = 100;

    // mersenne twister prng
    std::mt19937 rnd32;
    std::mt19937_64 rnd64;

    printf("Radix sort uint32_t key (%u iterations)\n", iterations);
    puts("  size    radix8   radix11 std::sort  ::stable");
    for (uint32_t i = start; i <= end; i = i << 1)
    {
        perf_test<uint32_t>(rnd32, i, iterations);
    }

    printf("\nRadix sort uint64_t key (%u iterations)\n", iterations);
    puts("  size    radix8   radix11 std::sort  ::stable");
    for (uint32_t i = start; i <= end; i = i << 1)
    {
        perf_test<uint64_t>(rnd64, i, iterations);
    }

    printf("\nRadix sort float key (%u iterations)\n", iterations);
    puts("  size    radix8   radix11 std::sort  ::stable");
    for (uint32_t i = start; i <= end; i = i << 1)
    {
        perf_test<float>(rnd32, i, iterations);
    }
}
