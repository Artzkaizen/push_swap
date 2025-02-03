#include "push_swap.h"
#include "ft_printf.h"


// static t_bool is_sorted(int *tab, int size)
// {
// 	int i;

// 	i = 0;
// 	while (i < size - 1)
// 	{
// 		if (tab[i] > tab[i + 1])
// 			return (FALSE);
// 		i++;
// 	}
// 	return (TRUE);
// }
#include <stdio.h>
static int *create_tab(char **str)
{
	int i;
	int len;
	int *tab;
	long tmp;

	i = 0;
	len = 0;
	while (str[len])
		len++;
	tab = (int *)malloc(sizeof(int) * len +1 );
	if (!tab)
		return (NULL);
	while (i < len)
	{
		if (!ft_isalnum(str[i]))
			return (free(tab), NULL);
		tmp = ft_atoi(str[i]);

		if (tmp > INT_MAX || tmp < INT_MIN)
			return (free(tab), NULL);
		tab[i] = (int)tmp;
		i++;
	}
	i = 0;
	while (i < len)
	{
		tmp = 0;
		while ((int)tmp < len)
		{
			if (i != tmp && tab[i] == tab[tmp])
				return (free(tab), NULL);
			tmp++;
		}
		// printf("new{%d} = %d\n", i, tab[i]);
		i++;

	}
	
	return (tab);
}

int	 main(int argc, char **argv)
{
	int *tab;

	if (argc == 1)
		return (0);
	if (argc == 2)
		tab = create_tab(ft_split(argv[1], ' '));
	else
		tab = create_tab(argv + 1);
	if (!tab)
		return (ft_printf("Error\n"), EXIT_FAILURE);
	free(tab);
	return (EXIT_SUCCESS);
}

