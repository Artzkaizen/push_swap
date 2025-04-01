#include "./push_swap.h"

void sa(t_tabs *t)
{
    int temp_value;
    int temp_orig;
    t_node *first_node;

    first_node = t->a.head;
    temp_value = first_node->value;
    temp_orig = first_node->orig;
    if (t->a.size < 2)
        return;
    first_node->value = first_node->next->value;
    first_node->orig = first_node->next->orig;
    first_node->next->value = temp_value;
    first_node->next->orig = temp_orig;
}
void sb(t_tabs *t)
{
    int temp_value;
    int temp_orig;
    t_node *first_node;

    first_node = t->b.head;
    temp_value = first_node->value;
    temp_orig = first_node->orig;
    if (t->b.size < 2)
        return;
    first_node->value = first_node->next->value;
    first_node->orig = first_node->next->orig;
    first_node->next->value = temp_value;
    first_node->next->orig = temp_orig;
}

void ss(t_tabs *t)
{
    sa(t);
    sb(t);
}
void pa(t_tabs *t)
{
    t_node *head;
    head = t->b.head;
    if (t->b.size == 0)
        return;
    if (t->b.size == 1)
        t->b.head = NULL;
    else
    {
        t->b.head = head->next;
        head->prev->next = head->next;
        head->next->prev = head->prev;
    }
    t->b.size--;
    if (t->a.size == 0)
    {
        t->a.head = head;
        head->next = head;
        head->prev = head;
    }
    else
    {
        head->next = t->a.head;
        head->prev = t->a.head->prev;
        t->a.head->prev->next = head;
        t->a.head->prev = head;
        t->a.head = head;
    }
    t->a.size++;
}
void pb(t_tabs *t)
{
    if (t->a.size == 0)
        return;
    t_node *m = t->a.head;
    if (t->a.size == 1)
        t->a.head = NULL;
    else
    {
        t->a.head = m->next;
        m->prev->next = m->next;
        m->next->prev = m->prev;
    }
    t->a.size--;
    if (t->b.size == 0)
    {
        t->b.head = m;
        m->next = m;
        m->prev = m;
    }
    else
    {
        m->next = t->b.head;
        m->prev = t->b.head->prev;
        t->b.head->prev->next = m;
        t->b.head->prev = m;
        t->b.head = m;
    }
    t->b.size++;
}
void ra(t_tabs *t)
{
    if (t->a.size >= 2)
        t->a.head = t->a.head->next;
}
void rb(t_tabs *t)
{
    if (t->b.size >= 2)
        t->b.head = t->b.head->next;
}
void rr(t_tabs *t)
{
    ra(t);
    rb(t);
}
void rra(t_tabs *t)
{
    if (t->a.size >= 2)
        t->a.head = t->a.head->prev;
}
void rrb(t_tabs *t)
{
    if (t->b.size >= 2)
        t->b.head = t->b.head->prev;
}
void rrr(t_tabs *t)
{
    rra(t);
    rrb(t);
}

/**
 * @brief Executes a stack move and prints its name to standard output.
 * @param tabs Pointer to the t_tabs structure containing stacks A and B.
 * @param move The t_stack_move enum value representing the move to perform.
 * @return void
 * 
 */
void move_and_print(t_tabs *t, t_stack_move m)
{
    if (m == SA)
        (sa(t), printf("sa\n"));
    else if (m == SB)
        (sb(t), printf("sb\n"));
    else if (m == SS)
        (ss(t), printf("ss\n"));
    else if (m == PA)
        (pa(t), printf("pa\n"));
    else if (m == PB)
        (pb(t), printf("pb\n"));
    else if (m == RA)
        (ra(t), printf("ra\n"));
    else if (m == RB)
        (rb(t), printf("rb\n"));
    else if (m == RR)
        (rr(t), printf("rr\n"));
    else if (m == RRA)
        (rra(t), printf("rra\n"));
    else if (m == RRB)
        (rrb(t), printf("rrb\n"));
    else if (m == RRR)
        (rrr(t), printf("rrr\n"));
    g_move_count++;
}
