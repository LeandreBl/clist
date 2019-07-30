#include "clist.h"

void clist_generic_push_before(void *plist, void *pnode)
{
	struct clist_node_internal *list = plist;
	struct clist_node_internal *node = pnode;

	node->next = list;
	node->prev = list->prev;
	node->prev->next = node;
	list->prev = node;
}
