#include "push_swap.h"

t_node *create_node(int num)
{
    t_node *node;

    node = malloc(sizeof(t_node));
    if (!node)
        return (NULL);
    node->orig = num;
    node->value = 0;
    node->next = node;
    node->prev = node;
    return (node);
}

void append_node(t_stack *stack, t_node *node)
{
    t_node *tail;

    if (!stack || !node)
        return;
    if (!stack->head)
    {
        stack->head = node;
        node->next = node;
        node->prev = node;
    }
    else
    {
        tail = stack->head->prev;
        tail->next = node;
        node->prev = tail;
        node->next = stack->head;
        stack->head->prev = node;
    }
    stack->size++;
}

void free_stack(t_stack *stack)
{
    t_node *c;
    t_node *next_n;
    t_node *tail;

    if (!stack || !stack->head)
        return;
    tail = stack->head->prev;
    tail->next = NULL; // Break circle
    c = stack->head;
    while (c != NULL)
    {
        next_n = c->next;
        free(c);
        c = next_n;
    }
    stack->head = NULL;
    stack->size = 0;
}

void free_tabs(t_tabs *tabs)
{
    if (!tabs)
        return;
    free_stack(&tabs->a);
    free_stack(&tabs->b);
    free(tabs);
}

void swap_ints(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}