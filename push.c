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
    t_node *a_head = tabs->a.head;

    // Remove node from stack B
    tabs->b.head = (tabs->b.size > 1) ? b_head->next : NULL;
    if (tabs->b.head) {
         t_node *current = tabs->b.head;
        while(current->next != b_head) {
            current = current->next;
         }
         current->next = tabs->b.head;

    }
    b_head->next = NULL; // Detach b_head

    tabs->b.size--;
    if (tabs->b.size == 0) {
        tabs->b.min = 0;
        tabs->b.max = 0;
    }

     // Add node to stack A
    if (tabs->a.size == 0) {
        tabs->a.head = b_head;
         b_head->next = b_head; // Make it circular
    } else {
        t_node *a_tail = a_head;
         while(a_tail->next != a_head) {
            a_tail = a_tail->next;
        }
        b_head->next = a_head;
        a_tail->next = b_head;
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
    t_node *b_head = tabs->b.head;

    // Remove node from stack A
    tabs->a.head = (tabs->a.size > 1) ? a_head->next : NULL;
     if (tabs->a.head) {
           t_node *current = tabs->a.head;
        while(current->next != a_head) {
            current = current->next;
         }
         current->next = tabs->a.head;

    }
    a_head->next = NULL; // Detach a_head

    tabs->a.size--;
    if (tabs->a.size == 0) {
        tabs->a.min = 0;
        tabs->a.max = 0;
    }

    // Add node to stack B
    if (tabs->b.size == 0) {
        tabs->b.head = a_head;
             a_head->next = a_head; // Make it circular
    } else {
           t_node *b_tail = b_head;
        while(b_tail->next != b_head) {
            b_tail = b_tail->next;
        }
        a_head->next = b_head;
        b_tail->next = a_head;
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
 * Main sorting function using Radix Sort with Zones
 */
void push_swap(t_tabs *tabs) {
     int size = tabs->a.size;
       if (size <= 1 || is_sorted(&tabs->a, ASCENDING)) return;


       //Handle small stacks
       if (size <= 5) {
           if (size == 2 && tabs->a.head->value > tabs->a.head->next->value)
              move_and_print(tabs, SA);
           else if (size == 3){
               t_node *head = tabs->a.head;
               int a = head->value;
               int b = head->next->value;
               int c = head->next->next->value;
                if (a > b && b < c && a < c){
                    move_and_print(tabs, SA);
                } else if(a > b && b > c){
                    move_and_print(tabs, SA);
                    move_and_print(tabs, RRA);
                } else if(a > b && b < c && a > c){
                    move_and_print(tabs, RA);
                } else if (a < b && b > c && a < c){
                   move_and_print(tabs, SA);
                   move_and_print(tabs, RA);
                } else if (a < b && b > c && a > c){
                   move_and_print(tabs, RRA);
                }
           }
          return;
       }

       // Simplify the numbers to be in the range [0, size)
       int *arr = (int *)malloc(sizeof(int) * size);
       t_node *current = tabs->a.head;
        // t_node *next;


       // Copy stack A values to an array
       for (int i = 0; i < size; i++) {
           arr[i] = current->value;
           current = current->next;
       }

       // Create a sorted copy (Merge Sort)
       int *sorted_arr = (int *)malloc(sizeof(int) * size);
       for (int i = 0; i < size; i++) {
           sorted_arr[i] = arr[i];
       }
       mergeSort(sorted_arr, 0, size - 1);

       // Replace values in stack A with their index in the sorted array
       current = tabs->a.head;
       for (int i = 0; i < size; i++) {
           for (int j = 0; j < size; j++) {
               if (arr[i] == sorted_arr[j]) {
                   current->value = j;
                   break;
               }
           }
           current = current->next;
       }

    free(arr);
    free(sorted_arr);

       //Radix sort implementation
       int max_num = size - 1;
       int max_bits = get_max_bits(max_num);
        // int zone_size = size / 4;

            for (int i = 0; i < max_bits; ++i) {
               int j = 0;

               while(j < size) {

                if (((tabs->a.head->value >> i) & 1) == 0) {
                move_and_print(tabs, PB);
                 } else {
                 move_and_print(tabs, RA);
                 }


                j++;
                  if(tabs->a.head != tabs->a.head->next) {
                   current = tabs->a.head;
                  }
               }

            while (tabs->b.size > 0) {
               move_and_print(tabs, PA);
             }

    }

}

int main(int argc, char **argv) {
    t_tabs *tabs;

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