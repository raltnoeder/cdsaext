/**
 * Vector map
 *
 * @version 2015-07-30_001
 * @author  Robert Altnoeder (r.altnoeder@gmx.net)
 *
 * Copyright (C) 2012, 2015 Robert ALTNOEDER
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
#include "vmap.h"

static inline vmap_node *vmap_impl_find_node(vmap *vmap_obj, void *key);
static inline void      vmap_impl_remove_node(vmap *vmap_obj, vmap_node *node);
static inline void      vmap_impl_unlink_node(vmap *vmap_obj, vmap_node *node);
static inline void      vmap_impl_prepend_node(vmap *vmap_obj, vmap_node *ins);
static inline void      vmap_impl_append_node(vmap *vmap_obj, vmap_node *ins);
static inline void      vmap_impl_insert_node_before(
    vmap      *vmap_obj,
    vmap_node *crt,
    vmap_node *ins
);
static inline void      vmap_impl_init(vmap *vmap_obj, vmap_cmp_func cmp_func_ptr);
static inline void      vmap_impl_clear(vmap *vmap_obj);


vmap *vmap_alloc(vmap_cmp_func cmp_func_ptr)
{
    vmap *vmap_obj = malloc(sizeof (vmap));
    if (vmap_obj != NULL)
    {
        vmap_impl_init(vmap_obj, cmp_func_ptr);
    }

    return vmap_obj;
}


void vmap_dealloc(vmap *vmap_obj)
{
    vmap_impl_clear(vmap_obj);
    free(vmap_obj);
}


void vmap_clear(vmap *vmap_obj)
{
    vmap_impl_clear(vmap_obj);
    vmap_obj->size = 0;
    vmap_obj->head = NULL;
    vmap_obj->tail = NULL;
}


void vmap_init(vmap *vmap_obj, vmap_cmp_func cmp_func_ptr)
{
    vmap_impl_init(vmap_obj, cmp_func_ptr);
}


vmap_rc vmap_prepend(
    vmap *vmap_obj,
    void *key,
    void *val
)
{
    vmap_node *ins = malloc(sizeof (vmap_node));
    if (ins == NULL)
    {
        return VMAP_ERR_NOMEM;
    }

    ins->key = key;
    ins->val = val;

    vmap_impl_prepend_node(vmap_obj, ins);

    return VMAP_PASS;
}


void vmap_prepend_node(vmap *vmap_obj, vmap_node *ins)
{
    vmap_impl_prepend_node(vmap_obj, ins);
}


vmap_rc vmap_append(
    vmap *vmap_obj,
    void *key,
    void *val
)
{
    vmap_node *ins = malloc(sizeof (vmap_node));
    if (ins == NULL)
    {
        return VMAP_ERR_NOMEM;
    }

    ins->key = key;
    ins->val = val;

    vmap_impl_append_node(vmap_obj, ins);

    return VMAP_PASS;
}


void vmap_append_node(vmap *vmap_obj, vmap_node *ins)
{
    vmap_impl_append_node(vmap_obj, ins);
}


vmap_rc vmap_insert_before(
    vmap      *vmap_obj,
    vmap_node *crt,
    void      *key,
    void      *val
)
{
    vmap_node *ins = malloc(sizeof (vmap_node));
    if (ins == NULL)
    {
        return VMAP_ERR_NOMEM;
    }

    ins->key = key;
    ins->val = val;

    vmap_impl_insert_node_before(vmap_obj, crt, ins);

    return VMAP_PASS;
}


void vmap_insert_node_before(
    vmap      *vmap_obj,
    vmap_node *crt,
    vmap_node *ins
)
{
    vmap_impl_insert_node_before(vmap_obj, crt, ins);
}


void vmap_remove(vmap *vmap_obj, void *key)
{
    vmap_node *node = vmap_impl_find_node(vmap_obj, key);
    if (node != NULL)
    {
        vmap_impl_remove_node(vmap_obj, node);
    }
}


void vmap_remove_node(vmap *vmap_obj, vmap_node *node)
{
    vmap_impl_remove_node(vmap_obj, node);
}


void vmap_unlink_node(vmap *vmap_obj, vmap_node *node)
{
    vmap_impl_unlink_node(vmap_obj, node);
}


void *vmap_get(vmap *vmap_obj, void *key)
{
    void      *val   = NULL;
    vmap_node *node  = vmap_impl_find_node(vmap_obj, key);
    if (node != NULL)
    {
        val = node->val;
    }

    return val;
}


vmap_node *vmap_get_node(vmap *vmap_obj, void *key)
{
    return vmap_impl_find_node(vmap_obj, key);
}


vmap_it *vmap_iterator(vmap *vmap_obj)
{
    vmap_it *it = malloc(sizeof (vmap_it));
    if (it != NULL)
    {
        it->next = vmap_obj->head;
    }

    return it;
}


void vmap_iterator_init(vmap *vmap_obj, vmap_it *it)
{
    it->next = vmap_obj->head;
}


vmap_node *vmap_next(vmap_it *it)
{
    vmap_node *crt = it->next;
    if (crt != NULL)
    {
        it->next = crt->next;
    }

    return crt;
}


static inline vmap_node *vmap_impl_find_node(vmap *vmap_obj, void *key)
{
    vmap_node *node;
    for (node = vmap_obj->head; node != NULL; node = node->next)
    {
        if (vmap_obj->vmap_cmp(node->key, key) == 0)
        {
            break;
        }
    }

    return node;
}


static inline void vmap_impl_remove_node(vmap *vmap_obj, vmap_node *node)
{
    vmap_impl_unlink_node(vmap_obj, node);
    free(node);
}


static inline void vmap_impl_unlink_node(vmap *vmap_obj, vmap_node *node)
{
    if (vmap_obj->head == node)
    {
        vmap_obj->head = node->next;
    }
    else
    {
        node->prev->next = node->next;
    }
    if (vmap_obj->tail == node)
    {
        vmap_obj->tail = node->prev;
    }
    else
    {
        node->next->prev = node->prev;
    }
    --(vmap_obj->size);
}


static inline void vmap_impl_prepend_node(vmap *vmap_obj, vmap_node *ins)
{
    vmap_node *next = vmap_obj->head;

    ins->prev = NULL;
    ins->next = next;
    if (next != NULL)
    {
        next->prev = ins;
    }
    vmap_obj->head = ins;

    if (vmap_obj->tail == NULL)
    {
        vmap_obj->tail = ins;
    }

    ++(vmap_obj->size);
}


static inline void vmap_impl_append_node(vmap *vmap_obj, vmap_node *ins)
{
    vmap_node *prev = vmap_obj->tail;

    ins->next = NULL;
    ins->prev = prev;
    if (prev != NULL)
    {
        prev->next = ins;
    }
    vmap_obj->tail = ins;
    if (vmap_obj->head == NULL)
    {
        vmap_obj->head = ins;
    }

    ++(vmap_obj->size);
}


static inline void vmap_impl_insert_node_before(
    vmap      *vmap_obj,
    vmap_node *crt,
    vmap_node *ins
)
{
    ins->prev = crt->prev;
    ins->next = crt;
    crt->prev = ins;
    if (ins->prev == NULL)
    {
        vmap_obj->head = ins;
    }
    else
    {
        ins->prev->next = ins;
    }

    ++(vmap_obj->size);
}


static inline void vmap_impl_init(vmap *vmap_obj, vmap_cmp_func cmp_func_ptr)
{
    vmap_obj->head     = NULL;
    vmap_obj->tail     = NULL;
    vmap_obj->size     = 0;
    vmap_obj->vmap_cmp = cmp_func_ptr;
}


static inline void vmap_impl_clear(vmap *vmap_obj)
{
    // The "node = next" assignment executes after
    // "next = node->next" assignment inside the "for" block
    vmap_node *next;
    for (vmap_node *node = vmap_obj->head; node != NULL; node = next)
    {
        next = node->next;
        free(node);
    }
}
