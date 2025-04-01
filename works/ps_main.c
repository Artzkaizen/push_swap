// File: ps_main.c
#include "push_swap.h"

// Definition of the global variable
int g_move_count = 0;

/**
 * @brief Main entry point for the chunk sort algorithm. Rotates stack A at the end.
 * @param data Pointer to the t_tabs structure.
 */
void chunk_sort(t_tabs *data)
{
    t_chunk chunk_all; // 1 var + param (main function)
    int min_pos_val;   // Renamed from min_pos

    chunk_all.loc = TOP_A;
    chunk_all.size = data->a.size;
    rec_chunk_sort(data, &chunk_all); // Start recursion
    // Final rotation using helpers
    if (data->a.size > 0 && data->a.head->value != 0)
    {
        if (find_min_pos(&data->a, &min_pos_val) == TRUE) // Find pos of 0
        {
            rotate_a_to_top(data, min_pos_val); // Rotate if needed
        }
    }
}

/**
 * @brief Orchestrates the sorting strategy.
 * @param data Pointer to the t_tabs structure.
 */
void sort_strategy(t_tabs *data)
{
    // If stack A has 0 or 1 elements, or is already sorted, do nothing.
    if (data->a.size <= 1 || is_raw_input_sorted(&data->a) == TRUE)
        return;
    // Normalize the stack
    normalize_stack(&data->a);
    // Sort if needed
    if (data->a.size > 1)
        chunk_sort(data);
}

/**
 * @brief Main function. Parses arguments, initializes, sorts, prints count, cleans up.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, 1 on error.
 */
int main(int argc, char **argv)
{
    t_tabs *tabs; // 1 variable

    if (argc < 2)
        return (0);
    tabs = create_tabs(argv + 1, argc - 1);
    if (!tabs)
        return (1);
    g_move_count = 0; // Reset global counter
    sort_strategy(tabs);
    fprintf(stderr, "Total moves: %d\n", g_move_count);
    free_tabs(tabs);
    return (0);
}