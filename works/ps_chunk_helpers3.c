// File: ps_chunk_helpers3.c
#include "push_swap.h"

t_bool check_partial_sort_loop(t_node *start_node, t_node *head_node)
{
	t_node *runner;
	int expected_value; // 2 variables + params

	if (!start_node || !head_node)
		return (FALSE);
	expected_value = start_node->value;
	runner = start_node->next;
	while (runner != head_node)
	{
		expected_value++;
		if (runner->value != expected_value)
			return (FALSE);
		runner = runner->next;
	}
	return (TRUE);
}