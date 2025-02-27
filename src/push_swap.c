// // #include "../includes/push_swap.h"
// // #include "../includes/ft_printf.h"

// // /*
// // * Print the stack
// // * @param *stack: the stack to print
// // */
// // void print_tabs(t_tabs *tabs)
// // {
// //     int idx;
// //     int max;
// //     t_node *a;
// //     t_node *b;

// //     a = tabs->a.head;
// //     b = tabs->b.head;

// //     if (tabs->a.size > tabs->b.size)
// //         max = tabs->a.size;
// //     else 
// //         max = tabs->b.size;
// //     idx = 0;
// //     ft_printf("stack \t\tstack b\n");
// //     ft_printf("-------\t\t-------\n");
// //     while (idx < max)
// //     {
// //         if (tabs->a.size > idx)
// //             ft_printf("a:[%d]\t", a->value);
// //         else
// //             ft_printf("a:[ ]\t");
// //         if (a->value <= 999)
// //             ft_printf("\t");
// //         if (b && tabs->b.size >= idx)
// //             ft_printf("b:[%d]\b\n", b->value);
// //         else
// //             ft_printf("b:[ ]\n");
// //         if (tabs->a.size > idx)
// //             a = a->next;
// //         if (a == tabs->a.head)
// //             break;
// //         if (b && tabs->b.size >= idx)
// //         {
// //             b = b->next;
// //             if (b == tabs->b.head)
// //                 b = NULL;
// //         }
// //         idx++;
// //     }
// //     ft_printf("-------\t\t-------\n");
// //     ft_printf("A = Size: %d\tMin: %d\tMax: %d\n", tabs->a.size, tabs->a.min, tabs->a.max);
// //     ft_printf("B = Size: %d\tMin: %d\tMax: %d\n", tabs->b.size, tabs->b.min, tabs->b.max);
// // }

// // /*
// // * Free the tabs
// // * @param *tabs: the tabs to fill
// // */
// // void free_tabs(t_tabs *tabs)
// // {
// //     t_node *tmp;
// //     t_node *current;

// //     if (tabs->a.size > 0)
// //     {
// //         current = tabs->a.head;
// //         while (current->next != tabs->a.head)
// //         {
// //             tmp = current;
// //             current = current->next;
// //             free(tmp);
// //         }
// //     }
// //     if (tabs->b.size > 0)
// //     {
// //         current = tabs->b.head;
// //         while (current->next != tabs->b.head)
// //         {
// //             tmp = current;
// //             current = current->next;
// //             free(tmp);
// //         }
        
// //     }
// //     free(tabs);
// // }

// // /*
// // * Create a stack from a string array
// // * @param **str: array of strings
// // * @return t_tabs: a struct containing two stacks
// // * @return NULL: if the stack is invalid
// // */
// // t_tabs *create_tabs(char **str)
// // {
// //     int len;
// //     t_tabs *tabs;

// //     len = 0;
// //     tabs = (t_tabs *)malloc(sizeof(t_tabs));
// //     if (!tabs)
// //         return (NULL);
// //     while (str[len])
// //         len++;
// //     if (!(fill_stack(&tabs->a, str, len) && fill_stack(&tabs->b, NULL, len)))
// //         return (free_tabs(tabs), NULL);
// //     // print_tabs(tabs);
// //     return (tabs);
// // }

// // /*
// // * Sort 3 elements in stack A
// // * @param *tabs: the tabs to sort
// // */
// // void sort_three(t_tabs *tabs)
// // {
// //     t_node *a = tabs->a.head;
// //     int first = a->value;
// //     int second = a->next->value;
// //     int third = a->next->next->value;
    
// //     if (first > second && second < third && first < third)
// //     {
// //         // case: 2 1 3
// //         move_stack(tabs, SA);
// //         // print_tabs(tabs);
// //     }
// //     else if (first > second && second > third)
// //     {
// //         // case: 3 2 1
// //         move_stack(tabs, SA);
// //         // print_tabs(tabs);
// //         move_stack(tabs, RRA);
// //         // print_tabs(tabs);
// //     }
// //     else if (first > second && second < third && first > third)
// //     {
// //         // case: 3 1 2
// //         move_stack(tabs, RA);
// //         // print_tabs(tabs);
// //     }
// //     else if (first < second && second > third && first < third)
// //     {
// //         // case: 1 3 2
// //         move_stack(tabs, SA);
// //         // print_tabs(tabs);
// //         move_stack(tabs, RA);
// //         // print_tabs(tabs);
// //     }
// //     else if (first < second && second > third && first > third)
// //     {
// //         // case: 2 3 1
// //         move_stack(tabs, RRA);
// //         // print_tabs(tabs);
// //     }
// //     // case: 1 2 3 is already sorted
// // }

// // /*
// // * Find position of the smallest element in stack
// // * @param *stack: the stack to check
// // * @return int: position of smallest element
// // */
// // int find_smallest_pos(t_stack *stack)
// // {
// //     int pos;
// //     int min_val;
// //     int min_pos;
// //     t_node *current;
    
    
// //     pos = 0;
// //     min_pos = 0;
// //     current = stack->head;
// //     min_val = current->value;
    
// //     while (current->next != stack->head)
// //      {
// //         if (current->value < min_val)
// //         {
// //             min_val = current->value;
// //             min_pos = pos;
// //         }
// //         current = current->next;
// //         pos++;
// //     };
// //     return min_pos;
// // }

// // /*
// // * Find position of the largest element in stack
// // * @param *stack: the stack to check
// // * @return int: position of largest element
// // */
// // int find_largest_pos(t_stack *stack)
// // {
// //     int pos;
// //     int max_pos;
// //     int max_val;
// //     t_node *current;
    
    
// //     pos = 0;
// //     max_pos = 0;
// //     current = stack->head;
// //     max_val = current->value;
// //     while (current->next != stack->head)
// //     {
// //         if (current->value > max_val)
// //         {
// //             max_val = current->value;
// //             max_pos = pos;
// //         }
// //         current = current->next;
// //         pos++;
// //     }
// //     return max_pos;
// // }

// // /*
// // * Push smallest elements to stack B
// // * @param *tabs: the tabs to work with
// // * @param count: how many elements to push
// // */
// // void push_smallest_to_b(t_tabs *tabs, int count)
// // {
// //     int idx;

// //     idx = 0;
// //     while (count > 0 && tabs->a.size > 3)
// //     {
// //         int smallest_pos = find_smallest_pos(&tabs->a);
// //         int rotations = smallest_pos;
        
// //         // Optimize rotations
// //         if (smallest_pos > tabs->a.size / 2)
// //             rotations = smallest_pos - tabs->a.size;
        
// //         if (rotations >= 0)
// //         {
// //             idx = 0;
// //             while (idx < rotations)
// //             {
// //                 move_stack(tabs, RA);
// //                 // print_tabs(tabs);
// //                 idx++;
// //             }
            
// //         }
// //         else
// //         {
// //             idx = 0;
// //             while (idx < -rotations)
// //             {
// //                 move_stack(tabs, RRA);
// //                 // print_tabs(tabs);
// //                 idx++;
// //             }
// //         }
        
// //         move_stack(tabs, PB);
// //         // print_tabs(tabs);
// //         count--;
// //     }
// // }

// // /*
// // * Main sorting function
// // * @param *tabs: the tabs to sort
// // */
// // void push_swap(t_tabs *tabs)
// // {
// //     // ft_printf("\n\nsorting stack\n\n");
    
// //     // If already sorted
// //     if (is_sorted(&tabs->a, ASCENDING))
// //     {
// //         // ft_printf("stack is already sorted\n");
// //         return;
// //     }
    
// //     // Special case for 2 elements
// //     if (tabs->a.size == 2)
// //     {
// //         if (tabs->a.head->value > tabs->a.head->next->value)
// //         {
// //             move_stack(tabs, SA);
// //             // print_tabs(tabs);
// //         }
// //         return;
// //     }
    
// //     // Special case for 3 elements
// //     if (tabs->a.size == 3)
// //     {
// //         sort_three(tabs);
// //         return;
// //     }
    
// //     // For larger arrays, push smallest elements to B except for 3
// //     push_smallest_to_b(tabs, tabs->a.size - 3);
    
// //     // Sort the remaining 3 elements in A
// //     sort_three(tabs);
    
// //     // Push elements back from B to A
// //     while (tabs->b.size > 0)
// //     {
// //         move_stack(tabs, PA);
// //         // print_tabs(tabs);
// //     }
    
// //     // print_tabs(tabs);
// // }
 
// // // int main(int argc, char **argv)
// // // {
// // //     t_tabs *tabs;

// // //     if (argc == 1)
// // // 		return (EXIT_FAILURE);
// // // 	if (argc == 2)
// // // 		tabs= create_tabs(ft_split(argv[1], ' '));
// // // 	else
// // // 		tabs = create_tabs(argv + 1);
// // // 	if (!tabs)
// // // 		return (ft_printf("Error\n"), EXIT_FAILURE);
// // //     print_tabs(tabs);
// // //     push_swap(tabs);

// // //     print_tabs(tabs);
// // // 	free_tabs(tabs);
// // // 	return (EXIT_SUCCESS);
// // // }


// #include "../includes/push_swap.h"
// #include "../includes/ft_printf.h"

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

// t_tabs *create_tabs(char **str) {
//     int len = 0;
//     t_tabs *tabs = (t_tabs *)malloc(sizeof(t_tabs));
//     if (!tabs) return NULL;
//     while (str[len]) len++;
//     if (!(fill_stack(&tabs->a, str, len) && fill_stack(&tabs->b, NULL, len)))
//         return free_tabs(tabs), NULL;
//     return tabs;
// }

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

// void push_smallest_to_b(t_tabs *tabs, int count) {
//     while (count-- > 0 && tabs->a.size > 3) {
//         int smallest_pos = find_smallest_pos(&tabs->a);
//         while (smallest_pos-- > 0)
//             move_stack(tabs, RA);
//         move_stack(tabs, PB);
//     }
// }

// void push_swap(t_tabs *tabs) {
//     if (is_sorted(&tabs->a, ASCENDING)) return;
//     if (tabs->a.size == 2 && tabs->a.head->value > tabs->a.head->next->value)
//         move_stack(tabs, SA);
//     else if (tabs->a.size == 3)
//         sort_three(tabs);
//     else {
//         push_smallest_to_b(tabs, tabs->a.size - 3);
//         sort_three(tabs);
//         while (tabs->b.size > 0)
//             move_stack(tabs, PA);
//     }
// }

// // int main(int argc, char **argv) {
// //     if (argc == 1) return EXIT_FAILURE;
// //     t_tabs *tabs = (argc == 2) ? create_tabs(ft_split(argv[1], ' ')) : create_tabs(argv + 1);
// //     if (!tabs) return ft_printf("Error\n"), EXIT_FAILURE;
// //     push_swap(tabs);
// //     // print_tabs(tabs);
// //     free_tabs(tabs);
// //     return EXIT_SUCCESS;
// // }



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

