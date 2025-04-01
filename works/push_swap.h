#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h> // For NULL, size_t, malloc, free, exit
# include <limits.h> // For INT_MIN, INT_MAX
# include <stdio.h>  // For fprintf, printf (used widely)
# include <errno.h>  // For errno

# define FT_INT_MAX 2147483647
# define FT_INT_MIN -2147483648
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
	int				value; // Normalized index (0 to size-1)
	int				orig;  // Original value
	struct s_node	*next;
	struct s_node	*prev;
}					t_node;

typedef struct s_stack
{
	t_node	*head;
	int		size;
}			t_stack;

typedef struct s_tabs
{
	t_stack	a;
	t_stack	b;
}			t_tabs;

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
	t_loc	loc;
	int		size;
}			t_chunk;
typedef struct s_split_dest
{
	t_chunk	min;
	t_chunk	mid;
	t_chunk	max;
}			t_split_dest;

/* --- Global Move Count --- */
// Declared as extern so it's accessible across files,
// will be defined in one .c file (e.g., ps_main.c)
extern int	g_move_count;

/* --- Function Prototypes --- */

// ps_utils.c
void	ft_putstr(char *str);
int		ft_isnumeric(char *c);
int		ft_strlen(const char *str);
long	ft_atol(const char *str);
// ps_list_utils.c
t_node	*create_node(int num);
void	append_node(t_stack *stack, t_node *node);
void	free_stack(t_stack *stack);
void	free_tabs(t_tabs *tabs);
void	swap_ints(int *a, int *b);

// ps_input_parsing.c
t_bool	is_valid_int_str(const char *str);
t_bool	has_duplicates(t_stack *stack);
t_bool	fill_stack(t_stack *stack, char **str, int len);
t_tabs	*create_tabs(char **str, int len);

// ps_sort_utils.c
void	normalize_stack(t_stack *a);
t_bool	is_sorted(t_stack *stack);
t_bool	is_raw_input_sorted(t_stack *stack);
void	sort_three_numbers(int *a, int *b, int *c);
t_bool	is_consecutive(int a, int b, int c, int d);

// ps_moves_base.c
void	sa(t_tabs *tabs);
void	sb(t_tabs *tabs);
void	ss(t_tabs *tabs);
void	pa(t_tabs *tabs);
void	pb(t_tabs *tabs);

// ps_moves_rotate.c
void	ra(t_tabs *tabs);
void	rb(t_tabs *tabs);
void	rr(t_tabs *tabs);
void	rra(t_tabs *tabs);
void	rrb(t_tabs *tabs);

// ps_moves_exec.c
void	rrr(t_tabs *tabs);
void	move_stack(t_tabs *tabs, t_stack_move move);
void	move_and_print(t_tabs *tabs, t_stack_move move);
int		move_from_to(t_tabs *data, t_loc from, t_loc to);

// ps_chunk_helpers1.c
t_stack	*loc_to_stack(t_tabs *data, t_loc loc);
int		chunk_value(t_tabs *data, t_chunk *chunk, int n);
int		chunk_min_value(t_tabs *data, t_chunk *chunk);
int		chunk_max_value(t_tabs *data, t_chunk *chunk);
void	innit_size(t_chunk *min, t_chunk *mid, t_chunk *max);

// ps_chunk_helpers2.c
void	set_third_pivots_by_value(t_chunk *chunk, int min_val, int max_val,
			int *pivot1_val, int *pivot2_val);
void	set_split_loc(t_loc loc, t_chunk *min, t_chunk *mid, t_chunk *max);
t_bool	a_partly_sort(t_tabs *data, int from_top_count);
void	split_max_reduction(t_tabs *data, t_chunk *max_chunk_dest);

// ps_chunk_sort.c
void	sort_one(t_tabs *data, t_chunk *to_sort);
void	sort_two(t_tabs *data, t_chunk *to_sort);
void	sort_three(t_tabs *data, t_chunk *to_sort);
void	chunk_split(t_tabs *data, t_chunk *to_split, t_split_dest *dest);
void	rec_chunk_sort(t_tabs *data, t_chunk *to_sort);

// ps_main.c
void	chunk_sort(t_tabs *data);
void	sort_strategy(t_tabs *data);
// main is not prototyped here, it's the entry point

#endif // PUSH_SWAP_H