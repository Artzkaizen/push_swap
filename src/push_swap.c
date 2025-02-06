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

int	 main(int argc, char **argv)
{
	t_tabs *tabs;

	if (argc == 1)
		return (0);
	if (argc == 2)
		tabs= create_tabs(ft_split(argv[1], ' '));
	else
		tabs = create_tabs(argv + 1);
	if (!tabs)
		return (ft_printf("Error\n"), EXIT_FAILURE);
	free(tabs);
	return (EXIT_SUCCESS);
}

