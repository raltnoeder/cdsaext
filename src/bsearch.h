#ifndef BSEARCH_H
#define	BSEARCH_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>

    extern const size_t BSEARCH_NPOS;

    size_t gbsearch(void **, size_t, void *, int (*)(void *, void *));
    size_t bsearch_ull(
        uint64_t *,
        size_t,
        uint64_t
    );

#ifdef	__cplusplus
}
#endif

#endif	/* BSEARCH_H */

