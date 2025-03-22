#include "../includes/push_swap.h"


// t_bool is_sorted(t_stack *stack, t_sort_order order)
// {
// 	t_node *next;
// 	t_node *current;

// 	if (!stack)
// 		return (FALSE);
// 	current = stack->head;
// 	if (!current)
// 		return (FALSE);
// 	while (current && current->next != stack->head)
// 	{
// 		next = current->next;
// 		while (next)
// 		{
// 			if (order == ASCENDING && current->value > next->value)
// 					return (FALSE);
// 			else if(order == DESCENDING && current->value < next->value)
// 				return (FALSE);
// 			next = next->next;
// 			if (next == stack->head)
// 				break;
// 		}
// 		current = current->next;
// 			if (current == stack->head)
// 				break;
// 	}
// 	return (TRUE);
// }

t_bool validate_stack(t_stack *stack)
{
	t_node *tmp;
	t_node *current;

	if (!stack)
		return (FALSE);
	current = stack->head;
	while (current && current->next != stack->head)
	{
		tmp = current;
        tmp = current->next;
        while (tmp && tmp != stack->head)
        {
            if (tmp->value == current->value)
                return (FALSE);
            tmp = tmp->next;
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
	new_node->orig = value;
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
		if (tmp > INT_MAXi || tmp < INT_MINi)
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
    if(!validate_stack(stack))
        return (FALSE);
	return (TRUE);
}



// void move_stack(t_tabs *tabs, t_stack_move move)
// {
//     t_node *head;
//     t_node *second;
//     t_node *tmp;

//     if (move == SA && tabs->a.size >= 2)
//     {
//         head = tabs->a.head;
//         second = head->next;
//         head->next = second->next;
//         second->next->prev = head;
//         second->next = head;
//         second->prev = head->prev;
//         head->prev->next = second;
//         head->prev = second;
//         tabs->a.head = second;
//         ft_printf("sa\n");
//     }
//     else if (move == SB && tabs->b.size >= 2)
//     {
//         head = tabs->b.head;
//         second = head->next;
//         head->next = second->next;
//         second->next->prev = head;
//         second->next = head;
//         second->prev = head->prev;
//         head->prev->next = second;
//         head->prev = second;
//         tabs->b.head = second;
//         ft_printf("sb\n");
//     }
//     else if (move == SS)
//     {
//         move_stack(tabs, SA);
//         move_stack(tabs, SB);
//         ft_printf("ss\n");
//     }
//     else if (move == PA && tabs->b.size > 0)
//     {
//         tmp = tabs->b.head;
//         if (tabs->b.size == 1)
//         {
//             tabs->b.head = NULL;
//             tabs->b.min = 0;
//             tabs->b.max = 0;
//         }
//         else
//         {
//             tabs->b.head = tmp->next;
//             tmp->next->prev = tmp->prev;
//             tmp->prev->next = tmp->next;
            
//             // Update min/max for stack B after removal
//             if (tmp->value == tabs->b.min || tmp->value == tabs->b.max)
//             {
//                 t_node *current = tabs->b.head;
//                 tabs->b.min = current->value;
//                 tabs->b.max = current->value;
//                 do {
//                     if (current->value < tabs->b.min)
//                         tabs->b.min = current->value;
//                     if (current->value > tabs->b.max)
//                         tabs->b.max = current->value;
//                     current = current->next;
//                 } while (current != tabs->b.head);
//             }
//         }

//         if (!tabs->a.head)
//         {
//             tabs->a.head = tmp;
//             tmp->next = tmp;
//             tmp->prev = tmp;
//         }
//         else
//         {
//             tmp->next = tabs->a.head;
//             tmp->prev = tabs->a.head->prev;
//             tabs->a.head->prev->next = tmp;
//             tabs->a.head->prev = tmp;
//             tabs->a.head = tmp;
//         }
//         tabs->a.size++;
//         tabs->b.size--;

//         // Update min/max values for stack A
//         if (tmp->value < tabs->a.min)
//             tabs->a.min = tmp->value;
//         if (tmp->value > tabs->a.max)
//             tabs->a.max = tmp->value;

//         ft_printf("pa\n");
//     }
//     else if (move == PB && tabs->a.size > 0)
//     {
//         tmp = tabs->a.head;
//         if (tabs->a.size == 1)
//         {
//             tabs->a.head = NULL;
//             tabs->a.min = 0;
//             tabs->a.max = 0;
//         }
//         else
//         {
//             tabs->a.head = tmp->next;
//             tmp->next->prev = tmp->prev;
//             tmp->prev->next = tmp->next;
            
//             // Update min/max for stack A after removal
//             if (tmp->value == tabs->a.min || tmp->value == tabs->a.max)
//             {
//                 t_node *current = tabs->a.head;
//                 tabs->a.min = current->value;
//                 tabs->a.max = current->value;
//                 do {
//                     if (current->value < tabs->a.min)
//                         tabs->a.min = current->value;
//                     if (current->value > tabs->a.max)
//                         tabs->a.max = current->value;
//                     current = current->next;
//                 } while (current != tabs->a.head);
//             }
//         }

//         if (!tabs->b.head)
//         {
//             tabs->b.head = tmp;
//             tmp->next = tmp;
//             tmp->prev = tmp;
//             tabs->b.min = tmp->value;
//             tabs->b.max = tmp->value;
//         }
//         else
//         {
//             tmp->next = tabs->b.head;
//             tmp->prev = tabs->b.head->prev;
//             tabs->b.head->prev->next = tmp;
//             tabs->b.head->prev = tmp;
//             tabs->b.head = tmp;
//             if (tmp->value < tabs->b.min)
//                 tabs->b.min = tmp->value;
//             if (tmp->value > tabs->b.max)
//                 tabs->b.max = tmp->value;
//         }
//         tabs->b.size++;
//         tabs->a.size--;
//         ft_printf("pb\n");
//     }
//     else if (move == RA && tabs->a.size > 1)
//     {
//         tabs->a.head = tabs->a.head->next;
//         ft_printf("ra\n");
//     }
//     else if (move == RB && tabs->b.size > 1)
//     {
//         tabs->b.head = tabs->b.head->next;
//         ft_printf("rb\n");
//     }
//     else if (move == RR)
//     {
//         move_stack(tabs, RA);
//         move_stack(tabs, RB);
//         ft_printf("rr\n");
//     }
//     else if (move == RRA && tabs->a.size > 1)
//     {
//         tabs->a.head = tabs->a.head->prev;
//         ft_printf("rra\n");
//     }
//     else if (move == RRB && tabs->b.size > 1)
//     {
//         tabs->b.head = tabs->b.head->prev;
//         ft_printf("rrb\n");
//     }
//     else if (move == RRR)
//     {
//         move_stack(tabs, RRA);
//         move_stack(tabs, RRB);
//         ft_printf("rrr\n");
//     }
// }


// void move_stack(t_tabs *tabs, t_stack_move move)
// {
// 	t_node *head;
// 	t_node *second;

// 	if (move == SA)
// 	{
// 		ft_printf("Stack head: (%d)\n", tabs->a.head->value);

// 		head = tabs->a.head;
// 		second = head->next;

// 		head->next = second->next;
// 		second->next->prev = head;
// 		second->next = head;
// 		second->prev = head->prev;
// 		head->prev->next = second;
// 		head->prev = second;

// 		tabs->a.head = second;
// 		ft_printf("Stack head: (%d)\n", tabs->a.head->value);

// 		ft_printf("sa\n");
// 	}
// 	else if (move == SB)
// 	{
// 		// current = tabs->a.head;
// 		tabs->a.head = tabs->a.head->next;
// 		ft_printf("sb\n");
// 	}
// 	else if (move == SS)
// 	{
// 		// swap the top two elements of both stacks
// 	}
// 	else if (move == PA)
// 	{
// 		// push the top element of stack b to stack a
// 		tabs->a.head = tabs->b.head;
// 		tabs->b.head = tabs->b.head->next;
// 		ft_printf("pa\n");
// 	}
// 	else if (move == PB)
// 	{
// 		t_node *tmp = tabs->a.head->next;
// 		t_node *head = tabs->a.head;

// 		tabs->a.head->next->prev = tabs->a.head->prev;
// 		tabs->a.head->prev->next = tabs->a.head->next;

// 		if (!tabs->b.head)
// 		{
// 			head->next = head;
// 			head->prev = head;
// 		}
// 		else 
// 		{

// 			head->next = tabs->b.head;
// 			tabs->b.head->prev = head;
// 			// head->next = tabs->b.head;
// 			// head->prev = tabs->b.head;
// 			// tabs->b.head->prev = head;
// 		}
		
		



// 		tabs->b.head = head;

// 		ft_printf("Stack head B: (%d)\n  ", tabs->b.head->value);
// 		ft_printf("Stack head B: (%d)\n  ", tabs->b.head->value);
		

// 		tabs->b.head->next = tabs->b.head;
// 		tabs->b.head->prev = tabs->b.head;

// 		tabs->a.head = tmp;

	
// 		ft_printf("pb\n");
// 	}
// 	else if (move == RA)
// 	{
// 		// rotate stack a
// 		tabs->a.head = tabs->a.head->next;

// 		ft_printf("ra\n");
// 	}
// 	else if (move == RB)
// 	{
// 		// rotate stack b
// 	}
// 	else if (move == RR)
// 	{
// 		// rotate both stacks
// 	}
// 	else if (move == RRA)
// 	{
// 		ft_printf("Stack head: (%d)\n", tabs->a.head->value);

// 		tabs->a.head = tabs->a.head->prev;
// 		ft_printf("Stack head: (%d)\n", tabs->a.head->value);

// 		ft_printf("rra\n");
// 	}
// 	else if (move == RRB)
// 	{
// 		// reverse rotate stack b
// 	}
// 	else if (move == RRR)
// 	{
// 		// reverse rotate both stacks
// 	}
// 	{
// 		/* code */
// 	}
	
// }
