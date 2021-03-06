/**
 * Binary search in a sorted array
 *
 * @version 2018-05-16_001
 * @author  Robert Altnoeder (r.altnoeder@gmx.net)
 *
 * Copyright (C) 2012 - 2018 Robert ALTNOEDER
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
    const void *const *const    array,
    const size_t                array_length,
    const void *const           value,
    const gbsearch_cmp_func     compare_func
)
{
    size_t result      = BSEARCH_NPOS;
    size_t start_index = 0;
    size_t end_index   = array_length;

    size_t width = array_length;
    while (width > 0)
    {
        const size_t mid_index = start_index + (width / 2);
        const int direction = compare_func(array[mid_index], value);
        if (direction < 0)
        {
            start_index = mid_index + 1;
        }
        else
        if (direction > 0)
        {
            end_index = mid_index;
        }
        else
        {
            result = mid_index;
            break;
        }
        width = end_index - start_index;
    }

    return result;
}


size_t bsearch_uint64(
    const uint64_t *const   array,
    const size_t            array_length,
    const uint64_t          value
)
{
    size_t result      = BSEARCH_NPOS;
    size_t start_index = 0;
    size_t end_index   = array_length;

    size_t width = array_length;
    while (width > 0)
    {
        const size_t mid_index = start_index + (width / 2);
        if (array[mid_index] < value)
        {
            start_index = mid_index + 1;
        }
        else
        if (array[mid_index] > value)
        {
            end_index = mid_index;
        }
        else
        {
            result = mid_index;
            break;
        }
        width = end_index - start_index;
    }

    return result;
}
