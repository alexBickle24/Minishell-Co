
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
		ft_exterror_exes(c_tocken->cmd_tb[0]);
	else if (error == 2)
		ft_exterror_cmd(c_tocken->cmd_tb[0]);
	else if (error == 0)
	{
		if (execve(c_tocken->cmd_tb[0], c_tocken->cmd_tb, c_tocken->env_tb) == -1)
			ft_exterrno();
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

void exec_builtin(t_msl *msl, t_tocken *c_tocken, int builtin)
{
	(void )msl;
	(void )c_tocken;
	if (builtin == 1)
	{
		// ft_echo(c_tocken->cmds, msl->own_env);
	}
	else if (builtin == 2)
	{
		// ft_cd(c_tocken->cmds, msl->own_env);
	}
	else if (builtin == 3)
	{
		// ft_pwd();
	}
	else if (builtin == 4)
	{
		// ft_export(c_tocken->cmds, msl->own_env);
	}
	else if (builtin == 5)
	{
		// ft_unset(c_tocken->cmds, msl->own_env);
	}
	else if (builtin == 6)
	{
		// ft_env(msl->own_env);
	}
	else if (builtin == 7)
	{
		// ft_exit(c_tocken->cmds, msl);
	}
}

void create_pipes(t_tocken *c_tocken)
{
	if (pipe(c_tocken->pipe_fds) == -1)
        ft_errerrno();
	else
		printf("Pipe created successfully\n");

}