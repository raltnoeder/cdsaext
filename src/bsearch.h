/* 
 * File:   bsearch.h
 * Author: bobby
 *
 * Created on September 30, 2013, 9:15 PM
 */

#ifndef BSEARCH_H
#define	BSEARCH_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <sys/types.h>

#define BSEARCH_NPOS ((size_t) ~0)

    size_t gbsearch(void **, size_t, void *, int (*)(void *, void *));
    size_t bsearch_ull(
        unsigned long long *, size_t,
        unsigned long long
    );

#ifdef	__cplusplus
}
#endif

#endif	/* BSEARCH_H */

