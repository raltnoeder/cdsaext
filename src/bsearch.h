#ifndef BSEARCH_H
#define	BSEARCH_H

#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>

extern const size_t BSEARCH_NPOS;

typedef int (*gbsearch_cmp_func)(void *value_a, void *value_b);

size_t gbsearch(
    void              *array[],
    size_t            array_length,
    void              *value,
    gbsearch_cmp_func compare_func
);

size_t bsearch_uint64(
    uint64_t array[],
    size_t   array_length,
    uint64_t value
);

#endif	/* BSEARCH_H */
