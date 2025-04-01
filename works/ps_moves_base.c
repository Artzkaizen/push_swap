// File: ps_moves_base.c
#include "push_swap.h"

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void sa(t_tabs *tabs)
{
    t_node *f;
    int v;
    int o;

    if (!tabs || tabs->a.size < 2)
        return;
    f = tabs->a.head;
    v = f->value;
    o = f->orig;
    f->value = f->next->value;
    f->orig = f->next->orig;
    f->next->value = v;
    f->next->orig = o;
}

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void sb(t_tabs *tabs)
{
    t_node *f;
    int v;
    int o;

    if (!tabs || tabs->b.size < 2)
        return;
    f = tabs->b.head;
    v = f->value;
    o = f->orig;
    f->value = f->next->value;
    f->orig = f->next->orig;
    f->next->value = v;
    f->next->orig = o;
}

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void ss(t_tabs *tabs)
{
    if (!tabs) return;
    sa(tabs); // Calls sa defined above
    sb(tabs); // Calls sb defined above
}

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void pa(t_tabs *tabs)
{
    t_node *m;

    if (!tabs || tabs->b.size == 0)
        return;
    m = tabs->b.head;
    if (tabs->b.size == 1)
        tabs->b.head = NULL;
    else
    {
        tabs->b.head = m->next;
        m->prev->next = m->next;
        m->next->prev = m->prev;
    }
    tabs->b.size--;
    if (tabs->a.size == 0)
    {
        tabs->a.head = m;
        m->next = m;
        m->prev = m;
    }
    else
    {
        m->next = tabs->a.head;
        m->prev = tabs->a.head->prev;
        tabs->a.head->prev->next = m;
        tabs->a.head->prev = m;
        tabs->a.head = m;
    }
    tabs->a.size++;
}

// <<<<< THIS FUNCTION NEEDS TO BE IN THIS FILE >>>>>
void pb(t_tabs *tabs)
{
    t_node *m;

    if (!tabs || tabs->a.size == 0)
        return;
    m = tabs->a.head;
    if (tabs->a.size == 1)
        tabs->a.head = NULL;
    else
    {
        tabs->a.head = m->next;
        m->prev->next = m->next;
        m->next->prev = m->prev;
    }
    tabs->a.size--;
    if (tabs->b.size == 0)
    {
        tabs->b.head = m;
        m->next = m;
        m->prev = m;
    }
    else
    {
        m->next = tabs->b.head;
        m->prev = tabs->b.head->prev;
        tabs->b.head->prev->next = m;
        tabs->b.head->prev = m;
        tabs->b.head = m;
    }
    tabs->b.size++;
}