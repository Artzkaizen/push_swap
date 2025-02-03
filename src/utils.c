
int ft_isspace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int	ft_isalnum(char *c)
{
    while (*c)
    {
        if (!(*c >= 48 && *c <= 57) && *c != '-' && *c != '+')
            return (0);
        c++;
    }
	return (1);
}