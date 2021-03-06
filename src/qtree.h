#ifndef QTREE_H
#define	QTREE_H

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
    QTREE_PASS       = 0,
    QTREE_ERR_NOMEM  = 1,
    QTREE_ERR_EXISTS = 2
}
qtree_rc;

typedef int (*qtree_cmp_func)(const void *val_alpha, const void *val_bravo);

typedef struct qtree_s      qtree;
typedef struct qtree_node_s qtree_node;
typedef struct qtree_it_s   qtree_it;

struct qtree_s
{
    qtree_node      *root;
    size_t          size;
    qtree_cmp_func  qtree_cmp;
};

struct qtree_node_s
{
    const void  *key;
    const void  *value;
    qtree_node  *less;
    qtree_node  *greater;
    qtree_node  *parent;
    int         balance;
};

struct qtree_it_s
{
    qtree_node  *next;
};

void        qtree_dealloc(qtree *qtree_obj);
void        qtree_clear(qtree *qtree_obj);
qtree       *qtree_alloc(qtree_cmp_func cmp_func_ptr);
void        qtree_init(qtree *qtree_obj, qtree_cmp_func cmp_func_ptr);
qtree_rc    qtree_insert(
    qtree       *qtree_obj,
    const void  *key,
    const void  *value
);
qtree_rc    qtree_insert_node(qtree *qtree_obj, qtree_node *node);
void        qtree_remove(qtree *qtree_obj, const void *key);
void        qtree_remove_node(qtree *qtree_obj, qtree_node *node);
void        qtree_unlink_node(qtree *qtree_obj, qtree_node *node);
void        *qtree_get(const qtree *qtree_obj, const void *key);
qtree_node  *qtree_get_node(const qtree *qtree_obj, const void *key);
qtree_it    *qtree_iterator(const qtree *qtree_obj);
void        qtree_iterator_init(const qtree *qtree_obj, qtree_it *iter);
qtree_node  *qtree_next(qtree_it *iter);
size_t      qtree_get_size(const qtree *qtree_obj);

#endif	/* QTREE_H */
