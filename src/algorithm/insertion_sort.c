#include "insertion_sort.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <alloca.h>

static inline void __insertion_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *));
static inline void linear_insert(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *));
static inline void __unguarded_linear_insert(char *last, void *value, size_t T_size, bool (*compare)(const void *, const void *));

void insertion_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    __insertion_sort((char *)first, (char *)last, T_size, compare);
}

void unguarded_linear_insert(void *last, void *value, size_t T_size, bool (*compare)(const void *, const void *))
{
    __unguarded_linear_insert((char *)last, value, T_size, compare);
}

static inline void __insertion_sort(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    for (char *i = first + T_size; i < last; i += T_size)
    {
        linear_insert(first, i, T_size, compare);
    }
}

static inline void linear_insert(char *first, char *last, size_t T_size, bool (*compare)(const void *, const void *))
{
    void *value = alloca(T_size);
    memcpy(value, last, T_size);

    //如果尾元素小于头元素 直接插入到 头元素前面就行
    if (compare(value, first))
    {
        memmove(first + T_size, first, last - first);
        memcpy(first, value, T_size);
    }
    else // 尾不小于头
        unguarded_linear_insert(last, value, T_size, compare);
}

static inline void __unguarded_linear_insert(char *last, void *value, size_t T_size, bool (*compare)(const void *, const void *))
{
    char *next = last - T_size;

    while (compare(value, next))
    { // 逆转对(inversion)存在
        memcpy(last, next, T_size);
        last = next;    // 调整迭代器
        next -= T_size; // 左移一个位置
    }
    memcpy(last, value, T_size); // value 的正确落脚处
}