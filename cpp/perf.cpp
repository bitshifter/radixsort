#include "helpers.hpp"
#include "radixsort.hpp"

#include <algorithm>
#include <chrono>
#include <vector>

//#define PRINT_ARRAY(keys, size) printArray((keys), (size))
#define PRINT_ARRAY(...)

template <typename KeyType, typename Rand>
void perfTest(Rand rng, uint32_t arraySize, uint32_t iterations)
{
	using namespace bits;

	using std::chrono::duration;
	using std::chrono::high_resolution_clock;
	using std::chrono::seconds;
	using std::vector;

	duration<double, seconds::period> radix8Total_s, radix11Total_s,
			stdSortTotal_s, stdStableTotal_s;
	double invIter = 1.0 / (double)iterations;

	vector<KeyType> keysOrig(arraySize);
	vector<KeyType> keys0(arraySize);
	vector<KeyType> keys1(arraySize);
	vector<uint32_t> valuesOrig(arraySize);
	vector<uint32_t> values0(arraySize);
	vector<uint32_t> values1(arraySize);

	for (uint32_t i = 0; i < iterations; ++i)
	{
		randKeys(rng, keysOrig.data(), valuesOrig.data(), keysOrig.data(), arraySize);

		{
			keys0 = keysOrig;
			values0 = valuesOrig;
			PRINT_ARRAY(keys0, arraySize);
			auto start = high_resolution_clock::now();
			auto result = radixSort8(keys0.data(), keys1.data(),
				values0.data(), values1.data(), arraySize);
			radix8Total_s += high_resolution_clock::now() - start;
			PRINT_ARRAY(result.first, arraySize);
			checkSorted(result.first, result.second, keysOrig.data(), arraySize);
		}

		{
			keys0 = keysOrig;
			values0 = valuesOrig;
			PRINT_ARRAY(keys0, arraySize);
			auto start = high_resolution_clock::now();
			auto result = radixSort11(keys0.data(), keys1.data(),
				values0.data(), values1.data(), arraySize);
			radix11Total_s += high_resolution_clock::now() - start;
			PRINT_ARRAY(result.first, arraySize);
			checkSorted(result.first, result.second, keysOrig.data(), arraySize);
		}

		{
			keys0 = keysOrig;
			PRINT_ARRAY(keys0, arraySize);
			auto start = high_resolution_clock::now();
			std::sort(std::begin(keys0), std::end(keys0));
			stdSortTotal_s += high_resolution_clock::now() - start;
			PRINT_ARRAY(keys1, arraySize);
		}

		{
			keys0 = keysOrig;
			PRINT_ARRAY(keys0, arraySize);
			auto start = high_resolution_clock::now();
			std::stable_sort(std::begin(keys0), std::end(keys0));
			stdStableTotal_s += high_resolution_clock::now() - start;
			PRINT_ARRAY(keys1, arraySize);
		}
	}

	printf("%6" PRIu32 "  %5f  %5f  %5f  %5f\n",
			arraySize, radix8Total_s.count() * invIter,
			radix11Total_s.count() * invIter,
			stdSortTotal_s.count() * invIter,
			stdStableTotal_s.count() * invIter);
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
		perfTest<uint32_t>(rnd32, i, iterations);
	}

	printf("\nRadix sort uint64_t key (%u iterations)\n", iterations);
	puts("  size    radix8   radix11 std::sort  ::stable");
	for (uint32_t i = start; i <= end; i = i << 1)
	{
		perfTest<uint64_t>(rnd64, i, iterations);
	}

	printf("\nRadix sort float key (%u iterations)\n", iterations);
	puts("  size    radix8   radix11 std::sort  ::stable");
	for (uint32_t i = start; i <= end; i = i << 1)
	{
		perfTest<float>(rnd32, i, iterations);
	}
}
