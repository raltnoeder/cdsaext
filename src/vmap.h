#ifndef VMAP_H
#define	VMAP_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define VMAP_ERR_NOMEM 1
#define VMAP_PASS      0

    typedef struct vmap_struct {
        struct vmap_node_struct  *head;
        struct vmap_node_struct  *tail;
        size_t                   size;
        int                      (*vmapCompare)(void *, void *);
    } vmap;

    typedef struct vmap_node_struct {
        struct vmap_node_struct  *next;
        struct vmap_node_struct  *prev;
        void*                    key;
        void*                    val;
    } vmap_node;

    typedef struct vmap_it_struct
    {
        struct vmap_node_struct* next;
    } vmap_it;

    vmap       *vmap_alloc(int (*)(void *, void *));
    void       vmap_dealloc(vmap *);
    int        vmap_insertbeforenode(vmap *, vmap_node *, void *, void *);
    int        vmap_prepend(vmap *, void *, void *);
    int        vmap_append(vmap *, void *, void *);
    void       vmap_remove(vmap *, void *);
    void       vmap_removenode(vmap *, vmap_node *);
    void       *vmap_get(vmap *, void *);
    vmap_node  *vmap_getnode(vmap *, void *);
    vmap_it    *vmap_iterator(vmap *);
    vmap_node  *vmap_next(vmap_it *);

    inline vmap_node  *_vmap_findnode(vmap *, void *);
    inline void       _vmap_removenode(vmap *, vmap_node *);

#ifdef	__cplusplus
}
#endif

#endif	/* VMAP_H */

