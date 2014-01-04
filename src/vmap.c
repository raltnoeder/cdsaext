/**
 * Vector map
 *
 * @version 2014-01-04_001
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
#include "vmap.h"

vmap *vmap_alloc(int (*vmap_cmp_func)(void *, void *))
{
    vmap *vmap_obj;

    vmap_obj = (vmap *) malloc((size_t) sizeof(vmap));
    if (vmap_obj != NULL)
    {
        vmap_obj->head = NULL;
        vmap_obj->tail = NULL;
        vmap_obj->size = 0;
        vmap_obj->vmapCompare = vmap_cmp_func;
    }
    
    return vmap_obj;
}

void vmap_dealloc(vmap *vmap_obj)
{
    vmap_node *node;
    vmap_node *next;
    
    for (node = vmap_obj->head; node != NULL; node = next)
    {
        next = node->next;
        free((void *) node);
    }
    free((void *) vmap_obj);
}


int vmap_prepend(vmap *vmap_obj, void *key, void *val)
{
    vmap_node *ins;
    vmap_node *next;

    ins = (vmap_node *) malloc((size_t) sizeof(vmap_node));
    if (ins == NULL)
    {
        return VMAP_ERR_NOMEM;
    }

    ins->key = key;
    ins->val = val;
    ins->prev = NULL;
    next = vmap_obj->head;
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
    
    return VMAP_PASS;
}

int vmap_append(vmap *vmap_obj, void *key, void *val)
{
    vmap_node *ins;
    vmap_node *prev;

    ins = (vmap_node *) malloc((size_t) sizeof(vmap_node));
    if (ins == NULL)
    {
        return VMAP_ERR_NOMEM;
    }

    ins->key = key;
    ins->val = val;
    prev = vmap_obj->tail;
    ins->prev = prev;
    if (prev != NULL)
    {
        prev->next = ins;
    }
    ins->next = NULL;
    vmap_obj->tail = ins;
    if (vmap_obj->head == NULL)
    {
        vmap_obj->head = ins;
    }

    ++(vmap_obj->size);

    return VMAP_PASS;
}

int vmap_insertbeforenode(vmap *vmap_obj, vmap_node *crt, void *key, void *val)
{
    vmap_node *ins;

    ins = (vmap_node *) malloc((size_t) sizeof(vmap_node));
    if (ins == NULL)
    {
        return VMAP_ERR_NOMEM;
    }

    ins->key = key;
    ins->val = val;
    ins->prev = crt->prev;
    ins->next = crt;
    crt->prev = ins;
    if (ins->prev == NULL)
    {
        vmap_obj->head = ins;
    } else {
        ins->prev->next = ins;
    }
    
    ++(vmap_obj->size);

    return VMAP_PASS;
}

void vmap_remove(vmap *vmap_obj, void *key)
{
    vmap_node *node;

    node = _vmap_findnode(vmap_obj, key);
    if (node != NULL)
    {
        _vmap_removenode(vmap_obj, node);
    }
}

void vmap_removenode(vmap *vmap_obj, vmap_node *node)
{
    _vmap_removenode(vmap_obj, node);
}

void *vmap_get(vmap *vmap_obj, void *key)
{
    vmap_node *node;

    node = _vmap_findnode(vmap_obj, key);
    if (node != NULL)
    {
        return node->val;
    }

    return NULL;
}

vmap_node *vmap_getnode(vmap *vmap_obj, void *key)
{
    return _vmap_findnode(vmap_obj, key);
}

vmap_it *vmap_iterator(vmap *vmap_obj)
{
    vmap_it *it;

    it = (vmap_it *) malloc((size_t) sizeof(vmap_it));
    if (it != NULL)
    {
        it->next = vmap_obj->head;
    }

    return it;
}

vmap_node *vmap_next(vmap_it *it)
{
    vmap_node *crt;

    crt = it->next;
    if (crt != NULL)
    {
        it->next = crt->next;
    }

    return crt;
}

inline vmap_node *_vmap_findnode(vmap *vmap_obj, void *key)
{
    vmap_node *node;

    for (node = vmap_obj->head; node != NULL; node = node->next)
    {
        if (vmap_obj->vmapCompare(node->key, key) == 0)
        {
            break;
        }
    }

    return node;
}

inline void _vmap_removenode(vmap *vmap_obj, vmap_node *node)
{
    if (vmap_obj->head == node)
    {
        vmap_obj->head = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (vmap_obj->tail == node)
    {
        vmap_obj->tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }

    free((void *) node);
    --(vmap_obj->size);
}
