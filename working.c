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
     tail->next = NULL; // Break the circle
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
     // Copy original values to array
     for (int i = 0; i < a->size; i++)
     {
         arr[i] = c->orig;
         c = c->next;
     }
 
     // Sort the array (using bubble sort for simplicity here)
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
 
     // Assign normalized values (indices) back to nodes
     c = a->head;
     for (int i = 0; i < a->size; i++)
     {
         for (int j = 0; j < a->size; j++)
         {
             if (c->orig == arr[j])
             {
                 c->value = j; // Assign index j as the normalized value
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
         // Check if current normalized value is greater than the next
         if (c->value > c->next->value)
             return FALSE;
         c = c->next;
     }
     // Final check: the last element must be smaller than the first in a sorted list
     // This check is implicitly handled by the loop comparing N-1 pairs.
     // The check 'c->value > c->next->value' will catch the wrap-around if the list is not sorted.
     // E.g., for 0 1 2, it checks 0>1 (F), 1>2 (F). For 1 2 0, it checks 1>2 (F), 2>0 (T) -> FALSE.
     return TRUE;
 }
 t_bool is_raw_input_sorted(t_stack *stack)
 {
     if (!stack || stack->size <= 1 || !stack->head)
         return TRUE;
     t_node *c = stack->head;
     for (int i = 0; i < stack->size - 1; i++)
     {
         // Check if current original value is greater than the next
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
     // Swap values and orig fields of the first two nodes
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
     // Swap values and orig fields of the first two nodes
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
     // Node to move is the head of B
     t_node *m = tabs->b.head;
 
     // Remove m from B's circular list
     if (tabs->b.size == 1)
     {
         tabs->b.head = NULL; // B becomes empty
     }
     else
     {
         tabs->b.head = m->next;         // New head of B is the next node
         m->prev->next = m->next;         // Tail of B points to new head
         m->next->prev = m->prev;         // New head's prev points to tail
     }
     tabs->b.size--;
 
     // Add m to the front of A's circular list
     if (tabs->a.size == 0)
     {
         tabs->a.head = m; // m becomes the only node in A
         m->next = m;
         m->prev = m;
     }
     else
     {
         m->next = tabs->a.head;         // m points to old head of A
         m->prev = tabs->a.head->prev;   // m's prev points to tail of A
         tabs->a.head->prev->next = m;    // Tail of A points to m
         tabs->a.head->prev = m;          // Old head's prev points to m
         tabs->a.head = m;                // m is the new head of A
     }
     tabs->a.size++;
 }
 void pb(t_tabs *tabs)
 {
     if (tabs->a.size == 0)
         return;
     // Node to move is the head of A
     t_node *m = tabs->a.head;
 
     // Remove m from A's circular list
     if (tabs->a.size == 1)
     {
         tabs->a.head = NULL; // A becomes empty
     }
     else
     {
         tabs->a.head = m->next;         // New head of A is the next node
         m->prev->next = m->next;         // Tail of A points to new head
         m->next->prev = m->prev;         // New head's prev points to tail
     }
     tabs->a.size--;
 
     // Add m to the front of B's circular list
     if (tabs->b.size == 0)
     {
         tabs->b.head = m; // m becomes the only node in B
         m->next = m;
         m->prev = m;
     }
     else
     {
         m->next = tabs->b.head;         // m points to old head of B
         m->prev = tabs->b.head->prev;   // m's prev points to tail of B
         tabs->b.head->prev->next = m;    // Tail of B points to m
         tabs->b.head->prev = m;          // Old head's prev points to m
         tabs->b.head = m;                // m is the new head of B
     }
     tabs->b.size++;
 }
 void ra(t_tabs *tabs)
 {
     // Rotate A: head becomes the next node
     if (tabs->a.size >= 2)
         tabs->a.head = tabs->a.head->next;
 }
 void rb(t_tabs *tabs)
 {
     // Rotate B: head becomes the next node
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
     // Reverse rotate A: head becomes the previous node (the tail)
     if (tabs->a.size >= 2)
         tabs->a.head = tabs->a.head->prev;
 }
 void rrb(t_tabs *tabs)
 {
     // Reverse rotate B: head becomes the previous node (the tail)
     if (tabs->b.size >= 2)
         tabs->b.head = tabs->b.head->prev;
 }
 void rrr(t_tabs *tabs)
 {
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
     // Print the move name
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
 
     // Execute the move
     move_stack(tabs, move);
     g_move_count++; // Increment global move counter
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
     else // TOP_B or BOTTOM_B
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
     t_stack *stk = loc_to_stack(data, chunk->loc);
     // Basic validation
     if (!stk || !stk->head || n <= 0 || n > chunk->size || n > stk->size)
         return INT_MIN; // Error condition
 
     t_node *current;
     if (chunk->loc == TOP_A || chunk->loc == TOP_B)
     {
         // Access from the head (top) downwards
         current = stk->head;
         for (int i = 1; i < n; i++)
             current = current->next;
     }
     else
     { // BOTTOM_A or BOTTOM_B
         // Access from the tail (bottom) upwards
         current = stk->head->prev; // Start at the last element
         for (int i = 1; i < n; i++)
             current = current->prev;
     }
     return current->value;
 }
 
 /**
  * @brief Finds the maximum normalized value within a specified chunk.
  * @param data Pointer to the t_tabs structure.
  * @param chunk Pointer to the t_chunk descriptor.
  * @return The maximum normalized `value` found in the chunk, or INT_MIN if chunk is empty/invalid.
  */
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
             if (i >= stk->size) break; // Boundary check
             current_val = current->value;
             if (current_val > max_val)
                 max_val = current_val;
             if (i < chunk->size - 1) // Avoid accessing next on last iteration if not needed
                 current = current->next;
         }
     }
     else // BOTTOM_A or BOTTOM_B
     {
         current = stk->head->prev; // Start from tail
         for (int i = 0; i < chunk->size; i++)
         {
              if (i >= stk->size) break; // Boundary check
             current_val = current->value;
             if (current_val > max_val)
                 max_val = current_val;
             if (i < chunk->size - 1)
                 current = current->prev;
         }
     }
     return max_val;
 }
 
 /**
  * @brief Finds the minimum normalized value within a specified chunk.
  * @param data Pointer to the t_tabs structure.
  * @param chunk Pointer to the t_chunk descriptor.
  * @return The minimum normalized `value` found in the chunk, or INT_MAX if chunk is empty/invalid.
  */
 int chunk_min_value(t_tabs *data, t_chunk *chunk)
 {
     t_stack *stk = loc_to_stack(data, chunk->loc);
     if (!stk || !stk->head || chunk->size <= 0)
         return INT_MAX; // Return max for min search failure
 
     t_node *current;
     int min_val = INT_MAX;
     int current_val;
 
     if (chunk->loc == TOP_A || chunk->loc == TOP_B)
     {
         current = stk->head;
         for (int i = 0; i < chunk->size; i++)
         {
              if (i >= stk->size) break; // Boundary check
             current_val = current->value;
             if (current_val < min_val)
                 min_val = current_val;
             if (i < chunk->size - 1)
                 current = current->next;
         }
     }
     else // BOTTOM_A or BOTTOM_B
     {
         current = stk->head->prev; // Start from tail
         for (int i = 0; i < chunk->size; i++)
         {
             if (i >= stk->size) break; // Boundary check
             current_val = current->value;
             if (current_val < min_val)
                 min_val = current_val;
              if (i < chunk->size - 1)
                 current = current->prev;
         }
     }
     return min_val;
 }
 
 
 /**
  * @brief Calculates two pivot values to divide a chunk's value range into thirds.
  * @param chunk Pointer to the chunk (used for potential future context, currently unused).
  * @param min_val The minimum normalized value in the chunk.
  * @param max_val The maximum normalized value in the chunk.
  * @param pivot1_val Pointer to store the first pivot value (separates min/mid).
  * @param pivot2_val Pointer to store the second pivot value (separates mid/max).
  * @note Pivots divide the range [min_val, max_val].
  *       Elements < pivot1_val -> min chunk
  *       Elements >= pivot1_val AND < pivot2_val -> mid chunk
  *       Elements >= pivot2_val -> max chunk
  */
 void set_third_pivots_by_value(t_chunk *chunk, int min_val, int max_val, int *pivot1_val, int *pivot2_val)
 {
     (void)chunk; // Mark chunk as unused for now to avoid compiler warnings
     int range = max_val - min_val + 1; // Total number of distinct values in the range
 
     // Handle small ranges carefully to ensure distinct pivots if possible
     if (range < 1) { // Empty or single element range
         *pivot1_val = min_val;
         *pivot2_val = max_val + 1; // Ensure everything falls below pivot2
         return;
     }
     if (range == 1) {
          *pivot1_val = min_val;
          *pivot2_val = min_val + 1;
          return;
     }
      if (range == 2) {
          *pivot1_val = min_val + 1; // Pivot 1 is the second element
          *pivot2_val = min_val + 1; // Pivot 2 is also the second element
          return;
     }
 
     // General case: Divide the range into roughly three parts
     *pivot1_val = min_val + range / 3;
     *pivot2_val = min_val + (2 * range) / 3;
 
      // Ensure pivot2 is strictly greater than pivot1 if possible,
      // which can happen with integer division e.g. range 4 -> p1=min+1, p2=min+2
      // range 5 -> p1=min+1, p2=min+3. Range 3 -> p1=min+1, p2=min+2. OK.
     // if (*pivot2_val <= *pivot1_val) {
     //     *pivot2_val = *pivot1_val + 1; // Ensure separation
     // }
 
     // Ensure pivots don't exceed the max value if range is small
     // *pivot1_val = (*pivot1_val > max_val) ? max_val : *pivot1_val;
     // *pivot2_val = (*pivot2_val > max_val) ? max_val + 1 : *pivot2_val; // +1 so max_val goes to mid/min if needed
      if (*pivot2_val > max_val)
         *pivot2_val = max_val + 1; // Make sure max_val doesn't go to max chunk unless intended
 
 
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
     // This logic defines how elements are partitioned between A and B
     if (loc == TOP_A || loc == BOTTOM_A) // Splitting a chunk currently in A
     {
         min->loc = BOTTOM_B; // Smallest elements go to the bottom of B (pb, rb)
         mid->loc = TOP_B;    // Middle elements go to the top of B (pb)
         max->loc = TOP_A;    // Largest elements stay at the top of A (ra)
     }
     else // Splitting a chunk currently in B (TOP_B or BOTTOM_B)
     {
         min->loc = BOTTOM_B; // Smallest elements stay at the bottom of B (rrb?) -> Should go to A maybe? Changed to TOP_B based on original code.
                              // Original code logic seemed complex here. Let's try a simpler push-to-A approach for B->A split.
                              // Let's stick to the translated logic for now:
                              // min->loc = BOTTOM_B; // This seems wrong if goal is A. Let's re-evaluate based on typical Quicksort A<->B
                              // Standard approach: When sorting B back to A:
                              // Max goes TOP_A (pa)
                              // Mid goes BOTTOM_A (pa, ra)
                              // Min stays TOP_B (rb) - to be processed later
         if (loc == TOP_B) {
              min->loc = BOTTOM_B; // Smallest pushed to B stay at bottom (rb)
              mid->loc = BOTTOM_A; // Mids pushed to A go to bottom (pa, ra)
              max->loc = TOP_A;    // Largest pushed to A go to top (pa)
         } else { // loc == BOTTOM_B
              min->loc = TOP_B;    // Smallest stay B, rotate to top (rrb)
              mid->loc = BOTTOM_A; // Mids pushed to A go to bottom (rrb, pa, ra)
              max->loc = TOP_A;    // Largest pushed to A go to top (rrb, pa)
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
  * @return 1 if a move was performed, 0 otherwise (e.g., invalid locations).
  * @note Handles rotations within the same stack if `from` and `to` imply it
  *       (e.g., moving from TOP_A to TOP_A requires an RA).
  */
 int move_from_to(t_tabs *data, t_loc from, t_loc to)
 {
     if (from == TOP_A)
     {
         if (to == TOP_B)         { move_and_print(data, PB); } // A -> B (Top)
         else if (to == BOTTOM_A) { move_and_print(data, RA); } // A -> A (Rotate)
         else if (to == BOTTOM_B) { move_and_print(data, PB); move_and_print(data, RB); } // A -> B (Bottom)
         else if (to == TOP_A)    { move_and_print(data, RA); } // A -> A (Stay Top -> Rotate)
         else return 0; // Invalid destination
     }
     else if (from == TOP_B)
     {
         if (to == TOP_A)         { move_and_print(data, PA); } // B -> A (Top)
         else if (to == BOTTOM_B) { move_and_print(data, RB); } // B -> B (Rotate)
         else if (to == BOTTOM_A) { move_and_print(data, PA); move_and_print(data, RA); } // B -> A (Bottom)
         else if (to == TOP_B)    { move_and_print(data, RB); } // B -> B (Stay Top -> Rotate)
         else return 0; // Invalid destination
     }
     else if (from == BOTTOM_A) // Accessing via RRA
     {
         if (to == TOP_A)         { move_and_print(data, RRA); } // A (Bottom) -> A (Stay Bottom -> Rotate Rev) -> Now Top after RRA? Needs PA/PB?
                                  // This needs careful thought. If partitioning bottom A, RRA brings element to top.
                                  // If destination is TOP_A, element is now there.
                                  // If destination is TOP_B, needs PB after RRA.
                                  // If destination is BOTTOM_B, needs PB, RB after RRA.
                                  // If destination is BOTTOM_A, needs RA after RRA? No, RRA moves it away. Logic is tricky.
                                  // Let's assume move_from_to applies to the *element currently accessible* at 'from'.
                                  // So for BOTTOM_A, the accessible element is stack->head->prev. RRA brings it to top.
         { move_and_print(data, RRA); } // If dest is TOP_A, RRA suffices. If dest is somewhere else, subsequent moves needed?
         if (to == TOP_B)         { move_and_print(data, RRA); move_and_print(data, PB); }
         else if (to == BOTTOM_B) { move_and_print(data, RRA); move_and_print(data, PB); move_and_print(data, RB); }
         else if (to == BOTTOM_A) { move_and_print(data, RRA); } // Stay Bottom A -> Reverse Rotate
         else if (to == TOP_A)    { /* RRA already done if needed */ } // Addressed above implicitly? If already TOP_A dest after RRA, ok.
         else return 0;
     }
      else if (from == BOTTOM_B) // Accessing via RRB
     {
         // Similar logic to BOTTOM_A: RRB brings element to top of B first.
          if (to == TOP_A)         { move_and_print(data, RRB); move_and_print(data, PA); }
          else if (to == BOTTOM_A) { move_and_print(data, RRB); move_and_print(data, PA); move_and_print(data, RA); }
          else if (to == TOP_B)    { move_and_print(data, RRB); } // If dest is TOP_B, RRB suffices.
          else if (to == BOTTOM_B) { move_and_print(data, RRB); } // Stay Bottom B -> Reverse Rotate
          else return 0;
     }
     else
         return 0; // Invalid source location
 
     return 1; // A move was made
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
  * @return true if the numbers form a consecutive sequence after sorting the first three, false otherwise.
  */
 bool is_consecutive(int a, int b, int c, int d)
 {
     // Sort the first three to handle any initial order
     sort_three_numbers(&a, &b, &c);
     // Check if a, b, c are consecutive and d follows c
     return ((b - a == 1) && (c - b == 1) && (d - c == 1));
 }
 
 /**
  * @brief Checks if stack A is sorted (by normalized value) from a given position downwards.
  * @param data Pointer to the t_tabs structure.
  * @param from_top_count The starting position (1-based index). 1 checks the whole stack,
  *                       2 checks from the second element, etc.
  * @return true if the specified suffix of stack A is sorted relative to itself, false otherwise.
  */
 bool a_partly_sort(t_tabs *data, int from_top_count)
 {
     t_stack *a = &data->a;
     // Trivial cases: empty, single element, or check starts beyond stack size
     if (!a->head || a->size < 2 || from_top_count > a->size)
         return TRUE;
     // If checking the whole stack (from_top_count == 1), use the main is_sorted function
     if (from_top_count <= 1 && a->size > 1) {
          // Check if a->head->value == a->head->next->value - 1 AND the rest is sorted
          t_node *start_node = a->head;
          int expected = start_node->value;
          t_node *runner = start_node->next;
          while(runner != a->head) {
              expected++;
              if (runner->value != expected) return false;
              runner = runner->next;
          }
          return true; // If we reach here, it's fully sorted
     }
 
 
     // Find the starting node for the partial check
     t_node *current = a->head;
     for (int i = 1; i < from_top_count; i++)
     {
         current = current->next;
     }
 
     // Check consecutive sorting from 'current' node until the node before head
     int expected_value = current->value;
     t_node *runner = current->next;
     while (runner != a->head) // Iterate through the rest of the stack
     {
         expected_value++;
         if (runner->value != expected_value)
             return FALSE; // Found an out-of-order element
         runner = runner->next;
     }
 
     // If the loop completes without returning false, the partial stack is sorted
     return TRUE;
 }
 
 /**
  * @brief Optimization applied during chunk splitting. If elements moved to the 'max'
  *        destination (often TOP_A) form a sortable sequence at the top of A,
  *        sort them immediately to reduce recursion depth / future moves.
  * @param data Pointer to the t_tabs structure.
  * @param max_chunk_dest Pointer to the chunk descriptor for the 'max' destination.
  *                       Its size may be decremented if elements are sorted here.
  */
 void split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest)
 {
     t_stack *a = &data->a;
 
     // Optimization only applies if the max destination is TOP_A
     if (max_chunk_dest->loc != TOP_A) return;
 
     // Condition 1: Top 3 in A are the largest overall (N-1, N-2, N-3) and rest of A is sorted.
     // Action: Sort these top 3 immediately.
     if (max_chunk_dest->size >= 3 && a->size >= 3)
     {
         int v0 = a->head->value;
         int v1 = a->head->next->value;
         int v2 = a->head->next->next->value;
         int total_size = data->a.size + data->b.size; // Overall N
         int n_minus_1 = total_size - 1;
         int n_minus_2 = total_size - 2;
         int n_minus_3 = total_size - 3;
 
         // Check if v0, v1, v2 are precisely n-1, n-2, n-3 in some order
         bool top_three_are_max = (
             (v0 == n_minus_1 || v0 == n_minus_2 || v0 == n_minus_3) &&
             (v1 == n_minus_1 || v1 == n_minus_2 || v1 == n_minus_3) &&
             (v2 == n_minus_1 || v2 == n_minus_2 || v2 == n_minus_3) &&
             (v0 != v1 && v0 != v2 && v1 != v2) // Ensure they are distinct
         );
 
 
         if (top_three_are_max && a_partly_sort(data, 4)) // Check rest is sorted from 4th element
         {
             // fprintf(stderr, "DEBUG: split_max_reduction Case 1 triggered\n");
             t_chunk temp_chunk = {TOP_A, 3}; // Define a chunk for sort_three
             sort_three(data, &temp_chunk);   // Sort the top 3 elements
             max_chunk_dest->size -= 3;      // Reduce the size of the max chunk accordingly
             return;                         // Exit optimization check
         }
     }
 
     // Condition 2: Swapping the top two (SA) would place the *new* top element correctly
     //              relative to the third element, and the rest below is sorted.
     // Action: Perform SA.
     if (max_chunk_dest->size >= 1 && a->size >= 3) // Need at least 3 in A for this check
     {
         // Check if a[1] (head->next) is one less than a[2] (head->next->next)
         // AND a[0] (head) is currently greater than a[1] (meaning SA would swap them)
         // AND the stack from a[2] downwards is sorted.
         if (a->head->next->value == a->head->next->next->value - 1 &&
             a->head->value > a->head->next->value &&
             a_partly_sort(data, 3)) // Check rest is sorted from 3rd element
         {
             // fprintf(stderr, "DEBUG: split_max_reduction Case 2 triggered\n");
             move_and_print(data, SA);
             // The element that was originally at head->next is now sorted at the top.
             max_chunk_dest->size--; // Reduce max chunk size by 1
             return; // Exit optimization check
         }
     }
 
     // Condition 3: The current top element of A is already in its correct final sorted
     //              position relative to the elements below it (which must also be sorted).
     // Action: Consider this element sorted and reduce max chunk size.
     if (max_chunk_dest->size >= 1 && a->size >= 1)
     {
         // Check if the stack from the second element downwards is sorted
         if (a_partly_sort(data, 2))
         {
             // Additionally check if the first element fits sequentially before the second
             if (a->size == 1 || (a->size > 1 && a->head->value == a->head->next->value - 1))
             {
                 // The top element is perfectly sorted relative to the rest.
                 // fprintf(stderr, "DEBUG: split_max_reduction Case 3 triggered\n");
                 max_chunk_dest->size--; // Reduce max chunk size by 1
                 return; // Exit optimization check
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
     // If the element is not already at TOP_A, move it there.
     if (to_sort->loc != TOP_A)
     {
         move_from_to(data, to_sort->loc, TOP_A);
     }
     // A chunk of size 1 is inherently sorted once in place.
 }
 
 /**
  * @brief Sorts a chunk of size 2. Ensures elements are at TOP_A and in order.
  * @param data Pointer to the t_tabs structure.
  * @param to_sort Pointer to the chunk descriptor (size must be 2).
  */
 void sort_two(t_tabs *data, t_chunk *to_sort)
 {
     // Move both elements to TOP_A if they aren't already.
     // The order they arrive might be arbitrary depending on 'from' location.
     if (to_sort->loc != TOP_A)
     {
         // Move the two elements conceptually belonging to this chunk to TOP_A
         // The actual implementation relies on move_from_to handling the element
         // currently accessible at to_sort->loc. Need to call it twice.
         move_from_to(data, to_sort->loc, TOP_A);
         move_from_to(data, to_sort->loc, TOP_A);
     }
 
     // Now that both elements are guaranteed to be at the top of A, check order.
     if (data->a.size >= 2 && data->a.head->value > data->a.head->next->value)
     {
         move_and_print(data, SA); // Swap if out of order
     }
 }
 
 /**
  * @brief Sorts a chunk of size 3. Ensures elements are at TOP_A and sorted.
  * @param data Pointer to the t_tabs structure.
  * @param to_sort Pointer to the chunk descriptor (size must be 3).
  * @note Uses specific move sequences for the 5 possible unsorted permutations of 3 elements.
  */
 void sort_three(t_tabs *data, t_chunk *to_sort)
 {
     // Move the three elements to TOP_A if they aren't already.
     if (to_sort->loc != TOP_A)
     {
         move_from_to(data, to_sort->loc, TOP_A);
         move_from_to(data, to_sort->loc, TOP_A);
         move_from_to(data, to_sort->loc, TOP_A);
     }
 
     // Ensure we actually have 3 elements at the top of A now.
     if (data->a.size < 3) return; // Should not happen if called correctly
 
     // Check if the top 3 are already sorted (relative to each other)
     // Using is_sorted might be too broad if A has > 3 elements. Check specific values.
     int v0 = data->a.head->value;
     int v1 = data->a.head->next->value;
     int v2 = data->a.head->next->next->value; // Access 3rd element
 
     // Target order: v0 < v1 < v2 (assuming these are the 3 elements of the chunk)
 
     // Check permutations (relative order based on normalized values):
     // Case 1: 1 0 2 (v0 > v1, v1 < v2, v0 < v2?) No, v0 > v2 is possible. Let's use min/max.
     // Let's simplify by finding min/max/mid of v0,v1,v2
     int min_v = v0, max_v = v0;
     if (v1 < min_v) min_v = v1; if (v1 > max_v) max_v = v1;
     if (v2 < min_v) min_v = v2; if (v2 > max_v) max_v = v2;
 
     // Sorted: min_v, mid_v, max_v (where mid_v is the remaining value)
     if (v0 == min_v && v1 != max_v) return; // Already sorted: 0 1 2
 
     // Find the permutation and apply optimal moves:
     // Note: Using head->prev for the third element might be more robust
     // if stack A size is exactly 3 due to circularity.
     // Let's re-read v2 using head->prev if size is 3.
     if (data->a.size == 3) v2 = data->a.head->prev->value;
     else v2 = data->a.head->next->next->value;
 
     // Permutations and moves (based on common push_swap sort_3 logic):
     // Values at indices: [0] [1] [2] (Top is [0])
     // Target:           min mid max
     if (v0 > v1 && v1 < v2 && v0 < v2) // 1 0 2 -> SA -> 0 1 2
         move_and_print(data, SA);
     else if (v0 > v1 && v1 > v2) // 2 1 0 -> SA, RRA -> 1 2 0 -> RRA -> 0 1 2
     {
         move_and_print(data, SA);
         move_and_print(data, RRA);
     }
     else if (v0 > v1 && v1 < v2 && v0 > v2) // 2 0 1 -> RA -> 0 1 2
         move_and_print(data, RA);
     else if (v0 < v1 && v1 > v2 && v0 < v2) // 0 2 1 -> SA, RA -> 2 0 1 -> RA -> 0 1 2
     {
         move_and_print(data, SA);
         move_and_print(data, RA);
     }
     else if (v0 < v1 && v1 > v2 && v0 > v2) // 1 2 0 -> RRA -> 0 1 2
         move_and_print(data, RRA);
     // else: 0 1 2 (already sorted) - handled by initial check potentially, or no moves needed.
 }
 
 
 /**
  * @brief Splits a chunk into three sub-chunks (min, mid, max) based on pivots.
  * @param data Pointer to the t_tabs structure.
  * @param to_split Pointer to the chunk descriptor to be split.
  * @param dest Pointer to the t_split_dest structure to store the resulting sub-chunks.
  * @details Calculates pivots, determines destination locations, iterates through
  *          the `to_split` chunk, moves elements to their respective destination
  *          chunks using `move_from_to`, and applies `split_max_reduction` optimization.
  */
 void chunk_split(t_tabs *data, t_chunk *to_split, t_split_dest *dest)
 {
     int pivot1_val, pivot2_val;
     int min_val = chunk_min_value(data, to_split);
     int max_val = chunk_max_value(data, to_split);
     int initial_chunk_size = to_split->size; // Store original size for loop limit
 
     // Check for degenerate cases where min/max couldn't be found
     if (min_val > max_val) return; // Empty or invalid chunk
 
     set_third_pivots_by_value(to_split, min_val, max_val, &pivot1_val, &pivot2_val);
 
     // fprintf(stderr, "DEBUG: Splitting chunk loc=%d, size=%d (minV=%d, maxV=%d, p1V=%d, p2V=%d)\n",
     // to_split->loc, initial_chunk_size, min_val, max_val, pivot1_val, pivot2_val);
 
     // Iterate 'initial_chunk_size' times to process all elements originally in the chunk
     for (int i = 0; i < initial_chunk_size; i++)
     {
         // Get value of the element currently accessible at the start of the 'to_split' location
         // Note: chunk_value needs careful implementation if stack size changes during split.
         // It should ideally get the value from the correct stack based on to_split->loc.
         // Let's assume chunk_value(data, to_split, 1) correctly gets the value of the
         // element about to be moved.
         int next_value = chunk_value(data, to_split, 1);
 
         // Check for error from chunk_value
         if (next_value == INT_MIN) {
              // fprintf(stderr, "Warning: chunk_value returned error during split (i=%d)\n", i);
              break; // Stop splitting if we can't get values reliably
         }
 
         // fprintf(stderr, "  Processing value %d (iter %d/%d)\n", next_value, i, initial_chunk_size-1);
 
         // Decide destination based on value and pivots
         if (next_value >= pivot2_val) // Max chunk
         {
             // fprintf(stderr, "    -> Max chunk (loc %d)\n", dest->max.loc);
             // Move the element and increment the size of the destination chunk
             dest->max.size += move_from_to(data, to_split->loc, dest->max.loc);
             // Apply optimization AFTER the move, specifically if dest is TOP_A
             split_max_reduction(data, &dest->max);
         }
         else if (next_value >= pivot1_val) // Mid chunk
         {
             // fprintf(stderr, "    -> Mid chunk (loc %d)\n", dest->mid.loc);
             dest->mid.size += move_from_to(data, to_split->loc, dest->mid.loc);
         }
         else // Min chunk
         {
             // fprintf(stderr, "    -> Min chunk (loc %d)\n", dest->min.loc);
             dest->min.size += move_from_to(data, to_split->loc, dest->min.loc);
         }
         // The size of the original chunk `to_split` effectively decreases with each move,
         // but the loop runs based on the `initial_chunk_size`.
     }
     // fprintf(stderr, "DEBUG: Split finished. Sizes: min=%d, mid=%d, max=%d\n", dest->min.size, dest->mid.size, dest->max.size);
 }
 
 /**
  * @brief Recursive function to sort a given chunk.
  * @param data Pointer to the t_tabs structure.
  * @param to_sort Pointer to the chunk descriptor to be sorted.
  * @details Handles base cases (size <= 3) by calling specialized sort functions.
  *          For larger chunks, it splits the chunk into min, mid, max sub-chunks
  *          and recursively calls itself on these sub-chunks (typically max, then mid, then min).
  */
 void rec_chunk_sort(t_tabs *data, t_chunk *to_sort)
 {
     t_split_dest dest; // Structure to hold the results of the split
 
     // fprintf(stderr, "REC_CHUNK_SORT: loc=%d, size=%d\n", to_sort->loc, to_sort->size);
 
     // --- BASE CASES ---
     // If the chunk size is small enough, sort it directly.
     if (to_sort->size <= 3)
     {
         // fprintf(stderr, "  Base Case: size=%d\n", to_sort->size);
         if (to_sort->size == 3)      sort_three(data, to_sort);
         else if (to_sort->size == 2) sort_two(data, to_sort);
         else if (to_sort->size == 1) sort_one(data, to_sort);
         // If size is 0, do nothing.
         // fprintf(stderr, "  Base Case Handled.\n");
         return; // Return from recursion
     }
 
     // --- RECURSIVE STEP (size > 3) ---
     // 1. Initialize destination chunk sizes
     innit_size(&dest.min, &dest.mid, &dest.max);
     // 2. Determine where the sub-chunks should go based on current location
     set_split_loc(to_sort->loc, &dest.min, &dest.mid, &dest.max);
     // 3. Split the current chunk into the three destination chunks
     chunk_split(data, to_sort, &dest); // This performs the partitioning and moves
 
     // 4. Recurse on the sub-chunks (order matters for efficiency)
     // Typically sort Max first (often stays/goes to A), then Mid, then Min.
     rec_chunk_sort(data, &dest.max); // Recurse on the largest elements
     rec_chunk_sort(data, &dest.mid); // Recurse on the middle elements
     rec_chunk_sort(data, &dest.min); // Recurse on the smallest elements
 }
 
 /**
  * @brief Main entry point for the chunk sort algorithm.
  * @param data Pointer to the t_tabs structure.
  * @details Initializes the first chunk covering all of stack A and starts the
  *          recursive sorting process. Afterwards, performs a final rotation
  *          on stack A if necessary to bring the element with value 0 to the top.
  */
 void chunk_sort(t_tabs *data)
 {
     // Define the initial chunk covering the entire stack A
     t_chunk chunk_all;
     chunk_all.loc = TOP_A;
     chunk_all.size = data->a.size;
 
     // Start the recursive sorting process
     rec_chunk_sort(data, &chunk_all);
 
     // --- Final Rotation ---
     // After recursion, all elements should be sorted in stack A,
     // but the element with normalized value 0 might not be at the top.
     // Find the position of element 0 and rotate A accordingly.
     if (data->a.size > 0 && data->a.head->value != 0)
     {
         // fprintf(stderr, "DEBUG: Final rotation needed. A head is %d\n", data->a.head->value);
         t_node *current = data->a.head;
         int min_pos = -1; // Position of the node with value 0 (0-indexed)
         int current_pos = 0;
 
         // Find the node with value 0 and its position
         do
         {
             if (current->value == 0)
             {
                 min_pos = current_pos;
                 break;
             }
             current = current->next;
             current_pos++;
         } while (current != data->a.head && current_pos < data->a.size);
 
 
         // If element 0 was found and is not already at the top (pos 0)
         if (min_pos > 0)
         {
             // Decide whether to use RA or RRA (whichever is fewer moves)
             if (min_pos <= data->a.size / 2)
             {
                 // Rotate forward (RA) is shorter
                 for (int i = 0; i < min_pos; i++)
                     move_and_print(data, RA);
             }
             else
             {
                 // Rotate backward (RRA) is shorter
                 for (int i = 0; i < data->a.size - min_pos; i++)
                     move_and_print(data, RRA);
             }
         }
         // fprintf(stderr, "DEBUG: Final rotation done.\n");
     }
 }
 
 /**
  * @brief Orchestrates the sorting strategy.
  * @param data Pointer to the t_tabs structure.
  * @details Checks if the stack is already sorted. If not, normalizes stack A
  *          and calls the main chunk_sort function.
  */
 void sort_strategy(t_tabs *data)
 {
     // If stack A has 0 or 1 elements, or is already sorted based on original values, do nothing.
     if (data->a.size <= 1 || is_raw_input_sorted(&data->a))
         return;
 
     // Normalize the stack (map original values to 0..N-1 indices)
     normalize_stack(&data->a);
 
     // If stack A has more than 1 element (and wasn't already sorted), sort it.
     if (data->a.size > 1)
         chunk_sort(data);
 }
 
 /* --- Main --- */
 /**
  * @brief Main function. Parses command-line arguments, initializes stacks,
  *        runs the sorting algorithm, prints the total move count, and frees memory.
  * @param argc Argument count.
  * @param argv Argument vector (strings representing integers).
  * @return 0 on success, 1 on error (e.g., invalid input, memory allocation failure).
  */
 int main(int argc, char **argv)
 {
     t_tabs *tabs;
 
     // Require at least one number argument
     if (argc < 2)
         return 0; // No numbers to sort, considered success.
 
     // Create stacks and parse input numbers
     // argv+1 skips the program name, argc-1 is the number of integers
     tabs = create_tabs(argv + 1, argc - 1);
     if (!tabs)
     {
         // Error messages are printed within create_tabs/fill_stack
         return 1; // Indicate error
     }
 
     g_move_count = 0; // Reset global move counter
 
     // Execute the sorting strategy
     sort_strategy(tabs);
 
     // Print the total number of moves to standard error
     fprintf(stderr, "Total moves: %d\n", g_move_count);
 
     // Free allocated memory
     free_tabs(tabs);
 
     return 0; // Success
 }