#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "./includes/push_swap.h"

int g_move_count = 0;

/*
 * Swap the first 2 elements at the top of stack a
 */
void sa(t_tabs *tabs) {
    t_node *head = tabs->a.head;
    if (tabs->a.size < 2 || !head || !head->next) return;

    int tmp = head->value;
    head->value = head->next->value;
    head->next->value = tmp;
}

/*
 * Swap the first 2 elements at the top of stack b
 */
void sb(t_tabs *tabs) {
    t_node *head = tabs->b.head;
    if (tabs->b.size < 2 || !head || !head->next) return;

    int tmp = head->value;
    head->value = head->next->value;
    head->next->value = tmp;
}

/*
 * ss : sa and sb at the same time.
 */
void ss(t_tabs *tabs) {
    sa(tabs);
    sb(tabs);
}

/*
 * pa (push a): Take the first element at the top of b and put it at the top of a.
 */
void pa(t_tabs *tabs) {
    if (tabs->b.size == 0) return;

    t_node *b_head = tabs->b.head;
    
    // Remove from B
    if (tabs->b.size == 1) {
        tabs->b.head = NULL;
    } else {
        tabs->b.head = b_head->next;
        b_head->prev->next = b_head->next;
        b_head->next->prev = b_head->prev;
    }
    tabs->b.size--;

    // Add to A
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

/*
 * pb (push b): Take the first element at the top of a and put it at the top of b.
 */
void pb(t_tabs *tabs) {
    if (tabs->a.size == 0) return;

    t_node *a_head = tabs->a.head;
    
    // Remove from A
    if (tabs->a.size == 1) {
        tabs->a.head = NULL;
    } else {
        tabs->a.head = a_head->next;
        a_head->prev->next = a_head->next;
        a_head->next->prev = a_head->prev;
    }
    tabs->a.size--;

    // Add to B
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

/*
 * ra (rotate a): Shift up all elements of stack a by 1.
 */
void ra(t_tabs *tabs) {
    if (tabs->a.size < 2 || !tabs->a.head) return;
    tabs->a.head = tabs->a.head->next;
}

/*
 * rb (rotate b): Shift up all elements of stack b by 1.
 */
void rb(t_tabs *tabs) {
    if (tabs->b.size < 2 || !tabs->b.head) return;
    tabs->b.head = tabs->b.head->next;
}

/*
 * rr : ra and rb at the same time.
 */
void rr(t_tabs *tabs) {
    ra(tabs);
    rb(tabs);
}

/*
 * rra (reverse rotate a): Shift down all elements of stack a by 1.
 */
void rra(t_tabs *tabs) {
     if (tabs->a.size < 2 || !tabs->a.head) return;

    t_node *current = tabs->a.head;
    while(current->next != tabs->a.head) {
        current = current->next;
    }
    tabs->a.head = current;
}

/*
 * rrb (reverse rotate b): Shift down all elements of stack b by 1.
 */
void rrb(t_tabs *tabs) {
     if (tabs->b.size < 2 || !tabs->b.head) return;

    t_node *current = tabs->b.head;
    while(current->next != tabs->b.head) {
        current = current->next;
    }
    tabs->b.head = current;
}



/*
 * rrr : rra and rrb at the same time.
 */
void rrr(t_tabs *tabs) {
    rra(tabs);
    rrb(tabs);
}

/*
 * Move the stack
 */
void move_stack(t_tabs *tabs, t_stack_move move) {
    switch (move) {
        case SA: sa(tabs); break;
        case SB: sb(tabs); break;
        case SS: ss(tabs); break;
        case PA: pa(tabs); break;
        case PB: pb(tabs); break;
        case RA: ra(tabs); break;
        case RB: rb(tabs); break;
        case RR: rr(tabs); break;
        case RRA: rra(tabs); break;
        case RRB: rrb(tabs); break;
        case RRR: rrr(tabs); break;
    }
}


/*
 * Check if the stack is sorted
 */
// t_bool is_sorted(t_stack *stack, t_sort_order order) {
//     if (!stack || stack->size <= 1) return TRUE;

//     t_node *current = stack->head;
//     t_node *next = current->next;

//     while (next != stack->head) {
//         if (order == ASCENDING && current->value > next->value)
//             return FALSE;
//         else if (order == DESCENDING && current->value < next->value)
//             return FALSE;
//         current = next;
//         next = next->next;
//     }
//     return TRUE;
// }

// Merge Sort implementation (No stdlib)
void merge(int *arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2]; // Temporary arrays

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int *arr, int left, int right);

void mergeSort(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
       merge(arr, left, mid, right);
    }
}


/*
 * Create a stack from a string array
 */
t_tabs *create_tabs(char **str) {
    int len = 0;
    t_tabs *tabs = (t_tabs *)malloc(sizeof(t_tabs));
    if (!tabs) return NULL;
    tabs->a.head = NULL;
	tabs->b.head = NULL;
    tabs->a.size = 0;
	tabs->b.size = 0;
    tabs->a.min = 0;
	tabs->b.min = 0;
    tabs->a.max = 0;
	tabs->b.max = 0;
    while (str[len]) len++;
    if (!(fill_stack(&tabs->a, str, len))) {
        free_tabs(tabs);
        return NULL;
    }
    if (!fill_stack(&tabs->b, NULL, len)) {
        free_tabs(tabs);
        return NULL;
    }
    return tabs;
}

/*
 * Print the stack (for debugging)
 */
void print_tabs(t_tabs *tabs) {
    int idx;
    int max;
    t_node *a = tabs->a.head;
    t_node *b = tabs->b.head;

    max = (tabs->a.size > tabs->b.size) ? tabs->a.size : tabs->b.size;
    idx = 0;
    ft_printf("stack a\t\tstack b\n");
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
        if (tabs->a.size > idx) {
            a = a->next;
        }
        if (b && tabs->b.size > idx) {
            b = b->next;
        }
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
     if (!tabs) return;
   free_stack(&tabs->a);
   free_stack(&tabs->b);
   free(tabs);
}

/*
 * Function to print the move and then perform the move
 */
void move_and_print(t_tabs *tabs, t_stack_move move) {
    move_stack(tabs, move);
    switch (move) {
        case SA: ft_printf("sa\n"); break;
        case SB: ft_printf("sb\n"); break;
        case SS: ft_printf("ss\n"); break;
        case RA: ft_printf("ra\n"); break;
        case RB: ft_printf("rb\n"); break;
        case RR: ft_printf("rr\n"); break;
        case RRA: ft_printf("rra\n"); break;
        case RRB: ft_printf("rrb\n"); break;
        case RRR: ft_printf("rrr\n"); break;
        case PA: ft_printf("pa\n"); break;
        case PB: ft_printf("pb\n"); break;
        default: ft_printf("Unknown move\n"); break;
    }
    g_move_count++; // Increment the global move count
}

// Function to find maximum number of bits
int get_max_bits(int max_num) {
    int max_bits = 0;
    while ((max_num >> max_bits) != 0)
        ++max_bits;
    return max_bits;
}

/*
 * Main sorting function using Radix Sort
 */
void push_swap(t_tabs *tabs) {
    int size = tabs->a.size;
    
    // Check if already sorted or too small
    if (size <= 1 || is_sorted(&tabs->a, ASCENDING)) 
        return;

    // Handle small stacks efficiently
    if (size <= 5) {
        sort_small_stack(tabs);
        return;
    }

    // Normalize numbers
    int *values = malloc(sizeof(int) * size);
    int *sorted = malloc(sizeof(int) * size);
    t_node *current = tabs->a.head;
    
    // Copy values
    for (int i = 0; i < size; i++) {
        values[i] = current->orig;
        sorted[i] = current->orig;
        current = current->next;
    }

    // Sort array to get position mapping
    qsort(sorted, size, sizeof(int), compare_ints);
    
    // Create position mapping
    current = tabs->a.head;
    while (current) {
        for (int i = 0; i < size; i++) {
            if (current->orig == sorted[i]) {
                current->value = i;
                break;
            }
        }
        current = current->next;
    }

    free(values);
    free(sorted);

    // For 100 numbers, use chunk sort
    if (size <= 100) {
        chunk_sort(tabs);
    } else {
        // For larger sizes, use radix sort
        radix_sort(tabs);
    }
}

void chunk_sort(t_tabs *tabs) {
    int size = tabs->a.size;
    int chunk_size = size / 5;  // Split into 5 chunks for 100 numbers
    int min = 0;
    int max = chunk_size;

    // Push numbers to B in chunks, smallest first
    while (tabs->a.size > 0) {
        t_node *current = tabs->a.head;
        int closest_pos = find_closest_in_range(tabs->a, min, max);
        
        if (closest_pos == -1) {
            min += chunk_size;
            max += chunk_size;
            continue;
        }

        // Rotate to the closest number in current chunk
        if (closest_pos <= tabs->a.size / 2) {
            while (closest_pos-- > 0)
                move_and_print(tabs, RA);
        } else {
            while (closest_pos++ < tabs->a.size)
                move_and_print(tabs, RRA);
        }
        
        move_and_print(tabs, PB);
    }

    // Push back to A in descending order
    while (tabs->b.size > 0) {
        int max_pos = find_max_position(tabs->b);
        
        if (max_pos <= tabs->b.size / 2) {
            while (max_pos-- > 0)
                move_and_print(tabs, RB);
        } else {
            while (max_pos++ < tabs->b.size)
                move_and_print(tabs, RRB);
        }
        
        move_and_print(tabs, PA);
    }
}

int find_closest_in_range(t_stack stack, int min, int max) {
    int closest_pos = -1;
    int i = 0;
    t_node *current = stack.head;

    while (current) {
        if (current->value >= min && current->value < max) {
            closest_pos = i;
            break;
        }
        current = current->next;
        i++;
    }

    return closest_pos;
}

int find_max_position(t_stack stack) {
    int max_val = -1;
    int max_pos = 0;
    int curr_pos = 0;
    t_node *current = stack.head;

    while (current) {
        if (current->value > max_val) {
            max_val = current->value;
            max_pos = curr_pos;
        }
        current = current->next;
        curr_pos++;
    }

    return max_pos;
}

// Helper function for qsort
int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void radix_sort(t_tabs *tabs) {
    int max_bits = 0;
    int size = tabs->a.size;
    int max_num = size - 1;  // After normalization, largest number is size-1
    
    // Calculate number of bits needed
    while ((max_num >> max_bits) != 0)
        max_bits++;

    // Sort by each bit
    for (int bit = 0; bit < max_bits; bit++) {
        for (int i = 0; i < size; i++) {
            if (((tabs->a.head->value >> bit) & 1) == 0)
                move_and_print(tabs, PB);
            else
                move_and_print(tabs, RA);
        }
        
        // Push all numbers back to A
        while (tabs->b.size > 0)
            move_and_print(tabs, PA);
    }
}

int main(int argc, char **argv) {
    t_tabs *tabs;

    ft_printf("THis is the new verision");
    if (argc < 2)
        return (0);
    tabs = create_tabs(argv + 1);
    if (!tabs)
        return (0);
    print_tabs(tabs);
    g_move_count = 0;  // Reset move count
    push_swap(tabs); // Call push_swap
    ft_printf("Total moves: %d\n", g_move_count);
    print_tabs(tabs);

    free_tabs(tabs);
    return (0);
}

