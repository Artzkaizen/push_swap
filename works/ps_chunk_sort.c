#include "push_swap.h"

void sort_one(t_tabs *data, t_chunk *to_sort)
{
    if (to_sort->loc != TOP_A)
        move_from_to(data, to_sort->loc, TOP_A);
}

void sort_two(t_tabs *data, t_chunk *to_sort)
{
    if (to_sort->loc != TOP_A) {
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
    }
    if (data->a.size >= 2 && data->a.head->value > data->a.head->next->value)
        move_and_print(data, SA);
}

void sort_three(t_tabs *data, t_chunk *to_sort)
{
    int v0, v1, v2;

    if (to_sort->loc != TOP_A) {
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
    }
    if (data->a.size < 3) return;
    v0 = data->a.head->value; v1 = data->a.head->next->value;
    if (data->a.size == 3) v2 = data->a.head->prev->value;
    else v2 = data->a.head->next->next->value;

    if (v0 > v1 && v1 < v2 && v0 < v2) move_and_print(data, SA);
    else if (v0 > v1 && v1 > v2) { move_and_print(data, SA); move_and_print(data, RRA); }
    else if (v0 > v1 && v1 < v2 && v0 > v2) move_and_print(data, RA);
    else if (v0 < v1 && v1 > v2 && v0 < v2) { move_and_print(data, SA); move_and_print(data, RA); }
    else if (v0 < v1 && v1 > v2 && v0 > v2) move_and_print(data, RRA);
}

void chunk_split(t_tabs *data, t_chunk *to_split, t_split_dest *dest)
{
    int p1, p2, min_v, max_v, init_size, i, next_v; // Variable names shortened

    min_v = chunk_min_value(data, to_split);
    max_v = chunk_max_value(data, to_split);
    init_size = to_split->size;
    if (min_v > max_v || init_size <= 0) return;
    set_third_pivots_by_value(to_split, min_v, max_v, &p1, &p2);
    i = 0;
    while (i < init_size) {
        next_v = chunk_value(data, to_split, 1);
        if (next_v == INT_MIN) break;
        if (next_v >= p2) {
            dest->max.size += move_from_to(data, to_split->loc, dest->max.loc);
            split_max_reduction(data, &dest->max);
        } else if (next_v >= p1) {
            dest->mid.size += move_from_to(data, to_split->loc, dest->mid.loc);
        } else {
            dest->min.size += move_from_to(data, to_split->loc, dest->min.loc);
        }
        i++;
    }
}

void rec_chunk_sort(t_tabs *data, t_chunk *to_sort)
{
    t_split_dest dest;

    if (to_sort->size <= 3) {
        if (to_sort->size == 3) sort_three(data, to_sort);
        else if (to_sort->size == 2) sort_two(data, to_sort);
        else if (to_sort->size == 1) sort_one(data, to_sort);
        return;
    }
    innit_size(&dest.min, &dest.mid, &dest.max);
    set_split_loc(to_sort->loc, &dest.min, &dest.mid, &dest.max);
    chunk_split(data, to_sort, &dest);
    rec_chunk_sort(data, &dest.max);
    rec_chunk_sort(data, &dest.mid);
    rec_chunk_sort(data, &dest.min);
}