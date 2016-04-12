#ifndef BITS_HELPERS_H
#define BITS_HELPERS_H

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#define CHECK_SORTED(keys, values, orig, size)                                                     \
    for (uint32_t i = 1; i < (uint32_t)(size); ++i)                                                \
    {                                                                                              \
        if ((keys)[i - 1] > (keys)[i])                                                             \
        {                                                                                          \
            printf("Key sort error at index %u\n", i - 1);                                         \
            assert(false);                                                                         \
        }                                                                                          \
        if ((keys)[i] != (orig)[(values)[i]])                                                      \
        {                                                                                          \
            printf("Value error at index %u\n", i);                                                \
            assert(false);                                                                         \
        }                                                                                          \
    }

#define PRINT_FLOAT_ARRAY(keys, size)                                                              \
    for (uint32_t i = 0; i < (uint32_t)(size); ++i)                                                \
    {                                                                                              \
        printf("%f ", (keys)[i]);                                                                  \
    }                                                                                              \
    putchar('\n')

#define PRINT_UINT32_ARRAY(keys, size)                                                             \
    for (uint32_t i = 0; i < (uint32_t)(size); ++i)                                                \
    {                                                                                              \
        printf("%08" PRIx32 " ", (keys)[i]);                                                       \
    }                                                                                              \
    putchar('\n')

#define PRINT_UINT64_ARRAY(keys, size)                                                             \
    for (uint32_t i = 0; i < (uint32_t)(size); ++i)                                                \
    {                                                                                              \
        printf("%016" PRIx64 " ", (keys)[i]);                                                      \
    }                                                                                              \
    putchar('\n')

#define RAND_UINT32_KEYS(keys, values, copy, size)                                                 \
    for (uint32_t i = 0; i < (uint32_t)(size); ++i)                                                \
    {                                                                                              \
        (keys)[i] = (uint32_t)rand();                                                              \
        (copy)[i] = (keys)[i];                                                                     \
        (values)[i] = i;                                                                           \
    }

#define RAND_UINT64_KEYS(keys, values, copy, size)                                                 \
    for (uint32_t i = 0; i < (uint32_t)(size); ++i)                                                \
    {                                                                                              \
        (keys)[i] = ((uint64_t)rand() << 32) | ((uint64_t)rand());                                 \
        (copy)[i] = (keys)[i];                                                                     \
        (values)[i] = i;                                                                           \
    }

#define RAND_FLOAT_KEYS(keys, values, copy, size)                                                  \
    for (uint32_t i = 0; i < size; ++i)                                                            \
    {                                                                                              \
        (keys)[i] = (float)(rand()) / 2048.0f;                                                     \
        if (rand() & 1)                                                                            \
        {                                                                                          \
            (keys)[i] = -(keys)[i];                                                                \
        }                                                                                          \
        (copy)[i] = (keys)[i];                                                                     \
        (values)[i] = i;                                                                           \
    }

#endif // BITS_HELPERS_H
