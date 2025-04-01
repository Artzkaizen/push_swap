#include "./push_swap.h"

void free_tabs(t_tabs *tabs)
{
    if (!tabs)
        return;
    free_stack(&tabs->a);
    free_stack(&tabs->b);
    free(tabs);
}

t_tabs *create_tabs(char **str)
{
    int len;

    len = 0;
    t_tabs *tabs = malloc(sizeof(t_tabs));
    if (!tabs)
        return NULL;
    while (str[len])
        len++;
    tabs->a.head = NULL;
    tabs->a.size = 0;
    tabs->b.head = NULL;
    tabs->b.size = 0;
    if (!fill_stack(&tabs->a, str, len))
        return (free_tabs(tabs), NULL);
    return tabs;
}