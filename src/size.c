#include "clist.h"

size_t clist_generic_size(void *p)
{
	size_t i = 1;
	struct clist_node_internal *list = p;

	if (p == NULL)
		return 0;
	while (list->next != p) {
		list = list->next;
		++i;
	}
	return i;
}
