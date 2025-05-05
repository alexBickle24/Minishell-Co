#include "minishell.h"

int g_signal; //Variable global

void ft_sigint(int signal)
{
    (void)signal;
    printf("hello\n")
    //if (g_signal == S_SIGINT)
}

void    ft_signal_init(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    g_signal = S_INIT;
    sa_int.sa_handler = ft_sigint;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
    if (g_signal == S_INIT)
    {
        rl_on_new_line(); // Comienza una nueva linea
        rl_redisplay(); // Actualiza el display
    }
}