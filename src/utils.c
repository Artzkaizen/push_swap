
int ft_isspace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}
#include <stdio.h>
int	ft_isnumeric(char *c)
{
    while (*c)
    {
        if (!(*c >= 48 && *c <= 57) && !((*c == '-' || *c == '+') && (*(c+ 1) >= 48 && *(c + 1) <= 57)))
            return (0);
        c++;
    }
	return (1);
}