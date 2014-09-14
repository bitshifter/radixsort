#include <cstring>

namespace bits
{

namespace detail
{

/**
 * Flip a float for sorting.
 *  finds SIGN of fp number.
 *  if it's 1 (negative float), it flips all bits
 *  if it's 0 (positive float), it flips the sign only
 */
struct FloatFlip
{
	inline uint32_t operator()(uint32_t f) const
	{
		uint32_t mask = -((int32_t)(f >> 31)) | 0x80000000;
		return f ^ mask;
	}
};


/**
 * Flip a float back (invert floatFlip)
 *  signed was flipped from above, so:
 *  if sign is 1 (negative), it flips the sign bit back
 *  if sign is 0 (positive), it flips all bits back
 */
struct InvFloatFlip
{
	inline uint32_t operator()(uint32_t f) const
	{
		uint32_t mask = ((f >> 31) - 1) | 0x80000000;
		return f ^ mask;
	}
};


/**
 * Pass input through unmodified
 */
struct PassThrough
{
	template <typename KeyType>
	inline KeyType operator()(KeyType f) const { return f; }
};

/**
 * Internal function object for performing radix sort.
 * Non integral key types like float should provide decode and encode
 * operators for conversion to the given KeyType.
 */
template <uint32_t kRadixBits, typename KeyType, typename ValueType,
	typename DecodeOp = PassThrough, typename EncodeOp = PassThrough>
struct RadixSort
{
private:
	static const uint32_t kHistBuckets = 1 + (((sizeof(KeyType) * 8) - 1) / kRadixBits);
	static const uint32_t kHistSize = (1 << kRadixBits);
	static const uint32_t kHistMask = kHistSize - 1;

	/**
	 * Perform a radix sort pass for the given bit shift and mask.
	 */
	template <typename PassDecodeOp, typename PassEncodeOp>
	static inline void radixPass(
		const KeyType * restrict keysIn, KeyType * restrict keysOut,
		const ValueType * restrict valuesIn, ValueType * restrict valuesOut,
		uint32_t size, uint32_t * restrict hist, KeyType shift,
		PassDecodeOp decodeOp, PassEncodeOp encodeOp)
	{
		for (uint32_t i = 0; i < size; ++i)
		{
			const KeyType key = decodeOp(keysIn[i]);
			const KeyType pos = (key >> shift) & kHistMask;
			const uint32_t index = hist[pos]++;
			keysOut[index] = encodeOp(key);
			valuesOut[index] = valuesIn[i];
		}
	}

public:
	std::pair<KeyType *, ValueType *> operator()(KeyType * restrict keysIn, KeyType * restrict keysTemp,
		ValueType * restrict valuesIn, ValueType * restrict valuesTemp, uint32_t size)
	{
		DecodeOp decodeOp;
		EncodeOp encodeOp;
		PassThrough passThrough;

		// Initialise each histogram bucket with the key value
		uint32_t hist[kHistBuckets][kHistSize] = { 0 };
		for (uint32_t i = 0; i < size; ++i)
		{
			const KeyType key = decodeOp(keysIn[i]);
			for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
			{
				const uint32_t shift = bucket * kRadixBits;
				const uint32_t pos = (key >> shift) & kHistMask;
				++hist[bucket][pos];
			}
		}

		// Update the histogram data so each entry sums the previous entries
		uint32_t sum[kHistBuckets];
		for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
		{
			sum[bucket] = hist[bucket][0];
			hist[bucket][0] = 0;
		}

		uint32_t tsum;
		for (uint32_t i = 1; i < kHistSize; ++i)
		{
			for (uint32_t bucket = 0; bucket < kHistBuckets; ++bucket)
			{
				tsum = hist[bucket][i] + sum[bucket];
				hist[bucket][i] = sum[bucket];
				sum[bucket] = tsum;
			}
		}

		// alternate input and output buffers on each radix pass
		KeyType * restrict keys[2] = { keysIn, keysTemp };
		ValueType * restrict values[2] = { valuesIn, valuesTemp };

		{
			// decode key on first radix pass
			const uint32_t bucket = 0;
			const uint32_t in = bucket & 1; const uint32_t out = !in;
			radixPass(keys[in], keys[out], values[in], values[out], size,
				hist[bucket], bucket * kRadixBits, decodeOp, passThrough);
		}

		for (uint32_t bucket = 1; bucket < kHistBuckets - 1; ++bucket)
		{
			const uint32_t in = bucket & 1; const uint32_t out = !in;
			radixPass(keys[in], keys[out], values[in], values[out], size,
				hist[bucket], bucket * kRadixBits, passThrough, passThrough);
		}

		{
			// encode key on last radix pass
			const uint32_t bucket = kHistBuckets - 1;
			const uint32_t in = bucket & 1; const uint32_t out = !in;
			radixPass(keys[in], keys[out], values[in], values[out], size,
				hist[bucket], bucket * kRadixBits, passThrough, encodeOp);

			return std::make_pair(keys[out], values[out]);
		}
	}
};

} // namespace detail


template <typename ValueType>
inline std::pair<uint32_t *, ValueType *> radixSort8(
	uint32_t * restrict keysInOut, uint32_t * restrict keysTemp,
	ValueType * restrict valuesInOut, ValueType * valuesTemp,
	uint32_t size)
{
	detail::RadixSort<8, uint32_t, ValueType> sort;
	return sort(keysInOut, keysTemp, valuesInOut, valuesTemp, size);
}


template <typename ValueType>
inline std::pair<uint64_t *, ValueType *> radixSort8(
	uint64_t * restrict keysInOut, uint64_t * restrict keysTemp,
	ValueType * restrict valuesInOut, ValueType * valuesTemp,
	uint32_t size)
{
	detail::RadixSort<8, uint64_t, ValueType> sort;
	return sort(keysInOut, keysTemp, valuesInOut, valuesTemp, size);
}


template <typename ValueType>
inline std::pair<uint32_t *, ValueType *> radixSort11(
	uint32_t * restrict keysIn, uint32_t * restrict keysOut,
	ValueType * restrict valuesIn, ValueType * restrict valuesOut,
	uint32_t size)
{
	detail::RadixSort<11, uint32_t, ValueType> sort;
	return sort(keysIn, keysOut, valuesIn, valuesOut, size);
}


template <typename ValueType>
inline std::pair<uint64_t *, ValueType *> radixSort11(
	uint64_t * restrict keysIn, uint64_t * restrict keysOut,
	ValueType * restrict valuesIn, ValueType * restrict valuesOut,
	uint32_t size)
{
	detail::RadixSort<11, uint64_t, ValueType> sort;
	return sort(keysIn, keysOut, valuesIn, valuesOut, size);
}


template <typename ValueType>
inline std::pair<float *, ValueType *> radixSort8(
	float * restrict keysInOutf, float * restrict keysTempf,
	ValueType * restrict valuesInOut, ValueType * restrict valuesTemp,
	uint32_t size)
{
	// create uint32_t pointers to inputs to avoid float to int casting
	uint32_t * restrict keysInOut = (uint32_t*)keysInOutf;
	uint32_t * restrict keysTemp = (uint32_t*)keysTempf;

	detail::RadixSort<8, uint32_t, ValueType,
		detail::FloatFlip, detail::InvFloatFlip> sort;
	auto result = sort(keysInOut, keysTemp, valuesInOut, valuesTemp, size);
	return std::make_pair((float*)result.first, result.second);
}


template <typename ValueType>
inline std::pair<float *, ValueType *> radixSort11(
	float * restrict keysInf, float * restrict keysOutf,
	ValueType * restrict valuesIn, ValueType * restrict valuesOut,
	uint32_t size)
{
	// create uint32_t pointers to inputs to avoid float to int casting
	uint32_t * restrict keysIn = (uint32_t*)keysInf;
	uint32_t * restrict keysOut = (uint32_t*)keysOutf;

	detail::RadixSort<11, uint32_t, ValueType,
		detail::FloatFlip, detail::InvFloatFlip> sort;
	auto result = sort(keysIn, keysOut, valuesIn, valuesOut, size);
	return std::make_pair((float*)result.first, result.second);
}

} // namespace bits
