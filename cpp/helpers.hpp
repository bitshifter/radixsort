#ifndef BITS_HELPERS_HPP
#define BITS_HELPERS_HPP

#include <cinttypes>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <random>

#if defined(_MSC_VER)
#define PRIu32 "lu"
#define PRIx32 "lx"
#define PRIx64 "llx"
#endif

namespace bits
{

template <typename KeyType, typename ValueType>
void checkSorted(const KeyType * keys, const ValueType * values,
				 const KeyType * orig, size_t size)
{
	for (uint32_t i = 1; i < (uint32_t)(size); ++i) {
		if ((keys)[i - 1] > (keys)[i])
		{									\
			printf("Key sort error at index %u\n", i - 1);
			assert(false);
		}
		if ((keys)[i] != (orig)[(values)[i]])
		{
			printf("Value error at index %u\n", i);
			assert(false);
		}
	}
}

void printArray(const float * keys, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
	{
		printf("%f ", (keys)[i]);
	}
	putchar('\n');
}

void printArray(const uint32_t * keys, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
	{
		printf("%08" PRIx32 " ", keys[i]);
	}
	putchar('\n');
}

void printArray(const uint64_t * keys, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
	{
		printf("%016" PRIx64 " ", keys[i]);
	}
	putchar('\n');
}

void randKeys(std::mt19937 & rnd32, uint32_t * keys, uint32_t * indices,
	uint32_t * copy, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
	{
		keys[i] = rnd32();
		copy[i] = keys[i];
		indices[i] = i;
	}
}

void randKeys(std::mt19937_64 & rnd64, uint64_t * keys, uint32_t * indices,
	uint64_t * copy, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
	{
		keys[i] = rnd64();
		copy[i] = keys[i];
		indices[i] = i;
	}
}

void randKeys(std::mt19937 & rnd32, float * keys, uint32_t * indices,
	float * copy, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
	{
		keys[i] = (float)(rnd32()) / 2048.0f;
		if (rand() & 1)
		{
			keys[i] = -keys[i];
		}
		copy[i] = keys[i];
		indices[i] = i;
	}
}

} // namespace bits

#endif // BITS_HELPERS_HPP
