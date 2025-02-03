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

int	ft_isspace(char c);
int	ft_isalnum(char *c);
long	ft_atoi(const char *str);
size_t	ft_strlen(const char *str);
int	ft_printf(const char *str, ...);
char	**ft_split(char *str, char sep);

#endif //PUSH_SWAP_H
