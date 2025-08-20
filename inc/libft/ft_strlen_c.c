

//falta el header y norminette
#include "libft.h"

int ft_strlen_c(char *str)
{
    int i;
    
    i = 0;
    while (str[i])
        i++;
    return (i);
}