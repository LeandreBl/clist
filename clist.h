#ifndef CLIB_H_
#define CLIB_H_

#define clist(type)                                                            \
	struct {                                                               \
		void *prev;                                                    \
		void *next;                                                    \
		type object;                                                   \
	}

#define clist_foreach(item, list_ptr)                                          \
	for (__typeof__((list_ptr)->object) *item = &(list_ptr)->object;       \
	     item != NULL;                                                     \
	     item = (((clist(__typeof__(                                       \
			      (list_ptr)->object)) *)(item - sizeof(*item)))   \
			     ->next                                            \
		     == list_ptr)                                              \
			    ? NULL                                             \
			    : &((clist(__typeof__((list_ptr)->object)) *)(*(   \
					void **)(item - sizeof(*item))))       \
				       ->object)

#define clist_init(list) ((list)->next = (list), (list)->prev = (list))

#endif