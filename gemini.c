// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include "./includes/push_swap.h"

// int g_move_count = 0;

// /*
//  * Swap the first 2 elements at the top of stack a.
//  */
// void sa(t_tabs *tabs) {
//     t_node *head = tabs->a.head;
//     if (tabs->a.size < 2 || !head || !head->next)
//         return;
//     int tmp = head->value;
//     head->value = head->next->value;
//     head->next->value = tmp;
// }

// /*
//  * Swap the first 2 elements at the top of stack b.
//  */
// void sb(t_tabs *tabs) {
//     t_node *head = tabs->b.head;
//     if (tabs->b.size < 2 || !head || !head->next)
//         return;
//     int tmp = head->value;
//     head->value = head->next->value;
//     head->next->value = tmp;
// }

// /*
//  * ss : perform sa and sb at the same time.
//  */
// void ss(t_tabs *tabs) {
//     sa(tabs);
//     sb(tabs);
// }

// /*
//  * pa (push a): Take the top element of stack b and push it onto stack a.
//  */
// void pa(t_tabs *tabs) {
//     if (tabs->b.size == 0)
//         return;
//     t_node *b_head = tabs->b.head;
//     // Remove from B:
//     if (tabs->b.size == 1) {
//         tabs->b.head = NULL;
//     } else {
//         tabs->b.head = b_head->next;
//         b_head->prev->next = b_head->next;
//         b_head->next->prev = b_head->prev;
//     }
//     tabs->b.size--;
//     // Add to A:
//     if (tabs->a.size == 0) {
//         tabs->a.head = b_head;
//         b_head->next = b_head;
//         b_head->prev = b_head;
//     } else {
//         b_head->next = tabs->a.head;
//         b_head->prev = tabs->a.head->prev;
//         tabs->a.head->prev->next = b_head;
//         tabs->a.head->prev = b_head;
//         tabs->a.head = b_head;
//     }
//     tabs->a.size++;
// }

// /*
//  * pb (push b): Take the top element of stack a and push it onto stack b.
//  */
// void pb(t_tabs *tabs) {
//     if (tabs->a.size == 0)
//         return;
//     t_node *a_head = tabs->a.head;
//     // Remove from A:
//     if (tabs->a.size == 1) {
//         tabs->a.head = NULL;
//     } else {
//         tabs->a.head = a_head->next;
//         a_head->prev->next = a_head->next;
//         a_head->next->prev = a_head->prev;
//     }
//     tabs->a.size--;
//     // Add to B:
//     if (tabs->b.size == 0) {
//         tabs->b.head = a_head;
//         a_head->next = a_head;
//         a_head->prev = a_head;
//     } else {
//         a_head->next = tabs->b.head;
//         a_head->prev = tabs->b.head->prev;
//         tabs->b.head->prev->next = a_head;
//         tabs->b.head->prev = a_head;
//         tabs->b.head = a_head;
//     }
//     tabs->b.size++;
// }

// /*
//  * ra (rotate a): Shift up all elements of stack a by 1.
//  */
// void ra(t_tabs *tabs) {
//     if (tabs->a.size < 2 || !tabs->a.head)
//         return;
//     tabs->a.head = tabs->a.head->next;
// }

// /*
//  * rb (rotate b): Shift up all elements of stack b by 1.
//  */
// void rb(t_tabs *tabs) {
//     if (tabs->b.size < 2 || !tabs->b.head)
//         return;
//     tabs->b.head = tabs->b.head->next;
// }

// /*
//  * rr : perform ra and rb at the same time.
//  */
// void rr(t_tabs *tabs) {
//     ra(tabs);
//     rb(tabs);
// }

// /*
//  * rra (reverse rotate a): Shift down all elements of stack a by 1.
//  */
// void rra(t_tabs *tabs) {
//     if (tabs->a.size < 2 || !tabs->a.head)
//         return;
//     tabs->a.head = tabs->a.head->prev;
// }

// /*
//  * rrb (reverse rotate b): Shift down all elements of stack b by 1.
//  */
// void rrb(t_tabs *tabs) {
//     if (tabs->b.size < 2 || !tabs->b.head)
//         return;
//     tabs->b.head = tabs->b.head->prev;
// }

// /*
//  * rrr : perform rra and rrb at the same time.
//  */
// void rrr(t_tabs *tabs) {
//     rra(tabs);
//     rrb(tabs);
// }

// /*
//  * Move the stack according to the move, and print the move.
//  */
// void move_stack(t_tabs *tabs, t_stack_move move) {
//     switch (move) {
//     case SA:
//         ft_printf("sa\n");
//         sa(tabs);
//         break;
//     case SB:
//         ft_printf("sb\n");
//         sb(tabs);
//         break;
//     case SS:
//         ft_printf("ss\n");
//         ss(tabs);
//         break;
//     case PA:
//         ft_printf("pa\n");
//         pa(tabs);
//         break;
//     case PB:
//         ft_printf("pb\n");
//         pb(tabs);
//         break;
//     case RA:
//         ft_printf("ra\n");
//         ra(tabs);
//         break;
//     case RB:
//         ft_printf("rb\n");
//         rb(tabs);
//         break;
//     case RR:
//         ft_printf("rr\n");
//         rr(tabs);
//         break;
//     case RRA:
//         ft_printf("rra\n");
//         rra(tabs);
//         break;
//     case RRB:
//         ft_printf("rrb\n");
//         rrb(tabs);
//         break;
//     case RRR:
//         ft_printf("rrr\n");
//         rrr(tabs);
//         break;
//     }
//     g_move_count++;
// }

// void move_and_print(t_tabs *tabs, t_stack_move move) {
//     move_stack(tabs, move);
// }

// /*
//  * Check if the stack is sorted.
//  */
// t_bool is_sorted(t_stack *stack, t_sort_order order) {
//     if (!stack || stack->size <= 1)
//         return TRUE;
//     t_node *current = stack->head;
//     t_node *next = current->next;
//     do {
//         if (order == ASCENDING && current->value > next->value)
//             return FALSE;
//         if (order == DESCENDING && current->value < next->value)
//             return FALSE;
//         current = next;
//         next = next->next;
//     } while (next != stack->head);
//     return TRUE;
// }

// /*
//  * Merge Sort (for normalization)
//  */
// void merge(int *arr, int left, int mid, int right) {
//     int i, j, k;
//     int n1 = mid - left + 1;
//     int n2 = right - mid;
//     int L[n1], R[n2];
//     for (i = 0; i < n1; i++)
//         L[i] = arr[left + i];
//     for (j = 0; j < n2; j++)
//         R[j] = arr[mid + 1 + j];
//     i = 0;
//     j = 0;
//     k = left;
//     while (i < n1 && j < n2) {
//         if (L[i] <= R[j]) {
//             arr[k] = L[i];
//             i++;
//         } else {
//             arr[k] = R[j];
//             j++;
//         }
//         k++;
//     }
//     while (i < n1) {
//         arr[k] = L[i];
//         i++;
//         k++;
//     }
//     while (j < n2) {
//         arr[k] = R[j];
//         j++;
//         k++;
//     }
// }

// void mergeSort(int *arr, int left, int right) {
//     if (left < right) {
//         int mid = left + (right - left) / 2;
//         mergeSort(arr, left, mid);
//         mergeSort(arr, mid + 1, right);
//         merge(arr, left, mid, right);
//     }
// }

// /*
//  * Create a stack from a string array.
//  * (Note: fill_stack and free_stack should be defined elsewhere.)
//  */
// t_tabs *create_tabs(char **str) {
//     int len = 0;
//     t_tabs *tabs = (t_tabs *)malloc(sizeof(t_tabs));
//     if (!tabs)
//         return NULL;
//     tabs->a.head = NULL;
//     tabs->b.head = NULL;
//     tabs->a.size = 0;
//     tabs->b.size = 0;
//     tabs->a.min = 0;
//     tabs->b.min = 0;
//     tabs->a.max = 0;
//     tabs->b.max = 0;
//     while (str[len])
//         len++;
//     if (!fill_stack(&tabs->a, str, len)) {
//         free_tabs(tabs);
//         return NULL;
//     }
//     if (!fill_stack(&tabs->b, NULL, len)) {
//         free_tabs(tabs);
//         return NULL;
//     }
//     return tabs;
// }

// /*
//  * Print the stacks (for debugging)
//  */
// void print_tabs(t_tabs *tabs) {
//     int idx = 0;
//     int max = (tabs->a.size > tabs->b.size) ? tabs->a.size : tabs->b.size;
//     t_node *a = tabs->a.head;
//     t_node *b = tabs->b.head;
//     ft_printf("stack a\t\tstack b\n");
//     ft_printf("-------\t\t-------\n");
//     while (idx < max) {
//         if (tabs->a.size > idx)
//             ft_printf("a:[%d]\t", a->value);
//         else
//             ft_printf("a:[ ]\t");
//         if (b && tabs->b.size > idx)
//             ft_printf("b:[%d]\n", b->value);
//         else
//             ft_printf("b:[ ]\n");
//         if (tabs->a.size > idx)
//             a = a->next;
//         if (b && tabs->b.size > idx)
//             b = b->next;
//         idx++;
//     }
//     ft_printf("-------\t\t-------\n");
//     ft_printf("A = Size: %d\tMin: %d\tMax: %d\n", tabs->a.size,
//               tabs->a.min, tabs->a.max);
//     ft_printf("B = Size: %d\tMin: %d\tMax: %d\n", tabs->b.size,
//               tabs->b.min, tabs->b.max);
// }

// /*
//  * Free the tabs.
//  */
// void free_tabs(t_tabs *tabs) {
//     if (!tabs)
//         return;
//     free_stack(&tabs->a);
//     free_stack(&tabs->b);
//     free(tabs);
// }

// /*
//  * Find maximum number of bits (used in radix sort, if desired).
//  */
// int get_max_bits(int max_num) {
//     int max_bits = 0;
//     while ((max_num >> max_bits) != 0)
//         ++max_bits;
//     return max_bits;
// }

// /*
//  * Sort 3 elements in stack A.
//  */
// void sort_three(t_tabs *tabs) {
//     t_node *head = tabs->a.head;
//     int a = head->value;
//     int b = head->next->value;
//     int c = head->next->next->value;
//     if (a > b && b < c && a < c)
//         move_and_print(tabs, SA);
//     else if (a > b && b > c) {
//         move_and_print(tabs, SA);
//         move_and_print(tabs, RRA);
//     } else if (a > b && b < c && a > c)
//         move_and_print(tabs, RA);
//     else if (a < b && b > c && a < c) {
//         move_and_print(tabs, SA);
//         move_and_print(tabs, RA);
//     } else if (a < b && b > c && a > c)
//         move_and_print(tabs, RRA);
// }

// /*
//  * Sort small stacks (5 or fewer elements).
//  */
// void sort_small_stack(t_tabs *tabs) {
//     int size = tabs->a.size;
//     if (size == 2 && tabs->a.head->value > tabs->a.head->next->value)
//         move_and_print(tabs, SA);
//     else if (size == 3) {
//         sort_three(tabs);
//     } else if (size <= 5) {
//         /* For 4 or 5 elements, push the smallest elements to B until 3 remain */
//         while (tabs->a.size > 3) {
//             t_node *current = tabs->a.head;
//             int min_val = current->value;
//             int min_pos = 0;
//             int pos = 0;
//             do {
//                 if (current->value < min_val) {
//                     min_val = current->value;
//                     min_pos = pos;
//                 }
//                 current = current->next;
//                 pos++;
//             } while (current != tabs->a.head);
//             if (min_pos <= tabs->a.size / 2) {
//                 while (min_pos-- > 0)
//                     move_and_print(tabs, RA);
//             } else {
//                 while (min_pos++ < tabs->a.size)
//                     move_and_print(tabs, RRA);
//             }
//             move_and_print(tabs, PB);
//         }
//         sort_three(tabs);
//         while (tabs->b.size > 0)
//             move_and_print(tabs, PA);
//     }
// }

// ///////////////////////////////////////////
// // New helper functions for chunk strategy
// ///////////////////////////////////////////

// /*
//  * find_index_in_range:
//  * Returns the index (starting from 0) of the first node in the given stack
//  * whose value lies between low and high (inclusive). Returns -1 if none found.
//  */
// int find_index_in_range(t_stack *stack, int low, int high) {
//     if (!stack || stack->size == 0)
//         return -1;
//     int index = 0;
//     t_node *current = stack->head;
//     do {
//         if (current->value >= low && current->value <= high)
//             return index;
//         current = current->next;
//         index++;
//     } while (current != stack->head);
//     return -1;
// }

// /*
//  * push_chunks:
//  * Divide stack A into 'chunk_count' ranges (based on normalized values) and
//  * push elements belonging to each chunk from A to B using smart rotations.
//  */
// void push_chunks(t_tabs *tabs, int chunk_count) {
//     int size = tabs->a.size;
//     int chunk_size = size / chunk_count;
//     for (int i = 0; i < chunk_count; i++) {
//         int lower_bound = i * chunk_size;
//         int upper_bound = (i == chunk_count - 1) ? (size - 1) : ((i + 1) * chunk_size - 1);
//         while (find_index_in_range(&tabs->a, lower_bound, upper_bound) != -1) {
//             int idx = find_index_in_range(&tabs->a, lower_bound, upper_bound);
//             if (idx == -1)
//                 break;
//             /* Smart rotate: if the target is in the upper half, do reverse rotation */
//             if (idx <= tabs->a.size / 2) {
//                 while (idx-- > 0)
//                     move_stack(tabs, RA);
//             } else {
//                 int ridx = tabs->a.size - idx;
//                 while (ridx-- > 0)
//                     move_stack(tabs, RRA);
//             }
//             move_stack(tabs, PB);
//         }
//     }
// }

// /*
//  * reassemble_a:
//  * While stack B is not empty, find the maximum element in B,
//  * smart-rotate B to bring it on top, then push it back to A.
//  */
// void reassemble_a(t_tabs *tabs) {
//     while (tabs->b.size > 0) {
//         int max_val = -1;
//         int max_index = 0;
//         int index = 0;
//         (void)index;
//         t_node *cur = tabs->b.head;
//         for (int i = 0; i < tabs->b.size; i++) {
//             if (cur->value > max_val) {
//                 max_val = cur->value;
//                 max_index = i;
//             }
//             cur = cur->next;
//         }
//         if (max_index <= tabs->b.size / 2) {
//             while (max_index-- > 0)
//                 move_stack(tabs, RB);
//         } else {
//             int r = tabs->b.size - max_index;
//             while (r-- > 0)
//                 move_stack(tabs, RRB);
//         }
//         move_stack(tabs, PA);
//     }
// }

// /*
//  * push_swap:
//  * This version normalizes the numbers, then uses a chunk strategy with smart
//  * rotations to push portions of A to B and finally reassemble A from B.
//  */
// void push_swap(t_tabs *tabs) {
//     int size = tabs->a.size;
//     if (size <= 1 || is_sorted(&tabs->a, ASCENDING))
//         return;
//     if (size <= 5) {
//         sort_small_stack(tabs);
//         return;
//     }
//     // --- Normalization ---
//     int *values = malloc(sizeof(int) * size);
//     int *sorted  = malloc(sizeof(int) * size);
//     t_node *current = tabs->a.head;
//     for (int i = 0; i < size; i++) {
//         values[i] = current->value;
//         sorted[i] = current->value;
//         current = current->next;
//     }
//     mergeSort(sorted, 0, size - 1);
//     current = tabs->a.head;
//     for (int i = 0; i < size; i++) {
//         int value = current->value;
//         for (int j = 0; j < size; j++) {
//             if (sorted[j] == value) {
//                 int count = 0;
//                 for (int k = 0; k < i; k++) {
//                     if (values[k] == value)
//                         count++;
//                 }
//                 for (int k = 0; k < count && j < size - 1; k++) {
//                     if (sorted[j] == sorted[j + 1])
//                         j++;
//                 }
//                 current->value = j;
//                 break;
//             }
//         }
//         current = current->next;
//     }
//     free(values);
//     free(sorted);
//     // --- End normalization ---
//     // Adjust the chunk_count (e.g. 5) experimentally to lower moves.
//     int chunk_count = 5;
//     push_chunks(tabs, chunk_count);
//     // Reassemble A from B in descending order.
//     reassemble_a(tabs);
// }

// int main(int argc, char **argv) {
//     t_tabs *tabs;
//     ft_printf("Push Swap Program\n");
//     if (argc < 2)
//         return 0;
//     tabs = create_tabs(argv + 1);
//     if (!tabs)
//         return 0;
//     print_tabs(tabs);
//     g_move_count = 0; // Reset move count
//     push_swap(tabs);
//     print_tabs(tabs);
//     ft_printf("Total moves: %d\n", g_move_count);
//     free_tabs(tabs);
//     return 0;
// }



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "./includes/push_swap.h"

/* --- Type Definitions --- */


/* --- Global Move Count --- */
int g_move_count = 0;

/* --- Utility Functions for Node/Stack Management --- */



/*
 * append_node:
 * Appends the node to the circular doubly-linked list of the stack.
 */
// void append_node(t_stack *stack, t_node *node) {
//     if (!stack->head) {
//         stack->head = node;
//     } else {
//         t_node *tail = stack->head->prev;
//         tail->next = node;
//         node->prev = tail;
//         node->next = stack->head;
//         stack->head->prev = node;
//     }
//     stack->size++;
// }

/*
 * fill_stack:
 * If str is not NULL, converts the string array (each string convertible to int)
 * into nodes which are appended to the stack.
 * If str is NULL, then the stack remains empty.
 */
// t_bool fill_stack(t_stack *stack, char **str, int len) {
//     if (!str)
//         return TRUE; // leave empty
//     for (int i = 0; i < len; i++) {
//         errno = 0;
//         int num = atoi(str[i]); // you may want to use a safer conversion
//         if (errno != 0)
//             return FALSE;
//         t_node *node = create_node(num);
//         if (!node)
//             return FALSE;
//         append_node(stack, node);
//     }
//     return TRUE;
// }

/*
 * free_stack:
 * Free all nodes in the given stack.
 */
// void free_stack(t_stack *stack) {
//     if (!stack || !(stack->head))
//         return;
//     t_node *cur = stack->head;
//     t_node *tmp;
//     for (int i = 0; i < stack->size; i++) {
//         tmp = cur;
//         cur = cur->next;
//         free(tmp);
//     }
//     stack->head = NULL;
//     stack->size = 0;
// }

/* --- Basic Moves --- */

/*
 * sa: Swap the first two elements of stack a.
 */
void sa(t_tabs *tabs) {
    t_node *head = tabs->a.head;
    if (tabs->a.size < 2 || !head || !head->next)
        return;
    int tmp = head->value;
    head->value = head->next->value;
    head->next->value = tmp;
    // (Note: orig values remain unchanged.)
}

/*
 * sb: Swap the first two elements of stack b.
 */
void sb(t_tabs *tabs) {
    t_node *head = tabs->b.head;
    if (tabs->b.size < 2 || !head || !head->next)
        return;
    int tmp = head->value;
    head->value = head->next->value;
    head->next->value = tmp;
}

/*
 * ss: Perform sa and sb simultaneously.
 */
void ss(t_tabs *tabs) {
    sa(tabs);
    sb(tabs);
}

/*
 * pa: Push the top element from stack b to stack a.
 */
void pa(t_tabs *tabs) {
    if (tabs->b.size == 0)
        return;
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
 * pb: Push the top element from stack a to stack b.
 */
void pb(t_tabs *tabs) {
    if (tabs->a.size == 0)
        return;
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
 * ra: Rotate stack a upward.
 */
void ra(t_tabs *tabs) {
    if (tabs->a.size < 2 || !tabs->a.head)
        return;
    tabs->a.head = tabs->a.head->next;
}

/*
 * rb: Rotate stack b upward.
 */
void rb(t_tabs *tabs) {
    if (tabs->b.size < 2 || !tabs->b.head)
        return;
    tabs->b.head = tabs->b.head->next;
}

/*
 * rr: Rotate both stacks upward.
 */
void rr(t_tabs *tabs) {
    ra(tabs);
    rb(tabs);
}

/*
 * rra: Reverse rotate stack a.
 */
void rra(t_tabs *tabs) {
    if (tabs->a.size < 2 || !tabs->a.head)
        return;
    tabs->a.head = tabs->a.head->prev;
}

/*
 * rrb: Reverse rotate stack b.
 */
void rrb(t_tabs *tabs) {
    if (tabs->b.size < 2 || !tabs->b.head)
        return;
    tabs->b.head = tabs->b.head->prev;
}

/*
 * rrr: Reverse rotate both stacks.
 */
void rrr(t_tabs *tabs) {
    rra(tabs);
    rrb(tabs);
}

/*
 * move_stack: Execute a move and print a message.
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

/*
 * is_sorted: Check if a stack is sorted in the given order.
 */
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

/*
 * merge and mergeSort: Standard merge sort implementation.
 */
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
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
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

/*
 * create_tabs: Create and initialize the tabs structure using input strings.
 */
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
    tabs->b.min = 0;
    tabs->a.max = INT_MIN;
    tabs->b.max = 0;
    while (str[len])
        len++;
    if (!fill_stack(&tabs->a, str, len)) {
        free(tabs);
        return NULL;
    }
    /* Initially stack b is empty */
    return tabs;
}

/*
 * free_tabs: Free both stacks and then free the tabs structure.
 */
void free_tabs(t_tabs *tabs) {
    if (!tabs)
        return;
    free_stack(&tabs->a);
    free_stack(&tabs->b);
    free(tabs);
}

/* --- Debug Printing --- */

/*
 * print_tabs: Print both stacks. Here we print the original values (orig)
 * so that the final sorted stack shows the original numbers.
 */
void print_tabs(t_tabs *tabs) {
    int idx = 0;
    int max = (tabs->a.size > tabs->b.size) ? tabs->a.size : tabs->b.size;
    t_node *a = tabs->a.head;
    t_node *b = tabs->b.head;
    printf("stack a\t\tstack b\n");
    printf("-------\t\t-------\n");
    while (idx < max) {
        if (tabs->a.size > idx)
            printf("a:[%d]\t", a->orig);  // print original value
        else
            printf("a:[ ]\t");
        if (b && tabs->b.size > idx)
            printf("b:[%d]\n", b->orig);
        else
            printf("b:[ ]\n");
        if (tabs->a.size > idx)
            a = a->next;
        if (b && tabs->b.size > idx)
            b = b->next;
        idx++;
    }
    printf("-------\t\t-------\n");
    printf("A = Size: %d\n", tabs->a.size);
    printf("B = Size: %d\n", tabs->b.size);
}

/*
 * get_max_bits: Return the number of bits required to represent max_num.
 */
int get_max_bits(int max_num) {
    int max_bits = 0;
    while ((max_num >> max_bits) != 0)
        ++max_bits;
    return max_bits;
}

/* --- Small Stack Sorts --- */

/*
 * sort_three: Sort exactly three elements in stack A.
 */
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

/*
 * sort_small_stack: Sort stacks of size <= 5 using a dedicated method.
 */
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
 * Scans the given stack (usually A) and returns the index of an element whose
 * normalized value is between low and high (inclusive) and which requires the fewest
 * rotations; returns -1 if no element qualifies.
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
 * Divides stack A’s normalized values (which range from 0 to size-1) into chunk_count
 * ranges and pushes each candidate (using the best_index_in_range function) from A to B.
 */
void push_chunks(t_tabs *tabs, int chunk_count) {
    int size = tabs->a.size;
    int chunk_size = size / chunk_count;
    for (int i = 0; i < chunk_count; i++) {
        int lower_bound = i * chunk_size;
        int upper_bound = (i == chunk_count - 1) ?
            (size - 1) : ((i + 1) * chunk_size - 1);
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
        }
    }
}

/*
 * reassemble_a:
 * In stack B, finds the element with the maximum normalized value and uses smart
 * rotations to bring it to the top before pushing it to A.
 */
void reassemble_a(t_tabs *tabs) {
    while (tabs->b.size > 0) {
        int max_val = -1;
        int max_index = 0, index = 0;
        (void)index;
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
 * This is the main sorting function. It first normalizes the numbers (updating only
 * the normalized field “value” while preserving the original value in “orig”),
 * then uses a chunk strategy (with a tunable chunk count) to move portions of A
 * to B and finally reassembles A from B.
 */
void push_swap(t_tabs *tabs) {
    int size = tabs->a.size;
    if (size <= 1 || is_sorted(&tabs->a, ASCENDING))
        return;
    if (size <= 5) {
        sort_small_stack(tabs);
        return;
    }
    // --- Normalization ---
    int *values = malloc(sizeof(int) * size);
    int *sorted  = malloc(sizeof(int) * size);
    t_node *current = tabs->a.head;
    for (int i = 0; i < size; i++) {
        values[i] = current->orig;  // use the original value
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
                // Skip duplicate occurrences if any.
                for (int k = 0; k < count && j < size - 1; k++) {
                    if (sorted[j] == sorted[j + 1])
                        j++;
                }
                current->value = j; // normalized index
                break;
            }
        }
        current = current->next;
    }
    free(values);
    free(sorted);
    // --- End normalization ---
    // Tuning parameter: try 6 chunks for 100 numbers.
    int chunk_count = 6;
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
    print_tabs(tabs);
    g_move_count = 0; // Reset move count
    push_swap(tabs);
    print_tabs(tabs);
    printf("Total moves: %d\n", g_move_count);
    free_tabs(tabs);
    return 0;
}
