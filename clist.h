#ifndef CLIB_H_
#define CLIB_H_

#include <stdio.h>
#include <stddef.h>

struct clist_node_internal
{
	void *prev;
	void *next;
	void (*destructor)(void *);
};

#define clist(type)                                \
	struct                                     \
	{                                          \
		void *prev;                        \
		void *next;                        \
		void (*destructor)(type * object); \
		type object;                       \
	}

#define clist_init(list_ptr, destructor_fn) ((list_ptr)->next = (list_ptr), (list_ptr)->prev = (list_ptr), (list_ptr)->destructor = destructor_fn)

#define clist_next(list_ptr) ((typeof(list_ptr))(list_ptr)->next)

#define clist_prev(list_ptr) ((typeof(list_ptr))(list_ptr)->prev)

#define clist_next_object(list_ptr) (((typeof(list_ptr))(list_ptr)->next)->object)

#define clist_prev_object(list_ptr) (((typeof(list_ptr))(list_ptr)->prev)->object)

#define clist_from_object(object_addr) ((clist(typeof(*object_addr)) *)((char *)object_addr - offsetof(clist(typeof(*object_addr)), object)))

#define clist_assign(dest_ptr, src_ptr) (dest_ptr = (void *)src_ptr)

#define clist_assign_from_object(list_ptr, object_addr) clist_assign(list_ptr, clist_from_object(object_addr))

#define clist_front(list_ptr) (list_ptr->object)

#define clist_back(list_ptr) (clist_prev_object(list_ptr))

#define clist_new_node(list_ptr, item, destructor_fn)     \
	do                                                \
	{                                                 \
		(list_ptr) = malloc(sizeof(*(list_ptr))); \
		if ((list_ptr) == NULL)                   \
			break;                            \
		clist_init(list_ptr, destructor_fn);      \
		(list_ptr)->object = item;                \
	} while (0);

#define clist_foreach(item, list_ptr)                                     \
	for (typeof((list_ptr)->object) *item = &(list_ptr)->object;      \
	     list_ptr != NULL && item != NULL;                            \
	     item = (((clist(typeof(                                      \
			  (list_ptr)->object)) *)clist_from_object(item)) \
			 ->next == (list_ptr))                            \
			? NULL                                            \
			: &(clist_next_object(clist_from_object(item))))

#define clist_backward_foreach(list_ptr)                                  \
	for (typeof((list_ptr)->object) *item = &(list_ptr)->object;      \
	     item != NULL;                                                \
	     item = (((clist(typeof(                                      \
			  (list_ptr)->object)) *)clist_from_object(item)) \
			 ->prev == (list_ptr))                            \
			? NULL                                            \
			: &(clist_prev_object(clist_from_object(item))))

#define clist_push_back(list_ptr, item, destructor_fn) \
	clist_push_before(list_ptr, item, destructor_fn)

#define clist_emplace_back(list_ptr, destructor_fn, constructor_fn, ...) \
	do                                                               \
	{                                                                \
		typeof(list_ptr) __i__ = malloc(sizeof(*__i__));         \
		if (__i__ == NULL)                                       \
			break;                                           \
		if (constructor_fn(&__i__->object, ##__VA_ARGS__) < 0)   \
		{                                                        \
			free(__i__);                                     \
			break;                                           \
		}                                                        \
		if ((list_ptr) == NULL)                                  \
		{                                                        \
			clist_init(__i__, destructor_fn);                \
			clist_assign(list_ptr, __i__);                   \
			break;                                           \
		}                                                        \
		__i__->destructor = destructor_fn;                       \
		__i__->prev = (list_ptr)->prev;                          \
		(list_ptr)->prev = __i__;                                \
		clist_prev(__i__)->next = __i__;                         \
		__i__->next = (list_ptr);                                \
	} while (0)

#define clist_push_front(list_ptr, item, destructor_fn)           \
	do                                                        \
	{                                                         \
		clist_push_before(list_ptr, item, destructor_fn); \
		clist_assign(list_ptr, (list_ptr)->prev);         \
	} while (0)

#define clist_emplace_front(list_ptr, destructor_fn, constructor_fn, ...) \
	do                                                                \
	{                                                                 \
		typeof(list_ptr) __i__ = malloc(sizeof(*__i__));          \
		if (__i__ == NULL)                                        \
			break;                                            \
		if (constructor_fn(&__i__->object, ##__VA_ARGS__) < 0)    \
		{                                                         \
			free(__i__);                                      \
			break;                                            \
		}                                                         \
		if ((list_ptr) == NULL)                                   \
		{                                                         \
			clist_init(__i__, destructor_fn);                 \
			clist_assign(list_ptr, __i__);                    \
			break;                                            \
		}                                                         \
		__i__->destructor = destructor_fn;                        \
		__i__->prev = (list_ptr)->prev;                           \
		(list_ptr)->prev = __i__;                                 \
		clist_prev(__i__)->next = __i__;                          \
		__i__->next = (list_ptr);                                 \
		clist_assign(list_ptr, __i__);                            \
	} while (0)

#define clist_destroy(list_ptr)                                  \
	do                                                       \
	{                                                        \
		if (list_ptr == NULL)                            \
			break;                                   \
		typeof(list_ptr) node = (list_ptr);              \
		if (node->destructor)                            \
		{                                                \
			do                                       \
			{                                        \
				node->destructor(&node->object); \
				if (node->next == list_ptr)      \
				{                                \
					free(node);              \
					break;                   \
				}                                \
				node = node->next;               \
				free(node->prev);                \
			} while (node != (void *)list_ptr);      \
		}                                                \
		else                                             \
		{                                                \
			do                                       \
			{                                        \
				if (node->next == list_ptr)      \
				{                                \
					free(node);              \
					break;                   \
				}                                \
				node = node->next;               \
				free(node->prev);                \
			} while (node != (void *)list_ptr);      \
		}                                                \
		list_ptr = NULL;                                 \
	} while (0)

#define clist_clear(list_ptr) clist_destroy(list_ptr)

static inline bool clist_inline_empty(void *p)
{
	return p == NULL;
}

#define clist_empty(list_ptr) clist_inline_empty(list_ptr)

static inline size_t clist_inline_size(void *p)
{
	size_t i = 1;
	struct clist_node_internal *list = p;

	if (p == NULL)
		return 0;
	while (list->next != p)
	{
		list = list->next;
		++i;
	}
	return i;
}

#define clist_size(list_ptr) clist_inline_size(list_ptr)

#define clist_push_after(list_ptr, item, destructor_fn)     \
	do                                                  \
	{                                                   \
		typeof(list_ptr) __i__;                     \
		clist_new_node(__i__, item, destructor_fn); \
		if (list_ptr == NULL)                       \
		{                                           \
			clist_assign(list_ptr, __i__);      \
			break;                              \
		}                                           \
		__i__->prev = (list_ptr);                   \
		__i__->next = (list_ptr)->next;             \
		clist_next(__i__)->prev = __i__;            \
		(list_ptr)->next = __i__;                   \
	} while (0)

#define clist_emplace_after(list_ptr, destructor_fn, constructor_fn, ...) \
	do                                                                \
	{                                                                 \
		typeof(list_ptr) __i__ = malloc(sizeof(*(list_ptr)));     \
		if (__i__ == NULL)                                        \
			break;                                            \
		if (constructor_fn(&__i__->object, ##__VA_ARGS__) < 0)    \
		{                                                         \
			free(__i__);                                      \
			break;                                            \
		}                                                         \
		if ((list_ptr) == NULL)                                   \
		{                                                         \
			clist_init(__i__, destructor_fn);                 \
			clist_assign(list_ptr, __i__);                    \
			break;                                            \
		}                                                         \
		__i__->destructor = destructor_fn;                        \
		__i__->prev = (list_ptr);                                 \
		__i__->next = (list_ptr)->next;                           \
		(list_ptr)->next = __i__;                                 \
		clist_next(__i__)->prev = __i__;                          \
	} while (0)

#define clist_push_before(list_ptr, item, destructor_fn)    \
	do                                                  \
	{                                                   \
		typeof(list_ptr) __i__;                     \
		clist_new_node(__i__, item, destructor_fn); \
		if ((list_ptr) == NULL)                     \
		{                                           \
			clist_assign(list_ptr, __i__);      \
			break;                              \
		}                                           \
		__i__->next = (list_ptr);                   \
		__i__->prev = (list_ptr)->prev;             \
		clist_prev(__i__)->next = __i__;            \
		(list_ptr)->prev = __i__;                   \
	} while (0)

#define clist_emplace_before(list_ptr, destructor_fn, constructor_fn, ...) \
	do                                                                 \
	{                                                                  \
		typeof(list_ptr) __i__ = malloc(sizeof(*(list_ptr)));      \
		if (__i__ == NULL)                                         \
			break;                                             \
		if (constructor_fn(&__i__->object, ##__VA_ARGS__) < 0)     \
		{                                                          \
			free(__i__);                                       \
			break;                                             \
		}                                                          \
		if ((list_ptr) == NULL)                                    \
		{                                                          \
			clist_init(__i__, destructor_fn);                  \
			clist_assign(list_ptr, __i__);                     \
			break;                                             \
		}                                                          \
		__i__->destructor = destructor_fn;                         \
		__i__->prev = (list_ptr)->prev;                            \
		__i__->next = (list_ptr);                                  \
		clist_prev(__i__)->next = __i__;                           \
		(list_ptr)->prev = __i__;                                  \
	} while (0)

#endif
