#include "heap_sort.h"

#include <stdbool.h>
#include <stddef.h>
#include <alloca.h>
#include <string.h>

static inline void __heap_push(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *));
static inline void __heap_pop(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *));

void heap_push(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    __heap_push((char *)first, (char *)last, T_size, compare);
}

void heap_pop(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    __heap_pop((char *)first, (char *)last, T_size, compare);
}

void heap_make(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    for (size_t i = T_size; i < last - first; i += T_size)
    {
        heap_push(first, first + i, T_size, compare);
    }
}

void heap_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    heap_make(first, last, T_size, compare);

    for (size_t i = 0; i < last - first; i += T_size)
    {
        heap_pop(first, last - i, T_size, compare);
    }
}

static inline void __heap_push(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    void *value = alloca(T_size);
    ptrdiff_t holeIndex = (last - first) / T_size - 1;
    memcpy(value, holeIndex * T_size + first, T_size);

    ptrdiff_t parent = (last - first) / T_size - 2;
    while (holeIndex != 0 && compare(parent * T_size + first, value))
    {
        memcpy(holeIndex * T_size + first, parent * T_size + first, T_size);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }

    memcpy(holeIndex * T_size + first, value, T_size);
}

static inline void __heap_pop(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    void *tmp = alloca(T_size);
    ptrdiff_t len = (last - first) / T_size;
    memcpy(tmp, first, T_size);
    memcpy(first, last - T_size, T_size);
    memcpy(last - T_size, tmp, T_size);

    ptrdiff_t holeIndex = 0;
    ptrdiff_t secondChild = holeIndex * 2 + 1;

    while (secondChild < len - 1)
    {
        if (secondChild + 1 < len - 1)
            secondChild += !compare(secondChild * T_size + first, (secondChild + 1) * T_size + first) ? 0 : 1;

        if (compare(holeIndex * T_size + first, secondChild * T_size + first))
        {
            memcpy(tmp, holeIndex * T_size + first, T_size);
            memcpy(holeIndex * T_size + first, secondChild * T_size + first, T_size);
            memcpy(secondChild * T_size + first, tmp, T_size);

            holeIndex = secondChild;
            secondChild = holeIndex * 2 + 1;
        }
        else
        {
            break;
        }
    }
}