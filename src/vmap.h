#ifndef VMAP_H
#define	VMAP_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

typedef enum
{
    VMAP_PASS      = 0,
    VMAP_ERR_NOMEM = 1
}
vmap_rc;

typedef int (*vmap_cmp_func)(const void *val_alpha, const void *val_bravo);

typedef struct vmap_s      vmap;
typedef struct vmap_node_s vmap_node;
typedef struct vmap_it_s   vmap_it;

struct vmap_s
{
    vmap_node       *head;
    vmap_node       *tail;
    size_t          size;
    vmap_cmp_func   vmap_cmp;
};

struct vmap_node_s
{
    vmap_node   *next;
    vmap_node   *prev;
    const void  *key;
    const void  *val;
};

struct vmap_it_s
{
    vmap_node   *next;
};

vmap       *vmap_alloc(vmap_cmp_func cmp_func_ptr);
void       vmap_init(vmap *vmap_obj, vmap_cmp_func cmp_func_ptr);
void       vmap_dealloc(vmap *vmap_obj);
void       vmap_clear(vmap *vmap_obj);
vmap_rc    vmap_insert_before(
    vmap        *vmap_obj,
    vmap_node   *node,
    const void  *key,
    const void  *value
);
void       vmap_insert_node_before(
    vmap      *vmap_obj,
    vmap_node *node,
    vmap_node *ins_node
);
void       vmap_prepend_node(vmap *vmap_obj, vmap_node *node);
vmap_rc    vmap_prepend(
    vmap        *vmap_obj,
    const void  *key,
    const void  *value
);
void       vmap_append_node(vmap *vmap_obj, vmap_node *node);
vmap_rc    vmap_append(
    vmap        *vmap_obj,
    const void  *key,
    const void  *value
);
void       vmap_remove(vmap *vmap_obj, const void *key);
void       vmap_remove_node(vmap *vmap_obj, vmap_node *node);
void       vmap_unlink_node(vmap *vmap_obj, vmap_node *node);
void       *vmap_get(const vmap *vmap_obj, const void *key);
vmap_node  *vmap_get_node(const vmap *vmap_obj, const void *key);
vmap_it    *vmap_iterator(const vmap *vmap_obj);
void       vmap_iterator_init(const vmap *vmap_obj, vmap_it *iter);
vmap_node  *vmap_next(vmap_it *iter);

#endif	/* VMAP_H */
