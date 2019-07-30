#include <stdlib.h>

#include "clist.h"

#include <stdio.h>
static void with_destructor(struct clist_node_internal *p)
{
	struct clist_node_internal *node = p;

	do {
		node->destructor((char *)node + offsetof(clist(int), object));
		if (node->next == p) {
			free(node);
			return;
		}
		node = node->next;
		free(node->prev);
	} while (node != p);
}

static void no_destructor(struct clist_node_internal *p)
{
	struct clist_node_internal *node = p;

	do {
		if (node->next == p) {
			free(node);
			return;
		}
		node = node->next;
		free(node->prev);
	} while (node != p);
}

void clist_generic_destroy(void *list)
{
	struct clist_node_internal *p = list;

	if (list == NULL)
		return;
	if (p->destructor == NULL)
		no_destructor(p);
	else
		with_destructor(p);
}
