// File: ps_sort_utils.c
#include "push_swap.h"

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void normalize_stack(t_stack *a)
{
    int *arr;
    t_node *c;
    t_bool s;
    int i;
    int j;

    if (!a || a->size <= 1)
        return;
    arr = malloc(sizeof(int) * a->size);
    if (!arr)
    {
        fprintf(stderr, "Error: Malloc failed during normalization\n");
        exit(1); // Consider better error handling
    }
    c = a->head;
    i = 0;
    while (i < a->size)
    {
        arr[i] = c->orig;
        c = c->next;
        i++;
    }
    i = 0;
    while (i < a->size - 1)
    {
        s = FALSE;
        j = 0;
        while (j < a->size - 1 - i)
        {
            if (arr[j] > arr[j + 1])
            {
                swap_ints(&arr[j], &arr[j + 1]);
                s = TRUE;
            }
            j++;
        }
        if (s == FALSE)
            break;
        i++;
    }
    c = a->head;
    i = 0;
    while (i < a->size)
    {
        j = 0;
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

    if (*a > *b) { temp = *a; *a = *b; *b = temp; }
    if (*a > *c) { temp = *a; *a = *c; *c = temp; }
    if (*b > *c) { temp = *b; *b = *c; *c = temp; }
}

t_bool is_consecutive(int a, int b, int c, int d)
{
    sort_three_numbers(&a, &b, &c);
    if ((b - a == 1) && (c - b == 1) && (d - c == 1))
        return (TRUE);
    else
        return (FALSE);
}