#include <criterion/criterion.h>

#include "clist.h"

Test(clist, foreach)
{
	clist(int) * first;
	clist(int) * second;

	clist_new_node(first, 12, NULL);
	clist_new_node(second, 42, NULL);
	clist_assign(first->base.next, second);
	clist_assign(first->base.prev, second);
	clist_assign(second->base.next, first);
	clist_assign(second->base.prev, first);

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
	free(first);
	free(second);
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
	clist_foreach(val, list)
	{
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
	clist(int) * node;
	clist_assign(node, list->base.next);
	int i = 10;
	clist_foreach(val, node)
	{
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
	clist(int) * node;
	clist_assign(node, list->base.next);
	int i = 0;
	clist_foreach(val, node)
	{
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

	for (int i = 0; i < 10; ++i) {
		clist_emplace_back(list, NULL, set_value, i);
	}
	cr_assert_eq(clist_size(list), 5); /* 0 2 4 6 8 */
	int i = 0;
	clist_foreach(val, list)
	{
		cr_assert_eq(*val, i * 2);
		++i;
	}
	clist_destroy(list);
}

Test(clist, emplace_front)
{
	clist(int) *list = NULL;

	for (int i = 0; i < 10; ++i)
		clist_emplace_front(list, NULL, set_value, i);
	cr_assert_eq(clist_size(list), 5); /* 8 6 4 2 0 */
	int i = 10;
	clist_foreach(val, list)
	{
		i -= 2;
		cr_assert_eq(*val, i);
	}
	clist_destroy(list);
}

Test(clist, emplace_after)
{
	clist(int) *list = NULL;

	clist_emplace_after(list, NULL, set_value, -2);
	cr_assert_eq(list->object, -2);
	for (int i = 0; i < 10; ++i) {
		clist_emplace_after(list, NULL, set_value, i * 2);
		cr_assert_eq(clist_next_object(list), i * 2);
	}
	clist(int) * node;
	clist_assign(node, list->base.next);
	int i = 10;
	clist_foreach(val, node)
	{
		--i;
		cr_assert_eq(*val, i * 2);
	}
	clist_destroy(list);
}

Test(clist, emplace_before)
{
	clist(int) *list = NULL;

	clist_emplace_before(list, NULL, set_value, 20);
	cr_assert_eq(list->object, 20);
	for (int i = 0; i < 10; ++i) {
		clist_emplace_before(list, NULL, set_value, i * 2);
		cr_assert_eq(clist_prev_object(list), i * 2);
	}
	clist(int) * node;
	clist_assign(node, list->base.next);
	int i = 0;
	clist_foreach(val, node)
	{
		cr_assert_eq(*val, i * 2);
		++i;
	}
	clist_destroy(list);
}

Test(clist, swap)
{
	clist(int) *list = NULL;
	clist(int) * p;

	clist_push_back(list, 5, NULL);
	clist_push_back(list, 14344, NULL);
	clist_assign(p, clist_next(list));

	cr_assert_eq(p->object, 14344);
	cr_assert_eq(p->base.prev, list);
	cr_assert_eq(p->base.next, list);

	cr_assert_eq(list->object, 5);
	cr_assert_eq(list->base.prev, p);
	cr_assert_eq(list->base.next, p);

	clist_swap(p, list);

	cr_assert_eq(p->object, 5);
	cr_assert_eq(p->base.prev, p);
	cr_assert_eq(p->base.next, p);

	cr_assert_eq(list->object, 14344);
	cr_assert_eq(list->base.prev, list);
	cr_assert_eq(list->base.next, list);

	clist_destroy(list);
}

Test(clist, remove)
{
	clist(int) *list = NULL;
	clist(int) * p;

	for (int i = 0; i < 20; ++i)
		clist_push_back(list, i, NULL);
	clist_foreach(var, list)
	{
		clist_assign(p, clist_from_object(var)->base.next);
		if (p->object > 5) {
			clist_remove(list, p);
			free(p);
		}
	}
	cr_assert_eq(clist_size(list), 13);
	int i = 0;
	clist_foreach(var, list)
	{
		if (i <= 5)
			cr_assert_eq(*var, i);
		else
			cr_assert_eq(*var, i * 2 - 5);
		++i;
	}
	clist_destroy(list);
}
