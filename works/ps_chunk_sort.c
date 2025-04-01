// File: ps_chunk_sort.c
#include "push_swap.h"

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void sort_one(t_tabs *data, t_chunk *to_sort)
{
    if (to_sort->loc != TOP_A)
        move_from_to(data, to_sort->loc, TOP_A);
}

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void sort_two(t_tabs *data, t_chunk *to_sort)
{
    if (to_sort->loc != TOP_A)
    {
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
    }
    if (data->a.size >= 2 && data->a.head->value > data->a.head->next->value)
        move_and_print(data, SA);
}

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void sort_three(t_tabs *data, t_chunk *to_sort)
{
    int v0;
    int v1;
    int v2; // 3 variables + params

    if (to_sort->loc != TOP_A)
    {
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
    }
    if (data->a.size < 3)
        return;
    v0 = data->a.head->value;
    v1 = data->a.head->next->value;
    if (data->a.size == 3)
        v2 = data->a.head->prev->value;
    else
        v2 = data->a.head->next->next->value;
    if (v0 > v1 && v1 < v2 && v0 < v2)
        move_and_print(data, SA);
    else if (v0 > v1 && v1 > v2)
    {
        move_and_print(data, SA);
        move_and_print(data, RRA);
    }
    else if (v0 > v1 && v1 < v2 && v0 > v2)
        move_and_print(data, RA);
    else if (v0 < v1 && v1 > v2 && v0 < v2)
    {
        move_and_print(data, SA);
        move_and_print(data, RA);
    }
    else if (v0 < v1 && v1 > v2 && v0 > v2)
        move_and_print(data, RRA);
}

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void rec_chunk_sort(t_tabs *data, t_chunk *to_sort)
{
    t_split_dest dest; // 1 variable + params

    if (to_sort->size <= 3)
    {
        if (to_sort->size == 3)
            sort_three(data, to_sort);
        else if (to_sort->size == 2)
            sort_two(data, to_sort);
        else if (to_sort->size == 1)
            sort_one(data, to_sort);
        return;
    }
    innit_size(&dest.min, &dest.mid, &dest.max);
    set_split_loc(to_sort->loc, &dest.min, &dest.mid, &dest.max);
    chunk_split(data, to_sort, &dest); // Call the function in ps_chunk_split.c
    rec_chunk_sort(data, &dest.max);
    rec_chunk_sort(data, &dest.mid);
    rec_chunk_sort(data, &dest.min);
}