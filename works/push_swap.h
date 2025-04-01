#ifndef PUSH_SWAP_H
#define PUSH_SWAP_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define FT_INT_MAX 2147483647
#define FT_INT_MIN -2147483648

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
	int value;
	int orig;
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

/* Structure for check_reduce_cond1 variables */
typedef struct s_reduce_cond1_vars
{
	int v0;
	int v1;
	int v2;
	int total_size;
	int n_m1;
	int n_m2;
	int n_m3;
	t_bool is_top_3_max;
} t_reduce_cond1_vars;

/* --- Global Move Count --- */
extern int g_move_count;

/* --- Function Prototypes --- */

// ps_list_utils.c
t_node *create_node(int num);
void append_node(t_stack *stack, t_node *node);
void free_stack(t_stack *stack);
void free_tabs(t_tabs *tabs);
void swap_ints(int *a, int *b);

// ps_input_parsing.c
t_bool is_valid_int_str(const char *str);
t_bool has_duplicates(t_stack *stack);
t_bool fill_stack(t_stack *stack, char **str, int len);
t_tabs *create_tabs(char **str, int len);

// ps_sort_utils.c
t_bool is_sorted(t_stack *stack);
t_bool is_raw_input_sorted(t_stack *stack);
void sort_three_numbers(int *a, int *b, int *c);
t_bool is_consecutive(int a, int b, int c, int d);

// ps_normalize.c (Moved from ps_sort_utils.c)
void normalize_stack(t_stack *a);
void copy_orig_to_array(t_stack *a, int *arr);
void sort_array(int *arr, int size);
void assign_normalized_values(t_stack *a, int *arr);

// ps_moves_base.c
void sa(t_tabs *tabs);
void sb(t_tabs *tabs);
void ss(t_tabs *tabs);
void pa(t_tabs *tabs);
void pb(t_tabs *tabs);

// ps_moves_rotate.c
void ra(t_tabs *tabs);
void rb(t_tabs *tabs);
void rr(t_tabs *tabs);
void rra(t_tabs *tabs);
void rrb(t_tabs *tabs);

// ps_moves_exec.c
void rrr(t_tabs *tabs);
void move_stack(t_tabs *tabs, t_stack_move move);
void move_and_print(t_tabs *tabs, t_stack_move move);
int move_from_to(t_tabs *data, t_loc from, t_loc to);

// ps_chunk_helpers1.c
t_stack *loc_to_stack(t_tabs *data, t_loc loc);
int chunk_value(t_tabs *data, t_chunk *chunk, int n);
void innit_size(t_chunk *min, t_chunk *mid, t_chunk *max);

// ps_chunk_minmax.c (Moved from ps_chunk_helpers1.c)
int chunk_min_value(t_tabs *data, t_chunk *chunk);
int chunk_max_value(t_tabs *data, t_chunk *chunk);
int iterate_chunk_and_compare(t_tabs *data, t_chunk *chunk,
							  t_bool (*compare)(int, int *), int initial_val); // Changed signature
t_bool compare_min(int current_val, int *result);
t_bool compare_max(int current_val, int *result);

// ps_chunk_helpers2.c
void set_third_pivots_by_value(t_chunk *chunk, int min_val, int max_val,
							   int *pivot1_val, int *pivot2_val);
void set_split_loc(t_loc loc, t_chunk *min, t_chunk *mid, t_chunk *max);
t_bool a_partly_sort(t_tabs *data, int from_top_count);
void split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest);

// ps_chunk_helpers3.c
t_bool check_partial_sort_loop(t_node *start_node, t_node *head_node);

// ps_reduce_helpers.c (Moved from ps_chunk_helpers3.c)
void init_reduce_cond1_vars(t_tabs *data, t_reduce_cond1_vars *vars);
void calculate_top_3_max(t_reduce_cond1_vars *vars);
t_bool check_reduce_cond1(t_tabs *data);
t_bool check_reduce_cond2(t_tabs *data);
t_bool check_reduce_cond3(t_tabs *data);

// ps_chunk_sort.c
void sort_one(t_tabs *data, t_chunk *to_sort);
void sort_two(t_tabs *data, t_chunk *to_sort);
void sort_three(t_tabs *data, t_chunk *to_sort);
void rec_chunk_sort(t_tabs *data, t_chunk *to_sort);

// ps_chunk_split.c (Moved from ps_chunk_sort.c)
void chunk_split(t_tabs *data, t_chunk *to_split, t_split_dest *dest);
t_bool get_split_params(t_tabs *data, t_chunk *to_split, int *p1, int *p2);
void process_split_loop(t_tabs *data, t_chunk *to_split,
						t_split_dest *dest, int p1, int p2);

// ps_main.c
void chunk_sort(t_tabs *data);
void sort_strategy(t_tabs *data);
// main is not prototyped

// ps_main_utils.c (Helpers for ps_main.c)
t_bool find_min_pos(t_stack *a, int *min_pos_ptr);
void rotate_a_to_top(t_tabs *data, int min_pos);
void ft_putstr(char *str);
int ft_isnumeric(char *c);
long ft_atol(const char *str);

#endif // PUSH_SWAP_H