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
        return (INT_MIN);
    i = 1;
    if (chunk->loc == TOP_A || chunk->loc == TOP_B)
    {
        current = stk->head;
        while (i < n) { current = current->next; i++; }
    }
    else
    {
        current = stk->head->prev;
        while (i < n) { current = current->prev; i++; }
    }
    return (current->value);
}

int chunk_min_value(t_tabs *data, t_chunk *chunk)
{
    t_stack *stk;
    t_node *current;
    int min_val;
    int current_val;
    int i;

    stk = loc_to_stack(data, chunk->loc);
    if (!stk || !stk->head || chunk->size <= 0) return (INT_MAX);
    min_val = INT_MAX;
    i = 0;
    if (chunk->loc == TOP_A || chunk->loc == TOP_B) {
        current = stk->head;
        while (i < chunk->size) {
            if (i >= stk->size) break;
            current_val = current->value;
            if (current_val < min_val) min_val = current_val;
            if (i < chunk->size - 1) current = current->next;
            i++;
        }
    } else {
        current = stk->head->prev;
        while (i < chunk->size) {
            if (i >= stk->size) break;
            current_val = current->value;
            if (current_val < min_val) min_val = current_val;
             if (i < chunk->size - 1) current = current->prev;
            i++;
        }
    }
    return (min_val);
}

int chunk_max_value(t_tabs *data, t_chunk *chunk)
{
    t_stack *stk;
    t_node *current;
    int max_val;
    int current_val;
    int i;

    stk = loc_to_stack(data, chunk->loc);
    if (!stk || !stk->head || chunk->size <= 0) return (INT_MIN);
    max_val = INT_MIN;
    i = 0;
    if (chunk->loc == TOP_A || chunk->loc == TOP_B) {
        current = stk->head;
        while (i < chunk->size) {
            if (i >= stk->size) break;
            current_val = current->value;
            if (current_val > max_val) max_val = current_val;
            if (i < chunk->size - 1) current = current->next;
            i++;
        }
    } else {
        current = stk->head->prev;
        while (i < chunk->size) {
             if (i >= stk->size) break;
            current_val = current->value;
            if (current_val > max_val) max_val = current_val;
            if (i < chunk->size - 1) current = current->prev;
            i++;
        }
    }
    return (max_val);
}

void innit_size(t_chunk *min, t_chunk *mid, t_chunk *max)
{
    min->size = 0;
    mid->size = 0;
    max->size = 0;
}