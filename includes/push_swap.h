#ifndef PUSH_SWAP_H
#define PUSH_SWAP_H

#include <unistd.h>
#include <stdlib.h>
#include "ft_printf.h"

#define TRUE 1
#define FALSE 0
#define INT_MAX 2147483647
#define INT_MIN -2147483648

typedef int t_bool;

typedef struct s_stack
{
	int *tab;
	int size;
} t_stack;

typedef struct s_tabs
{
	t_stack a;
	t_stack b;
} t_tabs;

void free_tabs(t_tabs *tabs);
t_tabs *create_tabs(char **str);
void print_tabs(t_tabs *tabs, int size);

// utils
int	ft_isspace(char c);
int	ft_isnumeric(char *c);
long	ft_atol(const char *str);
size_t	ft_strlen(const char *str);
int	ft_printf(const char *str, ...);
char	**ft_split(char *str, char sep);

#endif //PUSH_SWAP_H
