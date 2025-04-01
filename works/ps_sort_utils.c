// File: ps_sort_utils.c
#include "push_swap.h"

t_bool is_sorted(t_stack *stack)
{
    t_node *c;
    int i;

    if (!stack || !stack->head || stack->size <= 1)
        return (TRUE);
    c = stack->head;
    i = 0;
    while (i < stack->size - 1)
    {
        if (c->value > c->next->value)
            return (FALSE);
        c = c->next;
        i++;
    }
    return (TRUE);
}

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
t_bool is_raw_input_sorted(t_stack *stack)
{
    t_node *c;
    int i;

    if (!stack || !stack->head || stack->size <= 1)
        return (TRUE);
    c = stack->head;
    i = 0;
    while (i < stack->size - 1)
    {
        if (c->orig > c->next->orig)
            return (FALSE);
        c = c->next;
        i++;
    }
    return (TRUE);
}

void sort_three_numbers(int *a, int *b, int *c)
{
    int temp;

    if (*a > *b)
    {
        temp = *a;
        *a = *b;
        *b = temp;
    }
    if (*a > *c)
    {
        temp = *a;
        *a = *c;
        *c = temp;
    }
    if (*b > *c)
    {
        temp = *b;
        *b = *c;
        *c = temp;
    }
}

t_bool is_consecutive(int a, int b, int c, int d)
{
    sort_three_numbers(&a, &b, &c);
    if ((b - a == 1) && (c - b == 1) && (d - c == 1))
        return (TRUE);
    else
        return (FALSE);
}