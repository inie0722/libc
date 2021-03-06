#include "intro_sort.h"

#include "insertion_sort.h"
#include "heap_sort.h"
#include "quick_sort.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <alloca.h>

#define THRESHOLD 16

static void introsort_loop(void *first, void *last, size_t depth_limit, size_t T_size, bool (*compare)(const void *, const void *));

static inline size_t lg(size_t n)
{
    size_t i = 0;
    for (; n > 1; ++i)
    {
        n >>= 1;
    }
    return i;
}

void intro_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    void *value = alloca(T_size);
    if (first != last)
    {
        //最多允许递归logn*2
        introsort_loop(first, last, lg((last - first) / T_size * 2), T_size, compare);
        if (last - first > THRESHOLD * T_size)
        {
            //对第一段元素进行排序
            insertion_sort(first, first + THRESHOLD * T_size, T_size, compare);

            //因为这些元素都被快排过了 第一段元素必定拥有最小的元素 因此直接进入内部循环即可

            int t = 0;
            for (void *i = first + THRESHOLD * T_size; i != last; i += T_size)
            {
                ++t;
                memcpy(value, i, T_size);
                unguarded_linear_insert(i, value, T_size, compare);
            }
        }
        else
        {
            insertion_sort(first, last, T_size, compare);
        }
    }
}

static void introsort_loop(void *first, void *last, size_t depth_limit, size_t T_size, bool (*compare)(const void *, const void *))
{
    void *value = alloca(T_size);
    while (last - first > THRESHOLD * T_size)
    {
        //如果到达递归深度极限
        if (depth_limit == 0)
        {
            //剩余部分直接采用堆排序
            heap_sort(first, last, T_size, compare);
            break;
        }
        else
        {
            --depth_limit;
            //进行快排分割

            size_t d = (last - first) / T_size / 8 * T_size;

            void *med[3];
            med[0] = median(first, first + d, first + d * 2, compare);
            med[1] = median(first + d * 3, first + d * 4, first + d * 5, compare);
            med[2] = median(first + d * 6, first + d * 7, last - T_size, compare);
            med[0] = median(med[0], med[1], med[2], compare);
            memcpy(value, med[0], T_size);

            void *cur = unguarded_partition(first, last, value, T_size, compare);

            //对右半段递归处理
            introsort_loop(cur, last, depth_limit, T_size, compare);
            //introsort_loop(first, cur, depth_limit);
            //左半段循环处理
            last = cur;
        }
    }
}