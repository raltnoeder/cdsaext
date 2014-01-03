#ifndef QTREE_H
#define	QTREE_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
#define QTREE_ERR_NOMEM 1
#define QTREE_PASS      0
    
#define QTREE_STATE_ENTER_L 0
#define QTREE_STATE_ENTER_H 1
#define QTREE_STATE_LEAVE   2
    
    typedef struct qtree_struct {
        struct   qtreenode_struct  *root;
        size_t                     size;
        int                        (*qtree_cmp)(void*, void*);
    } qtree;
    
    typedef struct qtreenode_struct {
        void                     *key;
        void                     *val;
        struct qtreenode_struct  *l;
        struct qtreenode_struct  *r;
        struct qtreenode_struct  *p;
        int                      blnc;
    } qtree_node;
    
    typedef struct qTreeIt_struct {
        struct qtreenode_struct  *c;
        struct qtreenode_struct  *p;
        struct qtreenode_struct  *r;
        int                      state;
    } qtree_it;

    qtree       *qtree_alloc(int (*)(void *, void *));
    void        qtree_dealloc(qtree *);
    int         qtree_insert(qtree *, void *, void *);
    void        qtree_remove(qtree *, void *);
    void        qtree_removenode(qtree *, qtree_node *);
    void        *qtree_get(qtree *, void *);
    qtree_node  *qtree_getnode(qtree *, void *);
    qtree_it    *qtree_iterator(qtree *);
    qtree_node  *qtree_next(qtree_it *);
    size_t      qtree_get_size(qtree *);

    inline qtree_node  *_qtree_findnode(qtree *, void *);
    inline void        _qtree_removenode(qtree *, qtree_node *);
    inline void        _qtree_insblnc(qtree *, qtree_node *, qtree_node *);
    inline void        _qtree_rmblnc(qtree *, int, qtree_node *, qtree_node *);

#ifdef	__cplusplus
}
#endif

#endif	/* QTREE_H */
