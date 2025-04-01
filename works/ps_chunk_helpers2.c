// File: ps_chunk_helpers2.c
#include "push_swap.h"

// set_third_pivots_by_value - unchanged, already compliant
void set_third_pivots_by_value(t_chunk *chunk, int min_val, int max_val,
                               int *pivot1_val, int *pivot2_val)
{
    int range;

    (void)chunk;
    range = max_val - min_val + 1;
    if (range < 1)
    {
        *pivot1_val = min_val;
        *pivot2_val = max_val + 1;
    }
    else if (range == 1)
    {
        *pivot1_val = min_val;
        *pivot2_val = min_val + 1;
    }
    else if (range == 2)
    {
        *pivot1_val = min_val + 1;
        *pivot2_val = min_val + 1;
    }
    else
    {
        *pivot1_val = min_val + range / 3;
        *pivot2_val = min_val + (2 * range) / 3;
        if (*pivot2_val > max_val)
            *pivot2_val = max_val + 1;
    }
}

// set_split_loc - unchanged, already compliant
void set_split_loc(t_loc loc, t_chunk *min, t_chunk *mid, t_chunk *max)
{
    if (loc == TOP_A || loc == BOTTOM_A)
    {
        min->loc = BOTTOM_B;
        mid->loc = TOP_B;
        max->loc = TOP_A;
    }
    else
    {
        if (loc == TOP_B)
        {
            min->loc = BOTTOM_B;
            mid->loc = BOTTOM_A;
            max->loc = TOP_A;
        }
        else
        { // loc == BOTTOM_B
            min->loc = TOP_B;
            mid->loc = BOTTOM_A;
            max->loc = TOP_A;
        }
    }
}

// a_partly_sort - unchanged, already compliant
t_bool a_partly_sort(t_tabs *data, int from_top_count)
{
    t_stack *a;
    t_node *current;
    int i;

    a = &data->a;
    if (!a->head || a->size < 2 || from_top_count > a->size)
        return (TRUE);
    if (from_top_count <= 1)
        return (check_partial_sort_loop(a->head, a->head));
    current = a->head;
    i = 1;
    while (i < from_top_count)
    {
        if (!current)
            return (FALSE);
        current = current->next;
        i++;
    }
    return (check_partial_sort_loop(current, a->head));
}

// split_max_reduction - unchanged, calls helpers now, already compliant
void split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest)
{
    t_chunk temp_chunk; // Only 1 variable + params

    if (max_chunk_dest->loc != TOP_A)
        return;

    if (max_chunk_dest->size >= 3 && data->a.size >= 3)
    {
        if (check_reduce_cond1(data) == TRUE)
        {
            temp_chunk.loc = TOP_A;
            temp_chunk.size = 3;
            sort_three(data, &temp_chunk);
            max_chunk_dest->size -= 3;
            return;
        }
    }
    if (max_chunk_dest->size >= 1 && data->a.size >= 3)
    {
        if (check_reduce_cond2(data) == TRUE)
        {
            move_and_print(data, SA);
            max_chunk_dest->size--;
            return;
        }
    }
    if (max_chunk_dest->size >= 1 && data->a.size >= 1)
    {
        if (check_reduce_cond3(data) == TRUE)
        {
            max_chunk_dest->size--;
            return;
        }
    }
}