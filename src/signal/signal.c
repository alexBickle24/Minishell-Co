#include "minishell.h"

int g_signal;

void    signal_init(void)
{
    g_signal = S_INIT;
    if (g_signal == S_INIT)
    {
        rl_on_new_line(); // Comienza una nueva linea
        rl_redisplay();
        ft_putstr_fd("\n", 1);
        //rl_replace_line("", 0);
        //rl_on_new_line();
        //rl_redisplay();
    }
}