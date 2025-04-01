#include "push_swap.h"

void ra(t_tabs *tabs)
{
    if (!tabs || tabs->a.size < 2)
        return;
    tabs->a.head = tabs->a.head->next;
}

void rb(t_tabs *tabs)
{
    if (!tabs || tabs->b.size < 2)
        return;
    tabs->b.head = tabs->b.head->next;
}

void rr(t_tabs *tabs)
{
    if (!tabs) return;
    ra(tabs);
    rb(tabs);
}

void rra(t_tabs *tabs)
{
    if (!tabs || tabs->a.size < 2)
        return;
    tabs->a.head = tabs->a.head->prev;
}

void rrb(t_tabs *tabs)
{
    if (!tabs || tabs->b.size < 2)
        return;
    tabs->b.head = tabs->b.head->prev;
}