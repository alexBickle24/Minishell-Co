#include "minishell.h"

int g_signal; //Variable global

void ft_sigint(int signal)
{
    (void)signal;
    write(1, "\nCtrl+C\n", 8);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay(); 
    g_signal = S_SIGINT;
}


void    ft_signal_init(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    g_signal = S_INIT;
    sa_int.sa_handler = ft_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_int.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}