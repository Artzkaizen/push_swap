/**
 * @file push_swap_chunk_sort.c
 * @brief Solves the push_swap problem using a recursive chunk sort algorithm.
 *
 * @details
 * The push_swap problem requires sorting a stack of unique integers (Stack A)
 * using a limited set of operations and an auxiliary stack (Stack B), aiming
 * to minimize the number of moves.
 *
 * This implementation utilizes a "chunk sort" strategy, which is a recursive,
 * divide-and-conquer algorithm inspired by Quicksort:
 *
 * 1.  **Normalization:** Input integers are mapped to their sorted indices (0 to N-1).
 *     This simplifies comparisons and pivot calculations.
 * 2.  **Chunk Representation:** Logical "chunks" of elements (defined by their
 *     normalized value range and location) are processed. Locations can be
 *     TOP_A, BOTTOM_A, TOP_B, BOTTOM_B.
 * 3.  **Recursion:**
 *     - **Base Cases:** Chunks of size 1, 2, or 3 are sorted directly using
 *       optimized move sequences (`sort_one`, `sort_two`, `sort_three`).
 *     - **Recursive Step:** Larger chunks are split into three sub-chunks (min, mid, max)
 *       based on two pivot values calculated from the chunk's value range.
 *       Elements are partitioned and moved to specific destination locations
 *       between Stack A and Stack B using the `move_from_to` function.
 *       Optimizations (`split_max_reduction`) are applied during splitting to
 *       sort elements at the top of Stack A early if possible.
 *       The algorithm then recurses on the max, mid, and min sub-chunks.
 * 4.  **Final Rotation:** After the recursion completes, Stack A is rotated
 *     (if necessary) to bring the smallest element (normalized value 0) to the top.
 *
 * Allowed Moves:
 * - sa: swap the first 2 elements at the top of stack A.
 * - sb: swap the first 2 elements at the top of stack B.
 * - ss: sa and sb at the same time.
 * - pa: take the first element from B and put it at the top of A.
 * - pb: take the first element from A and put it at the top of B.
 * - ra: shift up all elements of stack A by 1 (first becomes last).
 * - rb: shift up all elements of stack B by 1 (first becomes last).
 * - rr: ra and rb at the same time.
 * - rra: shift down all elements of stack A by 1 (last becomes first).
 * - rrb: shift down all elements of stack B by 1 (last becomes first).
 * - rrr: rra and rrb at the same time.
 *
 * Data Structures:
 * - `t_node`: Represents an element with original and normalized values, using
 *            `next` and `prev` pointers for a doubly linked circular list.
 * - `t_stack`: Represents Stack A or B, holding the head node and size.
 * - `t_tabs`: Container for both `t_stack` A and B.
 * - `t_loc`: Enum defining logical locations (TOP_A, BOTTOM_A, TOP_B, BOTTOM_B).
 * - `t_chunk`: Represents a logical group of elements to be sorted, defined by
 *              its location and size.
 * - `t_split_dest`: Holds the three resulting chunks after splitting.
 *
 * Output:
 * - The sequence of push_swap moves required to sort the stack is printed to stdout.
 * - The total number of moves is printed to stderr.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <limits.h>
 #include <errno.h>
 // #include <stdbool.h> // Removed - using custom t_bool
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
 t_bool has_duplicates(t_stack *stack);
 t_bool is_valid_int_str(const char *str);
 
 
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
 t_bool is_consecutive(int a, int b, int c, int d); // Changed return type
 t_bool a_partly_sort(t_tabs *data, int from_top_count); // Changed return type
 void split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest); // Optimization during split
 
 /* --- Global Move Count --- */
 int g_move_count = 0;
 
 /* --- User Function Implementations --- */
 
 /**
  * @brief Allocates and initializes a new stack node.
  * @param num The original integer value for the node.
  * @return Pointer to the newly created node, or NULL on allocation failure.
  */
 t_node *create_node(int num)
 {
     t_node *node;
 
     node = malloc(sizeof(t_node));
     if (!node)
         return (NULL);
     node->orig = num;
     node->value = 0; // Normalized value initialized later
     node->next = node; // Initialize as circular list of one
     node->prev = node;
     return (node);
 }
 
 /**
  * @brief Appends a node to the end of a stack's circular linked list.
  * @param stack Pointer to the stack structure.
  * @param node Pointer to the node to append.
  */
 void append_node(t_stack *stack, t_node *node)
 {
     t_node *tail;
 
     if (!stack || !node)
         return;
     if (!stack->head) // If stack is empty
     {
         stack->head = node;
         node->next = node;
         node->prev = node;
     }
     else // Stack is not empty
     {
         tail = stack->head->prev; // Find the current tail
         tail->next = node;        // Current tail points to new node
         node->prev = tail;        // New node points back to old tail
         node->next = stack->head; // New node points forward to head
         stack->head->prev = node; // Head points back to new node (new tail)
     }
     stack->size++;
 }
 
 /**
  * @brief Checks for duplicate original values ('orig' field) in the stack
  *        using only pointer iteration and while loops.
  * @param stack Pointer to the t_stack structure.
  * @return t_bool TRUE if duplicates are found, FALSE otherwise.
  */
 t_bool has_duplicates(t_stack *stack)
 {
     t_node *outer_node;
     t_node *inner_node;
     int outer_count;
     int inner_count; // Counter for inner loop iterations
 
     // Handle edge cases: empty stack, null stack, or single element stack
     if (!stack || !stack->head || stack->size <= 1)
         return (FALSE); // No duplicates possible
 
     outer_node = stack->head;
     outer_count = 0;
     // Outer loop: Iterate through each node once
     while (outer_count < stack->size)
     {
         // Inner loop: Start from the node *after* outer_node
         inner_node = outer_node->next;
         inner_count = 0;
         // Compare outer_node with ALL nodes *after* it by iterating N-1 times
         while (inner_count < stack->size - 1)
         {
             if (outer_node->orig == inner_node->orig)
             {
                 fprintf(stderr, "Error: Duplicate %d\n", outer_node->orig);
                 return (TRUE); // Found a duplicate
             }
             inner_node = inner_node->next; // Advance inner pointer (will wrap around)
             inner_count++;
         }
         // Advance the outer loop pointer
         outer_node = outer_node->next;
         outer_count++;
     }
     // If we finish all loops without returning TRUE, no duplicates were found
     return (FALSE);
 }
 
 
 /**
  * @brief Validates if a string represents a valid integer within INT_MIN/INT_MAX.
  * @param str The string to validate.
  * @return t_bool TRUE if valid, FALSE otherwise.
  */
 t_bool is_valid_int_str(const char *str)
 {
     int i;
     long num;
     char *endptr; // For strtol error checking
 
     i = 0;
     if (!str || *str == '\0') // Check for NULL or empty string
         return (FALSE);
 
     // Check for optional sign
     if (str[i] == '-' || str[i] == '+')
     {
         i++;
         if (str[i] == '\0') // Sign only is invalid
             return (FALSE);
     }
     // Check if first char after sign is a digit
     if (str[i] < '0' || str[i] > '9')
         return (FALSE);
 
     // Check remaining characters are digits
     while (str[i])
     {
         if (str[i] < '0' || str[i] > '9')
             return (FALSE); // Invalid character
         i++;
     }
 
     // Use strtol for range checking and full validation
     errno = 0; // Reset errno before calling strtol
     num = strtol(str, &endptr, 10);
 
     if (errno == ERANGE || num > INT_MAX || num < INT_MIN)
         return (FALSE); // Out of integer range
 
     // If we reach here, the string is a valid integer representation
     return (TRUE);
 }
 
 /**
  * @brief Fills stack A with nodes created from input string arguments.
  *        Validates input strings and checks for duplicates.
  * @param stack Pointer to stack A (must be initialized).
  * @param str Array of strings representing numbers.
  * @param len Number of strings in the array.
  * @return t_bool TRUE on success, FALSE on error (invalid input, malloc fail, duplicate).
  */
 t_bool fill_stack(t_stack *stack, char **str, int len)
 {
     int i;
     long n_l;
     t_node *node;
     char *ep; // For strtol error checking
 
     i = 0;
     while (i < len)
     {
         if (!is_valid_int_str(str[i]))
         {
             fprintf(stderr, "Error: Invalid input '%s'.\n", str[i]);
             return (FALSE);
         }
         errno = 0;
         n_l = strtol(str[i], &ep, 10);
         if (errno == ERANGE || *ep != '\0' || n_l > INT_MAX || n_l < INT_MIN)
         {
             fprintf(stderr, "Error: Invalid integer/range '%s'.\n", str[i]);
             return (FALSE);
         }
         node = create_node((int)n_l);
         if (!node)
         {
             fprintf(stderr, "Error: Malloc failed creating node.\n");
             return (FALSE); // Malloc failure
         }
         append_node(stack, node);
         i++;
     }
     if (has_duplicates(stack))
         return (FALSE);
 
     return (TRUE); // Success
 }
 
 /**
  * @brief Frees all nodes in a stack and resets its head and size.
  * @param stack Pointer to the stack structure to free.
  */
 void free_stack(t_stack *stack)
 {
     t_node *c;
     t_node *next_n;
     t_node *tail;
 
     if (!stack || !stack->head)
         return;
 
     tail = stack->head->prev;
     tail->next = NULL; // Break the circle
 
     c = stack->head;
     while (c != NULL)
     {
         next_n = c->next;
         free(c);
         c = next_n;
     }
     stack->head = NULL;
     stack->size = 0;
 }
 
 /**
  * @brief Creates the main tabs structure, initializes stacks A and B,
  *        and fills stack A from input arguments.
  * @param str Array of input number strings.
  * @param len Number of input strings.
  * @return Pointer to the allocated and initialized t_tabs structure,
  *         or NULL on error (malloc fail, invalid input).
  */
 t_tabs *create_tabs(char **str, int len)
 {
     t_tabs *tabs;
 
     tabs = malloc(sizeof(t_tabs));
     if (!tabs)
         return (NULL);
 
     tabs->a.head = NULL;
     tabs->a.size = 0;
     tabs->b.head = NULL;
     tabs->b.size = 0;
 
     if (!fill_stack(&tabs->a, str, len))
     {
         free_tabs(tabs); // Clean up on error
         return (NULL);
     }
     return (tabs);
 }
 
 /**
  * @brief Frees the memory associated with both stacks and the tabs structure itself.
  * @param tabs Pointer to the t_tabs structure to free.
  */
 void free_tabs(t_tabs *tabs)
 {
     if (!tabs)
         return;
     free_stack(&tabs->a);
     free_stack(&tabs->b);
     free(tabs);
 }
 
 /**
  * @brief Swaps the values of two integers.
  * @param a Pointer to the first integer.
  * @param b Pointer to the second integer.
  */
 void swap_ints(int *a, int *b)
 {
     int temp;
 
     temp = *a;
     *a = *b;
     *b = temp;
 }
 
 /**
  * @brief Normalizes the stack values: assigns each node's `value` field
  *        to its sorted index (0 to N-1) based on its `orig` field.
  * @param a Pointer to the stack (usually stack A) to normalize.
  * @note Uses a temporary array and a simple sort (Bubble Sort here).
  *       Exits if memory allocation fails.
  */
 void normalize_stack(t_stack *a)
 {
     int *arr;
     t_node *c;
     t_bool s; // Use t_bool for the swap flag
     int i;
     int j;
 
     if (!a || a->size <= 1)
         return;
 
     arr = malloc(sizeof(int) * a->size);
     if (!arr)
     {
         fprintf(stderr, "Error: Malloc failed during normalization\n");
         exit(1);
     }
 
     c = a->head;
     i = 0;
     while (i < a->size)
     {
         arr[i] = c->orig;
         c = c->next;
         i++;
     }
 
     i = 0;
     while (i < a->size - 1)
     {
         s = FALSE; // Initialize swap flag to FALSE
         j = 0;
         while (j < a->size - 1 - i)
         {
             if (arr[j] > arr[j + 1])
             {
                 swap_ints(&arr[j], &arr[j + 1]);
                 s = TRUE; // Set flag to TRUE if swap occurred
             }
             j++;
         }
         if (s == FALSE)
             break;
         i++;
     }
 
     c = a->head;
     i = 0;
     while (i < a->size)
     {
         j = 0;
         while (j < a->size)
         {
             if (c->orig == arr[j])
             {
                 c->value = j;
                 break;
             }
             j++;
         }
         c = c->next;
         i++;
     }
 
     free(arr);
 }
 
 /**
  * @brief Checks if a stack is sorted based on the normalized `value` field.
  * @param stack Pointer to the stack structure.
  * @return t_bool TRUE if sorted or size <= 1, FALSE otherwise.
  */
 t_bool is_sorted(t_stack *stack)
 {
     t_node *c;
     int i;
 
     if (!stack || !stack->head || stack->size <= 1)
         return (TRUE);
 
     c = stack->head;
     i = 0;
     while (i < stack->size - 1)
     {
         if (c->value > c->next->value)
             return (FALSE);
         c = c->next;
         i++;
     }
     return (TRUE);
 }
 
 /**
  * @brief Checks if a stack is sorted based on the original `orig` field.
  * @param stack Pointer to the stack structure.
  * @return t_bool TRUE if sorted or size <= 1, FALSE otherwise.
  */
 t_bool is_raw_input_sorted(t_stack *stack)
 {
     t_node *c;
     int i;
 
     if (!stack || !stack->head || stack->size <= 1)
         return (TRUE);
 
     c = stack->head;
     i = 0;
     while (i < stack->size - 1)
     {
         if (c->orig > c->next->orig)
             return (FALSE);
         c = c->next;
         i++;
     }
     return (TRUE);
 }
 
 /* --- Stack Move Implementations --- */
 
 /** @brief Swaps the top two elements of stack A (sa). Does nothing if size < 2. */
 void sa(t_tabs *tabs)
 {
     t_node *f;
     int v;
     int o;
 
     if (!tabs || tabs->a.size < 2)
         return;
     f = tabs->a.head;
     v = f->value;
     o = f->orig;
     f->value = f->next->value;
     f->orig = f->next->orig;
     f->next->value = v;
     f->next->orig = o;
 }
 
 /** @brief Swaps the top two elements of stack B (sb). Does nothing if size < 2. */
 void sb(t_tabs *tabs)
 {
     t_node *f;
     int v;
     int o;
 
     if (!tabs || tabs->b.size < 2)
         return;
     f = tabs->b.head;
     v = f->value;
     o = f->orig;
     f->value = f->next->value;
     f->orig = f->next->orig;
     f->next->value = v;
     f->next->orig = o;
 }
 
 /** @brief Performs sa and sb simultaneously (ss). */
 void ss(t_tabs *tabs)
 {
     if (!tabs) return;
     sa(tabs);
     sb(tabs);
 }
 
 /** @brief Pushes the top element of stack B onto stack A (pa). Does nothing if B is empty. */
 void pa(t_tabs *tabs)
 {
     t_node *m;
 
     if (!tabs || tabs->b.size == 0)
         return;
 
     m = tabs->b.head;
 
     if (tabs->b.size == 1)
     {
         tabs->b.head = NULL;
     }
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
 
 /** @brief Pushes the top element of stack A onto stack B (pb). Does nothing if A is empty. */
 void pb(t_tabs *tabs)
 {
     t_node *m;
 
     if (!tabs || tabs->a.size == 0)
         return;
 
     m = tabs->a.head;
 
     if (tabs->a.size == 1)
     {
         tabs->a.head = NULL;
     }
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
 
 /** @brief Rotates stack A (ra): first element becomes the last. Does nothing if size < 2. */
 void ra(t_tabs *tabs)
 {
     if (!tabs || tabs->a.size < 2)
         return;
     tabs->a.head = tabs->a.head->next;
 }
 
 /** @brief Rotates stack B (rb): first element becomes the last. Does nothing if size < 2. */
 void rb(t_tabs *tabs)
 {
     if (!tabs || tabs->b.size < 2)
         return;
     tabs->b.head = tabs->b.head->next;
 }
 
 /** @brief Performs ra and rb simultaneously (rr). */
 void rr(t_tabs *tabs)
 {
     if (!tabs) return;
     ra(tabs);
     rb(tabs);
 }
 
 /** @brief Reverse rotates stack A (rra): last element becomes the first. Does nothing if size < 2. */
 void rra(t_tabs *tabs)
 {
     if (!tabs || tabs->a.size < 2)
         return;
     tabs->a.head = tabs->a.head->prev;
 }
 
 /** @brief Reverse rotates stack B (rrb): last element becomes the first. Does nothing if size < 2. */
 void rrb(t_tabs *tabs)
 {
     if (!tabs || tabs->b.size < 2)
         return;
     tabs->b.head = tabs->b.head->prev;
 }
 
 /** @brief Performs rra and rrb simultaneously (rrr). */
 void rrr(t_tabs *tabs)
 {
     if (!tabs) return;
     rra(tabs);
     rrb(tabs);
 }
 
 /**
  * @brief Executes a specific stack move operation.
  * @param tabs Pointer to the t_tabs structure containing stacks A and B.
  * @param move The t_stack_move enum value representing the move to perform.
  */
 void move_stack(t_tabs *tabs, t_stack_move move)
 {
     switch (move)
     {
     case SA:  sa(tabs);  break;
     case SB:  sb(tabs);  break;
     case SS:  ss(tabs);  break;
     case PA:  pa(tabs);  break;
     case PB:  pb(tabs);  break;
     case RA:  ra(tabs);  break;
     case RB:  rb(tabs);  break;
     case RR:  rr(tabs);  break;
     case RRA: rra(tabs); break;
     case RRB: rrb(tabs); break;
     case RRR: rrr(tabs); break;
     }
 }
 
 /**
  * @brief Executes a stack move and prints its name to standard output.
  * @param tabs Pointer to the t_tabs structure containing stacks A and B.
  * @param move The t_stack_move enum value representing the move to perform and print.
  */
 void move_and_print(t_tabs *tabs, t_stack_move move)
 {
     if (move == SA)       printf("sa\n");
     else if (move == SB)  printf("sb\n");
     else if (move == SS)  printf("ss\n");
     else if (move == PA)  printf("pa\n");
     else if (move == PB)  printf("pb\n");
     else if (move == RA)  printf("ra\n");
     else if (move == RB)  printf("rb\n");
     else if (move == RR)  printf("rr\n");
     else if (move == RRA) printf("rra\n");
     else if (move == RRB) printf("rrb\n");
     else if (move == RRR) printf("rrr\n");
 
     move_stack(tabs, move);
     g_move_count++;
 }
 
 /* --- Chunk Sort Algorithm Implementation & Helpers --- */
 
 /**
  * @brief Returns a pointer to the stack (A or B) associated with a logical location.
  * @param data Pointer to the t_tabs structure.
  * @param loc The t_loc enum value (TOP_A, BOTTOM_A, TOP_B, BOTTOM_B).
  * @return Pointer to the corresponding t_stack (data->a or data->b).
  */
 t_stack *loc_to_stack(t_tabs *data, t_loc loc)
 {
     if (loc == TOP_A || loc == BOTTOM_A)
         return (&data->a);
     else
         return (&data->b);
 }
 
 /**
  * @brief Gets the normalized value of the nth element within a chunk.
  * @param data Pointer to the t_tabs structure.
  * @param chunk Pointer to the t_chunk descriptor.
  * @param n The position within the chunk (1-based index: 1=first, 2=second...).
  * @return The normalized `value` of the element, or INT_MIN on error.
  * @note Access direction depends on chunk->loc (TOP_* uses ->next, BOTTOM_* uses ->prev).
  */
 int chunk_value(t_tabs *data, t_chunk *chunk, int n)
 {
     t_stack *stk;
     t_node *current;
     int i;
 
     stk = loc_to_stack(data, chunk->loc);
     if (!stk || !stk->head || n <= 0 || n > chunk->size || n > stk->size)
         return (INT_MIN);
 
     i = 1;
     if (chunk->loc == TOP_A || chunk->loc == TOP_B)
     {
         current = stk->head;
         while (i < n)
         {
             current = current->next;
             i++;
         }
     }
     else
     {
         current = stk->head->prev;
         while (i < n)
         {
             current = current->prev;
             i++;
         }
     }
     return (current->value);
 }
 
 /**
  * @brief Finds the maximum normalized value within a specified chunk.
  * @param data Pointer to the t_tabs structure.
  * @param chunk Pointer to the t_chunk descriptor.
  * @return The maximum normalized `value` found in the chunk, or INT_MIN if chunk is empty/invalid.
  */
 int chunk_max_value(t_tabs *data, t_chunk *chunk)
 {
     t_stack *stk;
     t_node *current;
     int max_val;
     int current_val;
     int i;
 
     stk = loc_to_stack(data, chunk->loc);
     if (!stk || !stk->head || chunk->size <= 0)
         return (INT_MIN);
 
     max_val = INT_MIN;
     i = 0;
 
     if (chunk->loc == TOP_A || chunk->loc == TOP_B)
     {
         current = stk->head;
         while (i < chunk->size)
         {
             if (i >= stk->size) break;
             current_val = current->value;
             if (current_val > max_val)
                 max_val = current_val;
             if (i < chunk->size - 1)
                 current = current->next;
             i++;
         }
     }
     else
     {
         current = stk->head->prev;
         while (i < chunk->size)
         {
              if (i >= stk->size) break;
             current_val = current->value;
             if (current_val > max_val)
                 max_val = current_val;
             if (i < chunk->size - 1)
                 current = current->prev;
             i++;
         }
     }
     return (max_val);
 }
 
 /**
  * @brief Finds the minimum normalized value within a specified chunk.
  * @param data Pointer to the t_tabs structure.
  * @param chunk Pointer to the t_chunk descriptor.
  * @return The minimum normalized `value` found in the chunk, or INT_MAX if chunk is empty/invalid.
  */
 int chunk_min_value(t_tabs *data, t_chunk *chunk)
 {
     t_stack *stk;
     t_node *current;
     int min_val;
     int current_val;
     int i;
 
     stk = loc_to_stack(data, chunk->loc);
     if (!stk || !stk->head || chunk->size <= 0)
         return (INT_MAX);
 
     min_val = INT_MAX;
     i = 0;
 
     if (chunk->loc == TOP_A || chunk->loc == TOP_B)
     {
         current = stk->head;
         while (i < chunk->size)
         {
              if (i >= stk->size) break;
             current_val = current->value;
             if (current_val < min_val)
                 min_val = current_val;
             if (i < chunk->size - 1)
                 current = current->next;
             i++;
         }
     }
     else
     {
         current = stk->head->prev;
         while (i < chunk->size)
         {
             if (i >= stk->size) break;
             current_val = current->value;
             if (current_val < min_val)
                 min_val = current_val;
              if (i < chunk->size - 1)
                 current = current->prev;
             i++;
         }
     }
     return (min_val);
 }
 
 
 /**
  * @brief Calculates two pivot values to divide a chunk's value range into thirds.
  * @param chunk Pointer to the chunk (unused parameter).
  * @param min_val The minimum normalized value in the chunk.
  * @param max_val The maximum normalized value in the chunk.
  * @param pivot1_val Pointer to store the first pivot value.
  * @param pivot2_val Pointer to store the second pivot value.
  */
 void set_third_pivots_by_value(t_chunk *chunk, int min_val, int max_val, int *pivot1_val, int *pivot2_val)
 {
     int range;
 
     (void)chunk;
     range = max_val - min_val + 1;
 
     if (range < 1)
     {
         *pivot1_val = min_val;
         *pivot2_val = max_val + 1;
     }
     else if (range == 1)
     {
          *pivot1_val = min_val;
          *pivot2_val = min_val + 1;
     }
      else if (range == 2)
      {
          *pivot1_val = min_val + 1;
          *pivot2_val = min_val + 1;
      }
      else
      {
         *pivot1_val = min_val + range / 3;
         *pivot2_val = min_val + (2 * range) / 3;
         if (*pivot2_val > max_val)
             *pivot2_val = max_val + 1;
      }
 }
 
 
 /**
  * @brief Determines the destination locations for the min, mid, and max sub-chunks
  *        based on the location of the chunk being split.
  * @param loc The t_loc of the chunk being split.
  * @param min Pointer to the destination chunk descriptor for minimum values.
  * @param mid Pointer to the destination chunk descriptor for middle values.
  * @param max Pointer to the destination chunk descriptor for maximum values.
  */
 void set_split_loc(t_loc loc, t_chunk *min, t_chunk *mid, t_chunk *max)
 {
     if (loc == TOP_A || loc == BOTTOM_A)
     {
         min->loc = BOTTOM_B;
         mid->loc = TOP_B;
         max->loc = TOP_A;
     }
     else
     {
         if (loc == TOP_B) {
              min->loc = BOTTOM_B;
              mid->loc = BOTTOM_A;
              max->loc = TOP_A;
         } else { // loc == BOTTOM_B
              min->loc = TOP_B;
              mid->loc = BOTTOM_A;
              max->loc = TOP_A;
         }
     }
 }
 
 
 /**
  * @brief Initializes the sizes of the three destination chunks to zero.
  * @param min Pointer to the min destination chunk.
  * @param mid Pointer to the mid destination chunk.
  * @param max Pointer to the max destination chunk.
  */
 void innit_size(t_chunk *min, t_chunk *mid, t_chunk *max)
 {
     min->size = 0;
     mid->size = 0;
     max->size = 0;
 }
 
 /**
  * @brief Moves the element at the 'start' of the 'from' location to the 'to' location
  *        using the necessary push_swap operations.
  * @param data Pointer to the t_tabs structure.
  * @param from The t_loc source location.
  * @param to The t_loc destination location.
  * @return 1 if a move was performed, 0 otherwise.
  */
 int move_from_to(t_tabs *data, t_loc from, t_loc to)
 {
     if (from == TOP_A)
     {
         if (to == TOP_B)         { move_and_print(data, PB); }
         else if (to == BOTTOM_A) { move_and_print(data, RA); }
         else if (to == BOTTOM_B) { move_and_print(data, PB); move_and_print(data, RB); }
         else if (to == TOP_A)    { move_and_print(data, RA); }
         else return (0);
     }
     else if (from == TOP_B)
     {
         if (to == TOP_A)         { move_and_print(data, PA); }
         else if (to == BOTTOM_B) { move_and_print(data, RB); }
         else if (to == BOTTOM_A) { move_and_print(data, PA); move_and_print(data, RA); }
         else if (to == TOP_B)    { move_and_print(data, RB); }
         else return (0);
     }
     else if (from == BOTTOM_A)
     {
         if (to == TOP_B)         { move_and_print(data, RRA); move_and_print(data, PB); }
         else if (to == BOTTOM_B) { move_and_print(data, RRA); move_and_print(data, PB); move_and_print(data, RB); }
         else if (to == BOTTOM_A || to == TOP_A) { move_and_print(data, RRA); }
         else return (0);
     }
      else if (from == BOTTOM_B)
     {
          if (to == TOP_A)         { move_and_print(data, RRB); move_and_print(data, PA); }
          else if (to == BOTTOM_A) { move_and_print(data, RRB); move_and_print(data, PA); move_and_print(data, RA); }
          else if (to == TOP_B || to == BOTTOM_B) { move_and_print(data, RRB); }
          else return (0);
     }
     else
         return (0);
 
     return (1);
 }
 
 
 /* --- Optimization Helpers (Translated) --- */
 
 /**
  * @brief Sorts three integers in ascending order using swaps.
  * @param a Pointer to the first integer.
  * @param b Pointer to the second integer.
  * @param c Pointer to the third integer.
  */
 void sort_three_numbers(int *a, int *b, int *c)
 {
     int temp;
 
     if (*a > *b) { temp = *a; *a = *b; *b = temp; }
     if (*a > *c) { temp = *a; *a = *c; *c = temp; }
     if (*b > *c) { temp = *b; *b = *c; *c = temp; }
 }
 
 /**
  * @brief Checks if four integer values are consecutive (e.g., 5, 6, 7, 8).
  * @param a First integer.
  * @param b Second integer.
  * @param c Third integer.
  * @param d Fourth integer.
  * @return t_bool TRUE if the numbers form a consecutive sequence, FALSE otherwise.
  */
 t_bool is_consecutive(int a, int b, int c, int d)
 {
     sort_three_numbers(&a, &b, &c);
     if ((b - a == 1) && (c - b == 1) && (d - c == 1))
         return (TRUE);
     else
         return (FALSE);
 }
 
 /**
  * @brief Checks if stack A is sorted (by normalized value) from a given position downwards.
  * @param data Pointer to the t_tabs structure.
  * @param from_top_count The starting position (1-based index).
  * @return t_bool TRUE if the specified suffix of stack A is sorted, FALSE otherwise.
  */
 t_bool a_partly_sort(t_tabs *data, int from_top_count)
 {
     t_stack *a;
     t_node *current;
     t_node *runner;
     int expected_value;
     int i;
 
     a = &data->a;
     if (!a->head || a->size < 2 || from_top_count > a->size)
         return (TRUE);
 
     if (from_top_count <= 1)
     {
          current = a->head;
          expected_value = current->value;
          runner = current->next;
          while (runner != a->head)
          {
              expected_value++;
              if (runner->value != expected_value) return (FALSE); // Changed return
              runner = runner->next;
          }
          return (TRUE); // Changed return
     }
 
     current = a->head;
     i = 1;
     while (i < from_top_count)
     {
         current = current->next;
         i++;
     }
 
     expected_value = current->value;
     runner = current->next;
     while (runner != a->head)
     {
         expected_value++;
         if (runner->value != expected_value)
             return (FALSE);
         runner = runner->next;
     }
 
     return (TRUE);
 }
 
 /**
  * @brief Optimization applied during chunk splitting. Sorts elements at TOP_A early.
  * @param data Pointer to the t_tabs structure.
  * @param max_chunk_dest Pointer to the chunk descriptor for the 'max' destination.
  */
 void split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest)
 {
     t_stack *a;
     int v0, v1, v2;
     int total_size;
     int n_minus_1, n_minus_2, n_minus_3;
     t_bool top_three_are_max; // Changed type to t_bool
     t_chunk temp_chunk;
 
     a = &data->a;
     if (max_chunk_dest->loc != TOP_A) return;
 
     if (max_chunk_dest->size >= 3 && a->size >= 3)
     {
         v0 = a->head->value;
         v1 = a->head->next->value;
         v2 = a->head->next->next->value;
         total_size = data->a.size + data->b.size;
         n_minus_1 = total_size - 1;
         n_minus_2 = total_size - 2;
         n_minus_3 = total_size - 3;
         // Check if v0, v1, v2 are precisely n-1, n-2, n-3 in some order
         // Result of comparisons is 0 or 1, directly assignable to t_bool
         top_three_are_max = (
             (v0 == n_minus_1 || v0 == n_minus_2 || v0 == n_minus_3) &&
             (v1 == n_minus_1 || v1 == n_minus_2 || v1 == n_minus_3) &&
             (v2 == n_minus_1 || v2 == n_minus_2 || v2 == n_minus_3) &&
             (v0 != v1 && v0 != v2 && v1 != v2)
         );
         if (top_three_are_max == TRUE && a_partly_sort(data, 4) == TRUE)
         {
             temp_chunk.loc = TOP_A;
             temp_chunk.size = 3;
             sort_three(data, &temp_chunk);
             max_chunk_dest->size -= 3;
             return;
         }
     }
 
     if (max_chunk_dest->size >= 1 && a->size >= 3)
     {
         if (a->head->next->value == a->head->next->next->value - 1 &&
             a->head->value > a->head->next->value &&
             a_partly_sort(data, 3) == TRUE)
         {
             move_and_print(data, SA);
             max_chunk_dest->size--;
             return;
         }
     }
 
     if (max_chunk_dest->size >= 1 && a->size >= 1)
     {
         if (a_partly_sort(data, 2) == TRUE)
         {
             if (a->size == 1 || (a->size > 1 && a->head->value == a->head->next->value - 1))
             {
                 max_chunk_dest->size--;
                 return;
             }
         }
     }
 }
 
 /* --- Main Sorting Logic (Using Chunk Sort) --- */
 
 /**
  * @brief Sorts a chunk of size 1. Ensures the element is at TOP_A.
  * @param data Pointer to the t_tabs structure.
  * @param to_sort Pointer to the chunk descriptor (size must be 1).
  */
 void sort_one(t_tabs *data, t_chunk *to_sort)
 {
     if (to_sort->loc != TOP_A)
     {
         move_from_to(data, to_sort->loc, TOP_A);
     }
 }
 
 /**
  * @brief Sorts a chunk of size 2. Ensures elements are at TOP_A and in order.
  * @param data Pointer to the t_tabs structure.
  * @param to_sort Pointer to the chunk descriptor (size must be 2).
  */
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
 
 /**
  * @brief Sorts a chunk of size 3. Ensures elements are at TOP_A and sorted.
  * @param data Pointer to the t_tabs structure.
  * @param to_sort Pointer to the chunk descriptor (size must be 3).
  */
 void sort_three(t_tabs *data, t_chunk *to_sort)
 {
     int v0;
     int v1;
     int v2;
 
     if (to_sort->loc != TOP_A)
     {
         move_from_to(data, to_sort->loc, TOP_A);
         move_from_to(data, to_sort->loc, TOP_A);
         move_from_to(data, to_sort->loc, TOP_A);
     }
 
     if (data->a.size < 3) return;
 
     v0 = data->a.head->value;
     v1 = data->a.head->next->value;
     if (data->a.size == 3)
         v2 = data->a.head->prev->value;
     else
         v2 = data->a.head->next->next->value;
 
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
 
 
 /**
  * @brief Splits a chunk into three sub-chunks (min, mid, max) based on pivots.
  * @param data Pointer to the t_tabs structure.
  * @param to_split Pointer to the chunk descriptor to be split.
  * @param dest Pointer to the t_split_dest structure to store the resulting sub-chunks.
  */
 void chunk_split(t_tabs *data, t_chunk *to_split, t_split_dest *dest)
 {
     int pivot1_val;
     int pivot2_val;
     int min_val;
     int max_val;
     int initial_chunk_size;
     int i;
     int next_value;
 
     min_val = chunk_min_value(data, to_split);
     max_val = chunk_max_value(data, to_split);
     initial_chunk_size = to_split->size;
 
     if (min_val > max_val || initial_chunk_size <= 0) return;
 
     set_third_pivots_by_value(to_split, min_val, max_val, &pivot1_val, &pivot2_val);
 
     i = 0;
     while (i < initial_chunk_size)
     {
         next_value = chunk_value(data, to_split, 1);
         if (next_value == INT_MIN) {
              break;
         }
 
         if (next_value >= pivot2_val)
         {
             dest->max.size += move_from_to(data, to_split->loc, dest->max.loc);
             split_max_reduction(data, &dest->max);
         }
         else if (next_value >= pivot1_val)
         {
             dest->mid.size += move_from_to(data, to_split->loc, dest->mid.loc);
         }
         else
         {
             dest->min.size += move_from_to(data, to_split->loc, dest->min.loc);
         }
         i++;
     }
 }
 
 /**
  * @brief Recursive function to sort a given chunk.
  * @param data Pointer to the t_tabs structure.
  * @param to_sort Pointer to the chunk descriptor to be sorted.
  */
 void rec_chunk_sort(t_tabs *data, t_chunk *to_sort)
 {
     t_split_dest dest;
 
     if (to_sort->size <= 3)
     {
         if (to_sort->size == 3)      sort_three(data, to_sort);
         else if (to_sort->size == 2) sort_two(data, to_sort);
         else if (to_sort->size == 1) sort_one(data, to_sort);
         return;
     }
 
     innit_size(&dest.min, &dest.mid, &dest.max);
     set_split_loc(to_sort->loc, &dest.min, &dest.mid, &dest.max);
     chunk_split(data, to_sort, &dest);
 
     rec_chunk_sort(data, &dest.max);
     rec_chunk_sort(data, &dest.mid);
     rec_chunk_sort(data, &dest.min);
 }
 
 /**
  * @brief Main entry point for the chunk sort algorithm. Rotates stack A at the end.
  * @param data Pointer to the t_tabs structure.
  */
 void chunk_sort(t_tabs *data)
 {
     t_chunk chunk_all;
     t_node *current;
     int min_pos;
     int current_pos;
     int i;
 
     chunk_all.loc = TOP_A;
     chunk_all.size = data->a.size;
 
     rec_chunk_sort(data, &chunk_all);
 
     if (data->a.size > 0 && data->a.head->value != 0)
     {
         current = data->a.head;
         min_pos = -1;
         current_pos = 0;
 
         while (current_pos < data->a.size)
         {
             if (current->value == 0)
             {
                 min_pos = current_pos;
                 break;
             }
             current = current->next;
             current_pos++;
             if (current == data->a.head && current_pos > 0)
                 break;
         }
 
         if (min_pos > 0)
         {
             if (min_pos <= data->a.size / 2)
             {
                 i = 0;
                 while (i < min_pos)
                 {
                     move_and_print(data, RA);
                     i++;
                 }
             }
             else
             {
                 i = 0;
                 while (i < data->a.size - min_pos)
                 {
                     move_and_print(data, RRA);
                     i++;
                 }
             }
         }
     }
 }
 
 /**
  * @brief Orchestrates the sorting strategy.
  * @param data Pointer to the t_tabs structure.
  */
 void sort_strategy(t_tabs *data)
 {
     if (data->a.size <= 1 || is_raw_input_sorted(&data->a) == TRUE)
         return;
 
     normalize_stack(&data->a);
 
     if (data->a.size > 1)
         chunk_sort(data);
 }
 
 /* --- Main --- */
 /**
  * @brief Main function. Parses arguments, initializes, sorts, prints count, cleans up.
  * @param argc Argument count.
  * @param argv Argument vector.
  * @return 0 on success, 1 on error.
  */
 int main(int argc, char **argv)
 {
     t_tabs *tabs;
 
     if (argc < 2)
         return (0);
 
     tabs = create_tabs(argv + 1, argc - 1);
     if (!tabs)
     {
         return (1);
     }
 
     g_move_count = 0;
 
     sort_strategy(tabs);
 
     fprintf(stderr, "Total moves: %d\n", g_move_count);
 
     free_tabs(tabs);
 
     return (0);
 }