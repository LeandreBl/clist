#include <stdlib.h>

#include "clist.h"

void clist_generic_destroy(void *list)
{
	struct clist_node_internal *p = list;
	struct clist_node_internal *node = p;

	if (list == NULL)
		return;
	do {
		if (node->destructor)
			node->destructor((char *)node + offsetof(clist(int), object));
		if (node->next == p) {
			free(node);
			return;
		}
		node = node->next;
		free(node->prev);
	} while (node != p);
}
