#ifndef QTREE_H
#define	QTREE_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum
    {
        QTREE_PASS       = 0,
        QTREE_ERR_NOMEM  = 1,
        QTREE_ERR_EXISTS = 2
    }
    qtree_rc;

    typedef int (*qtree_cmp_func)(void *, void *);

    typedef struct qtree_struct
    {
        struct   qtreenode_struct  *root;
        size_t                     size;
        qtree_cmp_func             qtree_cmp;
    }
    qtree;

    typedef struct qtreenode_struct
    {
        void                     *key;
        void                     *val;
        struct qtreenode_struct  *less;
        struct qtreenode_struct  *greater;
        struct qtreenode_struct  *parent;
        int                      balance;
    }
    qtree_node;

    typedef struct qtree_it_struct
    {
        struct qtreenode_struct  *next;
    }
    qtree_it;

    void        qtree_dealloc(qtree *);
    void        qtree_clear(qtree *);
    qtree       *qtree_alloc(int (*)(void *, void *));
    void        qtree_init(qtree *, int (*)(void *, void *));
    qtree_rc    qtree_insert(qtree *, void *, void *);
    qtree_rc    qtree_insertnode(qtree *, qtree_node *);
    void        qtree_remove(qtree *, void *);
    void        qtree_removenode(qtree *, qtree_node *);
    qtree_node  *qtree_unlinknode(qtree *, qtree_node *);
    void        *qtree_get(qtree *, void *);
    qtree_node  *qtree_getnode(qtree *, void *);
    qtree_it    *qtree_iterator(qtree *);
    void        qtree_iteratorinit(qtree *, qtree_it *);
    qtree_node  *qtree_next(qtree_it *);
    size_t      qtree_get_size(qtree *);

#ifdef	__cplusplus
}
#endif

#endif	/* QTREE_H */
