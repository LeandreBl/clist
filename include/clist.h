#ifndef CLIB_H_
#define CLIB_H_

#include <stddef.h>
#include <stdbool.h>

struct clist_node_internal {
	struct clist_node_internal *prev;
	struct clist_node_internal *next;
	void (*destructor)(void *);
};

#define clist(type)                                                            \
	struct {                                                               \
		struct clist_node_internal base;                               \
		type object;                                                   \
	}

#define clist_init(list_ptr, destructor_fn)                                    \
	clist_generic_init(list_ptr, destructor_fn)

#define clist_next(list_ptr) ((typeof(list_ptr))(list_ptr)->base.next)

#define clist_prev(list_ptr) ((typeof(list_ptr))(list_ptr)->base.prev)

#define clist_next_object(list_ptr)                                            \
	(((typeof(list_ptr))(list_ptr)->base.next)->object)

#define clist_prev_object(list_ptr)                                            \
	(((typeof(list_ptr))(list_ptr)->base.prev)->object)

#define clist_from_object(object_addr)                                         \
	((clist(typeof(                                                        \
		*object_addr)) *)((char *)object_addr                          \
				  - offsetof(clist(typeof(*object_addr)),      \
					     object)))

#define clist_assign(dest_ptr, src_ptr) (dest_ptr = (void *)src_ptr)

#define clist_assign_from_object(list_ptr, object_addr)                        \
	clist_assign(list_ptr, clist_from_object(object_addr))

#define clist_front(list_ptr) (list_ptr->object)

#define clist_back(list_ptr) (clist_prev_object(list_ptr))

#define clist_new_node(list_ptr, item, destructor_fn)                          \
	do {                                                                   \
		(list_ptr) = malloc(sizeof(*(list_ptr)));                      \
		if ((list_ptr) == NULL)                                        \
			break;                                                 \
		clist_init(list_ptr, destructor_fn);                           \
		(list_ptr)->object = item;                                     \
	} while (0);

#define clist_foreach(item, list_ptr)                                          \
	for (typeof((list_ptr)->object) *item = &(list_ptr)->object;           \
	     list_ptr != NULL && item != NULL;                                 \
	     item = (((clist(typeof((list_ptr)->object)) *)clist_from_object(  \
			      item))                                           \
			     ->base.next                                       \
		     == (void *)(list_ptr))                                    \
			    ? NULL                                             \
			    : &(clist_next_object(clist_from_object(item))))

#define clist_backward_foreach(list_ptr)                                       \
	for (typeof((list_ptr)->object) *item = &(list_ptr)->object;           \
	     item != NULL;                                                     \
	     item = (((clist(typeof((list_ptr)->object)) *)clist_from_object(  \
			      item))                                           \
			     ->base.prev                                       \
		     == (list_ptr))                                            \
			    ? NULL                                             \
			    : &(clist_prev_object(clist_from_object(item))))

#define clist_push_back(list_ptr, item, destructor_fn)                         \
	clist_push_before(list_ptr, item, destructor_fn)

#define clist_emplace_back(list_ptr, destructor_fn, constructor_fn, ...)       \
	do {                                                                   \
		typeof(list_ptr) __i__ = malloc(sizeof(*__i__));               \
		if (__i__ == NULL)                                             \
			break;                                                 \
		if (constructor_fn(&__i__->object, ##__VA_ARGS__) < 0) {       \
			free(__i__);                                           \
			break;                                                 \
		}                                                              \
		if ((list_ptr) == NULL) {                                      \
			clist_init(__i__, destructor_fn);                      \
			clist_assign(list_ptr, __i__);                         \
			break;                                                 \
		}                                                              \
		clist_generic_emplace_back(list_ptr, __i__, destructor_fn);    \
	} while (0)

#define clist_push_front(list_ptr, item, destructor_fn)                        \
	do {                                                                   \
		clist_push_before(list_ptr, item, destructor_fn);              \
		clist_assign(list_ptr, (list_ptr)->base.prev);                 \
	} while (0)

#define clist_emplace_front(list_ptr, destructor_fn, constructor_fn, ...)      \
	do {                                                                   \
		typeof(list_ptr) __i__ = malloc(sizeof(*__i__));               \
		if (__i__ == NULL)                                             \
			break;                                                 \
		if (constructor_fn(&__i__->object, ##__VA_ARGS__) < 0) {       \
			free(__i__);                                           \
			break;                                                 \
		}                                                              \
		if ((list_ptr) == NULL) {                                      \
			clist_init(__i__, destructor_fn);                      \
			clist_assign(list_ptr, __i__);                         \
			break;                                                 \
		}                                                              \
		clist_generic_emplace_front(list_ptr, __i__, destructor_fn);   \
		clist_assign(list_ptr, __i__);                                 \
	} while (0)

#define clist_destroy(list_ptr)                                                \
	do {                                                                   \
		clist_generic_destroy(list_ptr);                               \
		list_ptr = NULL;                                               \
	} while (0)

#define clist_clear(list_ptr) clist_destroy(list_ptr)

#define clist_empty(list_ptr) clist_generic_empty(list_ptr)

#define clist_size(list_ptr) clist_generic_size(list_ptr)

#define clist_length(list_ptr) clist_size(list_ptr)

#define clist_push_after(list_ptr, item, destructor_fn)                        \
	do {                                                                   \
		typeof(list_ptr) __i__;                                        \
		clist_new_node(__i__, item, destructor_fn);                    \
		if (list_ptr == NULL) {                                        \
			clist_assign(list_ptr, __i__);                         \
			break;                                                 \
		}                                                              \
		clist_generic_push_after(list_ptr, __i__);                     \
	} while (0)

#define clist_emplace_after(list_ptr, destructor_fn, constructor_fn, ...)      \
	do {                                                                   \
		typeof(list_ptr) __i__ = malloc(sizeof(*(list_ptr)));          \
		if (__i__ == NULL)                                             \
			break;                                                 \
		if (constructor_fn(&__i__->object, ##__VA_ARGS__) < 0) {       \
			free(__i__);                                           \
			break;                                                 \
		}                                                              \
		if ((list_ptr) == NULL) {                                      \
			clist_init(__i__, destructor_fn);                      \
			clist_assign(list_ptr, __i__);                         \
			break;                                                 \
		}                                                              \
		clist_generic_emplace_after(list_ptr, __i__, destructor_fn);   \
	} while (0)

#define clist_push_before(list_ptr, item, destructor_fn)                       \
	do {                                                                   \
		typeof(list_ptr) __i__;                                        \
		clist_new_node(__i__, item, destructor_fn);                    \
		if ((list_ptr) == NULL) {                                      \
			clist_assign(list_ptr, __i__);                         \
			break;                                                 \
		}                                                              \
		clist_generic_push_before(list_ptr, __i__);                    \
	} while (0)

#define clist_emplace_before(list_ptr, destructor_fn, constructor_fn, ...)     \
	do {                                                                   \
		typeof(list_ptr) __i__ = malloc(sizeof(*(list_ptr)));          \
		if (__i__ == NULL)                                             \
			break;                                                 \
		if (constructor_fn(&__i__->object, ##__VA_ARGS__) < 0) {       \
			free(__i__);                                           \
			break;                                                 \
		}                                                              \
		if ((list_ptr) == NULL) {                                      \
			clist_init(__i__, destructor_fn);                      \
			clist_assign(list_ptr, __i__);                         \
			break;                                                 \
		}                                                              \
		clist_generic_emplace_before(list_ptr, __i__, destructor_fn);  \
	} while (0)

#define clist_swap(list_ptr1, list_ptr2)                                       \
	do {                                                                   \
		clist_generic_swap(list_ptr1, list_ptr2);                      \
		void *tmp = list_ptr1;                                         \
		clist_assign(list_ptr1, list_ptr2);                            \
		clist_assign(list_ptr2, tmp);                                  \
	} while (0)

#define clist_remove(list_ptr, node)                                           \
	do {                                                                   \
		if (node == (void *)list_ptr) {                                \
			if (clist_next(node) == node)                          \
				clist_assign(list_ptr, NULL);                  \
			else                                                   \
				clist_assign(list_ptr, clist_next(node));      \
		}                                                              \
		clist_generic_remove(node);                                    \
		clist_assign((node)->base.prev, node);                         \
		clist_assign((node)->base.next, node);                         \
	} while (0)

#define clist_erase(list_ptr, node)                                            \
	do {                                                                   \
		if (node == list_ptr) {                                        \
			if (node->next == node)                                \
				clist_assign(list_ptr, NULL);                  \
			else                                                   \
				clist_assign(list_ptr, node->next);            \
		}                                                              \
		clis_generic_erase(node);                                      \
	} while (0)

#define clist_move_after(node, dest_node)                                      \
	do {                                                                   \
		bool null = clist_next(node) == node;                          \
		if (dest_node)                                                 \
			clist_generic_move_after(node, dest_node);             \
		else {                                                         \
			clist_generic_remove(node);                            \
			clist_assign(dest_node, node);                         \
		}                                                              \
		if (null)                                                      \
			node = NULL;                                           \
	} while (0)

#define clist_move_before(node, dest_node)                                     \
	do {                                                                   \
		bool null = clist_next(node) == node;                          \
		if (dest_node)                                                 \
			clist_generic_move_before(node, dest_node);            \
		else {                                                         \
			clist_generic_remove(node);                            \
			clist_assign(dest_node, node);                         \
		}                                                              \
		if (null)                                                      \
			node = NULL;                                           \
	} while (0)

#define clist_move_back(node, list)                                            \
	do {                                                                   \
		bool null = clist_next(node) == node;                          \
		if (list)                                                      \
			clist_generic_move_before(node, list);                 \
		else {                                                         \
			clist_generic_remove(node);                            \
			clist_assign(list, node);                              \
		}                                                              \
		if (null)                                                      \
			node = NULL;                                           \
	} while (0)

#define clist_move_front(node, list)                                           \
	do {                                                                   \
		bool null = clist_next(node) == node;                          \
		if (list)                                                      \
			clist_generic_move_front(node, list);                  \
		else {                                                         \
			clist_generic_remove(node);                            \
			clist_assign(list, node);                              \
		}                                                              \
		if (null)                                                      \
			node = NULL;                                           \
		clist_assign(list, node);                                      \
	} while (0)

/* This should not be use as it */
size_t clist_generic_size(void *list);
void clist_generic_swap(void *list1, void *list2);
bool clist_generic_empty(void *list);
void clist_generic_destroy(void *list);
void clist_generic_erase(void *list);
void clist_generic_remove(void *list);
void clist_generic_init(void *list, void (*destructor_p)());
void clist_generic_emplace_back(void *plist, void *pnode,
				void (*destructor_p)());
void clist_generic_emplace_front(void *plist, void *pnode,
				 void (*destructor_p)());
void clist_generic_emplace_before(void *plist, void *pnode,
				  void (*pdestructor)());
void clist_generic_emplace_after(void *plist, void *pnode,
				 void (*pdestructor)());
void clist_generic_push_after(void *plist, void *pnode);
void clist_generic_push_before(void *plist, void *pnode);
void clist_generic_move_after(void *pnode, void *pdest);
void clist_generic_move_before(void *pnode, void *pdest);

#endif
