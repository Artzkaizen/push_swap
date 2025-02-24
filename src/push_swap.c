#include "../includes/push_swap.h"
#include "../includes/ft_printf.h"

/*
* Print the stack
* @param *stack: the stack to print
*/
void print_tabs(t_tabs *tabs)
{
	int idx;
	int max;
	t_node *a;
	t_node *b;

	a = tabs->a.head;
	b = tabs->b.head;

	if (tabs->a.size > tabs->b.size)
		max = tabs->a.size;
	else 
		max = tabs->b.size;
	idx = 0;
	ft_printf("stack \t\tstack b\n");
	ft_printf("-------\t\t-------\n");
	while (idx < max)
	{
		if (tabs->a.size > idx)
			ft_printf("a:[%d]\t", a->value);
		else
			ft_printf("a:[ ]\t");
		if (a->value <= 999)
			ft_printf("\t");
		if (b && b->value && tabs->b.size > idx)
			ft_printf("b:[%d]\b\n", b->value);
		else
			ft_printf("b:[ ]\n");
		a = a->next;
		if (a == tabs->a.head)
			break;
		if (b && b->next)
			b = b->next;
		idx++;
	}
	ft_printf("-------\t\t-------\n");
	ft_printf("A = Size: %d\tMin: %d\tMax: %d\n", tabs->a.size, tabs->a.min, tabs->a.max);
	ft_printf("B = Size: %d\tMin: %d\tMax: %d\n", tabs->b.size, tabs->b.min, tabs->b.max);
}

// /*
// * Free the tabs
// * @param *tabs: the tabs to fill
// */

void free_tabs(t_tabs *tabs)
{
    // free(tabs->a.tab);
    // free(tabs->b.tab);
    free(tabs);
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
		return (free_tabs(tabs), NULL);
	print_tabs(tabs);
	return (tabs);
}



void push_swap(t_tabs *tabs, t_sort_order order)
{
	t_node *top;
	ft_printf("\n\nsorting stack\n\n");
	top = tabs->a.head;

	// check top and top + 1
	// if top > top + 1 check if top > bottom
	// else if top < top + 1 sa
	// check if top > bottom rra
	// else if top < bottom pb
	if (order)
	{
		/* code */
	}
	if(tabs->a.size == 3 && !is_sorted(&tabs->a, ASCENDING))
	{
		// if (top->value == tabs->a.max)
		// 	move_stack(tabs, RA);
		// if (top->value == tabs->a.min && top->next->value == tabs->a.max)
		// 	move_stack(tabs, SA);
		// if (!is_sorted(&tabs->a, ASCENDING))
		// 	push_swap(tabs, order);
	}
	else 
	{
		if (top->value > top->next->value)
		{
			if (top->prev->value < top->value)
				move_stack(tabs, RRA);
			print_tabs(tabs);
			move_stack(tabs, PB);
			print_tabs(tabs);
			move_stack(tabs, PB);
			
			print_tabs(tabs);
			move_stack(tabs, PB);
			print_tabs(tabs);
			move_stack(tabs, PB);
			print_tabs(tabs);
			move_stack(tabs, PB);
			print_tabs(tabs);
			// move_stack(tabs, PB);
			// if (!is_sorted(&tabs->a, ASCENDING))
			// {
			// 	push_swap(tabs, ASCENDING);
			// }
			
		}
		// if (top->value > top->next->value)
		// 	move_stack(tabs, SA);
		// if (top->value > top->next->value)
		// 	move_stack(tabs, RRA);
		// while(top->value < top->next->value)
		// {
		// 	ft_printf("top: %d\tnext: %d\n", top->value, top->next->value);
		// 	move_stack(tabs, PB);
		// 	top = tabs->a.head->next;
		// }
		// if (top->value > top->prev->value)
		// 	move_stack(tabs, RRA);
		// if (!is_sorted(&tabs->a, ASCENDING))
		// 	push_swap(tabs, order);
	}
	print_tabs(tabs);

}

int	 main(int argc, char **argv)
{
	t_tabs *tabs;
	t_sort_order order;

	(void)argv;
	(void)argc;

	char *arr = "2323 32 42 34 234 23 4";

	// if (argc == 1)
	// 	return (0);
	// if (argc == 2)
	// 	tabs= create_tabs(ft_split(argv[1], ' '));
	// else
	// 	tabs = create_tabs(argv + 1);

	tabs = create_tabs(ft_split(arr, ' '));

	if (!tabs)
		return (ft_printf("Error\n"), EXIT_FAILURE);
	if (tabs->a.head->value > tabs->a.head->next->value)
		order = DESCENDING;
	else
		order = ASCENDING;
	if (!is_sorted(&tabs->a, order))
		push_swap(tabs, UNSORTED);
	else if (order == DESCENDING)
		push_swap(tabs, DESCENDING);
	else
		ft_printf("stack is sorted\n");
	free(tabs);
	return (EXIT_SUCCESS);
}

