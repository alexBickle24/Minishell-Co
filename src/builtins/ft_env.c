#include "../inc/minishell.h"

/*
    env:
    Primero vemos cuantos argumentos hay
    Si tod es correcto entonces vamos recorriendo los argumentos
    Cuando encontremos uno con _ lo hardcodeamos
    Ya que es el ultimo codigo escrito
*/

//Meto el cotejo de argumentos para que veas como seria y la pongo en tipo int
int	ft_env(t_msl *msl, t_pcmds *pcmds)
{
	t_env	*tmp;
	t_pcmds *current_arg;
	int		control;

	// Si hay mas de un argumento lo rechazamos
	control = 0;
	current_arg = pcmds;
	while (current_arg)
	{
		if (control >= 1)
		{
			ft_putendl_fd("minishell: env: Too many Arguments", 2);
			return (0);//lo pongo a 0 porque es un error de minishell (subject)
		}
		control++;
		current_arg = current_arg->next;
	}
	tmp = msl->own_env;
	if (tmp == NULL)
	{
		ft_putendl_fd("minishell: env: No such file or directory", 2);
		return (127);
	}
	//impresion del env
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
	return (0);
}
