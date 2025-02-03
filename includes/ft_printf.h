/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:20:25 by chuezeri          #+#    #+#             */
/*   Updated: 2024/12/10 18:20:31 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>

typedef unsigned int	t_u_int;

size_t	ft_strlen(const char *str);
int		ft_printf(const char *str, ...);
char	*ft_strchr(const char *s, int c);
int		ft_print_char_fd(char c, int fd);
int		ft_print_str_fd(char *str, int fd);
int		ft_print_ptr(void *ptr, char *base);
int		ft_is_valid_base(char *str);
int		ft_putnbr_base(long num, char *base, int count);

#endif