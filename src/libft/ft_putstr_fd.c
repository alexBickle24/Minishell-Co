#include "minishell.h"

void    ft_putstr_fd(char *str, int fd)
{
    int i;

    if (!str)
        return ;
    i = 0;
    while (str[i])
    {
        write(fd, &str[i], 1);
        i++;
    }
}