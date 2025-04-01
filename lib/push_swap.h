

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




#ifndef PUSH_SWAP_H
#define PUSH_SWAP_H


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

extern int g_move_count;


/* --- User's Data Structures --- */
typedef enum e_bool { FALSE = 0, TRUE = 1 } t_bool;
typedef enum e_stack_move { SA, SB, SS, PA, PB, RA, RB, RR, RRA, RRB, RRR } t_stack_move;
typedef struct s_node { int value; int orig; struct s_node *next; struct s_node *prev; } t_node;
typedef struct s_stack { t_node *head; int size; } t_stack;
typedef struct s_tabs { t_stack a; t_stack b; } t_tabs;

/* --- Chunk Sort Specific Structures --- */
typedef enum e_chunk_location { LOC_TOP_A, LOC_BOTTOM_A, LOC_TOP_B, LOC_BOTTOM_B } t_chunk_location;
typedef struct s_chunk { t_chunk_location location; int size; } t_chunk;
typedef struct s_partition_dest { t_chunk low_chunk; t_chunk mid_chunk; t_chunk high_chunk; } t_partition_dest;

/* --- Forward Declarations (User's Functions - Assume Implemented) --- */
void move_and_print(t_tabs *stacks, t_stack_move move);
void free_tabs(t_tabs *stacks);
void normalize_stack(t_stack *a);
t_bool is_stack_sorted(t_stack *stack);
t_bool is_input_sorted(t_stack *stack);
t_tabs *create_tabs(char **str);
void free_stack(t_stack *stack);

/* --- Forward Declarations (Chunk Sort Algorithm & Helpers) --- */
void push_swap(t_tabs *stacks);
void sort_chunk_recursive(t_tabs *stacks, t_chunk *chunk_to_sort);
void sort_chunk_three(t_tabs *stacks, t_chunk *chunk_to_sort);
void sort_chunk_two(t_tabs *stacks, t_chunk *chunk_to_sort);
void sort_chunk_one(t_tabs *stacks, t_chunk *chunk_to_sort);
void partition_chunk(t_tabs *stacks, t_chunk *chunk_to_split, t_partition_dest *destinations);
int get_chunk_element_value(t_tabs *stacks, t_chunk *chunk, int index_from_start);
int get_chunk_max_value(t_tabs *stacks, t_chunk *chunk);
int get_chunk_min_value(t_tabs *stacks, t_chunk *chunk);
t_stack *get_stack_for_location(t_tabs *stacks, t_chunk_location location);
void calculate_three_way_pivots(t_chunk *chunk, int min_val, int max_val, int *pivot1_val, int *pivot2_val);
void determine_partition_destinations(t_chunk_location source_loc, t_chunk *low, t_chunk *mid, t_chunk *high);
void initialize_partition_sizes(t_chunk *low, t_chunk *mid, t_chunk *high);
int move_element(t_tabs *stacks, t_chunk_location from, t_chunk_location to);
// Optimization Helpers
void sort_int_triple(int *a, int *b, int *c);
bool are_values_consecutive(int a, int b, int c, int d);
bool is_stack_a_sorted_from_node(t_tabs *stacks, int start_index);
void optimize_high_chunk_placement(t_tabs *stacks, t_chunk *high_chunk_dest);
void rotate_stack_a_to_value_zero(t_tabs *stacks);



char	**ft_split(char *str, char sep);
t_bool  fill_stack(t_stack *stack, char **str, int len);



#endif // PUSH_SWAP_H
