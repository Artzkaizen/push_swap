/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:22:28 by chuezeri          #+#    #+#             */
/*   Updated: 2024/12/10 18:20:55 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>
#include <unistd.h>

#define DEC_BASE "0123456789"
#define HEX_BASE_UPP "0123456789ABCDEF"
#define HEX_BASE_LOW "0123456789abcdef"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_putnbr_base_ul(unsigned long long num, char *base)
{
	int			radix;
	static int	count;

	count = 0;
	radix = ft_is_valid_base(base);
	if (radix > 1)
	{
		if (num >= (unsigned long)radix)
			ft_putnbr_base_ul(num / radix, base);
		count += ft_print_char_fd(base[num % radix], STDOUT_FILENO);
	}
	return (count);
}

int	ft_print_ptr(void *ptr, char *base)
{
	int	count;

	count = 0;
	if (!ptr)
		return (ft_print_str_fd("(nil)", STDOUT_FILENO));
	count = ft_print_str_fd("0x", STDOUT_FILENO);
	return (count + ft_putnbr_base_ul((unsigned long)ptr, base));
}

int	format_string(va_list *args, char c)
{
	if (c == 's')
		return (ft_print_str_fd(va_arg(*args, char *), STDOUT_FILENO));
	else if (c == 'c')
		return (ft_print_char_fd(va_arg(*args, int), STDOUT_FILENO));
	else if (c == 'i' || c == 'd')
		return (ft_putnbr_base(va_arg(*args, int), DEC_BASE, 0));
	else if (c == 'u')
		return (ft_putnbr_base(va_arg(*args, t_u_int), DEC_BASE, 0));
	else if (c == 'x')
		return (ft_putnbr_base(va_arg(*args, t_u_int), HEX_BASE_LOW, 0));
	else if (c == 'X')
		return (ft_putnbr_base(va_arg(*args, t_u_int), HEX_BASE_UPP, 0));
	else if (c == 'p')
		return (ft_print_ptr(va_arg(*args, void *), HEX_BASE_LOW));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	int			c;
	size_t		i;
	va_list		args;

	i = 0;
	c = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i + 1] && str[i] == '%')
		{
			if (ft_strchr("cspdiuxX", str[i + 1]))
				c += format_string(&args, str[i + 1]);
			else
				c += ft_print_char_fd(str[i], STDOUT_FILENO);
			i += 2;
		}
		else
			c += ft_print_char_fd(str[i++], STDOUT_FILENO);
	}
	va_end(args);
	return (c);
}