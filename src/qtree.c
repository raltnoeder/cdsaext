/**
 * Quick balanced binary search tree
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
#include "qtree.h"

typedef enum
{
    QTREE_DIR_LESS    = 0,
    QTREE_DIR_GREATER = 1
}
qtree_dir;


static inline qtree_node *qtree_impl_find_node(const qtree *qtree_obj, const void *key);
static inline qtree_rc   qtree_impl_insert_node(qtree *qtree_obj, qtree_node *ins_node);
static inline void       qtree_impl_remove_node(qtree *qtree_obj, qtree_node *rm_node);
static inline void       qtree_impl_unlink_node(qtree *qtree_obj, qtree_node *rm_node);
static inline void       qtree_impl_rebalance_insert(
    qtree      *qtree_obj,
    qtree_node *sub_node,
    qtree_node *rot_node
);
static inline void       qtree_impl_rebalance_remove(
    qtree      *qtree_obj,
    qtree_dir  dir,
    qtree_node *rot_node
);
static inline void       qtree_impl_init(qtree *qtree_obj, const qtree_cmp_func cmp_func_ptr);
static inline void       qtree_impl_iterator_init(const qtree *qtree_obj, qtree_it *iter);
static inline void       qtree_impl_clear(qtree *qtree_obj);


qtree *qtree_alloc(const qtree_cmp_func cmp_func_ptr)
{
    qtree *qtree_obj = malloc(sizeof (qtree));
    if (qtree_obj != NULL)
    {
        qtree_impl_init(qtree_obj, cmp_func_ptr);
    }

    return qtree_obj;
}


void qtree_dealloc(qtree *qtree_obj)
{
    qtree_impl_clear(qtree_obj);
    free(qtree_obj);
}


void qtree_clear(qtree *qtree_obj)
{
    qtree_impl_clear(qtree_obj);
    qtree_obj->size = 0;
    qtree_obj->root = NULL;
}


void qtree_init(qtree *qtree_obj, const qtree_cmp_func cmp_func_ptr)
{
    qtree_impl_init(qtree_obj, cmp_func_ptr);
}


qtree_rc qtree_insert(qtree *qtree_obj, const void *key_ptr, const void *value_ptr)
{
    qtree_rc rc = QTREE_PASS;

    qtree_node **ref_ins_node = NULL;
    qtree_node *parent_node = NULL;

    if (qtree_obj->root == NULL)
    {
        ref_ins_node = &qtree_obj->root;
    }
    else
    {
        parent_node = qtree_obj->root;
        while (true)
        {
            const int cmp_rc = qtree_obj->qtree_cmp(key_ptr, parent_node->key);
            if (cmp_rc < 0)
            {
                if (parent_node->less == NULL)
                {
                    ref_ins_node = &parent_node->less;
                    break;
                }
                else
                {
                    parent_node = parent_node->less;
                }
            }
            else
            if (cmp_rc > 0)
            {
                if (parent_node->greater == NULL)
                {
                    ref_ins_node = &parent_node->greater;
                    break;
                }
                else
                {
                    parent_node = parent_node->greater;
                }
            }
            else
            {
                rc = QTREE_ERR_EXISTS;
                break;
            }
        }
    }

    if (ref_ins_node != NULL)
    {
        qtree_node *ins_node = malloc(sizeof (qtree_node));
        if (ins_node != NULL)
        {
            *ref_ins_node     = ins_node;
            ins_node->key     = key_ptr;
            ins_node->value   = value_ptr;
            ins_node->parent  = parent_node;
            ins_node->less    = NULL;
            ins_node->greater = NULL;
            ins_node->balance = 0;
            ++(qtree_obj->size);
            if (parent_node != NULL)
            {
                qtree_impl_rebalance_insert(qtree_obj, ins_node, parent_node);
            }
        }
        else
        {
            rc = QTREE_ERR_NOMEM;
        }
    }

    return rc;
}


qtree_rc qtree_insert_node(qtree *qtree_obj, qtree_node *node)
{
    return qtree_impl_insert_node(qtree_obj, node);
}


void qtree_remove(qtree *qtree_obj, const void *key_ptr)
{
    qtree_node *node = qtree_impl_find_node(qtree_obj, key_ptr);
    if (node != NULL)
    {
        qtree_impl_remove_node(qtree_obj, node);
    }
}


void qtree_remove_node(qtree *qtree_obj, qtree_node *node)
{
    qtree_impl_remove_node(qtree_obj, node);
}


void qtree_unlink_node(qtree *qtree_obj, qtree_node *node)
{
    qtree_impl_unlink_node(qtree_obj, node);
}


void *qtree_get(const qtree *qtree_obj, const void *key_ptr)
{
    const void *value = NULL;
    qtree_node *node  = qtree_impl_find_node(qtree_obj, key_ptr);
    if (node != NULL)
    {
        value = node->value;
    }
    return (void *) value;
}


qtree_node *qtree_get_node(const qtree *qtree_obj, const void *key)
{
    return qtree_impl_find_node(qtree_obj, key);
}


size_t qtree_get_size(const qtree *qtree_obj)
{
    return qtree_obj->size;
}


qtree_it *qtree_iterator(const qtree *qtree_obj)
{
    qtree_it *iter = malloc(sizeof (qtree_it));
    if (iter != NULL)
    {
        qtree_impl_iterator_init(qtree_obj, iter);
    }
    return iter;
}


void qtree_iterator_init(const qtree *qtree_obj, qtree_it *iter)
{
    qtree_impl_iterator_init(qtree_obj, iter);
}


qtree_node *qtree_next(qtree_it *iter)
{
    qtree_node *ret_node = iter->next;

    if (ret_node != NULL)
    {
        qtree_node *next_node = ret_node;
        if (next_node->greater != NULL)
        {
            next_node = next_node->greater;
            while (next_node->less != NULL)
            {
                next_node = next_node->less;
            }
        }
        else
        {
            do
            {
                if (next_node->parent != NULL)
                {
                    if (next_node->parent->less == next_node)
                    {
                        next_node = next_node->parent;
                        break;
                    }
                }
                next_node = next_node->parent;
            }
            while (next_node != NULL);
        }
        iter->next = next_node;
    }

    return ret_node;
}


/**
 * Rebalances the tree after node removal
 *
 * WARNING! The order of statements, especially assignments, in this function
 *          is critical for the function's correct operation.
 *          DO NOT CHANGE THE ORDER OF ANY STATEMENTS.
 */
static inline void qtree_impl_rebalance_remove(
    qtree       *qtree_obj,
    qtree_dir   dir,
    qtree_node  *rot_node
)
{
    // update balance and perform rotations
    while (rot_node != NULL)
    {
        if (dir == QTREE_DIR_LESS)
        {
            // node was removed from left subtree
            ++rot_node->balance;
            if (rot_node->balance == 1)
            {
                break;
            }
        }
        else
        {
            // node was removed from right subtree
            --rot_node->balance;
            if (rot_node->balance == -1)
            {
                break;
            }
        }

        if (rot_node->parent != NULL)
        {
            if (rot_node->parent->less == rot_node)
            {
                dir = QTREE_DIR_LESS;
            }
            else
            {
                dir = QTREE_DIR_GREATER;
            }
        }

        // update balance and perform rotations
        if (rot_node->balance == -2)
        {
            qtree_node *sub_node = rot_node->less;
            // 0 or -1
            if (sub_node->balance <= 0)
            {
                // rotate R
                sub_node->parent = rot_node->parent;
                if (rot_node->parent != NULL)
                {
                    if (rot_node->parent->less == rot_node)
                    {
                        rot_node->parent->less = sub_node;
                    }
                    else
                    {
                        rot_node->parent->greater = sub_node;
                    }
                }
                else
                {
                    qtree_obj->root = sub_node;
                }

                rot_node->less = sub_node->greater;
                if (sub_node->greater != NULL)
                {
                    sub_node->greater->parent = rot_node;
                }

                sub_node->greater = rot_node;
                rot_node->parent  = sub_node;

                if (sub_node->balance == 0)
                {
                    rot_node->balance = -1;
                    sub_node->balance = 1;
                    break;
                }
                else
                {
                    rot_node->balance = 0;
                    sub_node->balance = 0;
                }
            }
            else
            {
                // rotate LR
                if (sub_node->greater->balance == -1)
                {
                    sub_node->balance = 0;
                    rot_node->balance = 1;
                }
                else
                if (sub_node->greater->balance == 1)
                {
                    sub_node->balance = -1;
                    rot_node->balance = 0;
                }
                else
                {
                    sub_node->balance = 0;
                    rot_node->balance = 0;
                }
                sub_node->greater->balance = 0;

                sub_node->parent         = sub_node->greater;
                sub_node->greater        = sub_node->greater->less;
                sub_node->parent->less   = sub_node;
                rot_node->less           = sub_node->parent->greater;
                sub_node->parent->parent = rot_node->parent;
                if (sub_node->greater != NULL)
                {
                    sub_node->greater->parent = sub_node;
                }
                if (rot_node->less != NULL)
                {
                    rot_node->less->parent = rot_node;
                }

                if (rot_node->parent != NULL)
                {
                    if (rot_node->parent->less == rot_node)
                    {
                        rot_node->parent->less = sub_node->parent;
                    }
                    else
                    {
                        rot_node->parent->greater = sub_node->parent;
                    }
                }
                else
                {
                    qtree_obj->root = sub_node->parent;
                }

                rot_node->parent          = sub_node->parent;
                sub_node->parent->greater = rot_node;
            }
            rot_node = rot_node->parent;
            // end of R / LR rotations
        }
        else
        if (rot_node->balance == 2)
        {
            qtree_node *sub_node = rot_node->greater;
            // 0 or 1
            if (sub_node->balance >= 0)
            {
                // rotate L
                sub_node->parent = rot_node->parent;
                if (rot_node->parent != NULL)
                {
                    if (rot_node->parent->less == rot_node)
                    {
                        rot_node->parent->less = sub_node;
                    }
                    else
                    {
                        rot_node->parent->greater = sub_node;
                    }
                }
                else
                {
                    qtree_obj->root = sub_node;
                }

                rot_node->greater = sub_node->less;
                if (sub_node->less != NULL)
                {
                    sub_node->less->parent = rot_node;
                }

                sub_node->less   = rot_node;
                rot_node->parent = sub_node;
                if (sub_node->balance == 0)
                {
                    rot_node->balance = 1;
                    sub_node->balance = -1;
                    break;
                }
                else
                {
                    rot_node->balance = 0;
                    sub_node->balance = 0;
                }
            }
            else
            {
                // rotate RL
                if (sub_node->less->balance == -1)
                {
                    sub_node->balance = 1;
                    rot_node->balance = 0;
                }
                else
                if (sub_node->less->balance == 1)
                {
                    sub_node->balance = 0;
                    rot_node->balance = -1;
                }
                else
                {
                    sub_node->balance = 0;
                    rot_node->balance = 0;
                }
                sub_node->less->balance = 0;

                sub_node->parent          = sub_node->less;
                sub_node->less            = sub_node->less->greater;
                sub_node->parent->greater = sub_node;
                rot_node->greater         = sub_node->parent->less;
                sub_node->parent->parent  = rot_node->parent;
                if (sub_node->less != NULL)
                {
                    sub_node->less->parent = sub_node;
                }
                if (rot_node->greater != NULL)
                {
                    rot_node->greater->parent = rot_node;
                }

                if (rot_node->parent != NULL)
                {
                    if (rot_node->parent->less == rot_node)
                    {
                        rot_node->parent->less = sub_node->parent;
                    }
                    else
                    {
                        rot_node->parent->greater = sub_node->parent;
                    }
                }
                else
                {
                    qtree_obj->root = sub_node->parent;
                }

                rot_node->parent       = sub_node->parent;
                sub_node->parent->less = rot_node;
            }
            rot_node = rot_node->parent;
            // end of L / RL rotations
        }
        rot_node = rot_node->parent;
    }
}


/**
 * Rebalances the tree after node insertion
 *
 * WARNING! The order of statements, especially assignments, in this function
 *          is critical for the function's correct operation.
 *          DO NOT CHANGE THE ORDER OF ANY STATEMENTS.
 */
static inline void qtree_impl_rebalance_insert(
    qtree      *qtree_obj,
    qtree_node *sub_node,
    qtree_node *rot_node
)
{
    // update balance and perform rotations
    do
    {
        if (rot_node->less == sub_node)
        {
            --(rot_node->balance);
        }
        else
        {
            ++(rot_node->balance);
        }

        if (rot_node->balance == 0)
        {
            break;
        }
        else
        if (rot_node->balance == -2)
        {
            if (sub_node->balance == -1)
            {
                // rotate R
                rot_node->balance = 0;
                sub_node->balance = 0;

                sub_node->parent = rot_node->parent;
                if (rot_node->parent != NULL)
                {
                    if (rot_node->parent->less == rot_node)
                    {
                        rot_node->parent->less = sub_node;
                    }
                    else
                    {
                        rot_node->parent->greater = sub_node;
                    }
                }
                else
                {
                    qtree_obj->root = sub_node;
                }

                rot_node->less = sub_node->greater;
                if (sub_node->greater != NULL)
                {
                    sub_node->greater->parent = rot_node;
                }

                sub_node->greater = rot_node;
                rot_node->parent = sub_node;
            }
            else
            {
                // rotate LR
                if (sub_node->greater->balance == -1)
                {
                    sub_node->balance = 0;
                    rot_node->balance = 1;
                }
                else
                if (sub_node->greater->balance == 1)
                {
                    sub_node->balance = -1;
                    rot_node->balance = 0;
                }
                else
                {
                    sub_node->balance = 0;
                    rot_node->balance = 0;
                }
                sub_node->greater->balance = 0;

                sub_node->parent         = sub_node->greater;
                sub_node->greater        = sub_node->greater->less;
                sub_node->parent->less   = sub_node;
                rot_node->less           = sub_node->parent->greater;
                sub_node->parent->parent = rot_node->parent;
                if (sub_node->greater != NULL)
                {
                    sub_node->greater->parent = sub_node;
                }
                if (rot_node->less != NULL)
                {
                    rot_node->less->parent = rot_node;
                }

                if (rot_node->parent != NULL)
                {
                    if (rot_node->parent->less == rot_node)
                    {
                        rot_node->parent->less = sub_node->parent;
                    }
                    else
                    {
                        rot_node->parent->greater = sub_node->parent;
                    }
                }
                else
                {
                    qtree_obj->root = sub_node->parent;
                }

                rot_node->parent          = sub_node->parent;
                sub_node->parent->greater = rot_node;
            }
            break;
        }
        else
        if (rot_node->balance == 2)
        {
            if (sub_node->balance == 1)
            {
                // rotate L
                rot_node->balance = 0;
                sub_node->balance = 0;

                sub_node->parent = rot_node->parent;
                if (rot_node->parent != NULL)
                {
                    if (rot_node->parent->less == rot_node)
                    {
                        rot_node->parent->less = sub_node;
                    }
                    else
                    {
                        rot_node->parent->greater = sub_node;
                    }
                }
                else
                {
                    qtree_obj->root = sub_node;
                }

                rot_node->greater = sub_node->less;
                if (sub_node->less != NULL)
                {
                    sub_node->less->parent = rot_node;
                }

                sub_node->less = rot_node;
                rot_node->parent = sub_node;
            }
            else
            {
                // rotate RL
                if (sub_node->less->balance == -1)
                {
                    sub_node->balance = 1;
                    rot_node->balance = 0;
                }
                else
                if (sub_node->less->balance == 1)
                {
                    sub_node->balance = 0;
                    rot_node->balance = -1;
                }
                else
                {
                    sub_node->balance = 0;
                    rot_node->balance = 0;
                }
                sub_node->less->balance = 0;

                sub_node->parent          = sub_node->less;
                sub_node->less            = sub_node->less->greater;
                sub_node->parent->greater = sub_node;
                rot_node->greater         = sub_node->parent->less;
                sub_node->parent->parent  = rot_node->parent;
                if (sub_node->less != NULL)
                {
                    sub_node->less->parent = sub_node;
                }
                if (rot_node->greater != NULL)
                {
                    rot_node->greater->parent = rot_node;
                }

                if (rot_node->parent != NULL)
                {
                    if (rot_node->parent->less == rot_node)
                    {
                        rot_node->parent->less = sub_node->parent;
                    }
                    else
                    {
                        rot_node->parent->greater = sub_node->parent;
                    }
                }
                else
                {
                    qtree_obj->root = sub_node->parent;
                }

                rot_node->parent       = sub_node->parent;
                sub_node->parent->less = rot_node;
            }
            break;
        }

        sub_node = rot_node;
        rot_node = rot_node->parent;
    }
    while (rot_node != NULL);
}


static inline qtree_rc qtree_impl_insert_node(qtree *qtree_obj, qtree_node *ins_node)
{
    qtree_rc rc = QTREE_PASS;

    if (qtree_obj->root == NULL)
    {
        qtree_obj->root   = ins_node;
        ins_node->less    = NULL;
        ins_node->greater = NULL;
        ins_node->parent  = NULL;
        ins_node->balance = 0;
        ++(qtree_obj->size);
    }
    else
    {
        qtree_node *parent_node = qtree_obj->root;
        while (true)
        {
            const int cmp_rc = qtree_obj->qtree_cmp(ins_node->key, parent_node->key);
            if (cmp_rc < 0)
            {
                if (parent_node->less == NULL)
                {
                    parent_node->less = ins_node;
                    ins_node->parent  = parent_node;
                    ins_node->less    = NULL;
                    ins_node->greater = NULL;
                    ins_node->balance = 0;
                    ++(qtree_obj->size);
                    qtree_impl_rebalance_insert(qtree_obj, ins_node, parent_node);
                    break;
                }
                else
                {
                    parent_node = parent_node->less;
                }
            }
            else
            if (cmp_rc > 0)
            {
                if (parent_node->greater == NULL)
                {
                    parent_node->greater = ins_node;
                    ins_node->parent     = parent_node;
                    ins_node->less       = NULL;
                    ins_node->greater    = NULL;
                    ins_node->balance    = 0;
                    ++(qtree_obj->size);
                    qtree_impl_rebalance_insert(qtree_obj, ins_node, parent_node);
                    break;
                }
                else
                {
                    parent_node = parent_node->greater;
                }
            }
            else
            {
                rc = QTREE_ERR_EXISTS;
                break;
            }
        }
    }

    return rc;
}


static inline void qtree_impl_remove_node(qtree *qtree_obj, qtree_node *crt)
{
    qtree_impl_unlink_node(qtree_obj, crt);
    free(crt);
}


static inline void qtree_impl_unlink_node(qtree *qtree_obj, qtree_node *rm_node)
{
    --(qtree_obj->size);

    if (rm_node->less == NULL && rm_node->greater == NULL)
    {
        // leaf node - removal without replacement
        if (qtree_obj->root == rm_node)
        {
            // root node leaf
            qtree_obj->root = NULL;
        }
        else
        {
            // non-root node leaf
            qtree_node *rot_node = rm_node->parent;
            qtree_dir  dir;
            if (rot_node->less == rm_node)
            {
                // node to remove is in the left subtree
                // of its parent

                // save direction
                dir = QTREE_DIR_LESS;
                rot_node->less = NULL;
            }
            else
            {
                // node to remove is in the right subtree
                // of its parent

                // save direction
                dir = QTREE_DIR_GREATER;
                rot_node->greater = NULL;
            }
            qtree_impl_rebalance_remove(qtree_obj, dir, rot_node);
        }
    }
    else
    {
        qtree_node *rep_node = NULL;
        // not a leaf node, removal by replacement
        // at least one child, or a child and a subtree, or two subtrees
        // find replacement node
        if (rm_node->balance == -1)
        {
            rep_node = rm_node->less;
            while (rep_node->greater != NULL)
            {
                rep_node = rep_node->greater;
            }
        }
        else
        {
            rep_node = rm_node->greater;
            while (rep_node->less != NULL)
            {
                rep_node = rep_node->less;
            }
        }
        qtree_node *rot_node = rep_node->parent;
        qtree_dir  dir;
        if (rot_node->less == rep_node)
        {
            // node to remove is in the left subtree
            // of its parent

            // save direction
            dir = QTREE_DIR_LESS;

            if (rep_node->less != NULL)
            {
                // replace node by its left child
                rot_node->less         = rep_node->less;
                rep_node->less->parent = rot_node;
            }
            else
            if (rep_node->greater != NULL)
            {
                // replace node by its right child
                rot_node->less            = rep_node->greater;
                rep_node->greater->parent = rot_node;
            }
            else
            {
                // non-root leaf node
                rot_node->less = NULL;
            }
        }
        else
        {
            // node to remove is in the right subtree
            // of its parent

            // save direction
            dir = QTREE_DIR_GREATER;

            if (rep_node->less != NULL)
            {
                // replace node by its left child
                rot_node->greater      = rep_node->less;
                rep_node->less->parent = rot_node;
            }
            else
            if (rep_node->greater != NULL)
            {
                // replace node by its right child
                rot_node->greater         = rep_node->greater;
                rep_node->greater->parent = rot_node;
            }
            else
            {
                // non-root leaf node
                rot_node->greater = NULL;
            }
        }

        // replace node contents
        if (rm_node->parent == NULL)
        {
            // Node to be removed is the root node
            qtree_obj->root = rep_node;
        }
        else
        {
            if (rm_node->parent->less == rm_node)
            {
                rm_node->parent->less = rep_node;
            }
            else
            {
                rm_node->parent->greater = rep_node;
            }
        }
        if (rm_node->less != NULL)
        {
            rm_node->less->parent = rep_node;
        }
        if (rm_node->greater != NULL)
        {
            rm_node->greater->parent = rep_node;
        }
        rep_node->parent  = rm_node->parent;
        rep_node->less    = rm_node->less;
        rep_node->greater = rm_node->greater;
        rep_node->balance = rm_node->balance;

        if (rot_node == rm_node)
        {
            rot_node = rep_node;
        }

        qtree_impl_rebalance_remove(qtree_obj, dir, rot_node);
    }
}


static inline qtree_node *qtree_impl_find_node(const qtree *qtree_obj, const void *key)
{
    qtree_node *node = qtree_obj->root;
    while (node != NULL)
    {
        int cmp_rc = qtree_obj->qtree_cmp(key, node->key);
        if (cmp_rc < 0)
        {
            node = node->less;
        }
        else
        if (cmp_rc > 0)
        {
            node = node->greater;
        }
        else
        {
            break;
        }
    }

    return node;
}


static inline void qtree_impl_init(qtree *qtree_obj, const qtree_cmp_func cmp_func_ptr)
{
    qtree_obj->root      = NULL;
    qtree_obj->size      = 0;
    qtree_obj->qtree_cmp = cmp_func_ptr;
}


static inline void qtree_impl_iterator_init(const qtree *qtree_obj, qtree_it *iter)
{
    if (qtree_obj->root != NULL)
    {
        iter->next = qtree_obj->root;
        while (iter->next->less != NULL)
        {
            iter->next = iter->next->less;
        }
    }
    else
    {
        iter->next = NULL;
    }
}


static inline void qtree_impl_clear(qtree *qtree_obj)
{
    if (qtree_obj != NULL)
    {
        qtree_node *node = qtree_obj->root;

        while (node != NULL)
        {
            if (node->less != NULL)
            {
                node = node->less;
            }
            else
            if (node->greater != NULL)
            {
                node = node->greater;
            }
            else
            {
                qtree_node *leaf = node;
                node = node->parent;
                if (node != NULL)
                {
                    if (leaf == node->less)
                    {
                        node->less = NULL;
                    }
                    else
                    {
                        node->greater = NULL;
                    }
                }
                free(leaf);
            }
        }
    }
}
