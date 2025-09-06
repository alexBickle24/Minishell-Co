/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:45:08 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/05 21:49:46 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//This funciontino maybe has to be fixed in line 37 to protect for a posible
//buffer overflow in the case target_len = 0
int	get_infcmds(t_msl *msl, char **target, char *command, char *args)
{
	pid_t	pid;
	int		pipe_fds[2];
	int		ret;

	if (pipe(pipe_fds) == -1)
		ft_errerrno();
	pid = fork();
	if (pid < 0)
		ft_errerrno();
	if (pid == 0)
		execute_cmd(command, args, pipe_fds, ft_env_to_table(msl->own_env));
	ret = wait_childs5(pid);
	if (ret == 0)
	{
		close(pipe_fds[1]);
		*target = get_next_line(pipe_fds[0]);
		close(pipe_fds[0]);
		if (*target == NULL)
			return (1);
		(*target)[ft_strlen(*target) - 1] = '\0';
		return (ret);
	}
	close_fds(pipe_fds);
	return (ret);
}

//This funciontino maybe has to be fixed in line 67 to protect for a posible
//buffer overflow in the case target_len = 0
char	*which_cmd(t_msl *msl, char *command, char *args)
{
	pid_t	pid;
	int		pipe_fds[2];
	char	*where;

	if (pipe(pipe_fds) == -1)
		ft_errerrno();
	pid = fork();
	if (pid < 0)
		ft_errerrno();
	if (pid == 0)
		execute_cmd(command, args, pipe_fds, ft_env_to_table(msl->own_env));
	if (wait_childs5(pid) == 0)
	{
		close(pipe_fds[1]);
		where = get_next_line(pipe_fds[0]);
		close(pipe_fds[0]);
		if (where == NULL)
			return (NULL);
		where[ft_strlen(where) - 1] = '\0';
		return (where);
	}
	close_fds(pipe_fds);
	return (NULL);
}

void	execute_cmd(char *command, char *option, int *pipes, char **env)
{
	char	**args;

	args = create_args(command, option);
	if (args == NULL)
		return ;
	pipe_forward(pipes, 1, STDOUT_FILENO);
	if (execve(command, args, env) == -1)
	{
		ft_free_table(env);
		ft_free_table(args);
		exit(1);
	}
}

char	**create_args(char *command, char *option)
{
	char	**args;

	if (option == NULL)
	{
		args = (char **)malloc(sizeof (char *) * 2);
		if (args == NULL)
			return (NULL);
		args[0] = ft_strdup(command);
		args[1] = NULL;
	}
	else
	{
		args = (char **)malloc(sizeof (char *) * 3);
		if (args == NULL)
			return (NULL);
		args[0] = ft_strdup(command);
		args[1] = ft_strdup(option);
		args[2] = NULL;
	}
	return (args);
}
