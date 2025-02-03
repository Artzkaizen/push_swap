/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 19:32:58 by chuezeri          #+#    #+#             */
/*   Updated: 2024/12/10 18:20:51 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_char_fd(char c, int fd)
{
	return (write(fd, &c, 1));
}

int	ft_print_str_fd(char *str, int fd)
{
	int	count;

	count = 0;
	if (!str)
		return (ft_print_str_fd("(null)", fd));
	while (*str)
	{
		count += ft_print_char_fd(*str, fd);
		str++;
	}
	return (count);
}

char	*ft_strchr(const char *s, int c)
{
	char	*str;

	str = (char *)s;
	while (*str || *str == (unsigned char)c)
	{
		if (*str == (unsigned char)c)
			return (str);
		str++;
	}
	return (NULL);
}

int	ft_is_valid_base(char *str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i + 1] && str[i])
	{
		if (str[i] == '-' || str[i] == '+' || (str[i] == ' '
				|| (str[i] >= '\t' && str[i] <= '\r')))
			return (0);
		j = i + 1;
		while (str[i] && str[j])
		{
			if (str[i] == str[j])
				return (0);
			j++;
		}
		i++;
	}
	return (++i);
}

int	ft_putnbr_base(long num, char *base, int count)
{
	int			radix;

	count = 0;
	radix = ft_is_valid_base(base);
	if (radix > 1)
	{
		if (num < 0)
		{
			count += ft_print_char_fd('-', STDOUT_FILENO);
			if (num <= -radix)
				count += ft_putnbr_base(((num / radix) * -1), base, count);
			num = (num % radix) * -1;
		}
		if (num >= radix)
			count += ft_putnbr_base(num / radix, base, count);
		count += ft_print_char_fd(base[num % radix], STDOUT_FILENO);
	}
	return (count);
}