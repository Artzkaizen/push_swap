#include <unistd.h>

int ft_isnumeric(char *c)
{
	while (*c)
	{
		if (!(*c >= 48 && *c <= 57) && !((*c == '-' || *c == '+') && (*(c + 1) >= 48 && *(c + 1) <= 57)))
			return (0);
		c++;
	}
	return (1);
}

long ft_atol(const char *str)
{
	int sign;
	long nb;

	nb = 0;
	sign = 1;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	while ((*str >= '0' && *str <= '9'))
	{
		nb = nb * 10 + (*str - '0');
		str++;
	}
	return (nb *= sign);
}

void ft_putstr(char *str)
{
	while (*str)
	{
		write(1, str, 1);
		str++;
	}
}

// File: ps_main_utils.c
#include "push_swap.h"

/**
 * @brief Finds the 0-indexed position of the node with value 0 in stack A.
 * @param a Pointer to stack A.
 * @param min_pos_ptr Pointer to store the found position.
 * @return t_bool TRUE if found, FALSE otherwise.
 */
t_bool find_min_pos(t_stack *a, int *min_pos_ptr)
{
	t_node *current;
	int current_pos; // 2 variables + params

	if (!a || !a->head)
		return (FALSE);
	current = a->head;
	current_pos = 0;
	while (current_pos < a->size) // Loop max N times
	{
		if (current->value == 0)
		{
			*min_pos_ptr = current_pos; // Store result via pointer
			return (TRUE);				// Found
		}
		current = current->next;
		current_pos++;
		if (current == a->head && current_pos > 0)
			break; // Safety break for circular list iteration
	}
	return (FALSE); // Not found
}

/**
 * @brief Rotates stack A using RA or RRA to bring the element at min_pos to the top.
 * @param data Pointer to t_tabs.
 * @param min_pos The 0-indexed position of the element to bring to the top.
 */
void rotate_a_to_top(t_tabs *data, int min_pos)
{
	int i; // 1 variable + params

	if (min_pos <= 0) // Already at top or invalid position
		return;
	if (min_pos <= data->a.size / 2)
	{
		// Rotate forward (RA) is shorter
		i = 0;
		while (i < min_pos)
		{
			move_and_print(data, RA);
			i++;
		}
	}
	else
	{
		// Rotate backward (RRA) is shorter
		i = 0;
		while (i < data->a.size - min_pos)
		{
			move_and_print(data, RRA);
			i++;
		}
	}
}