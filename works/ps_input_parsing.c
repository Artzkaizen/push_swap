#include "push_swap.h"

t_bool has_duplicates(t_stack *stack)
{
    t_node *outer_node;
    t_node *inner_node;
    int outer_count;
    int inner_count;

    if (!stack || !stack->head || stack->size <= 1)
        return (FALSE);
    outer_node = stack->head;
    outer_count = 0;
    while (outer_count < stack->size)
    {
        inner_node = outer_node->next;
        inner_count = 0;
        while (inner_count < stack->size - 1)
        {
            if (outer_node->orig == inner_node->orig)
                return (ft_putstr("Error\n"), TRUE);
            inner_node = inner_node->next;
            inner_count++;
        }
        outer_node = outer_node->next;
        outer_count++;
    }
    return (FALSE);
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
        return (stack->size = idx, FALSE);
    while (idx < tab_size)
    {
        if (!ft_isnumeric(str[idx]))
            return (free_stack(stack), FALSE);
        tmp = ft_atol(str[idx]);
        if (tmp > FT_INT_MAX || tmp < FT_INT_MIN)
            return (free_stack(stack), ft_putstr("Error\n"), FALSE);
        if (!ft_add_node(&stack->head, (int)tmp, NULL))
            return (free_stack(stack), FALSE);
        idx++;
        stack->size = idx;
    }
    return (!has_duplicates(stack));
}

t_tabs *create_tabs(char **str, int len)
{
    t_tabs *tabs;

    tabs = malloc(sizeof(t_tabs));
    if (!tabs)
        return (NULL);
    tabs->a.head = NULL;
    tabs->a.size = 0;
    tabs->b.head = NULL;
    tabs->b.size = 0;
    if (!fill_stack(&tabs->a, str, len))
    {
        free_tabs(tabs);
        return (NULL);
    }
    return (tabs);
}