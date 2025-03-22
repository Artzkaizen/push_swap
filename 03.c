#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

/* --- Type Definitions --- */

typedef enum e_bool {
    FALSE = 0,
    TRUE = 1
} t_bool;

typedef enum e_sort_order {
    ASCENDING,
    DESCENDING
} t_sort_order;

typedef enum e_stack_move {
    SA, SB, SS, PA, PB, RA, RB, RR, RRA, RRB, RRR
} t_stack_move;

typedef struct s_node {
    int value;         // normalized value (used for sorting comparisons)
    int orig;          // original value (preserved for display)
    struct s_node *next;
    struct s_node *prev;
} t_node;

typedef struct s_stack {
    t_node *head;
    int size;
    int min;
    int max;
} t_stack;

typedef struct s_tabs {
    t_stack a;
    t_stack b;
} t_tabs;

/* --- Global Move Count --- */
int g_move_count = 0;

/* --- Node / Stack Management --- */

/*
 * create_node:
 * Allocate a new node with both orig and value set to num.
 */
t_node *create_node(int num) {
    t_node *node = malloc(sizeof(t_node));
    if (!node)
        return NULL;
    node->orig = num;
    node->value = num;
    node->next = node;
    node->prev = node;
    return node;
}

/*
 * append_node:
 * Append a node to the circular doubly‑linked list in the given stack.
 */
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

/*
 * fill_stack:
 * Convert the array of input strings into nodes that are appended to the stack.
 */
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

/*
 * free_stack:
 * Free every node in the given stack.
 */
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
    t_node *head = tabs->a.head;
    if (tabs->a.size < 2 || !head || !head->next)
        return;
    int tmp = head->value;
    head->value = head->next->value;
    head->next->value = tmp;
}

void sb(t_tabs *tabs) {
    t_node *head = tabs->b.head;
    if (tabs->b.size < 2 || !head || !head->next)
        return;
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
    t_node *b_head = tabs->b.head;
    /* Remove node from b */
    if (tabs->b.size == 1)
        tabs->b.head = NULL;
    else {
        tabs->b.head = b_head->next;
        b_head->prev->next = b_head->next;
        b_head->next->prev = b_head->prev;
    }
    tabs->b.size--;
    /* Insert node into a */
    if (tabs->a.size == 0) {
        tabs->a.head = b_head;
        b_head->next = b_head;
        b_head->prev = b_head;
    } else {
        b_head->next = tabs->a.head;
        b_head->prev = tabs->a.head->prev;
        tabs->a.head->prev->next = b_head;
        tabs->a.head->prev = b_head;
        tabs->a.head = b_head;
    }
    tabs->a.size++;
}

void pb(t_tabs *tabs) {
    if (tabs->a.size == 0)
        return;
    t_node *a_head = tabs->a.head;
    /* Remove node from a */
    if (tabs->a.size == 1)
        tabs->a.head = NULL;
    else {
        tabs->a.head = a_head->next;
        a_head->prev->next = a_head->next;
        a_head->next->prev = a_head->prev;
    }
    tabs->a.size--;
    /* Insert node into b */
    if (tabs->b.size == 0) {
        tabs->b.head = a_head;
        a_head->next = a_head;
        a_head->prev = a_head;
    } else {
        a_head->next = tabs->b.head;
        a_head->prev = tabs->b.head->prev;
        tabs->b.head->prev->next = a_head;
        tabs->b.head->prev = a_head;
        tabs->b.head = a_head;
    }
    tabs->b.size++;
}

void ra(t_tabs *tabs) {
    if (tabs->a.size < 2 || !tabs->a.head)
        return;
    tabs->a.head = tabs->a.head->next;
}

void rb(t_tabs *tabs) {
    if (tabs->b.size < 2 || !tabs->b.head)
        return;
    tabs->b.head = tabs->b.head->next;
}

void rr(t_tabs *tabs) {
    ra(tabs);
    rb(tabs);
}

void rra(t_tabs *tabs) {
    if (tabs->a.size < 2 || !tabs->a.head)
        return;
    tabs->a.head = tabs->a.head->prev;
}

void rrb(t_tabs *tabs) {
    if (tabs->b.size < 2 || !tabs->b.head)
        return;
    tabs->b.head = tabs->b.head->prev;
}

void rrr(t_tabs *tabs) {
    rra(tabs);
    rrb(tabs);
}

/*
 * move_stack: Execute a move, print its mnemonic, and increment the move count.
 */
void move_stack(t_tabs *tabs, t_stack_move move) {
    switch (move) {
        case SA:
            printf("sa\n");
            sa(tabs);
            break;
        case SB:
            printf("sb\n");
            sb(tabs);
            break;
        case SS:
            printf("ss\n");
            ss(tabs);
            break;
        case PA:
            printf("pa\n");
            pa(tabs);
            break;
        case PB:
            printf("pb\n");
            pb(tabs);
            break;
        case RA:
            printf("ra\n");
            ra(tabs);
            break;
        case RB:
            printf("rb\n");
            rb(tabs);
            break;
        case RR:
            printf("rr\n");
            rr(tabs);
            break;
        case RRA:
            printf("rra\n");
            rra(tabs);
            break;
        case RRB:
            printf("rrb\n");
            rrb(tabs);
            break;
        case RRR:
            printf("rrr\n");
            rrr(tabs);
            break;
    }
    g_move_count++;
}

void move_and_print(t_tabs *tabs, t_stack_move move) {
    move_stack(tabs, move);
}

/* --- Helper Functions --- */

t_bool is_sorted(t_stack *stack, t_sort_order order) {
    if (!stack || stack->size <= 1)
        return TRUE;
    t_node *current = stack->head;
    t_node *next = current->next;
    do {
        if (order == ASCENDING && current->value > next->value)
            return FALSE;
        if (order == DESCENDING && current->value < next->value)
            return FALSE;
        current = next;
        next = next->next;
    } while (next != stack->head);
    return TRUE;
}

void merge(int *arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    if (!L || !R) {
        free(L);
        free(R);
        return;
    }
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    free(L);
    free(R);
}

void mergeSort(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

/* --- Stack Initialization and Freeing --- */

t_tabs *create_tabs(char **str) {
    int len = 0;
    t_tabs *tabs = malloc(sizeof(t_tabs));
    if (!tabs)
        return NULL;
    tabs->a.head = NULL;
    tabs->b.head = NULL;
    tabs->a.size = 0;
    tabs->b.size = 0;
    tabs->a.min = INT_MAX;
    tabs->a.max = INT_MIN;
    while (str[len])
        len++;
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
    int idx = 0;
    int max = (tabs->a.size > tabs->b.size) ? tabs->a.size : tabs->b.size;
    t_node *a = tabs->a.head;
    t_node *b = tabs->b.head;
    printf("stack a\t\tstack b\n");
    printf("-------\t\t-------\n");
    while (idx < max) {
        if (tabs->a.size > idx)
            printf("a:[%d]\t", a->orig);
        else
            printf("a:[ ]\t");
        if (tabs->b.size > idx)
            printf("b:[%d]\n", b->orig);
        else
            printf("b:[ ]\n");
        if (tabs->a.size > idx)
            a = a->next;
        if (tabs->b.size > idx)
            b = b->next;
        idx++;
    }
    printf("-------\t\t-------\n");
    printf("A = Size: %d\n", tabs->a.size);
    printf("B = Size: %d\n", tabs->b.size);
}

int get_max_bits(int max_num) {
    int max_bits = 0;
    while ((max_num >> max_bits) != 0)
        ++max_bits;
    return max_bits;
}

/* --- Small Stack Sorts --- */

void sort_three(t_tabs *tabs) {
    t_node *head = tabs->a.head;
    int a = head->value;
    int b = head->next->value;
    int c = head->next->next->value;
    if (a > b && b < c && a < c)
        move_and_print(tabs, SA);
    else if (a > b && b > c) {
        move_and_print(tabs, SA);
        move_and_print(tabs, RRA);
    }
    else if (a > b && b < c && a > c)
        move_and_print(tabs, RA);
    else if (a < b && b > c && a < c) {
        move_and_print(tabs, SA);
        move_and_print(tabs, RA);
    }
    else if (a < b && b > c && a > c)
        move_and_print(tabs, RRA);
}

void sort_small_stack(t_tabs *tabs) {
    int size = tabs->a.size;
    if (size == 2 && tabs->a.head->value > tabs->a.head->next->value)
        move_and_print(tabs, SA);
    else if (size == 3)
        sort_three(tabs);
    else if (size <= 5) {
        while (tabs->a.size > 3) {
            t_node *current = tabs->a.head;
            int min_val = current->value;
            int min_pos = 0, pos = 0;
            do {
                if (current->value < min_val) {
                    min_val = current->value;
                    min_pos = pos;
                }
                current = current->next;
                pos++;
            } while (current != tabs->a.head);
            if (min_pos <= tabs->a.size / 2) {
                while (min_pos-- > 0)
                    move_and_print(tabs, RA);
            } else {
                while (min_pos++ < tabs->a.size)
                    move_and_print(tabs, RRA);
            }
            move_and_print(tabs, PB);
        }
        sort_three(tabs);
        while (tabs->b.size > 0)
            move_and_print(tabs, PA);
    }
}

/* --- New Helper Functions for the Chunk Strategy --- */

/*
 * best_index_in_range:
 * Find the index (minimizing rotation cost) of an element in stack a whose
 * normalized value is between low and high (inclusive). Return -1 if none.
 */
int best_index_in_range(t_stack *stack, int low, int high) {
    if (!stack || stack->size == 0)
        return -1;
    int best_index = -1;
    int best_cost = 100000;
    int size = stack->size;
    int i = 0;
    t_node *cur = stack->head;
    do {
        if (cur->value >= low && cur->value <= high) {
            int cost = (i <= size / 2) ? i : (size - i);
            if (cost < best_cost) {
                best_cost = cost;
                best_index = i;
            }
        }
        i++;
        cur = cur->next;
    } while (cur != stack->head);
    return best_index;
}

/*
 * push_chunks:
 * Divide the normalized range of a (from 0 to original size-1) into chunk_count ranges.
 * For each chunk, repeatedly rotate a to bring the best candidate (the one
 * requiring the fewest moves) to the top, push it to b, and if its normalized value
 * is in the lower half of the chunk then immediately rotate b.
 */
void push_chunks(t_tabs *tabs, int chunk_count) {
    int original_size = tabs->a.size; // use original size for boundaries
    int chunk_size = original_size / chunk_count;
    for (int i = 0; i < chunk_count; i++) {
        int lower_bound = i * chunk_size;
        int upper_bound = (i == chunk_count - 1) ? (original_size - 1) : ((i + 1) * chunk_size - 1);
        int best_idx;
        while ((best_idx = best_index_in_range(&tabs->a, lower_bound, upper_bound)) != -1) {
            int current_size = tabs->a.size;
            if (best_idx <= current_size / 2) {
                while (best_idx-- > 0)
                    move_stack(tabs, RA);
            } else {
                int r = current_size - best_idx;
                while (r-- > 0)
                    move_stack(tabs, RRA);
            }
            move_stack(tabs, PB);
            /* If the pushed element is in the lower half of the chunk, rotate b */
            if (tabs->b.head->value <= lower_bound + (chunk_size / 2))
                move_stack(tabs, RB);
        }
    }
}

/*
 * reassemble_a:
 * While there are still nodes in b, find the maximum normalized value in b,
 * rotate b (using the minimal number of moves) until that element is at the top,
 * then push it to a.
 */
void reassemble_a(t_tabs *tabs) {
    while (tabs->b.size > 0) {
        int max_val = -1;
        int max_index = 0;
        t_node *cur = tabs->b.head;
        for (int i = 0; i < tabs->b.size; i++) {
            if (cur->value > max_val) {
                max_val = cur->value;
                max_index = i;
            }
            cur = cur->next;
        }
        if (max_index <= tabs->b.size / 2) {
            while (max_index-- > 0)
                move_stack(tabs, RB);
        } else {
            int r = tabs->b.size - max_index;
            while (r-- > 0)
                move_stack(tabs, RRB);
        }
        move_stack(tabs, PA);
    }
}

/*
 * push_swap:
 * 1. Normalize the values in stack a (preserving the original values in orig).
 * 2. Use a chunk strategy to push numbers from a to b.
 * 3. Reassemble a from b so that a is sorted in ascending order.
 */
void push_swap(t_tabs *tabs) {
    int size = tabs->a.size;
    if (size <= 1 || is_sorted(&tabs->a, ASCENDING))
        return;
    if (size <= 5) {
        sort_small_stack(tabs);
        return;
    }
    /* --- Normalization --- */
    int *values = malloc(sizeof(int) * size);
    int *sorted  = malloc(sizeof(int) * size);
    t_node *current = tabs->a.head;
    for (int i = 0; i < size; i++) {
        values[i] = current->orig;
        sorted[i] = current->orig;
        current = current->next;
    }
    mergeSort(sorted, 0, size - 1);
    current = tabs->a.head;
    for (int i = 0; i < size; i++) {
        int orig = current->orig;
        for (int j = 0; j < size; j++) {
            if (sorted[j] == orig) {
                int count = 0;
                for (int k = 0; k < i; k++) {
                    if (values[k] == orig)
                        count++;
                }
                for (int k = 0; k < count && j < size - 1; k++) {
                    if (sorted[j] == sorted[j + 1])
                        j++;
                }
                current->value = j;
                break;
            }
        }
        current = current->next;
    }
    free(values);
    free(sorted);
    /* --- End Normalization --- */
    int chunk_count = 3; // Tune this value as needed.
    push_chunks(tabs, chunk_count);
    reassemble_a(tabs);
}

/* --- Main --- */

int main(int argc, char **argv) {
    t_tabs *tabs;
    printf("Push Swap Program\n");
    if (argc < 2)
        return 0;
    tabs = create_tabs(argv + 1);
    if (!tabs)
        return 0;
    printf("Initial stacks:\n");
    print_tabs(tabs);
    g_move_count = 0; // Reset move count
    push_swap(tabs);
    printf("\nAfter sorting:\n");
    print_tabs(tabs);
    printf("Total moves: %d\n", g_move_count);
    free_tabs(tabs);
    return 0;
}
