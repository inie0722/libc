#pragma once

#include <stdbool.h>
#include <stddef.h>

extern void merge_sort(void *first, void *last, size_t T_size, bool (*compare)(const void *, const void *));

extern void merge(void *first_1, void *last_1, void *first_2, void *last_2, void *buf, size_t T_size, bool (*compare)(const void *, const void *));

extern void inplace_merge(void *first_1, void *last_1, void *first_2, void *last_2, size_t T_size, bool (*compare)(const void *, const void *));

extern void reverse(void *first, void *last, size_t T_size);