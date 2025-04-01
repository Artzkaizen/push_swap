// File: ps_chunk_split.c
#include "push_swap.h"

/**
 * @brief Gets pivot values for chunk splitting.
 * @param data Pointer to t_tabs.
 * @param to_split Pointer to the chunk being split.
 * @param p1 Pointer to store pivot1 value.
 * @param p2 Pointer to store pivot2 value.
 * @return t_bool TRUE if pivots were set, FALSE if chunk is invalid/empty.
 */
t_bool get_split_params(t_tabs *data, t_chunk *to_split, int *p1, int *p2)
{
    int min_v;
    int max_v; // 2 variables + params

    min_v = chunk_min_value(data, to_split);
    max_v = chunk_max_value(data, to_split);
    if (min_v > max_v || to_split->size <= 0)
        return (FALSE);
    set_third_pivots_by_value(to_split, min_v, max_v, p1, p2);
    return (TRUE);
}

/**
 * @brief Processes the main loop of chunk_split, moving elements.
 * @param data Pointer to t_tabs.
 * @param to_split Pointer to the chunk being split.
 * @param dest Pointer to the destination structure.
 * @param p1 Pivot 1 value.
 * @param p2 Pivot 2 value.
 */
void process_split_loop(t_tabs *data, t_chunk *to_split,
                        t_split_dest *dest, int p1, int p2)
{
    int i;
    int next_v;
    int initial_size; // 3 variables + params

    initial_size = to_split->size; // Store initial size for loop limit
    i = 0;
    while (i < initial_size)
    {
        next_v = chunk_value(data, to_split, 1);
        if (next_v == FT_INT_MIN)
            break; // Error getting value
        if (next_v >= p2)
        {
            dest->max.size += move_from_to(data, to_split->loc, dest->max.loc);
            split_max_reduction(data, &dest->max);
        }
        else if (next_v >= p1)
            dest->mid.size += move_from_to(data, to_split->loc, dest->mid.loc);
        else
            dest->min.size += move_from_to(data, to_split->loc, dest->min.loc);
        i++;
    }
}

/**
 * @brief Splits a chunk into three sub-chunks (min, mid, max) based on pivots.
 *        Main entry point for splitting.
 * @param data Pointer to the t_tabs structure.
 * @param to_split Pointer to the chunk descriptor to be split.
 * @param dest Pointer to the t_split_dest structure to store the results.
 */
void chunk_split(t_tabs *data, t_chunk *to_split, t_split_dest *dest)
{
    int pivot1_val;
    int pivot2_val; // 2 variables + params

    // Get pivots; return if chunk is invalid
    if (get_split_params(data, to_split, &pivot1_val, &pivot2_val) == FALSE)
        return;
    // Process the elements using the loop helper
    process_split_loop(data, to_split, dest, pivot1_val, pivot2_val);
}