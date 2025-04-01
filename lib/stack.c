#include "./push_swap.h"
#include "utils.h"

t_node *create_node(int num)
{
    t_node *node;

    node = malloc(sizeof(t_node));
    if (!node)
        return (NULL);
    node->orig = num;
    node->value = 0;
    node->next = node;
    node->prev = node;
    return (node);
}
void append_node(t_stack *s, t_node *node)
{
    t_node *tmp;

    if (!s->head)
    {
        s->head = node;
        node->next = node;
        node->prev = node;
    }
    else
    {
        tmp = s->head->prev;
        tmp->next = node;
        node->prev = tmp;
        node->next = s->head;
        s->head->prev = node;
    }
    s->size++;
}

t_bool has_duplicates(t_stack *stack)
{
    int i;
    int j;
    t_node *current_i;
    t_node *current_j;

    i = 0;
    j = 0;
    current_i = stack->head;
    if (stack->size <= 1)
        return (FALSE);
    while (i < stack->size)
    {
        current_j = current_i->next;
        j = i + 1;
        while (j < stack->size)
        {
            if (current_i->orig == current_j->orig)
                return (TRUE);
            if (current_j == stack->head->prev)
                break;
            current_j = current_j->next;
            j++;
        }
        if (current_i == stack->head->prev)
            break;
        current_i = current_i->next;
        i++;
    }
    return (FALSE);
}

t_bool is_valid_int_str(const char *str)
{
    if (!str || *str == '\0')
        return FALSE;
    int i = 0;
    if (str[i] == '-' || str[i] == '+')
    {
        i++;
        if (str[i] == '\0')
            return FALSE;
    }
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return FALSE;
        i++;
    }
    errno = 0;
    long num = strtol(str, NULL, 10);
    if (errno == ERANGE || num > INT_MAX || num < INT_MIN)
        return FALSE;
    return TRUE;
}

t_bool is_stack_sorted(t_stack *s)
{
    if (!s || s->size <= 1 || !s->head)
        return TRUE;
    t_node *c = s->head;
    int i = 0;
    while (i < s->size - 1)
    {
        if (c->value > c->next->value)
            return FALSE;
        c = c->next;
        i++;
    }
    return TRUE;
}

t_bool is_input_sorted(t_stack *s)
{
    if (!s || s->size <= 1 || !s->head)
        return TRUE;
    t_node *c = s->head;
    int i = 0;
    while (i < s->size - 1)
    {
        if (c->orig > c->next->orig)
            return FALSE;
        c = c->next;
        i++;
    }
    return TRUE;
}

void swap_ints(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void normalize_stack(t_stack *a)
{
    if (a->size <= 1)
        return;
    int *arr = malloc(sizeof(int) * a->size);
    if (!arr)
        exit(1);
    t_node *c = a->head;
    int i = 0;
    while (i < a->size)
    {
        arr[i] = c->orig;
        c = c->next;
        i++;
    }

    t_bool s;
    i = 0;
    while (i < a->size - 1)
    {
        s = FALSE;
        int j = 0;
        while (j < a->size - 1 - i)
        {
            if (arr[j] > arr[j + 1])
                (swap_ints(&arr[j], &arr[j + 1]), s = TRUE);
            j++;
        }
        if (!s)
            break;
        i++;
    }

    c = a->head;
    i = 0;
    while (i < a->size)
    {
        int j = 0;
        while (j < a->size)
        {
            if (c->orig == arr[j])
            {
                c->value = j;
                break;
            }
            j++;
        }
        c = c->next;
        i++;
    }
    free(arr);
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
		if (tmp > FT_INT_MAX || tmp < FT_INT_MIN)
			return (free_stack(stack), FALSE);
		if(!ft_add_node(&stack->head, (int)tmp, NULL))
			return (free_stack(stack), FALSE);
		idx++;
		stack->size = idx;
	}
    return (!has_duplicates(stack));
}

void free_stack(t_stack *s)
{
    t_node *tail;
    t_node *next_n;
    t_node *current;
    
    current = s->head;
    tail = s->head->prev;
    if (!s || !s->head)
        return;
    tail->next = NULL;
    while (current)
    {
        next_n = current->next;
        free(current);
        current = next_n;
    }
    s->head = NULL;
    s->size = 0;
}
