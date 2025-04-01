// File: ps_chunk_minmax.c
#include "push_swap.h"

/**
 * @brief Comparison function for finding minimum.
 * @param current_val The value of the current node being checked.
 * @param result Pointer to the current minimum found so far.
 * @return t_bool TRUE if current_val is smaller (update needed), FALSE otherwise.
 */
t_bool compare_min(int current_val, int *result)
{
    if (current_val < *result)
        return (TRUE);
    return (FALSE);
}

/**
 * @brief Comparison function for finding maximum.
 * @param current_val The value of the current node being checked.
 * @param result Pointer to the current maximum found so far.
 * @return t_bool TRUE if current_val is larger (update needed), FALSE otherwise.
 */
t_bool compare_max(int current_val, int *result)
{
    if (current_val > *result)
        return (TRUE);
    return (FALSE);
}

/**
 * @brief Iterates through a chunk, applying a comparison function to find min/max.
 * @param data Pointer to t_tabs (needed for loc_to_stack).
 * @param chunk Pointer to the chunk descriptor.
 * @param compare Function pointer to the comparison logic (compare_min or compare_max).
 * @param initial_val The initial value for the result (FT_INT_MAX for min, FT_INT_MIN for max).
 * @return The final result (min or max value found).
 */
int iterate_chunk_and_compare(t_tabs *data, t_chunk *chunk,
                              t_bool (*compare)(int, int *), int initial_val)
{
    t_stack *stk;
    t_node *current;
    int result;
    int i; // Only 3 variables + parameters

    stk = loc_to_stack(data, chunk->loc);
    if (!stk || !stk->head || chunk->size <= 0)
        return (initial_val); // Return initial if chunk invalid
    result = initial_val;
    i = 0;
    if (chunk->loc == TOP_A || chunk->loc == TOP_B)
    {
        current = stk->head;
        while (i < chunk->size && i < stk->size)
        {                                         // Iterate chunk or stack size
            if (compare(current->value, &result)) // Call compare function
                result = current->value;          // Update result if needed
            current = current->next;
            i++;
        }
    }
    else
    { // BOTTOM_A or BOTTOM_B
        current = stk->head->prev;
        while (i < chunk->size && i < stk->size)
        {
            if (compare(current->value, &result))
                result = current->value;
            current = current->prev;
            i++;
        }
    }
    return (result);
}

/**
 * @brief Finds the minimum normalized value within a specified chunk.
 * @param data Pointer to the t_tabs structure.
 * @param chunk Pointer to the t_chunk descriptor.
 * @return The minimum normalized `value` found, or FT_INT_MAX if chunk is empty/invalid.
 */
int chunk_min_value(t_tabs *data, t_chunk *chunk)
{
    return (iterate_chunk_and_compare(data, chunk, compare_min, FT_INT_MAX));
}

/**
 * @brief Finds the maximum normalized value within a specified chunk.
 * @param data Pointer to the t_tabs structure.
 * @param chunk Pointer to the t_chunk descriptor.
 * @return The maximum normalized `value` found, or FT_INT_MIN if chunk is empty/invalid.
 */
int chunk_max_value(t_tabs *data, t_chunk *chunk)
{
    return (iterate_chunk_and_compare(data, chunk, compare_max, FT_INT_MIN));
}