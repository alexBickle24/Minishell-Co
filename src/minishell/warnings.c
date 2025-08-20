

#include "../../inc/minishell.h"

void ft_shlvl_warning(char *str)
{
    ft_putstr_fd("minishell: warning: shell level ", 2);
    ft_putstr_fd("(", 2);
    ft_putstr_fd(str, 2);
    ft_putstr_fd(") too high, resetting to 1", 2);
}