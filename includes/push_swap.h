#ifndef PUSH_SWAP_H
#define PUSH_SWAP_H

#include <unistd.h>
#include <stdlib.h>
#include "../includes/ft_printf.h"

#define TRUE 1
#define FALSE 0
#define INT_MAXi 2147483647
#define INT_MINi -2147483648


#define ADD_BACK 0
#define ADD_FRONT 1

typedef int t_bool;

typedef enum e_sort_order
{
	UNSORTED,
	ASCENDING,
	DESCENDING
} t_sort_order;

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
	int min;
	int max;
} t_stack;

typedef struct s_tabs
{
	t_stack a;
	t_stack b;
} t_tabs;


// TABS

/*
* Create a stack from a string array
* @param **str: array of strings
* @return t_tabs: a struct containing two stacks
* @return NULL: if the stack is invalid
*/
t_tabs *create_tabs(char **str);
void print_tabs(t_tabs *tabs);


// STACK
/*
* Validate a stack
* @param *stack: the stack to validate
* @return TRUE: if the stack is valid
* @return FALSE: if the stack is invalid
*/
t_bool validate_stack(t_stack *stack);


/*
* Fill a stack with a string array
* @param *stack: the stack to fill
* @param **str: an array of strings
* @param tab_size: the size of the string array
* @return TRUE: if the stack is filled
* @return FALSE: if the stack is invalid
*/
t_bool fill_stack(t_stack *stack, char **str, int tab_size);


t_bool is_sorted(t_stack *stack, t_sort_order order);

// utils
int	ft_isspace(char c);
int	ft_isnumeric(char *c);
long	ft_atol(const char *str);
size_t	ft_strlen(const char *str);
int	ft_printf(const char *str, ...);
char	**ft_split(char *str, char sep);





void free_stack(t_stack *stack);
void move_stack(t_tabs *tabs, t_stack_move move);
t_bool ft_add_node(t_node **list, int value, t_node *new);

void push_swap(t_tabs *tabs);
void print_tabs(t_tabs *tabs);
void free_tabs(t_tabs *tabs);

#endif //PUSH_SWAP_H
