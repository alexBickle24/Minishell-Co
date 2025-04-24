#include "minishell.h"

int g_signal;

void    ft_signal_init(void)
{
    g_signal = S_INIT;
    if (g_signal == S_INIT)
    {
        rl_on_new_line(); // Comienza una nueva linea
        rl_redisplay(); // Actualiza el display
    }
}