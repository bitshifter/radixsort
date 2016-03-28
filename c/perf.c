#include "helpers.h"
#include "radixsort.h"
#include "timer.h"

#include <stdbool.h>
#include <string.h>

// disable printing for perf tests
#undef PRINT_FLOAT_ARRAY
#undef PRINT_UINT32_ARRAY
#undef PRINT_UINT64_ARRAY
#define PRINT_FLOAT_ARRAY(...)
#define PRINT_UINT32_ARRAY(...)
#define PRINT_UINT64_ARRAY(...)

void perf_test_u32(uint32_t array_size, uint32_t iterations)
{

    double radix8_total_s = 0, radix11_total_s = 0;
    double inv_iter = 1.0 / (double)iterations;

    uint32_t * keys_orig = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * keys0 = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * keys1 = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * values_orig = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * values0 = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * values1 = (uint32_t *)malloc(sizeof(uint32_t) * array_size);

    for (uint32_t i = 0; i < iterations; ++i)
    {
        RAND_UINT32_KEYS(keys_orig, values_orig, keys_orig, array_size);

        memcpy(keys0, keys_orig, sizeof(uint32_t) * array_size);
        memcpy(values0, values_orig, sizeof(uint32_t) * array_size);
        PRINT_UINT32_ARRAY(keys0, array_size);
        BITS_TIMER_START(radix8sort_u32);
        radix8sort_u32(keys0, keys1, values0, values1, array_size);
        radix8_total_s += BITS_TIMER_END(radix8sort_u32);
        PRINT_UINT32_ARRAY(keys0, array_size);
        CHECK_SORTED(keys0, values0, keys_orig, array_size);

        memcpy(keys0, keys_orig, sizeof(uint32_t) * array_size);
        memcpy(values0, values_orig, sizeof(uint32_t) * array_size);
        PRINT_UINT32_ARRAY(keys0, array_size);
        BITS_TIMER_START(radix11sort_u32);
        radix11sort_u32(keys0, keys1, values0, values1, array_size);
        radix11_total_s += BITS_TIMER_END(radix11sort_u32);
        PRINT_UINT32_ARRAY(keys1, array_size);
        CHECK_SORTED(keys1, values1, keys_orig, array_size);
    }

    free(keys_orig);
    free(keys0);
    free(keys1);
    free(values_orig);
    free(values0);
    free(values1);

    printf("%6" PRIu32 "  %5f  %5f\n", array_size, radix8_total_s * inv_iter,
        radix11_total_s * inv_iter);
}


void perf_test_u64(uint32_t array_size, uint32_t iterations)
{

    double radix8_total_s = 0, radix11_total_s = 0;
    double inv_iter = 1.0 / (double)iterations;

    uint64_t * keys_orig = (uint64_t *)malloc(sizeof(uint64_t) * array_size);
    uint64_t * keys0 = (uint64_t *)malloc(sizeof(uint64_t) * array_size);
    uint64_t * keys1 = (uint64_t *)malloc(sizeof(uint64_t) * array_size);
    uint32_t * values_orig = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * values0 = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * values1 = (uint32_t *)malloc(sizeof(uint32_t) * array_size);

    for (uint32_t i = 0; i < iterations; ++i)
    {
        RAND_UINT64_KEYS(keys_orig, values_orig, keys_orig, array_size);

        memcpy(keys0, keys_orig, sizeof(uint64_t) * array_size);
        memcpy(values0, values_orig, sizeof(uint32_t) * array_size);
        PRINT_UINT64_ARRAY(keys0, array_size);
        BITS_TIMER_START(radix8sort_u64);
        radix8sort_u64(keys0, keys1, values0, values1, array_size);
        radix8_total_s += BITS_TIMER_END(radix8sort_u64);
        PRINT_UINT64_ARRAY(keys0, array_size);
        CHECK_SORTED(keys0, values0, keys_orig, array_size);

        memcpy(keys0, keys_orig, sizeof(uint64_t) * array_size);
        memcpy(values0, values_orig, sizeof(uint32_t) * array_size);
        PRINT_UINT32_ARRAY(keys0, array_size);
        BITS_TIMER_START(radix11sort_u64);
        radix11sort_u64(keys0, keys1, values0, values1, array_size);
        radix11_total_s += BITS_TIMER_END(radix11sort_u64);
        PRINT_UINT32_ARRAY(keys0, array_size);
        CHECK_SORTED(keys0, values0, keys_orig, array_size);
    }

    free(keys_orig);
    free(keys0);
    free(keys1);
    free(values_orig);
    free(values0);
    free(values1);

    printf("%6" PRIu32 "  %5f  %5f\n", array_size, radix8_total_s * inv_iter,
        radix11_total_s * inv_iter);
}


void perf_test_f32(uint32_t array_size, uint32_t iterations)
{
    double radix8_total_s = 0, radix11_total_s = 0;
    double inv_iter = 1.0 / (double)iterations;

    float * keys_orig = (float *)malloc(sizeof(float) * array_size);
    float * keys0 = (float *)malloc(sizeof(float) * array_size);
    float * keys1 = (float *)malloc(sizeof(float) * array_size);
    uint32_t * values_orig = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * values0 = (uint32_t *)malloc(sizeof(uint32_t) * array_size);
    uint32_t * values1 = (uint32_t *)malloc(sizeof(uint32_t) * array_size);

    for (uint32_t i = 0; i < iterations; ++i)
    {
        RAND_FLOAT_KEYS(keys_orig, values_orig, keys_orig, array_size);

        memcpy(keys0, keys_orig, sizeof(float) * array_size);
        memcpy(values0, values_orig, sizeof(uint32_t) * array_size);
        PRINT_FLOAT_ARRAY(keys0, array_size);
        BITS_TIMER_START(radix8sort_f32);
        radix8sort_f32(keys0, keys1, values0, values1, array_size);
        radix8_total_s += BITS_TIMER_END(radix8sort_f32);
        PRINT_FLOAT_ARRAY(keys0, array_size);
        CHECK_SORTED(keys0, values0, keys_orig, array_size);

        memcpy(keys0, keys_orig, sizeof(float) * array_size);
        memcpy(values0, values_orig, sizeof(uint32_t) * array_size);
        PRINT_FLOAT_ARRAY(keys0, array_size);
        BITS_TIMER_START(radix11sort_f32);
        radix11sort_f32(keys0, keys1, values0, values1, array_size);
        radix11_total_s += BITS_TIMER_END(radix11sort_f32);
        PRINT_FLOAT_ARRAY(keys1, array_size);
        CHECK_SORTED(keys1, values1, keys_orig, array_size);
    }

    free(keys_orig);
    free(keys0);
    free(keys1);
    free(values0);
    free(values1);

    printf("%6" PRIu32 "  %5f  %5f\n", array_size, radix8_total_s * inv_iter,
        radix11_total_s * inv_iter);
}

int main()
{
    uint32_t start = 32, end = 65536, iterations = 100;

    printf("Radix sort uint32_t key (%u iterations)\n", iterations);
    puts("  size    radix8   radix11");
    for (uint32_t i = start; i <= end; i = i << 1)
    {
        perf_test_u32(i, iterations);
    }

    printf("\nRadix sort uint64_t key (%u iterations)\n", iterations);
    puts("  size    radix8   radix11");
    for (uint32_t i = start; i <= end; i = i << 1)
    {
        perf_test_u64(i, iterations);
    }

    printf("\nRadix sort float key (%u iterations)\n", iterations);
    puts("  size    radix8   radix11");
    for (uint32_t i = start; i <= end; i = i << 1)
    {
        perf_test_f32(i, iterations);
    }
}
