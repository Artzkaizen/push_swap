#include "../includes/push_swap.h"
#include "../includes/ft_printf.h"

/*
 * Print the stack
 */
void print_tabs(t_tabs *tabs) {
    int idx;
    int max;
    t_node *a = tabs->a.head;
    t_node *b = tabs->b.head;

    max = (tabs->a.size > tabs->b.size) ? tabs->a.size : tabs->b.size;
    idx = 0;
    ft_printf("stack \t\tstack b\n");
    ft_printf("-------\t\t-------\n");
    while (idx < max) {
        if (tabs->a.size > idx)
            ft_printf("a:[%d]\t", a->value);
        else
            ft_printf("a:[ ]\t");
        if (b && tabs->b.size > idx)
            ft_printf("b:[%d]\n", b->value);
        else
            ft_printf("b:[ ]\n");
        if (tabs->a.size > idx)
            a = a->next;
        if (b && tabs->b.size > idx)
            b = b->next;
        idx++;
    }
    ft_printf("-------\t\t-------\n");
    ft_printf("A = Size: %d\tMin: %d\tMax: %d\n", tabs->a.size, tabs->a.min, tabs->a.max);
    ft_printf("B = Size: %d\tMin: %d\tMax: %d\n", tabs->b.size, tabs->b.min, tabs->b.max);
}

/*
 * Free the tabs
 */
void free_tabs(t_tabs *tabs) {
    t_node *tmp;
    t_node *current;

    if (tabs->a.size > 0) {
        current = tabs->a.head;
        while (current->next != tabs->a.head) {
            tmp = current;
            current = current->next;
            free(tmp);
        }
    }
    if (tabs->b.size > 0) {
        current = tabs->b.head;
        while (current->next != tabs->b.head) {
            tmp = current;
            current = current->next;
            free(tmp);
        }
    }
    free(tabs);
}

/*
 * Create a stack from a string array
 */
t_tabs *create_tabs(char **str) {
    int len = 0;
    t_tabs *tabs = (t_tabs *)malloc(sizeof(t_tabs));
    if (!tabs) return NULL;
    while (str[len]) len++;
    if (!(fill_stack(&tabs->a, str, len) && fill_stack(&tabs->b, NULL, len)))
        return free_tabs(tabs), NULL;
    return tabs;
}

/*
 * Sort 3 elements in stack A
 */
void sort_three(t_tabs *tabs) {
    t_node *a = tabs->a.head;
    int first = a->value;
    int second = a->next->value;
    int third = a->next->next->value;

    if (first > second && second < third && first < third)
        move_stack(tabs, SA);
    else if (first > second && second > third)
        move_stack(tabs, SA), move_stack(tabs, RRA);
    else if (first > second && second < third && first > third)
        move_stack(tabs, RA);
    else if (first < second && second > third && first < third)
        move_stack(tabs, SA), move_stack(tabs, RA);
    else if (first < second && second > third && first > third)
        move_stack(tabs, RRA);
}

/*
 * Find position of the smallest element in stack
 */
int find_smallest_pos(t_stack *stack) {
    int pos = 0, min_pos = 0, min_val = stack->head->value;
    t_node *current = stack->head;
    while (current->next != stack->head) {
        if (current->value < min_val) {
            min_val = current->value;
            min_pos = pos;
        }
        current = current->next;
        pos++;
    }
    return min_pos;
}

/*
 * Push smallest elements to stack B
 */
void push_smallest_to_b(t_tabs *tabs, int count) {
    while (count-- > 0 && tabs->a.size > 3) {
        int smallest_pos = find_smallest_pos(&tabs->a);
        int rotations = smallest_pos;

        // Optimized rotation direction
        if (smallest_pos > tabs->a.size / 2)
            rotations = smallest_pos - tabs->a.size;

        if (rotations >= 0) {
            while (rotations-- > 0)
                move_stack(tabs, RA);
        } else {
            while (rotations++ < 0)
                move_stack(tabs, RRA);
        }
        move_stack(tabs, PB);
    }
}

/*
 * Main sorting function
 */
void push_swap(t_tabs *tabs) {
    if (is_sorted(&tabs->a, ASCENDING)) return;

    // Special case for 2 elements
    if (tabs->a.size == 2 && tabs->a.head->value > tabs->a.head->next->value)
        move_stack(tabs, SA);

    // Special case for 3 elements
    else if (tabs->a.size == 3)
        sort_three(tabs);

    // For larger arrays, push smallest elements to B except for 3
    else {
        push_smallest_to_b(tabs, tabs->a.size - 3);
        sort_three(tabs);
        while (tabs->b.size > 0)
            move_stack(tabs, PA);
    }
}

