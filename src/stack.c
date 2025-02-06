#include "push_swap.h"

// MOVES 
// sa : swap a - swap the first 2 elements at the top of stack a. Do nothing if there is only one or no elements).
// sb : swap b - swap the first 2 elements at the top of stack b. Do nothing if there is only one or no elements).
// ss : sa and sb at the same time.
// pa : push a - take the first element at the top of b and put it at the top of a. Do nothing if b is empty.
// pb : push b - take the first element at the top of a and put it at the top of b. Do nothing if a is empty.

// ra : rotate a - shift up all elements of stack a by 1. The first element becomes the last one.
// rb : rotate b - shift up all elements of stack b by 1. The first element becomes the last one.
// rr : ra and rb at the same time.
// rra : reverse rotate a - shift down all elements of stack a by 1. The last element becomes the first one.
// rrb : reverse rotate b - shift down all elements of stack b by 1. The last element becomes the first one.
// rrr : rra and rrb at the same time.

t_bool validate_stack(t_stack *stack)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < stack->size)
	{
		j = 0;
		while (j < stack->size)
		{
			if (i != j && stack->tab[i] == stack->tab[j])
				return (FALSE);
			j++;
		}
		i++;
	}
	return (TRUE);
}

/*
* Fill a stack with a string array
* @param *stack: the stack to fill
* @param **str: an array of strings
* @param tab_size: the size of the string array
* @return TRUE: if the stack is filled
* @return FALSE: if the stack is invalid
*/
t_bool fill_stack(t_stack *stack, char **str, int tab_size)
{
	int idx;
	long tmp;

	idx = 0;
	tmp = 0;

	if (tab_size < 1)
		return (FALSE);
	stack->tab = (int *)malloc(sizeof(int) * tab_size);
	if (!stack->tab)
		return (FALSE);
	if (!str)
		return(stack->size = idx, TRUE);
	while (idx < tab_size)
	{
		if (!ft_isnumeric(str[idx]))
		{
			return (free(stack->tab), FALSE);

		}
		tmp = ft_atol(str[idx]);
		if (tmp > INT_MAX || tmp < INT_MIN)
			return (free(stack->tab), FALSE);
		stack->tab[idx] = (int)tmp;
		idx++;
		stack->size = idx;
	}
	return (TRUE);
}


/*
* Create a stack from a string array
* @param **str: array of strings
* @return t_tabs: a struct containing two stacks
* @return NULL: if the stack is invalid
*/
t_tabs *create_tabs(char **str)
{
	int len;
    t_tabs *tabs;


	len = 0;
    tabs = (t_tabs *)malloc(sizeof(t_tabs));
    if (!tabs)
        return (NULL);
	while (str[len])
		len++;
	if (!(fill_stack(&tabs->a, str, len) && fill_stack(&tabs->b, NULL, len)))
		return (free(tabs), NULL);
	if (!(validate_stack(&tabs->a) && validate_stack(&tabs->b)))
		return (free_tabs(tabs), NULL);
	print_tabs(tabs, len);
	return (tabs);
}

/*
* Print the stack
* @param *stack: the stack to print
*/
void print_tabs(t_tabs *tabs, int size)
{
	int i;

	i = 0;
	ft_printf("stack a\t\tstack b\n");
	ft_printf("-------\t\t-------\n");
	while (i < size)
	{
		if (i < tabs->a.size)
			ft_printf("a:[%d]\t\t", tabs->a.tab[i]);
		else
			ft_printf("a:[ ]\t\t");
		if (i < tabs->b.size)
			ft_printf("b:[%d]\n", tabs->b.tab[i]);
		else
			ft_printf("b:[ ]\n");

		i++;
	}
}

void free_tabs(t_tabs *tabs)
{
    free(tabs->a.tab);
    free(tabs->b.tab);
    free(tabs);
}