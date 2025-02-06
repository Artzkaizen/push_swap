#include <stdio.h>

long	ft_atol(const char *str)
{
	int		sign;
	long	nb;

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