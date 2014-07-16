/**
 * Binary search in a sorted array
 *
 * @version 2014-07-14_001
 * @author  Robert Altnoeder (r.altnoeder@gmx.net)
 *
 * Copyright (C) 2012, 2014 Robert ALTNOEDER
 *
 * Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that
 * the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "bsearch.h"

const size_t BSEARCH_NPOS = ((size_t) ~0);

size_t gbsearch(
    void   *arr[],
    size_t arr_len,
    void   *val,
    int    (*comp_func)(void *, void *)
)
{
    size_t sidx;
    size_t eidx;
    size_t midx;
    size_t width;

    sidx = 0;
    eidx = arr_len;

    while ((width = eidx - sidx ) > 0)
    {
        midx = sidx + (width / 2);
        if (comp_func(arr[midx], val) == 0)
        {
            return midx;
        }
        else
        if (comp_func(arr[midx], val) > 0)
        {
            eidx = midx;
        }
        else
        {
            sidx = midx + 1;
        }
    }

    return BSEARCH_NPOS;
}

size_t bsearch_uint64(
    uint64_t arr[],
    size_t   arr_len,
    uint64_t val
)
{
    size_t sidx;
    size_t eidx;
    size_t midx;
    size_t width;

    sidx = 0;
    eidx = arr_len;

    while ((width = eidx - sidx ) > 0)
    {
        midx = sidx + (width / 2);
        if (arr[midx] == val)
        {
            return midx;
        }
        else
        if (arr[midx] > val)
        {
            eidx = midx;
        }
        else
        {
            sidx = midx + 1;
        }
    }

    return BSEARCH_NPOS;
}
