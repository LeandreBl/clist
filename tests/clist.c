#include <criterion/criterion.h>

#include "clist.h"

Test(clist, foreach)
{
	clist(int) * first;
	clist(int) * second;

	clist_new_node(first, 12, NULL);
	clist_new_node(second, 42, NULL);
	first->next = second;
	first->prev = second;
	second->next = first;
	second->prev = first;

	size_t i = 0;
	clist_foreach(p, first)
	{
		if (i == 0)
			cr_assert_eq(*p, 12);
		if (i == 1)
			cr_assert_eq(*p, 42);
		++i;
	}
	cr_assert_eq(i, 2);
}

Test(clist, push_back)
{
	clist(int) *list = NULL;

	for (int i = 0; i < 10; ++i) {
		clist_push_back(list, i, NULL);
		cr_assert_eq(i, clist_prev_object(list));
	}
	int i = 0;
	clist_foreach(val, list)
	{
		cr_assert_eq(*val, i);
		++i;
	}
	clist_destroy(list);
}

Test(clist, push_front)
{
	clist(int) *list = NULL;

	for (int i = 10; i >= 0; --i) {
		clist_push_front(list, i, NULL);
		cr_assert_eq(list->object, i);
	}
	int i = 0;
	clist_foreach(val, list) {
		cr_assert_eq(*val, i);
		++i;
	}
	clist_destroy(list);
}

Test(clist, front)
{
	clist(int) *list = NULL;

	clist_push_back(list, 56, NULL);
	clist_push_back(list, 2, NULL);
	clist_push_back(list, 6, NULL);
	clist_push_back(list, 28, NULL);
	cr_assert_eq(clist_front(list), 56);
	clist_destroy(list);
}

static void str_free(char **pstr)
{
	free(*pstr);
}

Test(clist, back)
{
	clist(char *) *list = NULL;

	clist_push_back(list, strdup("5"), str_free);
	clist_push_back(list, strdup("2"), str_free);
	clist_push_back(list, strdup("28"), str_free);
	clist_push_back(list, strdup("56"), str_free);
	cr_assert_str_eq(clist_back(list), "56");
	clist_destroy(list);
}

Test(clist, empty)
{
	clist(int) *list = NULL;

	cr_assert_eq(clist_empty(list), true);
	clist_push_back(list, 5, NULL);
	cr_assert_eq(clist_empty(list), false);
	clist_destroy(list);
	cr_assert_eq(clist_empty(list), true);
}

Test(clist, size)
{
	clist(int) *list = NULL;

	cr_assert_eq(clist_size(list), 0);
	for (size_t i = 0; i < 10; ++i) {
		clist_push_back(list, i, NULL);
		cr_assert_eq(clist_size(list), i + 1);
	}
	clist_destroy(list);
}

Test(clist, push_after)
{
	clist(int) *list = NULL;

	clist_push_after(list, -1, NULL);
	cr_assert_eq(list->object, -1);
	for (int i = 0; i < 10; ++i) {
		clist_push_after(list, i, NULL);
		cr_assert_eq(clist_next_object(list), i);
	}
	clist(int) *node;
	clist_assign(node, list->next);
	int i = 10;
	clist_foreach(val, node) {
		--i;
		cr_assert_eq(*val, i);
	}
	clist_destroy(list);
}

Test(clist, push_before)
{
	clist(int) *list = NULL;

	clist_push_before(list, 10, NULL);
	cr_assert_eq(list->object, 10);
	for (int i = 0; i < 10; ++i) {
		clist_push_before(list, i, NULL);
		cr_assert_eq(clist_prev_object(list), i);
	}
	clist(int) *node;
	clist_assign(node, list->next);
	int i = 0;
	clist_foreach(val, node) {
		cr_assert_eq(*val, i);
		++i;
	}
	clist_destroy(list);
}

static int set_value(int *val, int src)
{
	if (src % 2)
		return -1;
	*val = src;
	return 0;
}

Test(clist, emplace_back)
{
	clist(int) *list = NULL;

	for (int i = 0; i < 10; ++i)
		clist_emplace_back(list, NULL, set_value, i);
	cr_assert_eq(clist_size(list), 5); /* 0 2 4 6 8 */
	clist_foreach(val, list) {
		cr_assert(!(*val % 2));
	}
	clist_destroy(list);
}
