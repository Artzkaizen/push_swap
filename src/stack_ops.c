#include "../includes/push_swap.h"

t_bool ft_add_node(t_node **list, int value, t_node *new)
{
    t_node *new_node;

    if (new)
        new_node = new;
    else {
        new_node = (t_node *)malloc(sizeof(t_node));
        if (!new_node)
            return (FALSE);
    }
    new_node->value = value;

    if (!*list) {
        *list = new_node;
        new_node->next = new_node;
        new_node->prev = new_node;
    } else {
        new_node->next = *list;
        new_node->prev = (*list)->prev;
        (*list)->prev->next = new_node;
        (*list)->prev = new_node;
    }
    return (TRUE);
}

void free_stack(t_stack *stack)
{
    if (!stack || !stack->head)
        return;
    
    t_node *current = stack->head;
    t_node *next;
    
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != stack->head);
    
    stack->head = NULL;
    stack->size = 0;
    stack->min = 0;
    stack->max = 0;
} 