#include "radixsort.hpp"
#if HAVE_C99_SUPPORT
#include "radixsort.h"
#endif

#define NONIUS_RUNNER
#include <nonius.hpp>

namespace
{
template <typename KeyType>
struct RunData
{
    std::vector<KeyType> keys0_;
    std::vector<KeyType> keys1_;
    std::vector<uint32_t> values0_;
    std::vector<uint32_t> values1_;

    template <typename KeyItr, typename ValueItr>
    explicit RunData(uint32_t size, KeyItr keys_first, ValueItr values_first)
        : keys0_(keys_first, keys_first + size)
        , keys1_(size)
        , values0_(values_first, values_first + size)
        , values1_(size)
    {
    }
    KeyType* keys0() { return keys0_.data(); }
    KeyType* keys1() { return keys1_.data(); }
    uint32_t* values0() { return values0_.data(); }
    uint32_t* values1() { return values1_.data(); }
};

template <typename KeyType>
struct InitData
{
    static const uint32_t MAX_SIZE = 65536;
    std::vector<KeyType> keys_;
    std::vector<uint32_t> values_;

    template <typename Rand>
    InitData(Rand& rand)
        : keys_(MAX_SIZE)
        , values_(MAX_SIZE)
    {
        for (uint32_t i = 0; i < MAX_SIZE; ++i)
        {
            keys_[i] = static_cast<KeyType>(rand());
            values_[i] = i;
        }
    }
    RunData<KeyType> run_data(uint32_t array_size) const 
    {
        assert(array_size <= MAX_SIZE);
        return RunData<KeyType>(array_size, keys_.begin(), values_.begin());
    }
};

struct BenchmarkRegistrar
{
    // mersenne twister prng
    std::mt19937 rnd32;
    std::mt19937_64 rnd64;

    const InitData<uint32_t> uint32_data;
    const InitData<uint64_t> uint64_data;
    const InitData<float> float_data;

    BenchmarkRegistrar()
        : uint32_data(rnd32)
        , uint64_data(rnd64)
        , float_data(rnd32)
    {
        const uint32_t start = 64, end = 65536, inc = 2;

        char name[1024];
        auto& registry = nonius::global_benchmark_registry();
        for (uint32_t size = start; size <= end; size = size << inc)
        {
            sprintf(name, "%d uint32_t key bits::radix8sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint32_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return bits::radix8sort(keys0, keys1, values0, values1, size);
                            });
                    });

#if HAVE_C99_SUPPORT
            sprintf(name, "%d uint32_t key radix8sort_u32", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint32_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return radix8sort_u32(keys0, keys1, values0, values1, size);
                            });
                    });
#endif

            sprintf(name, "%d uint32_t key bits::radix11sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint32_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return bits::radix11sort(keys0, keys1, values0, values1, size);
                            });
                    });

#if HAVE_C99_SUPPORT
            sprintf(name, "%d uint32_t key radix11sort_u32", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint32_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return radix11sort_u32(keys0, keys1, values0, values1, size);
                            });
                    });
#endif

            sprintf(name, "%d uint32_t key std::sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint32_data.run_data(size);
                    meter.measure([first=std::begin(data.keys0_), last=std::end(data.keys0_)] {
                            std::sort(first, last);
                            return first;
                            });
                    });
        }

        for (uint32_t size = start; size <= end; size = size << inc)
        {
            sprintf(name, "%d uint64_t key bits::radix8sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint64_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return bits::radix8sort(keys0, keys1, values0, values1, size);
                            });
                    });

#if HAVE_C99_SUPPORT
            sprintf(name, "%d uint64_t key radix8sort_u64", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint64_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return radix8sort_u64(keys0, keys1, values0, values1, size);
                            });
                    });
#endif

            sprintf(name, "%d uint64_t key bits::radix11sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint64_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return bits::radix11sort(keys0, keys1, values0, values1, size);
                            });
                    });

#if HAVE_C99_SUPPORT
            sprintf(name, "%d uint64_t key radix11sort_u64", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint64_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return radix11sort_u64(keys0, keys1, values0, values1, size);
                            });
                    });
#endif

            sprintf(name, "%d uint64_t key std::sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = uint64_data.run_data(size);
                    meter.measure([first=std::begin(data.keys0_), last=std::end(data.keys0_)] {
                            std::sort(first, last);
                            return first;
                            });
                    });
        }

        for (uint32_t size = start; size <= end; size = size << inc)
        {
            sprintf(name, "%d float key bits::radix8sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = float_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return bits::radix8sort(keys0, keys1, values0, values1, size);
                            });
                    });

#if HAVE_C99_SUPPORT
            sprintf(name, "%d float key radix8sort_f32", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = float_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return radix8sort_f32(keys0, keys1, values0, values1, size);
                            });
                    });
#endif

            sprintf(name, "%d float key bits::radix11sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = float_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return bits::radix11sort(keys0, keys1, values0, values1, size);
                            });
                    });

#if HAVE_C99_SUPPORT
            sprintf(name, "%d float key radix11sort_f32", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = float_data.run_data(size);
                    meter.measure([keys0=data.keys0(), keys1=data.keys1(),
                            values0=data.values0(), values1=data.values1(), size] {
                            return radix11sort_f32(keys0, keys1, values0, values1, size);
                            });
                    });
#endif

            sprintf(name, "%d float key std::sort", size);
            registry.emplace_back(name, [this, size](nonius::chronometer meter) {
                    auto data = float_data.run_data(size);
                    meter.measure([first=std::begin(data.keys0_), last=std::end(data.keys0_)] {
                            std::sort(first, last);
                            return first;
                            });
                    });
        }
    }
};

BenchmarkRegistrar registrar;
}

