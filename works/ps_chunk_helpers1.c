// File: ps_chunk_helpers1.c
#include "push_swap.h"

t_stack *loc_to_stack(t_tabs *data, t_loc loc)
{
    if (loc == TOP_A || loc == BOTTOM_A)
        return (&data->a);
    else
        return (&data->b);
}

int chunk_value(t_tabs *data, t_chunk *chunk, int n)
{
    t_stack *stk;
    t_node *current;
    int i;

    stk = loc_to_stack(data, chunk->loc);
    if (!stk || !stk->head || n <= 0 || n > chunk->size || n > stk->size)
        return (FT_INT_MIN);
    i = 1;
    if (chunk->loc == TOP_A || chunk->loc == TOP_B)
    {
        current = stk->head;
        while (i < n)
        {
            current = current->next;
            i++;
        }
    }
    else
    {
        current = stk->head->prev;
        while (i < n)
        {
            current = current->prev;
            i++;
        }
    }
    return (current->value);
}

void innit_size(t_chunk *min, t_chunk *mid, t_chunk *max)
{
    min->size = 0;
    mid->size = 0;
    max->size = 0;
}