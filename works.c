#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h> // For strlen, strerror

/* --- User's Data Structures --- */
typedef enum e_bool
{
    FALSE = 0,
    TRUE = 1
} t_bool;
typedef enum e_stack_move
{
    SA,
    SB,
    SS,
    PA,
    PB,
    RA,
    RB,
    RR,
    RRA,
    RRB,
    RRR
} t_stack_move;

typedef struct s_node
{
    int value; // Normalized index (0 to size-1)
    int orig;  // Original value
    struct s_node *next;
    struct s_node *prev;
} t_node;

typedef struct s_stack
{
    t_node *head;
    int size;
} t_stack;

typedef struct s_tabs
{
    t_stack a;
    t_stack b;
} t_tabs;

/* --- Chunk Sort Specific Structures --- */
typedef enum e_loc
{
    TOP_A,
    BOTTOM_A,
    TOP_B,
    BOTTOM_B
} t_loc;
typedef struct s_chunk
{
    t_loc loc;
    int size;
} t_chunk;
typedef struct s_split_dest
{
    t_chunk min;
    t_chunk mid;
    t_chunk max;
} t_split_dest;

// --- Forward Declarations (User's Functions) ---
void move_and_print(t_tabs *tabs, t_stack_move move);
void free_tabs(t_tabs *tabs);
void normalize_stack(t_stack *a);
t_bool is_sorted(t_stack *stack);
t_bool is_raw_input_sorted(t_stack *stack);
t_tabs *create_tabs(char **str, int len);
void free_stack(t_stack *stack);

// --- Forward Declarations (Chunk Sort Algorithm & Helpers) ---
void chunk_sort(t_tabs *data);
void rec_chunk_sort(t_tabs *data, t_chunk *to_sort);
void sort_three(t_tabs *data, t_chunk *to_sort); // Now more critical for optimization
void sort_two(t_tabs *data, t_chunk *to_sort);
void sort_one(t_tabs *data, t_chunk *to_sort);
void chunk_split(t_tabs *data, t_chunk *to_split, t_split_dest *dest);
int chunk_value(t_tabs *data, t_chunk *chunk, int n);
int chunk_max_value(t_tabs *data, t_chunk *chunk);
int chunk_min_value(t_tabs *data, t_chunk *chunk);
t_stack *loc_to_stack(t_tabs *data, t_loc loc);
void set_third_pivots_by_value(t_chunk *chunk, int min_val, int max_val, int *pivot1_val, int *pivot2_val);
void set_split_loc(t_loc loc, t_chunk *min, t_chunk *mid, t_chunk *max);
void innit_size(t_chunk *min, t_chunk *mid, t_chunk *max);
int move_from_to(t_tabs *data, t_loc from, t_loc to);
// Optimization Helpers
void sort_three_numbers(int *a, int *b, int *c);
bool is_consecutive(int a, int b, int c, int d);
bool a_partly_sort(t_tabs *data, int from_top_count);            // Check if stack A is sorted from 'from_top_count' downwards
void split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest); // Optimization during split

/* --- Global Move Count --- */
int g_move_count = 0;

/* --- User Function Implementations (Copy from previous working example) --- */
// create_node, append_node, fill_stack, free_stack, create_tabs, free_tabs
// has_duplicates, is_valid_int_str, normalize_stack
// is_sorted, is_raw_input_sorted
// Basic moves: sa, sb, ss, pa, pb, ra, rb, rr, rra, rrb, rrr, move_and_print
// ... PASTE ALL IMPLEMENTATIONS HERE ...
t_node *create_node(int num)
{ /* ... */
    t_node *node = malloc(sizeof(t_node));
    if (!node)
        return NULL;
    node->orig = num;
    node->value = 0;
    node->next = node;
    node->prev = node;
    return node;
}
void append_node(t_stack *stack, t_node *node)
{ /* ... */
    if (!stack->head)
    {
        stack->head = node;
        node->next = node;
        node->prev = node;
    }
    else
    {
        t_node *tail = stack->head->prev;
        tail->next = node;
        node->prev = tail;
        node->next = stack->head;
        stack->head->prev = node;
    }
    stack->size++;
}
t_bool has_duplicates(t_stack *stack)
{ /* ... */
    if (stack->size <= 1)
        return FALSE;
    t_node *c = stack->head;
    for (int i = 0; i < stack->size; i++)
    {
        t_node *r = c->next;
        for (int j = i + 1; j < stack->size; j++)
        {
            if (c->orig == r->orig)
            {
                fprintf(stderr, "Error: Duplicate %d\n", c->orig);
                return TRUE;
            }
            if (r == stack->head->prev)
                break;
            r = r->next;
        }
        if (c == stack->head->prev)
            break;
        c = c->next;
    }
    return FALSE;
}
t_bool is_valid_int_str(const char *str)
{ /* ... */
    if (!str || *str == '\0')
        return FALSE;
    int i = 0;
    if (str[i] == '-' || str[i] == '+')
    {
        i++;
        if (str[i] == '\0')
            return FALSE;
    }
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return FALSE;
        i++;
    }
    errno = 0;
    long num = strtol(str, NULL, 10);
    if (errno == ERANGE || num > INT_MAX || num < INT_MIN)
        return FALSE;
    return TRUE;
}
t_bool fill_stack(t_stack *stack, char **str, int len)
{ /* ... */
    for (int i = 0; i < len; i++)
    {
        if (!is_valid_int_str(str[i]))
        {
            fprintf(stderr, "Error: Invalid input '%s'.\n", str[i]);
            return FALSE;
        }
        errno = 0;
        char *ep;
        long n_l = strtol(str[i], &ep, 10);
        if (errno == ERANGE || *ep != '\0' || n_l > INT_MAX || n_l < INT_MIN)
        {
            fprintf(stderr, "Error: Invalid integer/range '%s'.\n", str[i]);
            return FALSE;
        }
        t_node *node = create_node((int)n_l);
        if (!node)
        {
            fprintf(stderr, "Error: Malloc failed.\n");
            return FALSE;
        }
        append_node(stack, node);
    }
    if (has_duplicates(stack))
        return FALSE;
    return TRUE;
}
void free_stack(t_stack *stack)
{ /* ... */
    if (!stack || !stack->head)
        return;
    t_node *tail = stack->head->prev;
    tail->next = NULL;
    t_node *c = stack->head;
    t_node *next_n;
    while (c != NULL)
    {
        next_n = c->next;
        free(c);
        c = next_n;
    }
    stack->head = NULL;
    stack->size = 0;
}
t_tabs *create_tabs(char **str, int len)
{ /* ... */
    t_tabs *tabs = malloc(sizeof(t_tabs));
    if (!tabs)
        return NULL;
    tabs->a.head = NULL;
    tabs->a.size = 0;
    tabs->b.head = NULL;
    tabs->b.size = 0;
    if (!fill_stack(&tabs->a, str, len))
    {
        free_tabs(tabs);
        return NULL;
    }
    return tabs;
}
void free_tabs(t_tabs *tabs)
{ /* ... */
    if (!tabs)
        return;
    free_stack(&tabs->a);
    free_stack(&tabs->b);
    free(tabs);
}
void swap_ints(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void normalize_stack(t_stack *a)
{ /* ... bubble sort version 0 to N-1 ... */
    if (a->size <= 1)
        return;
    int *arr = malloc(sizeof(int) * a->size);
    if (!arr)
    {
        fprintf(stderr, "Error: Malloc normalize\n");
        exit(1);
    }
    t_node *c = a->head;
    for (int i = 0; i < a->size; i++)
    {
        arr[i] = c->orig;
        c = c->next;
    }
    t_bool s;
    for (int i = 0; i < a->size - 1; i++)
    {
        s = FALSE;
        for (int j = 0; j < a->size - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap_ints(&arr[j], &arr[j + 1]);
                s = TRUE;
            }
        }
        if (s == FALSE)
            break;
    }
    c = a->head;
    for (int i = 0; i < a->size; i++)
    {
        for (int j = 0; j < a->size; j++)
        {
            if (c->orig == arr[j])
            {
                c->value = j;
                break;
            }
        }
        c = c->next;
    }
    free(arr);
}
t_bool is_sorted(t_stack *stack)
{
    if (!stack || stack->size <= 1 || !stack->head)
        return TRUE;
    t_node *c = stack->head;
    for (int i = 0; i < stack->size - 1; i++)
    {
        if (c->value > c->next->value)
            return FALSE;
        c = c->next;
    }
    return TRUE;
}
t_bool is_raw_input_sorted(t_stack *stack)
{
    if (!stack || stack->size <= 1 || !stack->head)
        return TRUE;
    t_node *c = stack->head;
    for (int i = 0; i < stack->size - 1; i++)
    {
        if (c->orig > c->next->orig)
            return FALSE;
        c = c->next;
    }
    return TRUE;
}
void sa(t_tabs *tabs)
{
    if (tabs->a.size < 2)
        return;
    t_node *f = tabs->a.head;
    int v = f->value;
    int o = f->orig;
    f->value = f->next->value;
    f->orig = f->next->orig;
    f->next->value = v;
    f->next->orig = o;
}
void sb(t_tabs *tabs)
{
    if (tabs->b.size < 2)
        return;
    t_node *f = tabs->b.head;
    int v = f->value;
    int o = f->orig;
    f->value = f->next->value;
    f->orig = f->next->orig;
    f->next->value = v;
    f->next->orig = o;
}
void ss(t_tabs *tabs)
{
    sa(tabs);
    sb(tabs);
}
void pa(t_tabs *tabs)
{
    if (tabs->b.size == 0)
        return;
    t_node *m = tabs->b.head;
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
void pb(t_tabs *tabs)
{
    if (tabs->a.size == 0)
        return;
    t_node *m = tabs->a.head;
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
void ra(t_tabs *tabs)
{
    if (tabs->a.size >= 2)
        tabs->a.head = tabs->a.head->next;
}
void rb(t_tabs *tabs)
{
    if (tabs->b.size >= 2)
        tabs->b.head = tabs->b.head->next;
}
void rr(t_tabs *tabs)
{
    ra(tabs);
    rb(tabs);
}
void rra(t_tabs *tabs)
{
    if (tabs->a.size >= 2)
        tabs->a.head = tabs->a.head->prev;
}
void rrb(t_tabs *tabs)
{
    if (tabs->b.size >= 2)
        tabs->b.head = tabs->b.head->prev;
}
void rrr(t_tabs *tabs)
{
    rra(tabs);
    rrb(tabs);
}
void move_stack(t_tabs *tabs, t_stack_move move)
{
    switch (move)
    {
    case SA:
        sa(tabs);
        break;
    case SB:
        sb(tabs);
        break;
    case SS:
        ss(tabs);
        break;
    case PA:
        pa(tabs);
        break;
    case PB:
        pb(tabs);
        break;
    case RA:
        ra(tabs);
        break;
    case RB:
        rb(tabs);
        break;
    case RR:
        rr(tabs);
        break;
    case RRA:
        rra(tabs);
        break;
    case RRB:
        rrb(tabs);
        break;
    case RRR:
        rrr(tabs);
        break;
    }
}
void move_and_print(t_tabs *tabs, t_stack_move move)
{
    if (move == SA)
        printf("sa\n");
    else if (move == SB)
        printf("sb\n");
    else if (move == SS)
        printf("ss\n");
    else if (move == PA)
        printf("pa\n");
    else if (move == PB)
        printf("pb\n");
    else if (move == RA)
        printf("ra\n");
    else if (move == RB)
        printf("rb\n");
    else if (move == RR)
        printf("rr\n");
    else if (move == RRA)
        printf("rra\n");
    else if (move == RRB)
        printf("rrb\n");
    else if (move == RRR)
        printf("rrr\n");
    move_stack(tabs, move);
    g_move_count++;
}

/* --- Chunk Sort Algorithm Implementation & Helpers --- */

t_stack *loc_to_stack(t_tabs *data, t_loc loc)
{
    if (loc == TOP_A || loc == BOTTOM_A)
        return (&data->a);
    else
        return (&data->b);
}

// Gets value of node n steps away (1=first, 2=second, ...) from start of chunk
int chunk_value(t_tabs *data, t_chunk *chunk, int n)
{
    t_stack *stk = loc_to_stack(data, chunk->loc);
    if (!stk || !stk->head || n <= 0 || n > chunk->size)
        return INT_MIN; // Error

    t_node *current;
    if (chunk->loc == TOP_A || chunk->loc == TOP_B)
    {
        current = stk->head;
        for (int i = 1; i < n; i++)
            current = current->next;
    }
    else
    { // BOTTOM_A or BOTTOM_B
        current = stk->head->prev;
        for (int i = 1; i < n; i++)
            current = current->prev;
    }
    return current->value;
}

// Finds max normalized value in a chunk
int chunk_max_value(t_tabs *data, t_chunk *chunk)
{
    t_stack *stk = loc_to_stack(data, chunk->loc);
    if (!stk || !stk->head || chunk->size <= 0)
        return INT_MIN;
    t_node *current;
    int max_val = INT_MIN;
    int current_val;
    if (chunk->loc == TOP_A || chunk->loc == TOP_B)
    {
        current = stk->head;
        for (int i = 0; i < chunk->size; i++)
        {
            current_val = current->value;
            if (current_val > max_val)
                max_val = current_val;
            if (i < chunk->size - 1)
                current = current->next;
        }
    }
    else
    {
        current = stk->head->prev;
        for (int i = 0; i < chunk->size; i++)
        {
            current_val = current->value;
            if (current_val > max_val)
                max_val = current_val;
            if (i < chunk->size - 1)
                current = current->prev;
        }
    }
    return max_val;
}

// Finds min normalized value in a chunk
int chunk_min_value(t_tabs *data, t_chunk *chunk)
{
    t_stack *stk = loc_to_stack(data, chunk->loc);
    if (!stk || !stk->head || chunk->size <= 0)
        return INT_MAX;
    t_node *current;
    int min_val = INT_MAX;
    int current_val;
    if (chunk->loc == TOP_A || chunk->loc == TOP_B)
    {
        current = stk->head;
        for (int i = 0; i < chunk->size; i++)
        {
            current_val = current->value;
            if (current_val < min_val)
                min_val = current_val;
            if (i < chunk->size - 1)
                current = current->next;
        }
    }
    else
    {
        current = stk->head->prev;
        for (int i = 0; i < chunk->size; i++)
        {
            current_val = current->value;
            if (current_val < min_val)
                min_val = current_val;
            if (i < chunk->size - 1)
                current = current->prev;
        }
    }
    return min_val;
}

// Calculate pivots based on value range
void set_third_pivots_by_value(t_chunk *chunk, int min_val, int max_val, int *pivot1_val, int *pivot2_val)
{

    (void)chunk; 
    int range = max_val - min_val + 1;
    if (range < 3)
    {
        *pivot1_val = min_val + range / 3;
        *pivot2_val = min_val + (2 * range) / 3;
        if (*pivot2_val <= *pivot1_val && range > 1)
            *pivot2_val = *pivot1_val + 1;
        if (*pivot1_val < min_val)
            *pivot1_val = min_val;
        if (*pivot2_val < min_val)
            *pivot2_val = min_val;
    }
    else
    {
        *pivot1_val = min_val + range / 3;
        *pivot2_val = min_val + (2 * range) / 3;
    }
   
}

// Set destination chunk locations
void set_split_loc(t_loc loc, t_chunk *min, t_chunk *mid, t_chunk *max)
{
    if (loc == TOP_A)
    {
        min->loc = BOTTOM_B;
        mid->loc = TOP_B;
        max->loc = TOP_A;
    } // Max stays in A (needs rotation)
    else if (loc == BOTTOM_A)
    {
        min->loc = BOTTOM_B;
        mid->loc = TOP_B;
        max->loc = TOP_A;
    }
    else if (loc == TOP_B)
    {
        min->loc = BOTTOM_B;
        mid->loc = BOTTOM_A;
        max->loc = TOP_A;
    }
    else if (loc == BOTTOM_B)
    {
        min->loc = TOP_B;
        mid->loc = BOTTOM_A;
        max->loc = TOP_A;
    }
}

// Initialize sizes
void innit_size(t_chunk *min, t_chunk *mid, t_chunk *max)
{
    min->size = 0;
    mid->size = 0;
    max->size = 0;
}

// Move element between chunk locations
int move_from_to(t_tabs *data, t_loc from, t_loc to)
{
    // This implementation needs to handle the case where "from" and "to" are the same location but require rotation
    // E.g., moving from TOP_A to TOP_A should imply RA if partitioning requires keeping it in A but moving past it.
    if (from == TOP_A)
    {
        if (to == TOP_B)
            move_and_print(data, PB);
        else if (to == BOTTOM_A)
            move_and_print(data, RA); // Move within A needs rotation
        else if (to == BOTTOM_B)
        {
            move_and_print(data, PB);
            move_and_print(data, RB);
        }
        else if (to == TOP_A)
            move_and_print(data, RA); // Stay top A -> Rotate
        else
            return 0;
    }
    else if (from == TOP_B)
    {
        if (to == TOP_A)
            move_and_print(data, PA);
        else if (to == BOTTOM_B)
            move_and_print(data, RB);
        else if (to == BOTTOM_A)
        {
            move_and_print(data, PA);
            move_and_print(data, RA);
        }
        else if (to == TOP_B)
            move_and_print(data, RB); // Stay top B -> Rotate
        else
            return 0;
    }
    else if (from == BOTTOM_A)
    {
        if (to == TOP_A)
            move_and_print(data, RRA);
        else if (to == TOP_B)
        {
            move_and_print(data, RRA);
            move_and_print(data, PB);
        }
        else if (to == BOTTOM_B)
        {
            move_and_print(data, RRA);
            move_and_print(data, PB);
            move_and_print(data, RB);
        }
        else if (to == BOTTOM_A)
            move_and_print(data, RRA); // Stay bottom A -> Rotate
        else
            return 0;
    }
    else if (from == BOTTOM_B)
    {
        if (to == TOP_B)
            move_and_print(data, RRB);
        else if (to == TOP_A)
        {
            move_and_print(data, RRB);
            move_and_print(data, PA);
        }
        else if (to == BOTTOM_A)
        {
            move_and_print(data, RRB);
            move_and_print(data, PA);
            move_and_print(data, RA);
        }
        else if (to == BOTTOM_B)
            move_and_print(data, RRB); // Stay bottom B -> Rotate
        else
            return 0;
    }
    else
        return 0;
    return 1;
}

/* --- Optimization Helpers (Translated) --- */

// Simple helper to sort 3 integers passed by address
void sort_three_numbers(int *a, int *b, int *c)
{
    int temp;
    if (*a > *b)
    {
        temp = *a;
        *a = *b;
        *b = temp;
    }
    if (*a > *c)
    {
        temp = *a;
        *a = *c;
        *c = temp;
    }
    if (*b > *c)
    {
        temp = *b;
        *b = *c;
        *c = temp;
    }
}

// Check if 4 values are consecutive (e.g., 5, 6, 7, 8)
bool is_consecutive(int a, int b, int c, int d)
{
    sort_three_numbers(&a, &b, &c); // Sort first three
    // Now check if a,b,c are consecutive and d follows c
    return ((b - a == 1) && (c - b == 1) && (d - c == 1));
}

// Check if stack A is sorted from the 'from_top_count'-th element downwards
// Example: a_partly_sort(data, 1) checks if the entire stack A is sorted
// Example: a_partly_sort(data, 4) checks if a[3], a[4], ... a[N-1] are sorted relative to each other.
// This assumes 0-based normalization! N = data->a.size
bool a_partly_sort(t_tabs *data, int from_top_count)
{
    t_stack *a = &data->a;
    if (!a->head || a->size < from_top_count + 1)
        return TRUE; // Trivial if too small

    t_node *current = a->head;
    // Move to the starting node ('from_top_count'-th element, 0-indexed)
    for (int i = 1; i < from_top_count; i++)
    {
        if (!current)
            return FALSE; // Should not happen
        current = current->next;
    }

    // Check consecutive sorting from this point until the node before head
    int expected_value = current->value;
    t_node *runner = current->next;
    while (runner != a->head)
    {
        expected_value++;
        if (runner->value != expected_value)
            return FALSE;
        runner = runner->next;
    }

    return TRUE; // If loop completes, it's sorted from that point
}

// Optimization during splitting: if elements sent to max destination form
// a sortable sequence at the top of A, sort them immediately.
void split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest)
{
    t_stack *a = &data->a;

    // Condition 1: Chunk destination is TOP_A, it now has 3 elements,
    // the top 4 are consecutive, and the rest of A is sorted.
    // (Original check: is_consecutive(value(a, 1), value(a, 2), value(a, 3), value(a, 4)))
    // Adapting: Check if a->head, a->head->next, a->head->next->next have consecutive VALUES
    // and if a->head->next->next->next->value follows, AND rest is sorted.
    // This seems overly specific and complex to translate perfectly.
    // Let's simplify the check slightly for linked lists.

    // Simplified check: If destination is TOP_A and has gathered >= 3 elements...
    if (max_chunk_dest->loc == TOP_A && max_chunk_dest->size >= 3 && a->size >= 3)
    {
        // Check if top 3 are sortable via sort_three logic, and if they fit the partial sort criteria
        // Check if the top 3 elements are the LARGEST elements overall (N-1, N-2, N-3)
        int v0 = a->head->value;
        int v1 = a->head->next->value;
        int v2 = a->head->next->next->value;
        int total_size = data->a.size + data->b.size; // Get overall N
        bool top_three_are_max = TRUE;
        if (v0 < total_size - 3 || v1 < total_size - 3 || v2 < total_size - 3)
        {
            top_three_are_max = FALSE;
        }

        if (top_three_are_max && a_partly_sort(data, 4))
        { // If top 3 are largest AND rest is sorted
            // fprintf(stderr, "DEBUG: split_max_reduction Case 1 triggered\n");
            t_chunk temp_chunk = {TOP_A, 3}; // Create temp chunk for sort_three
            sort_three(data, &temp_chunk);
            max_chunk_dest->size -= 3; // We sorted 3 elements from the max chunk
            return;                    // Exit reduction check
        }
    }

    // Condition 2: If dest is TOP_A, check if swapping first two makes the top element sorted relative to the third
    // (Original check: value(a, 1) == value(a, 3) - 1 && a_partly_sort(data, 3))
    if (max_chunk_dest->loc == TOP_A && max_chunk_dest->size >= 1 && a->size >= 3)
    {
        if (a->head->next->value == a->head->next->next->value - 1 && // Check if a[1] fits before a[2]
            a->head->value > a->head->next->value &&                  // Check if swap is needed
            a_partly_sort(data, 3))                                   // Check if rest is sorted
        {
            // fprintf(stderr, "DEBUG: split_max_reduction Case 2 triggered\n");
            move_and_print(data, SA);
            // This swap effectively sorted a->head (originally a[1]), reduce max chunk size
            max_chunk_dest->size--;
            return;
        }
    }

    // Condition 3: If dest is TOP_A and the top element is already correctly sorted relative to the rest.
    // (Original check: a_partly_sort(data, 1)) - Check if whole stack A is sorted
    // Adapting: check if a->head is N-1 and rest is sorted, or if a->head fits with a[1] upwards.
    if (max_chunk_dest->loc == TOP_A && max_chunk_dest->size >= 1 && a->size >= 1)
    {
        if (a_partly_sort(data, 2))
        { // Check if stack starting from 2nd element is sorted
            if (a->size == 1 || (a->size > 1 && a->head->value == a->head->next->value - 1))
            {
                // Top element fits perfectly with the sorted sequence below it.
                // fprintf(stderr, "DEBUG: split_max_reduction Case 3 triggered\n");
                max_chunk_dest->size--; // This element is done.
                return;
            }
        }
    }
}

/* --- Main Sorting Logic (Using Chunk Sort) --- */

void sort_one(t_tabs *data, t_chunk *to_sort)
{
    if (to_sort->loc != TOP_A)
    {
        move_from_to(data, to_sort->loc, TOP_A);
    }
}

void sort_two(t_tabs *data, t_chunk *to_sort)
{
    if (to_sort->loc != TOP_A)
    {
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
    }
    if (data->a.size >= 2 && data->a.head->value > data->a.head->next->value)
    {
        move_and_print(data, SA);
    }
}

void sort_three(t_tabs *data, t_chunk *to_sort)
{
    if (to_sort->loc != TOP_A)
    {
        // Simplification: Move all to TOP_A then sort
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
        move_from_to(data, to_sort->loc, TOP_A);
    }

    if (data->a.size < 3)
        return; // Should not happen if called correctly
    if (is_sorted(&data->a))
        return; // Check top 3, but easier to check whole stack if size=3

    int v0 = data->a.head->value;
    int v1 = data->a.head->next->value;
    int v2 = data->a.head->prev->value; // Use prev for third element in circular list

    if (v0 > v1 && v1 < v2 && v0 < v2)
        move_and_print(data, SA);
    else if (v0 > v1 && v1 > v2)
    {
        move_and_print(data, SA);
        move_and_print(data, RRA);
    }
    else if (v0 > v1 && v1 < v2 && v0 > v2)
        move_and_print(data, RA);
    else if (v0 < v1 && v1 > v2 && v0 < v2)
    {
        move_and_print(data, SA);
        move_and_print(data, RA);
    }
    else if (v0 < v1 && v1 > v2 && v0 > v2)
        move_and_print(data, RRA);
}

void chunk_split(t_tabs *data, t_chunk *to_split, t_split_dest *dest)
{
    int pivot1_val, pivot2_val;
    int min_val = chunk_min_value(data, to_split);
    int max_val = chunk_max_value(data, to_split);
    int initial_chunk_size = to_split->size; // Store original size

    set_third_pivots_by_value(to_split, min_val, max_val, &pivot1_val, &pivot2_val);

    // fprintf(stderr, "DEBUG: Splitting chunk loc=%d, size=%d (minV=%d, maxV=%d, p1V=%d, p2V=%d)\n",
    // to_split->loc, initial_chunk_size, min_val, max_val, pivot1_val, pivot2_val);

    for (int i = 0; i < initial_chunk_size; i++)
    {
        int next_value = chunk_value(data, to_split, 1); // Value at the start of the chunk

        // fprintf(stderr, "  Processing value %d (iter %d/%d)\n", next_value, i, initial_chunk_size-1);

        if (next_value >= pivot2_val)
        { // Max chunk
            // fprintf(stderr, "    -> Max chunk (loc %d)\n", dest->max.loc);
            dest->max.size += move_from_to(data, to_split->loc, dest->max.loc);
            // Apply optimization AFTER the move
            split_max_reduction(data, &dest->max);
        }
        else if (next_value >= pivot1_val)
        { // Mid chunk
            // fprintf(stderr, "    -> Mid chunk (loc %d)\n", dest->mid.loc);
            dest->mid.size += move_from_to(data, to_split->loc, dest->mid.loc);
        }
        else
        { // Min chunk
            // fprintf(stderr, "    -> Min chunk (loc %d)\n", dest->min.loc);
            dest->min.size += move_from_to(data, to_split->loc, dest->min.loc);
        }
        // Note: We don't decrement to_split->size here, the loop runs based on initial_chunk_size
    }
    // fprintf(stderr, "DEBUG: Split finished. Sizes: min=%d, mid=%d, max=%d\n", dest->min.size, dest->mid.size, dest->max.size);
}

void rec_chunk_sort(t_tabs *data, t_chunk *to_sort)
{
    t_split_dest dest;

    // fprintf(stderr, "REC_CHUNK_SORT: loc=%d, size=%d\n", to_sort->loc, to_sort->size);

    // --- BASE CASES ---
    if (to_sort->size <= 3)
    {
        // fprintf(stderr, "  Base Case: size=%d\n", to_sort->size);
        if (to_sort->size == 3)
            sort_three(data, to_sort);
        else if (to_sort->size == 2)
            sort_two(data, to_sort);
        else if (to_sort->size == 1)
            sort_one(data, to_sort);
        // fprintf(stderr, "  Base Case Handled.\n");
        return;
    }

    // --- SPLIT ---
    innit_size(&dest.min, &dest.mid, &dest.max);
    set_split_loc(to_sort->loc, &dest.min, &dest.mid, &dest.max);
    chunk_split(data, to_sort, &dest); // This function now contains the loop and partitioning

    // --- RECURSE (Max -> Mid -> Min) ---
    rec_chunk_sort(data, &dest.max);
    rec_chunk_sort(data, &dest.mid);
    rec_chunk_sort(data, &dest.min);
}

void chunk_sort(t_tabs *data)
{
    t_chunk chunk_all;
    chunk_all.loc = TOP_A;
    chunk_all.size = data->a.size;
    rec_chunk_sort(data, &chunk_all);

    // Final rotation to bring 0 to top (should be unnecessary if logic is perfect, but good safeguard)
    if (data->a.size > 0 && data->a.head->value != 0)
    {
        // fprintf(stderr, "DEBUG: Final rotation needed. A head is %d\n", data->a.head->value);
        t_node *min_node = data->a.head;
        int min_pos = -1, current_pos = 0;
        do
        {
            if (min_node->value == 0)
            {
                min_pos = current_pos;
                break;
            }
            min_node = min_node->next;
            current_pos++;
        } while (current_pos < data->a.size);

        if (min_pos > 0)
        {
            if (min_pos <= data->a.size / 2)
            {
                for (int i = 0; i < min_pos; i++)
                    move_and_print(data, RA);
            }
            else
            {
                for (int i = 0; i < data->a.size - min_pos; i++)
                    move_and_print(data, RRA);
            }
        }
        // fprintf(stderr, "DEBUG: Final rotation done.\n");
    }
}

// Main sort function calling the strategy
void sort_strategy(t_tabs *data)
{
    if (data->a.size <= 1 || is_raw_input_sorted(&data->a))
        return;
    normalize_stack(&data->a);
    if (data->a.size > 1)
        chunk_sort(data);
}

/* --- Main --- */
int main(int argc, char **argv)
{
    t_tabs *tabs;
    if (argc < 2)
        return 0;
    tabs = create_tabs(argv + 1, argc - 1);
    if (!tabs)
        return 1;
    g_move_count = 0;
    sort_strategy(tabs);
    // fprintf(stderr, "\n--- Final Results ---\n"); // Moved to sort_strategy for clarity
    fprintf(stderr, "Total moves: %d\n", g_move_count);
    free_tabs(tabs);
    return 0;
}