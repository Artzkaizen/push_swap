#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

/* --- Type Definitions --- */

typedef enum e_bool {
    FALSE = 0,
    TRUE = 1
} t_bool;

typedef enum e_stack_move {
    SA, SB, SS, PA, PB, RA, RB, RR, RRA, RRB, RRR
} t_stack_move;

typedef struct s_node {
    int value;         // normalized value (used for sorting comparisons)
    int orig;          // original value (for display purposes)
    struct s_node *next;
    struct s_node *prev;
} t_node;

typedef struct s_stack {
    t_node *head;
    int size;
} t_stack;

typedef struct s_tabs {
    t_stack a;
    t_stack b;
} t_tabs;

/* --- Global Move Count --- */
int g_move_count = 0;

/* --- Node / Stack Management --- */

t_node *create_node(int num) {
    t_node *node = malloc(sizeof(t_node));
    if (!node)
        return NULL;
    node->orig = num;
    node->value = num; // will be replaced by normalized value later
    node->next = node;
    node->prev = node;
    return node;
}

void append_node(t_stack *stack, t_node *node) {
    if (!stack->head)
        stack->head = node;
    else {
        t_node *tail = stack->head->prev;
        tail->next = node;
        node->prev = tail;
        node->next = stack->head;
        stack->head->prev = node;
    }
    stack->size++;
}

t_bool fill_stack(t_stack *stack, char **str, int len) {
    if (!str)
        return TRUE;
    for (int i = 0; i < len; i++) {
        errno = 0;
        int num = atoi(str[i]);
        if (errno != 0)
            return FALSE;
        t_node *node = create_node(num);
        if (!node)
            return FALSE;
        append_node(stack, node);
    }
    return TRUE;
}

void free_stack(t_stack *stack) {
    if (!stack || !stack->head)
        return;
    t_node *cur = stack->head;
    t_node *tmp;
    for (int i = 0; i < stack->size; i++) {
        tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    stack->head = NULL;
    stack->size = 0;
}

/* --- Basic Moves --- */

void sa(t_tabs *tabs) {
    if (tabs->a.size < 2)
        return;
    t_node *head = tabs->a.head;
    int tmp = head->value;
    head->value = head->next->value;
    head->next->value = tmp;
}

void sb(t_tabs *tabs) {
    if (tabs->b.size < 2)
        return;
    t_node *head = tabs->b.head;
    int tmp = head->value;
    head->value = head->next->value;
    head->next->value = tmp;
}

void ss(t_tabs *tabs) {
    sa(tabs);
    sb(tabs);
}

void pa(t_tabs *tabs) {
    if (tabs->b.size == 0)
        return;
    t_node *node = tabs->b.head;
    if (tabs->b.size == 1)
        tabs->b.head = NULL;
    else {
        tabs->b.head = node->next;
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    tabs->b.size--;
    if (tabs->a.size == 0) {
        tabs->a.head = node;
        node->next = node;
        node->prev = node;
    } else {
        node->next = tabs->a.head;
        node->prev = tabs->a.head->prev;
        tabs->a.head->prev->next = node;
        tabs->a.head->prev = node;
        tabs->a.head = node;
    }
    tabs->a.size++;
}

void pb(t_tabs *tabs) {
    if (tabs->a.size == 0)
        return;
    t_node *node = tabs->a.head;
    if (tabs->a.size == 1)
        tabs->a.head = NULL;
    else {
        tabs->a.head = node->next;
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    tabs->a.size--;
    if (tabs->b.size == 0) {
        tabs->b.head = node;
        node->next = node;
        node->prev = node;
    } else {
        node->next = tabs->b.head;
        node->prev = tabs->b.head->prev;
        tabs->b.head->prev->next = node;
        tabs->b.head->prev = node;
        tabs->b.head = node;
    }
    tabs->b.size++;
}

void ra(t_tabs *tabs) {
    if (tabs->a.size > 1)
        tabs->a.head = tabs->a.head->next;
}

void rb(t_tabs *tabs) {
    if (tabs->b.size > 1)
        tabs->b.head = tabs->b.head->next;
}

void rr(t_tabs *tabs) {
    ra(tabs);
    rb(tabs);
}

void rra(t_tabs *tabs) {
    if (tabs->a.size > 1)
        tabs->a.head = tabs->a.head->prev;
}

void rrb(t_tabs *tabs) {
    if (tabs->b.size > 1)
        tabs->b.head = tabs->b.head->prev;
}

void rrr(t_tabs *tabs) {
    rra(tabs);
    rrb(tabs);
}

/* --- Move Wrapper (Prints move and increments counter) --- */

void move_stack(t_tabs *tabs, t_stack_move move) {
    switch (move) {
        case SA: printf("sa\n"); sa(tabs); break;
        case SB: printf("sb\n"); sb(tabs); break;
        case SS: printf("ss\n"); ss(tabs); break;
        case PA: printf("pa\n"); pa(tabs); break;
        case PB: printf("pb\n"); pb(tabs); break;
        case RA: printf("ra\n"); ra(tabs); break;
        case RB: printf("rb\n"); rb(tabs); break;
        case RR: printf("rr\n"); rr(tabs); break;
        case RRA: printf("rra\n"); rra(tabs); break;
        case RRB: printf("rrb\n"); rrb(tabs); break;
        case RRR: printf("rrr\n"); rrr(tabs); break;
    }
    g_move_count++;
}

/* --- Normalization --- 
   This function replaces each node's value with its rank in the sorted order.
*/
void normalize_stack(t_tabs *tabs) {
    int size = tabs->a.size;
    int *values = malloc(sizeof(int) * size);
    int *sorted = malloc(sizeof(int) * size);
    if (!values || !sorted)
        exit(EXIT_FAILURE);
    
    t_node *cur = tabs->a.head;
    for (int i = 0; i < size; i++) {
        values[i] = cur->orig;
        sorted[i] = cur->orig;
        cur = cur->next;
    }
    
    // Simple bubble sort (or you may use mergeSort for efficiency)
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                int tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }
    
    cur = tabs->a.head;
    while (cur) {
        // Find the index of cur->orig in sorted array
        for (int i = 0; i < size; i++) {
            if (sorted[i] == cur->orig) {
                cur->value = i;
                break;
            }
        }
        cur = (cur->next == tabs->a.head) ? NULL : cur->next;
    }
    
    free(values);
    free(sorted);
}

/* --- Radix Sort Implementation --- 
   For each bit from least-significant to most-significant,
   push numbers with a 0 in that bit to stack B; otherwise rotate.
   Then push everything from B back to A.
*/
void radix_sort(t_tabs *tabs) {
    int size = tabs->a.size;
    int max = size - 1;  // After normalization, max value equals size - 1
    int max_bits = 0;
    while ((max >> max_bits) != 0)
        max_bits++;
    
    for (int i = 0; i < max_bits; i++) {
        int count = tabs->a.size;
        for (int j = 0; j < count; j++) {
            // Check the i-th bit of the top value
            if (((tabs->a.head->value >> i) & 1) == 1)
                move_stack(tabs, RA);
            else
                move_stack(tabs, PB);
        }
        // Move all elements from B back to A.
        while (tabs->b.size > 0)
            move_stack(tabs, PA);
    }
}

/* --- Stack Initialization and Freeing --- */

t_tabs *create_tabs(char **str) {
    int len = 0;
    while (str[len])
        len++;
    t_tabs *tabs = malloc(sizeof(t_tabs));
    if (!tabs)
        return NULL;
    tabs->a.head = NULL;
    tabs->b.head = NULL;
    tabs->a.size = 0;
    tabs->b.size = 0;
    if (!fill_stack(&tabs->a, str, len)) {
        free(tabs);
        return NULL;
    }
    return tabs;
}

void free_tabs(t_tabs *tabs) {
    if (!tabs)
        return;
    free_stack(&tabs->a);
    free_stack(&tabs->b);
    free(tabs);
}

/* --- Debug Printing --- */

void print_tabs(t_tabs *tabs) {
    printf("stack a\t\tstack b\n");
    printf("-------\t\t-------\n");
    t_node *a = tabs->a.head;
    t_node *b = tabs->b.head;
    int max = (tabs->a.size > tabs->b.size) ? tabs->a.size : tabs->b.size;
    for (int i = 0; i < max; i++) {
        if (i < tabs->a.size)
            printf("a:[%d]\t", a->orig);
        else
            printf("a:[ ]\t");
        if (i < tabs->b.size)
            printf("b:[%d]\n", b->orig);
        else
            printf("b:[ ]\n");
        if (i < tabs->a.size)
            a = a->next;
        if (i < tabs->b.size)
            b = b->next;
    }
    printf("-------\t\t-------\n");
    printf("A = Size: %d\n", tabs->a.size);
    printf("B = Size: %d\n", tabs->b.size);
}

/* --- Main Function --- */

int main(int argc, char **argv) {
    if (argc < 2)
        return 0;
    t_tabs *tabs = create_tabs(argv + 1);
    if (!tabs)
        return 0;
    
    g_move_count = 0;
    normalize_stack(tabs);
    radix_sort(tabs);
    
    printf("Total moves: %d\n", g_move_count);
    print_tabs(tabs);
    free_tabs(tabs);
    return 0;
}
