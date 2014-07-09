#ifndef VMAP_H
#define	VMAP_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum
    {
        VMAP_PASS      = 0,
        VMAP_ERR_NOMEM = 1
    }
    vmap_rc;

    typedef int (*vmap_cmp_func)(void *, void *);

    typedef struct vmap_struct
    {
        struct vmap_node_struct  *head;
        struct vmap_node_struct  *tail;
        size_t                   size;
        vmap_cmp_func            vmap_cmp;
    }
    vmap;

    typedef struct vmap_node_struct
    {
        struct vmap_node_struct  *next;
        struct vmap_node_struct  *prev;
        void*                    key;
        void*                    val;
    }
    vmap_node;

    typedef struct vmap_it_struct
    {
        struct vmap_node_struct* next;
    }
    vmap_it;

    vmap       *vmap_alloc(int (*)(void *, void *));
    void       vmap_init(vmap *, int (*)(void *, void *));
    void       vmap_dealloc(vmap *);
    void       vmap_clear(vmap *);
    vmap_rc    vmap_insertbeforenode(vmap *, vmap_node *, void *, void *);
    void       vmap_insertnodebeforenode(vmap *, vmap_node *, vmap_node *);
    void       vmap_prependnode(vmap *, vmap_node *);
    vmap_rc    vmap_prepend(vmap *, void *, void *);
    void       vmap_appendnode(vmap *, vmap_node *);
    vmap_rc    vmap_append(vmap *, void *, void *);
    void       vmap_remove(vmap *, void *);
    void       vmap_removenode(vmap *, vmap_node *);
    void       vmap_unlinknode(vmap *, vmap_node *);
    void       *vmap_get(vmap *, void *);
    vmap_node  *vmap_getnode(vmap *, void *);
    vmap_it    *vmap_iterator(vmap *);
    void       vmap_iteratorinit(vmap *, vmap_it *);
    vmap_node  *vmap_next(vmap_it *);

#ifdef	__cplusplus
}
#endif

#endif	/* VMAP_H */

