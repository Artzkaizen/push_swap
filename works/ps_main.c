#include "push_swap.h"

// Definition of the global variable
int g_move_count = 0;

void chunk_sort(t_tabs *data)
{
    t_chunk chunk_all;
    t_node *current;
    int min_pos;
    int current_pos;
    int i;

    chunk_all.loc = TOP_A;
    chunk_all.size = data->a.size;
    rec_chunk_sort(data, &chunk_all);

    if (data->a.size > 0 && data->a.head->value != 0)
    {
        current = data->a.head;
        min_pos = -1;
        current_pos = 0;
        while (current_pos < data->a.size)
        {
            if (current->value == 0)
            {
                min_pos = current_pos;
                break;
            }
            current = current->next;
            current_pos++;
            if (current == data->a.head && current_pos > 0)
                break;
        }
        if (min_pos > 0)
        {
            if (min_pos <= data->a.size / 2)
            {
                i = 0;
                while (i < min_pos)
                {
                    move_and_print(data, RA);
                    i++;
                }
            }
            else
            {
                i = 0;
                while (i < data->a.size - min_pos)
                {
                    move_and_print(data, RRA);
                    i++;
                }
            }
        }
    }
}

void sort_strategy(t_tabs *data)
{
    if (data->a.size <= 1 || is_raw_input_sorted(&data->a) == TRUE)
        return;
    normalize_stack(&data->a);
    if (data->a.size > 1)
        chunk_sort(data);
}

int main(int argc, char **argv)
{
    t_tabs *tabs;

    if (argc < 2)
        return (0);
    tabs = create_tabs(argv + 1, argc - 1);
    if (!tabs)
        return (1);
    g_move_count = 0;
    sort_strategy(tabs);
    fprintf(stderr, "Total moves: %d\n", g_move_count);
    free_tabs(tabs);
    return (0);
}