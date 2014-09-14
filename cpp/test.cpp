#include "helpers.hpp"
#include "radixsort.hpp"

#include <array>

using namespace bits;

template <typename KeyType, typename Rng>
void testRadixSort8(Rng & rng)
{
	static const uint32_t arraySize = 8;
	std::array<KeyType, arraySize> keysInOut;
	std::array<KeyType, arraySize> keysTemp;
	std::array<KeyType, arraySize> keysCopy;
	std::array<uint32_t, arraySize> valuesInOut;
	std::array<uint32_t, arraySize> valuesTemp;

	randKeys(rng, keysInOut.data(), valuesInOut.data(), keysCopy.data(), arraySize);
	printArray(keysInOut.data(), arraySize);
	radixSort8(keysInOut.data(), keysTemp.data(), valuesInOut.data(), valuesTemp.data(), arraySize);
	printArray(keysInOut.data(), arraySize);
	checkSorted(keysInOut.data(), valuesInOut.data(), keysCopy.data(), arraySize);
}


template <typename KeyType, typename Rng>
void testRadixSort11(Rng & rng)
{
	static const uint32_t arraySize = 8;
	KeyType keysIn[arraySize];
	KeyType keysOut[arraySize];
	KeyType keysCopy[arraySize];
	uint32_t valuesIn[arraySize];
	uint32_t valuesOut[arraySize];

	randKeys(rng, keysIn, valuesIn, keysCopy, arraySize);
	printArray(keysIn, arraySize);
	auto result = radixSort11(keysIn, keysOut, valuesIn, valuesOut, arraySize);
	printArray(result.first, arraySize);
	checkSorted(result.first, result.second, keysCopy, arraySize);
}

int main()
{
	// mersenne twister prng
	std::mt19937 rnd32;
	std::mt19937_64 rnd64;

	puts("test radix8 uint32_t");
	testRadixSort8<uint32_t>(rnd32);

	puts("test radix8 uint64_t");
	testRadixSort8<uint64_t>(rnd64);

	puts("test radix8 float");
	testRadixSort8<float>(rnd32);

	puts("test radix11 uint32_t");
	testRadixSort11<uint32_t>(rnd32);

	puts("test radix11 uint64_t");
	testRadixSort11<uint64_t>(rnd64);

	puts("test radix11 float");
	testRadixSort11<float>(rnd32);
}
