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
    inline KeyType operator()(KeyType f) const
    {
        return f;
    }
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
    static inline void radix_pass(const KeyType* __restrict keys_in, KeyType* __restrict keys_out,
        const ValueType* __restrict values_in, ValueType* __restrict values_out, uint32_t size,
        uint32_t* __restrict hist, KeyType shift, PassDecodeOp decode_op, PassEncodeOp encode_op)
    {
        for (uint32_t i = 0; i < size; ++i)
        {
            const KeyType key = decode_op(keys_in[i]);
            const KeyType pos = (key >> shift) & kHistMask;
            const uint32_t index = hist[pos]++;
            keys_out[index] = encode_op(key);
            values_out[index] = values_in[i];
        }
    }

public:
    std::pair<KeyType*, ValueType*> operator()(KeyType* __restrict keys_in,
        KeyType* __restrict keys_temp, ValueType* __restrict values_in,
        ValueType* __restrict values_temp, uint32_t size)
    {
        DecodeOp decode_op;
        EncodeOp encode_op;
        PassThrough pass_through;

        // Initialise each histogram bucket with the key value
        uint32_t hist[kHistBuckets][kHistSize] = {0};
        for (uint32_t i = 0; i < size; ++i)
        {
            const KeyType key = decode_op(keys_in[i]);
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
        KeyType* __restrict keys[2] = {keys_in, keys_temp};
        ValueType* __restrict values[2] = {values_in, values_temp};

        {
            // decode key on first radix pass
            const uint32_t bucket = 0;
            const uint32_t in = bucket & 1;
            const uint32_t out = !in;
            radix_pass(keys[in], keys[out], values[in], values[out], size, hist[bucket],
                bucket * kRadixBits, decode_op, pass_through);
        }

        for (uint32_t bucket = 1; bucket < kHistBuckets - 1; ++bucket)
        {
            const uint32_t in = bucket & 1;
            const uint32_t out = !in;
            radix_pass(keys[in], keys[out], values[in], values[out], size, hist[bucket],
                bucket * kRadixBits, pass_through, pass_through);
        }

        {
            // encode key on last radix pass
            const uint32_t bucket = kHistBuckets - 1;
            const uint32_t in = bucket & 1;
            const uint32_t out = !in;
            radix_pass(keys[in], keys[out], values[in], values[out], size, hist[bucket],
                bucket * kRadixBits, pass_through, encode_op);

            return std::make_pair(keys[out], values[out]);
        }
    }
};

} // namespace detail


template <typename ValueType>
inline std::pair<uint32_t*, ValueType*> radix8sort(uint32_t* __restrict keys_in_out,
    uint32_t* __restrict keys_temp, ValueType* __restrict values_in_out, ValueType* values_temp,
    uint32_t size)
{
    detail::RadixSort<8, uint32_t, ValueType> sort;
    return sort(keys_in_out, keys_temp, values_in_out, values_temp, size);
}


template <typename ValueType>
inline std::pair<uint64_t*, ValueType*> radix8sort(uint64_t* __restrict keys_in_out,
    uint64_t* __restrict keys_temp, ValueType* __restrict values_in_out, ValueType* values_temp,
    uint32_t size)
{
    detail::RadixSort<8, uint64_t, ValueType> sort;
    return sort(keys_in_out, keys_temp, values_in_out, values_temp, size);
}


template <typename ValueType>
inline std::pair<uint32_t*, ValueType*> radix11sort(uint32_t* __restrict keys_in,
    uint32_t* __restrict keys_out, ValueType* __restrict values_in,
    ValueType* __restrict values_out, uint32_t size)
{
    detail::RadixSort<11, uint32_t, ValueType> sort;
    return sort(keys_in, keys_out, values_in, values_out, size);
}


template <typename ValueType>
inline std::pair<uint64_t*, ValueType*> radix11sort(uint64_t* __restrict keys_in,
    uint64_t* __restrict keys_out, ValueType* __restrict values_in,
    ValueType* __restrict values_out, uint32_t size)
{
    detail::RadixSort<11, uint64_t, ValueType> sort;
    return sort(keys_in, keys_out, values_in, values_out, size);
}


template <typename ValueType>
inline std::pair<float*, ValueType*> radix8sort(float* __restrict keys_in_out_f32,
    float* __restrict keys_temp_f32, ValueType* __restrict values_in_out,
    ValueType* __restrict values_temp, uint32_t size)
{
    // create uint32_t pointers to inputs to avoid float to int casting
    uint32_t* __restrict keys_in_out = reinterpret_cast<uint32_t*>(keys_in_out_f32);
    uint32_t* __restrict keys_temp = reinterpret_cast<uint32_t*>(keys_temp_f32);

    detail::RadixSort<8, uint32_t, ValueType, detail::FloatFlip, detail::InvFloatFlip> sort;
    auto result = sort(keys_in_out, keys_temp, values_in_out, values_temp, size);
    return std::make_pair((float*)result.first, result.second);
}


template <typename ValueType>
inline std::pair<float*, ValueType*> radix11sort(float* __restrict keys_in_f32,
    float* __restrict keys_out_f32, ValueType* __restrict values_in,
    ValueType* __restrict values_out, uint32_t size)
{
    // create uint32_t pointers to inputs to avoid float to int casting
    uint32_t* __restrict keys_in = reinterpret_cast<uint32_t*>(keys_in_f32);
    uint32_t* __restrict keys_out = reinterpret_cast<uint32_t*>(keys_out_f32);

    detail::RadixSort<11, uint32_t, ValueType, detail::FloatFlip, detail::InvFloatFlip> sort;
    auto result = sort(keys_in, keys_out, values_in, values_out, size);
    return std::make_pair((float*)result.first, result.second);
}

} // namespace bits
