#include "../inc/minishell.h"

/*
    env:
    Primero vemos cuantos argumentos hay
    Si tod es correcto entonces vamos recorriendo los argumentos
    Cuando encontremos uno con _ lo hardcodeamos
    Ya que es el ultimo codigo escrito
*/
void	ft_env(t_msl *msl)
{
	t_env	*tmp;

	// Si hay mas de un argumento lo rechazamos
	tmp = msl->own_env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->id, "_\0", 2))
			ft_putendl_fd("_=/usr/bin/env", 1);
		else
		{
			ft_putstr_fd(tmp->id, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(tmp->value, 1);
		}
		tmp = tmp->next;
	}
}