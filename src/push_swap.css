// #include "../includes/push_swap.h"
// #include "../includes/ft_printf.h"

// /*
//  * Print the stack
//  */
// void print_tabs(t_tabs *tabs) {
//     int idx;
//     int max;
//     t_node *a = tabs->a.head;
//     t_node *b = tabs->b.head;

//     max = (tabs->a.size > tabs->b.size) ? tabs->a.size : tabs->b.size;
//     idx = 0;
//     ft_printf("stack \t\tstack b\n");
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
//     ft_printf("A = Size: %d\tMin: %d\tMax: %d\n", tabs->a.size, tabs->a.min, tabs->a.max);
//     ft_printf("B = Size: %d\tMin: %d\tMax: %d\n", tabs->b.size, tabs->b.min, tabs->b.max);
// }

// /*
//  * Free the tabs
//  */
// void free_tabs(t_tabs *tabs) {
//     t_node *tmp;
//     t_node *current;

//     if (tabs->a.size > 0) {
//         current = tabs->a.head;
//         while (current->next != tabs->a.head) {
//             tmp = current;
//             current = current->next;
//             free(tmp);
//         }
//     }
//     if (tabs->b.size > 0) {
//         current = tabs->b.head;
//         while (current->next != tabs->b.head) {
//             tmp = current;
//             current = current->next;
//             free(tmp);
//         }
//     }
//     free(tabs);
// }

// /*
//  * Create a stack from a string array
//  */
// t_tabs *create_tabs(char **str) {
//     int len = 0;
//     t_tabs *tabs = (t_tabs *)malloc(sizeof(t_tabs));
//     if (!tabs) return NULL;
//     while (str[len]) len++;
//     if (!(fill_stack(&tabs->a, str, len) && fill_stack(&tabs->b, NULL, len)))
//         return free_tabs(tabs), NULL;
//     return tabs;
// }

// /*
//  * Sort 3 elements in stack A
//  */
// void sort_three(t_tabs *tabs) {
//     t_node *a = tabs->a.head;
//     int first = a->value;
//     int second = a->next->value;
//     int third = a->next->next->value;

//     if (first > second && second < third && first < third)
//         move_stack(tabs, SA);
//     else if (first > second && second > third)
//         move_stack(tabs, SA), move_stack(tabs, RRA);
//     else if (first > second && second < third && first > third)
//         move_stack(tabs, RA);
//     else if (first < second && second > third && first < third)
//         move_stack(tabs, SA), move_stack(tabs, RA);
//     else if (first < second && second > third && first > third)
//         move_stack(tabs, RRA);
// }

// /*
//  * Find position of the smallest element in stack
//  */
// int find_smallest_pos(t_stack *stack) {
//     int pos = 0, min_pos = 0, min_val = stack->head->value;
//     t_node *current = stack->head;
//     while (current->next != stack->head) {
//         if (current->value < min_val) {
//             min_val = current->value;
//             min_pos = pos;
//         }
//         current = current->next;
//         pos++;
//     }
//     return min_pos;
// }

// /*
//  * Push smallest elements to stack B
//  */
// void push_smallest_to_b(t_tabs *tabs, int count) {
//     while (count-- > 0 && tabs->a.size > 3) {
//         int smallest_pos = find_smallest_pos(&tabs->a);
//         int rotations = smallest_pos;

//         // Optimized rotation direction
//         if (smallest_pos > tabs->a.size / 2)
//             rotations = smallest_pos - tabs->a.size;

//         if (rotations >= 0) {
//             while (rotations-- > 0)
//                 move_stack(tabs, RA);
//         } else {
//             while (rotations++ < 0)
//                 move_stack(tabs, RRA);
//         }
//         move_stack(tabs, PB);
//     }
// }

// /*
//  * Main sorting function
//  */
// void push_swap(t_tabs *tabs) {
//     if (is_sorted(&tabs->a, ASCENDING)) return;

//     // Special case for 2 elements
//     if (tabs->a.size == 2 && tabs->a.head->value > tabs->a.head->next->value)
//         move_stack(tabs, SA);

//     // Special case for 3 elements
//     else if (tabs->a.size == 3)
//         sort_three(tabs);

//     // For larger arrays, push smallest elements to B except for 3
//     else {
//         push_smallest_to_b(tabs, tabs->a.size - 3);
//         sort_three(tabs);
//         while (tabs->b.size > 0)
//             move_stack(tabs, PA);
//     }
// }




// int main(int argc, char **argv) {
//     if (argc < 2) return 0;

//     t_tabs *tabs = create_tabs(argv + 1);
//     if (!tabs) return 0;

//     print_tabs(tabs);
//     push_swap(tabs);
//     free_tabs(tabs);
//     print_tabs(tabs);
//     return 0;
// }


// #include "../includes/push_swap.h"
// #include "../includes/ft_printf.h"
// void move_and_print(t_tabs *tabs, t_stack_manove move, int *move_count);
// void push_swap(t_tabs *tabs);

// /*
//  * Print the stack (for debugging)
//  */
// void print_tabs(t_tabs *tabs) {
//     int idx;
//     int max;
//     t_node *a = tabs->a.head;
//     t_node *b = tabs->b.head;

//     max = (tabs->a.size > tabs->b.size) ? tabs->a.size : tabs->b.size;
//     idx = 0;
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
//     ft_printf("A = Size: %d\tMin: %d\tMax: %d\n", tabs->a.size, tabs->a.min, tabs->a.max);
//     ft_printf("B = Size: %d\tMin: %d\tMax: %d\n", tabs->b.size, tabs->b.min, tabs->b.max);
// }

// /*
//  * Free the tabs
//  */
// void free_tabs(t_tabs *tabs) {
//     t_node *tmp;
//     t_node *current;

//     if (!tabs) return;  // Check if tabs is NULL before proceeding

//     if (tabs->a.size > 0) {
//         current = tabs->a.head;
//         if (current) { // Check if the stack isn't totally empty
//             while (current->next != tabs->a.head) {
//                 tmp = current;
//                 current = current->next;
//                 free(tmp);
//             }
//             free(current); // Free the last node to break the circle
//         }
//     }

//     if (tabs->b.size > 0) {
//         current = tabs->b.head;
//         if (current) { // Check if the stack isn't totally empty
//             while (current->next != tabs->b.head) {
//                 tmp = current;
//                 current = current->next;
//                 free(tmp);
//             }
//             free(current); // Free the last node to break the circle
//         }
//     }

//     free(tabs);
// }

// /*
//  * Create a stack from a string array
//  */
// t_tabs *create_tabs(char **str) {
//     int len = 0;
//     t_tabs *tabs = (t_tabs *)malloc(sizeof(t_tabs));
//     if (!tabs) return NULL;
//     while (str[len]) len++;
//     if (!(fill_stack(&tabs->a, str, len) && fill_stack(&tabs->b, NULL, len))) {
//         free_tabs(tabs);
//         return NULL;
//     }
//     return tabs;
// }

// /*
//  * Sort 3 elements in stack A
//  */
// void sort_three(t_tabs *tabs) {
//     t_node *a = tabs->a.head;
//     int first = a->value;
//     int second = a->next->value;
//     int third = a->next->next->value;

//     if (first > second && second < third && first < third) {
//         move_and_print(tabs, SA);
//     }
//     else if (first > second && second > third) {
//         move_and_print(tabs, SA);
//         move_and_print(tabs, RRA);
//     }
//     else if (first > second && second < third && first > third) {
//         move_and_print(tabs, RA);
//     }
//     else if (first < second && second > third && first < third) {
//         move_and_print(tabs, SA);
//         move_and_print(tabs, RA);
//     }
//     else if (first < second && second > third && first > third) {
//         move_and_print(tabs, RRA);
//     }
// }

// // --- MERGE SORT (No stdlib) ---

// void merge(int *arr, int left, int mid, int right) {
//     int i, j, k;
//     int n1 = mid - left + 1;
//     int n2 = right - mid;

//     int L[n1], R[n2]; // Temporary arrays

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
//         int mid = left + (right - left) / 2; // Avoid potential overflow
//         mergeSort(arr, left, mid);
//         mergeSort(arr, mid + 1, right);
//         merge(arr, left, mid, right);
//     }
// }

// /*
//  * Simplify the numbers in stack A to be in the range [0, N) without stdlib
//  */
// void simplify_stack(t_tabs *tabs) {
//     int size = tabs->a.size;
//     int *arr = (int *)malloc(sizeof(int) * size);
//     t_node *current = tabs->a.head;

//     // Copy stack A values to an array
//     for (int i = 0; i < size; i++) {
//         arr[i] = current->value;
//         current = current->next;
//     }

//     // Create a sorted copy (Merge Sort)
//     int *sorted_arr = (int *)malloc(sizeof(int) * size);
//     for (int i = 0; i < size; i++) {
//         sorted_arr[i] = arr[i];
//     }

//     mergeSort(sorted_arr, 0, size - 1); // Use Merge Sort


//     // Replace values in stack A with their index in the sorted array
//     current = tabs->a.head;
//     for (int i = 0; i < size; i++) {
//         for (int j = 0; j < size; j++) {
//             if (arr[i] == sorted_arr[j]) {
//                 current->value = j;
//                 break;
//             }
//         }
//         current = current->next;
//     }

//     free(arr);
//     free(sorted_arr);
// }

// // ---- BITONIC SORT (Conceptual Outline - Requires Stack Operations) ----

// //This is a placeholder implementation of Bitonic Sort. It uses Merge Sort to simplify sorting. The actual Bitonic Sort should not use any additional variables and it should only depend on the two stacks.

// void bitonic_sort(t_tabs *tabs) {
//      // Placeholder: For now, we're just going to sort the array using merge sort
//     //This is a placeholder, you should implement your sorting logic here. The moves must also be counted!
//      int size = tabs->a.size;
//     int *arr = (int *)malloc(sizeof(int) * size);
//     t_node *current = tabs->a.head;

//     // Copy stack A values to an array
//     for (int i = 0; i < size; i++) {
//         arr[i] = current->value;
//         current = current->next;
//     }
//     mergeSort(arr, 0, size -1);
//     current = tabs->a.head;
//     // Copy array to stack
//      for (int i = 0; i < size; i++) {
//        current->value = arr[i];
//         current = current->next;
//     }
//      free(arr);
// }

// /*
//  * Function to print the move and then perform the move
//  */
// void move_and_print(t_tabs *tabs, t_stack_move move) {
//     move_stack(tabs, move);
//     switch (move) {
//         case SA: ft_printf("sa\n"); break;
//         case SB: ft_printf("sb\n"); break;
//         case SS: ft_printf("ss\n"); break;
//         case RA: ft_printf("ra\n"); break;
//         case RB: ft_printf("rb\n"); break;
//         case RR: ft_printf("rr\n"); break;
//         case RRA: ft_printf("rra\n"); break;
//         case RRB: ft_printf("rrb\n"); break;
//         case RRR: ft_printf("rrr\n"); break;
//         case PA: ft_printf("pa\n"); break;
//         case PB: ft_printf("pb\n"); break;
//         default: ft_printf("Unknown move\n"); break;
//     }
// }

// /*
//  * Main sorting function
//  */
// void push_swap(t_tabs *tabs) {
//     if (is_sorted(&tabs->a, ASCENDING)) return;

//     // Special case for 2 elements
//     if (tabs->a.size == 2 && tabs->a.head->value > tabs->a.head->next->value)
//         move_and_print(tabs, SA);

//     // Special case for 3 elements
//     else if (tabs->a.size == 3)
//         sort_three(tabs);

//     // For larger arrays, use bitonic sort
//     else {
//         simplify_stack(tabs);
//         bitonic_sort(tabs);
//     }
// }

// int main(int argc, char **argv) {
//     t_tabs *tabs;

//     if (argc < 2)
//         return (0);
//     tabs = create_tabs(argv + 1);
//     if (!tabs)
//         return (0);
//     print_tabs(tabs);
//     push_swap(tabs); // Call push_swap
//     print_tabs(tabs); //Print tabs after sort

//     free_tabs(tabs);
//     return (0);
// }



#include "../includes/push_swap.h"
#include "../includes/ft_printf.h"

int g_move_count = 0; // Global variable to count moves

void move_and_print(t_tabs *tabs, t_stack_move move);
int get_max_bits(int max_num);

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
    t_node *tmp;
    t_node *current;

    if (!tabs) return;

    if (tabs->a.size > 0) {
        current = tabs->a.head;
        if (current) {
            while (current->next != tabs->a.head) {
                tmp = current;
                current = current->next;
                free(tmp);
            }
            free(current);
        }
    }

    if (tabs->b.size > 0) {
        current = tabs->b.head;
        if (current) {
            while (current->next != tabs->b.head) {
                tmp = current;
                current = current->next;
                free(tmp);
            }
            free(current);
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
    if (!(fill_stack(&tabs->a, str, len) && fill_stack(&tabs->b, NULL, len))) {
        free_tabs(tabs);
        return NULL;
    }
    return tabs;
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

    // Handle small stacks
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

    // 1. Simplify the numbers to be in the range [0, size)
    int *arr = (int *)malloc(sizeof(int) * size);
    t_node *current = tabs->a.head;

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

   // Radix sort implementation
    int max_num = size - 1;
    int max_bits = get_max_bits(max_num);
     int zone_size = size / 4;

     (void)zone_size;

         for (int i = 0; i < max_bits; ++i) {
            int j = 0;
            int ra_count = 0;

        while(j < size){


          if (((tabs->a.head->value >> i) & 1) == 0) {
                move_and_print(tabs, PB);
             } else {
                move_and_print(tabs, RA);
                     ra_count++;
             }


            j++;
          current = tabs->a.head;
        }



    for(int k = 0 ; k < (size - ra_count);k++){
        move_and_print(tabs, RRA);
    }


            while (tabs->b.size > 0) {
           move_and_print(tabs, PA);   // Move from stack B back to stack A
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