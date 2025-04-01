#include "./push_swap.h"

void sort_chunk_one(t_tabs *stacks, t_chunk *chunk_to_sort)
{
    if (chunk_to_sort->location != LOC_TOP_A)
        move_element(stacks, chunk_to_sort->location, LOC_TOP_A);
}

void sort_chunk_two(t_tabs *stacks, t_chunk *chunk_to_sort)
{
    if (chunk_to_sort->location != LOC_TOP_A)
    {
        move_element(stacks, chunk_to_sort->location, LOC_TOP_A);
        move_element(stacks, chunk_to_sort->location, LOC_TOP_A);
    }
    if (stacks->a.size >= 2 && stacks->a.head->value > stacks->a.head->next->value)
        move_and_print(stacks, SA);
}

void sort_chunk_three(t_tabs *stacks, t_chunk *chunk_to_sort)
{
    if (chunk_to_sort->location != LOC_TOP_A)
    {
        move_element(stacks, chunk_to_sort->location, LOC_TOP_A);
        move_element(stacks, chunk_to_sort->location, LOC_TOP_A);
        move_element(stacks, chunk_to_sort->location, LOC_TOP_A);
    }
    if (stacks->a.size < 3)
        return;
    if (is_stack_sorted(&stacks->a))
        return; // Simplified check assuming size 3 if called

    int v0 = stacks->a.head->value;
    int v1 = stacks->a.head->next->value;
    int v2 = stacks->a.head->prev->value;
    if (v0 > v1 && v1 < v2 && v0 < v2)
        move_and_print(stacks, SA);
    else if (v0 > v1 && v1 > v2)
    {
        move_and_print(stacks, SA);
        move_and_print(stacks, RRA);
    }
    else if (v0 > v1 && v1 < v2 && v0 > v2)
        move_and_print(stacks, RA);
    else if (v0 < v1 && v1 > v2 && v0 < v2)
    {
        move_and_print(stacks, SA);
        move_and_print(stacks, RA);
    }
    else if (v0 < v1 && v1 > v2 && v0 > v2)
        move_and_print(stacks, RRA);
}

void rotate_stack_a_to_value_zero(t_tabs *stacks)
{
    if (stacks->a.size <= 1 || !stacks->a.head || stacks->a.head->value == 0)
        return;

    t_node *current = stacks->a.head;
    int min_pos = -1;
    int current_pos = 0;
    // Find position using while loop
    t_bool found = FALSE;
    while (current_pos < stacks->a.size && !found)
    {
        if (current->value == 0)
        {
            min_pos = current_pos;
            found = TRUE;
        }
        else
        {
            current = current->next;
            current_pos++;
        }
    }

    if (min_pos > 0)
    {
        if (min_pos <= stacks->a.size / 2)
        {
            int i = 0;
            while (i < min_pos)
            {
                move_and_print(stacks, RA);
                i++;
            }
        }
        else
        {
            int i = 0;
            while (i < stacks->a.size - min_pos)
            {
                move_and_print(stacks, RRA);
                i++;
            }
        }
    }
}

void chunk_sort(t_tabs *stacks)
{
    t_chunk initial_chunk;
    initial_chunk.location = LOC_TOP_A;
    initial_chunk.size = stacks->a.size;
    sort_chunk_recursive(stacks, &initial_chunk);
    rotate_stack_a_to_value_zero(stacks);
}

void push_swap(t_tabs *stacks)
{
    if (stacks->a.size <= 1 || is_input_sorted(&stacks->a))
        return;
    normalize_stack(&stacks->a);
    if (is_stack_sorted(&stacks->a))
        return;
    chunk_sort(stacks);
}
