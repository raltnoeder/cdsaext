/**
 * Quick balanced binary search tree
 *
 * @version 2014-04-16_001
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
#include "qtree.h"

static inline qtree_node *_qtree_findnode(qtree *, void *);
static inline int        _qtree_insertnode(qtree *, qtree_node *);
static inline void       _qtree_removenode(qtree *, qtree_node *);
static inline qtree_node *_qtree_unlinknode(qtree *, qtree_node *);
static inline void       _qtree_insblnc(qtree *, qtree_node *, qtree_node *);
static inline void       _qtree_rmblnc(qtree *, int, qtree_node *);
static inline void       _qtree_init(qtree *, int (*)(void *, void *));
static inline void       _qtree_iteratorinit(qtree *, qtree_it *);
static inline void       _qtree_clear(qtree *);

qtree *qtree_alloc(int (*qtree_cmp_func)(void *, void *))
{
    qtree *qtree_obj;

    qtree_obj = malloc(sizeof(qtree));
    if (qtree_obj != NULL)
    {
        _qtree_init(qtree_obj, qtree_cmp_func);
    }

    return qtree_obj;
}

void qtree_dealloc(qtree *qtree_obj)
{
    _qtree_clear(qtree_obj);
    free(qtree_obj);
}

void qtree_clear(qtree *qtree_obj)
{
    _qtree_clear(qtree_obj);
    qtree_obj->size = 0;
    qtree_obj->root = NULL;
}

void qtree_init(qtree *qtree_obj, int (*qtree_cmp_func)(void *, void *))
{
    _qtree_init(qtree_obj, qtree_cmp_func);
}

int qtree_insert(qtree *qtree_obj, void *key, void *val)
{
    qtree_node *nx;
    int        rc;

    nx = malloc(sizeof(qtree_node));
    if (nx != NULL)
    {
        nx->blnc = 0;
        nx->key  = key;
        nx->val  = val;

        rc = _qtree_insertnode(qtree_obj, nx);
        if (rc == QTREE_ERR_EXISTS)
        {
            free(nx);
        }
    } else {
        rc = QTREE_ERR_NOMEM;
    }

    return rc;
}

int qtree_insertnode(qtree *qtree_obj, qtree_node *nx)
{
    return _qtree_insertnode(qtree_obj, nx);
}

void qtree_remove(qtree *qtree_obj, void *key)
{
    qtree_node *crt;

    crt = _qtree_findnode(qtree_obj, key);
    if (crt != NULL)
    {
        _qtree_removenode(qtree_obj, crt);
    }
}

void qtree_removenode(qtree *qtree_obj, qtree_node *crt)
{
    _qtree_removenode(qtree_obj, crt);
}

qtree_node *qtree_unlinknode(qtree *qtree_obj, qtree_node *crt)
{
    return _qtree_unlinknode(qtree_obj, crt);
}

void* qtree_get(qtree *qtree_obj, void *key)
{
    qtree_node *nx;

    nx = _qtree_findnode(qtree_obj, key);
    if (nx != NULL)
    {
        return nx->val;
    }

    return NULL;
}

qtree_node *qtree_getnode(qtree *qtree_obj, void *key)
{
    return _qtree_findnode(qtree_obj, key);
}

size_t qtree_get_size(qtree *qtree_obj)
{
    return qtree_obj->size;
}

qtree_it *qtree_iterator(qtree *qtree_obj)
{
    qtree_it *it;

    it = malloc(sizeof(qtree_it));
    if (it != NULL)
    {
        _qtree_iteratorinit(qtree_obj, it);
    }

    return it;
}

void qtree_iteratorinit(qtree *qtree_obj, qtree_it *it)
{
    _qtree_iteratorinit(qtree_obj, it);
}

qtree_node *qtree_next(qtree_it *qTreeItObj)
{
    if (qTreeItObj->c != NULL)
    {
        /* save current */
        qTreeItObj->r = qTreeItObj->c;

        /* update current */
        for (; qTreeItObj->c != NULL;)
        {
            if (qTreeItObj->state == QTREE_STATE_ENTER_L)
            {
                /* traversing down */
                /* find lowest-value node */
                for (; qTreeItObj->c->l != NULL; qTreeItObj->c = qTreeItObj->c->l) { }
                qTreeItObj->state = QTREE_STATE_ENTER_H;
                /* select current */
                break;
            } else
            if (qTreeItObj->state == QTREE_STATE_ENTER_H)
            {
                /* traversing down */
                if (qTreeItObj->c->r != NULL)
                {
                    qTreeItObj->c = qTreeItObj->c->r;
                    qTreeItObj->state = QTREE_STATE_ENTER_L;
                } else {
                    /* leaf node */
                    qTreeItObj->state = QTREE_STATE_LEAVE;
                }
            } else {
                /* traversing up (STATE_LEAVE) */
                qTreeItObj->p = qTreeItObj->c;
                qTreeItObj->c = qTreeItObj->c->p;
                if (qTreeItObj->c == NULL)
                {
                    break;
                }
                if (qTreeItObj->c->l == qTreeItObj->p)
                {
                    /* traversing up from lower-value node */
                    qTreeItObj->state = QTREE_STATE_ENTER_H;
                    /* select current */
                    break;
                } else {
                    /* traversing up from higher-value node */
                    continue;
                }
            }
        }

        return qTreeItObj->r;
    }

    return NULL;
}

static inline void _qtree_rmblnc(qtree *qtree_obj, int dir, qtree_node *r)
{
    qtree_node *n;

    /* update balance and perform rotations */
    for (; r != NULL; r = r->p)
    {
        if (dir < 0)
        {
            /* node was removed from left subtree */
            if (++(r->blnc) == 1)
            {
                break;
            }
        } else {
            /* node was removed from right subtree */
            if (--(r->blnc) == -1)
            {
                break;
            }
        }

        if (r->p != NULL)
        {
            if (r->p->l == r)
            {
                dir = -1;
            } else {
                dir = 1;
            }
        }

        /* update balance and perform rotations */
        if (r->blnc == -2)
        {
            n = r->l;
            /* 0 or -1 */
            if (n->blnc <= 0)
            {
                /* rotate R */
                n->p = r->p;
                if (r->p != NULL)
                {
                    if (r->p->l == r)
                    {
                        r->p->l = n;
                    } else {
                        r->p->r = n;
                    }
                } else {
                    qtree_obj->root = n;
                }

                r->l = n->r;
                if (n->r != NULL)
                {
                    n->r->p = r;
                }

                n->r = r;
                r->p = n;

                if (n->blnc == 0)
                {
                    r->blnc = -1;
                    n->blnc = 1;
                    break;
                } else {
                    r->blnc = 0;
                    n->blnc = 0;
                }
            } else {
                /* rotate LR */
                if (n->r->blnc == -1)
                {
                    n->blnc = 0;
                    r->blnc = 1;
                } else
                if (n->r->blnc == 1)
                {
                    n->blnc = -1;
                    r->blnc = 0;
                } else {
                    n->blnc = 0;
                    r->blnc = 0;
                }
                n->r->blnc = 0;

                n->p    = n->r;
                n->r    = n->r->l;
                n->p->l = n;
                r->l    = n->p->r;
                n->p->p = r->p;
                if (n->r != NULL)
                {
                    n->r->p = n;
                }
                if (r->l != NULL)
                {
                    r->l->p = r;
                }

                if (r->p != NULL)
                {
                    if (r->p->l == r)
                    {
                        r->p->l = n->p;
                    } else {
                        r->p->r = n->p;
                    }
                } else {
                    qtree_obj->root = n->p;
                }

                r->p    = n->p;
                n->p->r = r;
            }
            r = r->p;
            /* end of R / LR rotations */
        } else
        if (r->blnc == 2)
        {
            n = r->r;
            /* 0 or 1 */
            if (n->blnc >= 0)
            {
                /* rotate L */
                n->p = r->p;
                if (r->p != NULL)
                {
                    if (r->p->l == r)
                    {
                        r->p->l = n;
                    } else {
                        r->p->r = n;
                    }
                } else {
                    qtree_obj->root = n;
                }

                r->r = n->l;
                if (n->l != NULL)
                {
                    n->l->p = r;
                }

                n->l = r;
                r->p = n;
                if (n->blnc == 0)
                {
                    r->blnc = 1;
                    n->blnc = -1;
                    break;
                } else {
                    r->blnc = 0;
                    n->blnc = 0;
                }
            } else {
                /* rotate RL */
                if (n->l->blnc == -1)
                {
                    n->blnc = 1;
                    r->blnc = 0;
                } else
                if (n->l->blnc == 1)
                {
                    n->blnc = 0;
                    r->blnc = -1;
                } else {
                    n->blnc = 0;
                    r->blnc = 0;
                }
                n->l->blnc = 0;

                n->p    = n->l;
                n->l    = n->l->r;
                n->p->r = n;
                r->r    = n->p->l;
                n->p->p = r->p;
                if (n->l != NULL)
                {
                    n->l->p = n;
                }
                if (r->r != NULL)
                {
                    r->r->p = r;
                }

                if (r->p != NULL)
                {
                    if (r->p->l == r)
                    {
                        r->p->l = n->p;
                    } else {
                        r->p->r = n->p;
                    }
                } else {
                    qtree_obj->root = n->p;
                }

                r->p    = n->p;
                n->p->l = r;
            }
            r = r->p;
            /* end of L / RL rotations */
        }
    } /* end update */
}

static inline void _qtree_insblnc(qtree *qtree_obj, qtree_node *nx, qtree_node *ny)
{
    /* update balance and perform rotations */
    do
    {
        if (ny->l == nx)
        {
            --(ny->blnc);
        } else {
            ++(ny->blnc);
        }

        if (ny->blnc == 0)
        {
            break;
        } else
        if (ny->blnc == -2)
        {
            if (nx->blnc == -1)
            {
                /* rotate R */
                ny->blnc = 0;
                nx->blnc = 0;

                nx->p = ny->p;
                if (ny->p != NULL)
                {
                    if (ny->p->l == ny)
                    {
                        ny->p->l = nx;
                    } else {
                        ny->p->r = nx;
                    }
                } else {
                    qtree_obj->root = nx;
                }

                ny->l = nx->r;
                if (nx->r != NULL)
                {
                    nx->r->p = ny;
                }

                nx->r = ny;
                ny->p = nx;
            } else {
                /* rotate LR */
                if (nx->r->blnc == -1)
                {
                    nx->blnc = 0;
                    ny->blnc = 1;
                } else
                if (nx->r->blnc == 1)
                {
                    nx->blnc = -1;
                    ny->blnc = 0;
                } else {
                    nx->blnc = 0;
                    ny->blnc = 0;
                }
                nx->r->blnc = 0;

                nx->p = nx->r;
                nx->r = nx->r->l;
                nx->p->l = nx;
                ny->l = nx->p->r;
                nx->p->p = ny->p;
                if (nx->r != NULL)
                {
                    nx->r->p = nx;
                }
                if (ny->l != NULL)
                {
                    ny->l->p = ny;
                }

                if (ny->p != NULL)
                {
                    if (ny->p->l == ny)
                    {
                        ny->p->l = nx->p;
                    } else {
                        ny->p->r = nx->p;
                    }
                } else {
                    qtree_obj->root = nx->p;
                }

                ny->p = nx->p;
                nx->p->r = ny;
            }
            break;
        } else
        if (ny->blnc == 2)
        {
            if (nx->blnc == 1)
            {
                /* rotate L */
                ny->blnc = 0;
                nx->blnc = 0;

                nx->p = ny->p;
                if (ny->p != NULL)
                {
                    if (ny->p->l == ny)
                    {
                        ny->p->l = nx;
                    } else {
                        ny->p->r = nx;
                    }
                } else {
                    qtree_obj->root = nx;
                }

                ny->r = nx->l;
                if (nx->l != NULL)
                {
                    nx->l->p = ny;
                }

                nx->l = ny;
                ny->p = nx;
            } else {
                /* rotate RL */
                if (nx->l->blnc == -1)
                {
                    nx->blnc = 1;
                    ny->blnc = 0;
                } else
                if (nx->l->blnc == 1)
                {
                    nx->blnc = 0;
                    ny->blnc = -1;
                } else {
                    nx->blnc = 0;
                    ny->blnc = 0;
                }
                nx->l->blnc = 0;

                nx->p = nx->l;
                nx->l = nx->l->r;
                nx->p->r = nx;
                ny->r = nx->p->l;
                nx->p->p = ny->p;
                if (nx->l != NULL)
                {
                    nx->l->p = nx;
                }
                if (ny->r != NULL)
                {
                    ny->r->p = ny;
                }

                if (ny->p != NULL)
                {
                    if (ny->p->l == ny)
                    {
                        ny->p->l = nx->p;
                    } else {
                        ny->p->r = nx->p;
                    }
                } else {
                    qtree_obj->root = nx->p;
                }

                ny->p = nx->p;
                nx->p->l = ny;
            }
            break;
        }

        nx   = ny;
        ny = ny->p;
    } while (ny != NULL);
}

static inline int _qtree_insertnode(qtree *qtree_obj, qtree_node *nx)
{
    qtree_node *ny;
    int        rc;

    if (qtree_obj->root == NULL)
    {
        qtree_obj->root = nx;
        ++(qtree_obj->size);
        nx->l = NULL;
        nx->r = NULL;
        nx->p = NULL;
    } else {
        ny = qtree_obj->root;
        for (;;)
        {
            rc = qtree_obj->qtree_cmp(nx->key, ny->key);
            if (rc < 0)
            {
                if (ny->l == NULL)
                {
                    ny->l = nx;
                    nx->p = ny;
                    ++(qtree_obj->size);
                    break;
                } else {
                    ny = ny->l;
                }
            } else
            if (rc > 0)
            {
                if (ny->r == NULL)
                {
                    ny->r = nx;
                    nx->p = ny;
                    ++(qtree_obj->size);
                    break;
                } else {
                    ny = ny->r;
                }
            } else {
                return QTREE_ERR_EXISTS;
            }
        }
        _qtree_insblnc(qtree_obj, nx, ny);
    }

    return QTREE_PASS;
}

static inline void _qtree_removenode(qtree *qtree_obj, qtree_node *crt)
{
    free(_qtree_unlinknode(qtree_obj, crt));
}

static inline qtree_node *_qtree_unlinknode(qtree *qtree_obj, qtree_node *crt)
{
    qtree_node *n;
    qtree_node *r;
    int        dir;

    --(qtree_obj->size);

    if (crt->l == NULL && crt->r == NULL)
    {
        /* leaf node - removal without replacement */
        n = crt;
        if (qtree_obj->root == crt)
        {
            /* root node leaf */
            qtree_obj->root = NULL;
        } else {
            /* non-root node leaf */
            r = n->p;
            if (r->l == n)
            {
                /* node to remove is in the left subtree *
                 * of its parent                         */

                /* save direction */
                dir = -1;
                r->l = NULL;
            } else {
                /* node to remove is in the right subtree *
                 * of its parent                          */

                /* save direction */
                dir = 1;
                r->r = NULL;
            }
            _qtree_rmblnc(qtree_obj, dir, r);
        }
    } else {
        /* not a leaf node, removal by replacement                       *
         * at least one child, or a child and a subtree, or two subtrees *
         * find replacement node                                         */
        if (crt->blnc == -1)
        {
            for (n = crt->l; n->r != NULL; n = n->r) { }
        } else {
            for (n = crt->r; n->l != NULL; n = n->l) { }
        }
        r = n->p;
        if (r->l == n)
        {
            /* node to remove is in the left subtree *
             * of its parent                         */

            /* save direction */
            dir = -1;

            if (n->l != NULL)
            {
                /* replace node by its left child */
                r->l = n->l;
                n->l->p = r;
            } else
            if (n->r != NULL)
            {
                /* replace node by its right child */
                r->l = n->r;
                n->r->p = r;
            } else {
                /* non-root leaf node */
                r->l = NULL;
            }
        } else {
            /* node to remove is in the right subtree *
             * of its parent                          */

            /* save direction */
            dir = 1;

            if (n->l != NULL)
            {
                /* replace node by its left child */
                r->r = n->l;
                n->l->p = r;
            } else
            if (n->r != NULL)
            {
                /* replace node by its right child */
                r->r = n->r;
                n->r->p = r;
            } else {
                /* non-root leaf node */
                r->r = NULL;
            }
        }

        /* replace node contents */
        crt->key = n->key;
        crt->val = n->val;

        _qtree_rmblnc(qtree_obj, dir, r);
    }

    return n;
}

static inline qtree_node *_qtree_findnode(qtree *qtree_obj, void *key)
{
    qtree_node *nx;
    int rc;

    nx = qtree_obj->root;
    while (nx != NULL)
    {
        rc = qtree_obj->qtree_cmp(key, nx->key);
        if (rc < 0)
        {
            nx = nx->l;
        } else
        if (rc > 0)
        {
            nx = nx->r;
        } else {
            return nx;
        }
    }

    return NULL;
}

static inline void _qtree_init(qtree *qtree_obj, int (*qtree_cmp_func)(void *, void *))
{
    qtree_obj->root = NULL;
    qtree_obj->size = 0;
    qtree_obj->qtree_cmp = (int (*)(void *, void *)) qtree_cmp_func;
}

static inline void _qtree_iteratorinit(qtree *qtree_obj, qtree_it *it)
{
    if (qtree_obj->root != NULL)
    {
        for (it->c = qtree_obj->root; it->c->l != NULL; it->c = it->c->l) { }
    } else {
        it->c = NULL;
    }
    it->p = NULL;
    it->r = NULL;
    it->state = QTREE_STATE_ENTER_H;
}

static inline void _qtree_clear(qtree *qtree_obj)
{
    if (qtree_obj != NULL)
    {
        qtree_node *node;
        qtree_node *leaf;

        node = qtree_obj->root;

        while (node != NULL)
        {
            if (node->l != NULL)
            {
                node = node->l;
            } else
            if (node->r != NULL)
            {
                node = node->r;
            } else {
                leaf = node;
                node = node->p;
                if (node != NULL)
                {
                    if (leaf == node->l)
                    {
                        node->l = NULL;
                    } else {
                        node->r = NULL;
                    }
                }
                free(leaf);
            }
        }
    }
}