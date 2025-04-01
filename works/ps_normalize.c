// File: ps_normalize.c
#include "push_swap.h"

/**
 * @brief Copies original values from stack nodes to an integer array.
 * @param a Pointer to the stack.
 * @param arr Pointer to the destination integer array.
 */
void copy_orig_to_array(t_stack *a, int *arr)
{
    t_node *c;
    int i;

    c = a->head;
    i = 0;
    while (i < a->size)
    {
        arr[i] = c->orig;
        c = c->next;
        i++;
    }
}

/**
 * @brief Sorts an integer array using Bubble Sort.
 * @param arr Pointer to the integer array.
 * @param size The number of elements in the array.
 */
void sort_array(int *arr, int size)
{
    t_bool s; // Use t_bool
    int i;
    int j;

    i = 0;
    while (i < size - 1)
    {
        s = FALSE; // Initialize swap flag
        j = 0;
        while (j < size - 1 - i)
        {
            if (arr[j] > arr[j + 1])
            {
                swap_ints(&arr[j], &arr[j + 1]);
                s = TRUE; // Set flag if swapped
            }
            j++;
        }
        if (s == FALSE) // Optimization: stop if no swaps in a pass
            break;
        i++;
    }
}

/**
 * @brief Assigns normalized index values (from sorted array) back to stack nodes.
 * @param a Pointer to the stack.
 * @param arr Pointer to the sorted integer array containing original values.
 */
void assign_normalized_values(t_stack *a, int *arr)
{
    t_node *c;
    int i;
    int j;

    c = a->head;
    i = 0;
    while (i < a->size)
    {
        j = 0;
        while (j < a->size)
        {
            if (c->orig == arr[j])
            {
                c->value = j; // Assign the index as the normalized value
                break;        // Found the match, move to the next node
            }
            j++;
        }
        c = c->next;
        i++;
    }
}

/**
 * @brief Normalizes the stack values main function.
 * @param a Pointer to the stack (usually stack A) to normalize.
 */
void normalize_stack(t_stack *a)
{
    int *arr; // Only variable needed here besides parameter

    if (!a || a->size <= 1)
        return;
    // Allocate temporary array
    arr = malloc(sizeof(int) * a->size);
    if (!arr)
    {
        fprintf(stderr, "Error: Malloc failed during normalization\n");
        exit(1); // Or handle error differently
    }
    copy_orig_to_array(a, arr);       // Helper 1
    sort_array(arr, a->size);         // Helper 2
    assign_normalized_values(a, arr); // Helper 3
    free(arr);                        // Free temp array
}