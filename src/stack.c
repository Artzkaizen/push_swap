#include "push_swap.h"


t_bool is_sorted(t_stack *stack, t_sort_order order)
{
	t_node *next;
	t_node *current;

	if (!stack)
		return (FALSE);
	current = stack->head;
	if (!current)
		return (FALSE);
	while (current && current->next != stack->head)
	{
		next = current->next;
		while (next)
		{
			if (order == ASCENDING && current->value > next->value)
					return (FALSE);
			else if(order == DESCENDING && current->value < next->value)
				return (FALSE);
			next = next->next;
			if (next == stack->head)
				break;
		}
		current = current->next;
			if (current == stack->head)
				break;
	}
	return (TRUE);
}

t_bool validate_stack(t_stack *stack)
{
	t_node *tmp;
	t_node *current;

	if (!stack)
		return (FALSE);
	current = stack->head;
	while (current)
	{
		tmp = current;
		while (tmp)
		{
			if (tmp != current && tmp->value == current->value)
				return (FALSE);
			tmp = current->next;
		}
		current = current->next;
	}
	return (TRUE);
}



t_bool ft_add_node(t_node **list, int value, t_node *new)
{
    t_node *last;
    t_node *new_node;

	if (new)
		new_node = new;
	else
    new_node = (t_node *)malloc(sizeof(t_node));
    if (!new_node)
        return (FALSE);
    new_node->value = value;
    if (!*list)
    {
        *list = new_node;
        new_node->next = new_node;
        new_node->prev = new_node;
    }
    else
    {
        last = (*list)->prev;

        last->next = new_node;
        new_node->prev = last;
        new_node->next = *list;
		(*list)->prev = new_node;
    }
    return (TRUE);
}

void free_stack(t_stack *stack)
{
	t_node *tmp;
	t_node *current;

	if (!stack)
		return;
	current = stack->head;
	while (current)
	{
		tmp = current;
		current = current->next;
		free(tmp);
	}
	free(stack);
}

t_bool fill_stack(t_stack *stack, char **str, int tab_size)
{
	int idx;
	long tmp;

	idx = 0;
	tmp = 0;
	if (tab_size < 1 || !stack)
		return (FALSE);
	if (!str)
		return(stack->size = idx, TRUE);
	while (idx < tab_size)
	{
		if (!ft_isnumeric(str[idx]))
			return (free_stack(stack), FALSE);
		tmp = ft_atol(str[idx]);
		if (tmp > INT_MAX || tmp < INT_MIN)
			return (free_stack(stack), FALSE);
		if (!idx)
			(stack->min = (int)tmp, stack->max = (int)tmp);
		if(!ft_add_node(&stack->head, (int)tmp, NULL))
			return (free_stack(stack), FALSE);
		if (stack->max < (int)tmp)
			stack->max = (int)tmp;
		if (stack->min > (int)tmp)
			stack->min = (int)tmp;
		idx++;
		stack->size = idx;
	}
	return (TRUE);
}



void move_stack(t_tabs *tabs, t_stack_move move)
{
	t_node *head;
	t_node *second;

	if (move == SA)
	{
		head = tabs->a.head;
		second = head->next;

		head->next = second->next;
		second->next->prev = head;
		second->next = head;
		second->prev = head->prev;
		head->prev->next = second;
		head->prev = second;

		tabs->a.head = second;
		ft_printf("sa\n");
	}
	else if (move == SB)
	{
		// current = tabs->a.head;
		tabs->a.head = tabs->a.head->next;
		ft_printf("sb\n");
	}
	else if (move == SS)
	{
		// swap the top two elements of both stacks
	}
	else if (move == PA)
	{
		// push the top element of stack b to stack a
		tabs->a.head = tabs->b.head;
		tabs->b.head = tabs->b.head->next;
		ft_printf("pa\n");
	}
	else if (move == PB)
	{
		// push the top element of stack a to stack b
		tabs->b.head = tabs->a.head;
		tabs->b.size++;


		tabs->a.head->prev->next = tabs->a.head->next;
		tabs->a.head->next->prev = tabs->a.head->prev;
		tabs->a.head = tabs->a.head->next;

		tabs->b.head->next = tabs->b.head;
		tabs->b.head->prev = tabs->b.head;
		tabs->a.size--;
		ft_printf("pb\n");
	}
	else if (move == RA)
	{
		// rotate stack a
		tabs->a.head = tabs->a.head->next;

		ft_printf("ra\n");
	}
	else if (move == RB)
	{
		// rotate stack b
	}
	else if (move == RR)
	{
		// rotate both stacks
	}
	else if (move == RRA)
	{
		tabs->a.head = tabs->a.head->prev;
		ft_printf("rra\n");
	}
	else if (move == RRB)
	{
		// reverse rotate stack b
	}
	else if (move == RRR)
	{
		// reverse rotate both stacks
	}
	{
		/* code */
	}
	
}
