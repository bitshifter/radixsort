#include "helpers.h"
#include "radixsort.h"

void test_radix8sort_u32()
{
	static const uint32_t array_size = 8;
	uint32_t keys_in_out[array_size];
	uint32_t keys_temp[array_size];
	uint32_t keys_copy[array_size];
	uint32_t values_in_out[array_size];
	uint32_t values_temp[array_size];

	puts(__FUNCTION__);
	RAND_UINT32_KEYS(keys_in_out, values_in_out, keys_copy, array_size);
	PRINT_UINT32_ARRAY(keys_in_out, array_size);
	radix8sort_u32(keys_in_out, keys_temp, values_in_out, values_temp, array_size);
	PRINT_UINT32_ARRAY(keys_in_out, array_size);
	CHECK_SORTED(keys_in_out, values_in_out, keys_copy, array_size);
}


void test_radix11sort_u32()
{
	static const uint32_t array_size = 8;
	uint32_t keys_in[array_size];
	uint32_t keys_out[array_size];
	uint32_t keys_copy[array_size];
	uint32_t values_in[array_size];
	uint32_t values_out[array_size];

	puts(__FUNCTION__);
	RAND_UINT32_KEYS(keys_in, values_in, keys_copy, array_size);
	PRINT_UINT32_ARRAY(keys_in, array_size);
	radix11sort_u32(keys_in, keys_out, values_in, values_out, array_size);
	PRINT_UINT32_ARRAY(keys_out, array_size);
	CHECK_SORTED(keys_out, values_out, keys_copy, array_size);
}


void test_radix8sort_u64()
{
	static const uint32_t array_size = 8;
	uint64_t keys_in_out[array_size];
	uint64_t keys_temp[array_size];
	uint64_t keys_copy[array_size];
	uint32_t values_in_out[array_size];
	uint32_t values_temp[array_size];

	puts(__FUNCTION__);
	RAND_UINT64_KEYS(keys_in_out, values_in_out, keys_copy, array_size);
	PRINT_UINT64_ARRAY(keys_in_out, array_size);
	radix8sort_u64(keys_in_out, keys_temp, values_in_out, values_temp, array_size);
	PRINT_UINT64_ARRAY(keys_in_out, array_size);
	CHECK_SORTED(keys_in_out, values_in_out, keys_copy, array_size);
}


void test_radix11sort_u64()
{
	static const uint32_t array_size = 8;
	uint64_t keys_in_out[array_size];
	uint64_t keys_temp[array_size];
	uint64_t keys_copy[array_size];
	uint32_t values_in_out[array_size];
	uint32_t values_temp[array_size];

	puts(__FUNCTION__);
	RAND_UINT64_KEYS(keys_in_out, values_in_out, keys_copy, array_size);
	PRINT_UINT64_ARRAY(keys_in_out, array_size);
	radix11sort_u64(keys_in_out, keys_temp, values_in_out, values_temp, array_size);
	PRINT_UINT64_ARRAY(keys_in_out, array_size);
	CHECK_SORTED(keys_in_out, values_in_out, keys_copy, array_size);
}


void test_radix8sort_f32()
{
	static const uint32_t array_size = 8;
	float keys_in_out[array_size];
	float keys_temp[array_size];
	float keys_copy[array_size];
	uint32_t values_in_out[array_size];
	uint32_t values_temp[array_size];

	puts(__FUNCTION__);
	RAND_FLOAT_KEYS(keys_in_out, values_in_out, keys_copy, array_size);
	PRINT_FLOAT_ARRAY(keys_in_out, array_size);
	radix8sort_f32(keys_in_out, keys_temp, values_in_out, values_temp, array_size);
	PRINT_FLOAT_ARRAY(keys_in_out, array_size);
	CHECK_SORTED(keys_in_out, values_in_out, keys_copy, array_size);
}


void test_radix11sort_f32()
{
	static const uint32_t array_size = 8;
	float keys_in[array_size];
	float keys_out[array_size];
	float keys_copy[array_size];
	uint32_t values_in[array_size];
	uint32_t values_out[array_size];

	puts(__FUNCTION__);
	RAND_FLOAT_KEYS(keys_in, values_in, keys_copy, array_size);
	PRINT_FLOAT_ARRAY(keys_in, array_size);
	radix11sort_f32(keys_in, keys_out, values_in, values_out, array_size);
	PRINT_FLOAT_ARRAY(keys_out, array_size);
	CHECK_SORTED(keys_out, values_out, keys_copy, array_size);
}


int main()
{
	test_radix8sort_u32();
	test_radix11sort_u32();
	test_radix8sort_u64();
	test_radix11sort_u64();
	test_radix8sort_f32();
	test_radix11sort_f32();
}
