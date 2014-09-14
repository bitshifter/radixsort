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

void perfTestUInt32(uint32_t arraySize, uint32_t iterations)
{

	double radix8Total_s = 0, radix11Total_s = 0;
	double invIter = 1.0 / (double)iterations;

	uint32_t * keysOrig = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * keys0 = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * keys1 = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * valuesOrig = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * values0 = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * values1 = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);

	for (uint32_t i = 0; i < iterations; ++i)
	{
		RAND_UINT32_KEYS(keysOrig, valuesOrig, keysOrig, arraySize);

		memcpy(keys0, keysOrig, sizeof(uint32_t) * arraySize);
		memcpy(values0, valuesOrig, sizeof(uint32_t) * arraySize);
		PRINT_UINT32_ARRAY(keys0, arraySize);
		BITS_TIMER_START(radixSort8UInt32);
		radixSort8UInt32(keys0, keys1, values0, values1, arraySize);
		radix8Total_s += BITS_TIMER_END(radixSort8UInt32);
		PRINT_UINT32_ARRAY(keys0, arraySize);
		CHECK_SORTED(keys0, values0, keysOrig, arraySize);

		memcpy(keys0, keysOrig, sizeof(uint32_t) * arraySize);
		memcpy(values0, valuesOrig, sizeof(uint32_t) * arraySize);
		PRINT_UINT32_ARRAY(keys0, arraySize);
		BITS_TIMER_START(radixSort11UInt32);
		radixSort11UInt32(keys0, keys1, values0, values1, arraySize);
		radix11Total_s += BITS_TIMER_END(radixSort11UInt32);
		PRINT_UINT32_ARRAY(keys1, arraySize);
		CHECK_SORTED(keys1, values1, keysOrig, arraySize);
	}

	free(keysOrig);
	free(keys0);
	free(keys1);
	free(valuesOrig);
	free(values0);
	free(values1);

	printf("%6" PRIu32 "  %5f  %5f\n", arraySize, radix8Total_s * invIter,
		radix11Total_s * invIter);
}


void perfTestUInt64(uint32_t arraySize, uint32_t iterations)
{

	double radix8Total_s = 0, radix11Total_s = 0;
	double invIter = 1.0 / (double)iterations;

	uint64_t * keysOrig = (uint64_t *)malloc(sizeof(uint64_t) * arraySize);
	uint64_t * keys0 = (uint64_t *)malloc(sizeof(uint64_t) * arraySize);
	uint64_t * keys1 = (uint64_t *)malloc(sizeof(uint64_t) * arraySize);
	uint32_t * valuesOrig = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * values0 = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * values1 = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);

	for (uint32_t i = 0; i < iterations; ++i)
	{
		RAND_UINT64_KEYS(keysOrig, valuesOrig, keysOrig, arraySize);

		memcpy(keys0, keysOrig, sizeof(uint64_t) * arraySize);
		memcpy(values0, valuesOrig, sizeof(uint32_t) * arraySize);
		PRINT_UINT64_ARRAY(keys0, arraySize);
		BITS_TIMER_START(radixSort8UInt64);
		radixSort8UInt64(keys0, keys1, values0, values1, arraySize);
		radix8Total_s += BITS_TIMER_END(radixSort8UInt64);
		PRINT_UINT64_ARRAY(keys0, arraySize);
		CHECK_SORTED(keys0, values0, keysOrig, arraySize);

		memcpy(keys0, keysOrig, sizeof(uint64_t) * arraySize);
		memcpy(values0, valuesOrig, sizeof(uint32_t) * arraySize);
		PRINT_UINT32_ARRAY(keys0, arraySize);
		BITS_TIMER_START(radixSort11UInt64);
		radixSort11UInt64(keys0, keys1, values0, values1, arraySize);
		radix11Total_s += BITS_TIMER_END(radixSort11UInt64);
		PRINT_UINT32_ARRAY(keys0, arraySize);
		CHECK_SORTED(keys0, values0, keysOrig, arraySize);
	}

	free(keysOrig);
	free(keys0);
	free(keys1);
	free(valuesOrig);
	free(values0);
	free(values1);

	printf("%6" PRIu32 "  %5f  %5f\n", arraySize, radix8Total_s * invIter,
		radix11Total_s * invIter);
}


void perfTestFloat(uint32_t arraySize, uint32_t iterations)
{
	double radix8Total_s = 0, radix11Total_s = 0;
	double invIter = 1.0 / (double)iterations;

	float * keysOrig = (float *)malloc(sizeof(float) * arraySize);
	float * keys0 = (float *)malloc(sizeof(float) * arraySize);
	float * keys1 = (float *)malloc(sizeof(float) * arraySize);
	uint32_t * valuesOrig = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * values0 = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);
	uint32_t * values1 = (uint32_t *)malloc(sizeof(uint32_t) * arraySize);

	for (uint32_t i = 0; i < iterations; ++i)
	{
		RAND_FLOAT_KEYS(keysOrig, valuesOrig, keysOrig, arraySize);

		memcpy(keys0, keysOrig, sizeof(float) * arraySize);
		memcpy(values0, valuesOrig, sizeof(uint32_t) * arraySize);
		PRINT_FLOAT_ARRAY(keys0, arraySize);
		BITS_TIMER_START(radixSort8Float);
		radixSort8Float(keys0, keys1, values0, values1, arraySize);
		radix8Total_s += BITS_TIMER_END(radixSort8Float);
		PRINT_FLOAT_ARRAY(keys0, arraySize);
		CHECK_SORTED(keys0, values0, keysOrig, arraySize);

		memcpy(keys0, keysOrig, sizeof(float) * arraySize);
		memcpy(values0, valuesOrig, sizeof(uint32_t) * arraySize);
		PRINT_FLOAT_ARRAY(keys0, arraySize);
		BITS_TIMER_START(radixSort11Float);
		radixSort11Float(keys0, keys1, values0, values1, arraySize);
		radix11Total_s += BITS_TIMER_END(radixSort11Float);
		PRINT_FLOAT_ARRAY(keys1, arraySize);
		CHECK_SORTED(keys1, values1, keysOrig, arraySize);
	}

	free(keysOrig);
	free(keys0);
	free(keys1);
	free(values0);
	free(values1);

	printf("%6" PRIu32 "  %5f  %5f\n", arraySize, radix8Total_s * invIter,
		radix11Total_s * invIter);
}

int main()
{
	uint32_t start = 32, end = 65536, iterations = 100;

	printf("Radix sort uint32_t key (%u iterations)\n", iterations);
	puts("  size    radix8   radix11");
	for (uint32_t i = start; i <= end; i = i << 1)
	{
		perfTestUInt32(i, iterations);
	}

	printf("\nRadix sort uint64_t key (%u iterations)\n", iterations);
	puts("  size    radix8   radix11");
	for (uint32_t i = start; i <= end; i = i << 1)
	{
		perfTestUInt64(i, iterations);
	}

	printf("\nRadix sort float key (%u iterations)\n", iterations);
	puts("  size    radix8   radix11");
	for (uint32_t i = start; i <= end; i = i << 1)
	{
		perfTestFloat(i, iterations);
	}
}
