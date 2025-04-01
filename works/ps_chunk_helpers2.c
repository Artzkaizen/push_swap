#include "push_swap.h"

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

t_bool a_partly_sort(t_tabs *data, int from_top_count)
{
    t_stack *a;
    t_node *current;
    t_node *runner;
    int expected_value;
    int i;

    a = &data->a;
    if (!a->head || a->size < 2 || from_top_count > a->size)
        return (TRUE);
    if (from_top_count <= 1)
    {
        current = a->head;
        expected_value = current->value;
        runner = current->next;
        while (runner != a->head)
        {
            expected_value++;
            if (runner->value != expected_value)
                return (FALSE);
            runner = runner->next;
        }
        return (TRUE);
    }
    current = a->head;
    i = 1;
    while (i < from_top_count)
    {
        current = current->next;
        i++;
    }
    expected_value = current->value;
    runner = current->next;
    while (runner != a->head)
    {
        expected_value++;
        if (runner->value != expected_value)
            return (FALSE);
        runner = runner->next;
    }
    return (TRUE);
}

void split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest)
{
    t_stack *a;
    int v0, v1, v2;
    int total_size;
    int n_m1, n_m2, n_m3; // Shorter names
    t_bool top_3_max;
    t_chunk temp_chunk;

    a = &data->a;
    if (max_chunk_dest->loc != TOP_A)
        return;
    if (max_chunk_dest->size >= 3 && a->size >= 3)
    {
        v0 = a->head->value;
        v1 = a->head->next->value;
        v2 = a->head->next->next->value;
        total_size = data->a.size + data->b.size;
        n_m1 = total_size - 1;
        n_m2 = total_size - 2;
        n_m3 = total_size - 3;
        top_3_max = ((v0 == n_m1 || v0 == n_m2 || v0 == n_m3) && (v1 == n_m1 || v1 == n_m2 || v1 == n_m3) &&
                     (v2 == n_m1 || v2 == n_m2 || v2 == n_m3) && (v0 != v1 && v0 != v2 && v1 != v2));
        if (top_3_max == TRUE && a_partly_sort(data, 4) == TRUE)
        {
            temp_chunk.loc = TOP_A;
            temp_chunk.size = 3;
            sort_three(data, &temp_chunk);
            max_chunk_dest->size -= 3;
            return;
        }
    }
    if (max_chunk_dest->size >= 1 && a->size >= 3)
    {
        if (a->head->next->value == a->head->next->next->value - 1 &&
            a->head->value > a->head->next->value && a_partly_sort(data, 3) == TRUE)
        {
            move_and_print(data, SA);
            max_chunk_dest->size--;
            return;
        }
    }
    if (max_chunk_dest->size >= 1 && a->size >= 1)
    {
        if (a_partly_sort(data, 2) == TRUE)
        {
            if (a->size == 1 || (a->size > 1 && a->head->value == a->head->next->value - 1))
            {
                max_chunk_dest->size--;
                return;
            }
        }
    }
}