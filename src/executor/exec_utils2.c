
#include "../../inc/minishell.h"

/*

	**BRIEF: Para los builtins hay que selecionas que builtin es el nuestro
	y ejecutarlo. Esta fucnion vale para la funcion exec order y para la funcion
	exec builtin
*/

void exec_cmd(t_tocken *c_tocken)
{
	int error;
	
	error = c_tocken->error_cmd;
	if (error == 1)
		ft_exterror_exes(c_tocken->cmd_tb[0], 0);
	else if (error == 2)
		ft_exterror_cmd(c_tocken->cmd_tb[0]);
	else if (error == 3)
		ft_exterror_exes(c_tocken->cmd_tb[0], 1);
	else if (error == 0)
	{
		if (execve(c_tocken->pcmds->cmd, c_tocken->cmd_tb, c_tocken->env_tb) == -1)
			ft_exterrno();
		else
			exit(0);
	}
}

int is_builtin(t_tocken *tocken)
{
	if (!tocken->pcmds)
		return (0);
	if (!ft_strncmp(tocken->pcmds->cmd, "echo\0", 5))
		return(1);
	if (!ft_strncmp(tocken->pcmds->cmd, "cd\0", 3))
		return(2);
	if (!ft_strncmp(tocken->pcmds->cmd, "pwd\0", 4))
		return(3);
	if (!ft_strncmp(tocken->pcmds->cmd, "export\0", 7))
		return(4);
	if (!ft_strncmp(tocken->pcmds->cmd, "unset\0", 6))
		return(5);
	if (!ft_strncmp(tocken->pcmds->cmd, "env\0", 4))
		return(6);
	if (!ft_strncmp(tocken->pcmds->cmd, "exit\0", 5))
		return(7);
	return(0);
}

//esta funcion es para la ejecucion de los buildings en el proceso padre
//retorna un valor int. Ese valor se le va a dar directamente al minishell exit status
//ya que aqui no lo recoge waitpid,, no hacemos exit porque estamos en elproceso padre.
int exec_builtin(t_msl *msl, t_tocken *c_tocken, int builtin)
{
	(void )msl;
	(void )c_tocken;
	if (builtin == 1)
	{
		// return(ft_echo(c_tocken->cmds, msl->own_env));
	}
	else if (builtin == 2)
	{
		// return(ft_cd(c_tocken->cmds, msl->own_env));
	}
	else if (builtin == 3)
	{
		return(ft_pwd());
	}
	else if (builtin == 4)
	{
		// return(ft_export(c_tocken->cmds, msl->own_env));
	}
	else if (builtin == 5)
	{
		// return(ft_unset(c_tocken->cmds, msl->own_env));
	}
	else if (builtin == 6)
		return(ft_env(msl, c_tocken->pcmds));
	else if (builtin == 7)
	{
		// return(ft_exit(c_tocken->cmds, msl));
	}
	return(0);
}

void create_pipes(t_tocken *c_tocken)
{
	if (pipe(c_tocken->pipe_fds) == -1)
        ft_errerrno();
}

// void evaluate_local_variable(t_tocken *c_tocken)
// {
// 	int	i;
// 	t_pcmds *aux;
// 	char *tmp;

// 	i = 0;
// 	aux = c_tocken->pcmds;
// 	while (aux->cmd[i])
// 	{
// 		if (ft_strchr(aux->cmd, '=') == 1 && aux->next != NULL)
// 		{
// 			tmp = c_tocken->cmd_tb[i];
// 			while(c_tocken->cmd_tb[i] != NULL)
// 			{
// 				c_tocken->cmd_tb[i] = c_tocken->cmd_tb[i+1];
// 				i++;
// 			}
// 			free(tmp);
// 		}
// 		else
// 			//crear variable
// 		i = 0;
// 	}
// }

