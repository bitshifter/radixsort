#include "helpers.h"
#include "radixsort.h"

void testRadixSort8UInt32()
{
	static const uint32_t arraySize = 8;
	uint32_t keysInOut[arraySize];
	uint32_t keysTemp[arraySize];
	uint32_t keysCopy[arraySize];
	uint32_t valuesInOut[arraySize];
	uint32_t valuesTemp[arraySize];

	puts(__FUNCTION__);
	RAND_UINT32_KEYS(keysInOut, valuesInOut, keysCopy, arraySize);
	PRINT_UINT32_ARRAY(keysInOut, arraySize);
	radixSort8UInt32(keysInOut, keysTemp, valuesInOut, valuesTemp, arraySize);
	PRINT_UINT32_ARRAY(keysInOut, arraySize);
	CHECK_SORTED(keysInOut, valuesInOut, keysCopy, arraySize);
}


void testRadixSort11UInt32()
{
	static const uint32_t arraySize = 8;
	uint32_t keysIn[arraySize];
	uint32_t keysOut[arraySize];
	uint32_t keysCopy[arraySize];
	uint32_t valuesIn[arraySize];
	uint32_t valuesOut[arraySize];

	puts(__FUNCTION__);
	RAND_UINT32_KEYS(keysIn, valuesIn, keysCopy, arraySize);
	PRINT_UINT32_ARRAY(keysIn, arraySize);
	radixSort11UInt32(keysIn, keysOut, valuesIn, valuesOut, arraySize);
	PRINT_UINT32_ARRAY(keysOut, arraySize);
	CHECK_SORTED(keysOut, valuesOut, keysCopy, arraySize);
}


void testRadixSort8UInt64()
{
	static const uint32_t arraySize = 8;
	uint64_t keysInOut[arraySize];
	uint64_t keysTemp[arraySize];
	uint64_t keysCopy[arraySize];
	uint32_t valuesInOut[arraySize];
	uint32_t valuesTemp[arraySize];

	puts(__FUNCTION__);
	RAND_UINT64_KEYS(keysInOut, valuesInOut, keysCopy, arraySize);
	PRINT_UINT64_ARRAY(keysInOut, arraySize);
	radixSort8UInt64(keysInOut, keysTemp, valuesInOut, valuesTemp, arraySize);
	PRINT_UINT64_ARRAY(keysInOut, arraySize);
	CHECK_SORTED(keysInOut, valuesInOut, keysCopy, arraySize);
}


void testRadixSort11UInt64()
{
	static const uint32_t arraySize = 8;
	uint64_t keysInOut[arraySize];
	uint64_t keysTemp[arraySize];
	uint64_t keysCopy[arraySize];
	uint32_t valuesInOut[arraySize];
	uint32_t valuesTemp[arraySize];

	puts(__FUNCTION__);
	RAND_UINT64_KEYS(keysInOut, valuesInOut, keysCopy, arraySize);
	PRINT_UINT64_ARRAY(keysInOut, arraySize);
	radixSort11UInt64(keysInOut, keysTemp, valuesInOut, valuesTemp, arraySize);
	PRINT_UINT64_ARRAY(keysInOut, arraySize);
	CHECK_SORTED(keysInOut, valuesInOut, keysCopy, arraySize);
}


void testRadixSort8Float()
{
	static const uint32_t arraySize = 8;
	float keysInOut[arraySize];
	float keysTemp[arraySize];
	float keysCopy[arraySize];
	uint32_t valuesInOut[arraySize];
	uint32_t valuesTemp[arraySize];

	puts(__FUNCTION__);
	RAND_FLOAT_KEYS(keysInOut, valuesInOut, keysCopy, arraySize);
	PRINT_FLOAT_ARRAY(keysInOut, arraySize);
	radixSort8Float(keysInOut, keysTemp, valuesInOut, valuesTemp, arraySize);
	PRINT_FLOAT_ARRAY(keysInOut, arraySize);
	CHECK_SORTED(keysInOut, valuesInOut, keysCopy, arraySize);
}


void testRadixSort11Float()
{
	static const uint32_t arraySize = 8;
	float keysIn[arraySize];
	float keysOut[arraySize];
	float keysCopy[arraySize];
	uint32_t valuesIn[arraySize];
	uint32_t valuesOut[arraySize];

	puts(__FUNCTION__);
	RAND_FLOAT_KEYS(keysIn, valuesIn, keysCopy, arraySize);
	PRINT_FLOAT_ARRAY(keysIn, arraySize);
	radixSort11Float(keysIn, keysOut, valuesIn, valuesOut, arraySize);
	PRINT_FLOAT_ARRAY(keysOut, arraySize);
	CHECK_SORTED(keysOut, valuesOut, keysCopy, arraySize);
}


int main()
{
	testRadixSort8UInt32();
	testRadixSort11UInt32();
	testRadixSort8UInt64();
	testRadixSort11UInt64();
	testRadixSort8Float();
	testRadixSort11Float();
}
