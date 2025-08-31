

#include "../../inc/minishell.h"

// void exec_bashrc(void);
// void exec_profile(void);
void	get_hostname(void);
void	get_user(t_msl *msl, char **env);

void minishell_init(t_msl **msl, char **env, char **argv)
{
	if (msl != NULL)
	{
	   *msl = (t_msl *)ft_calloc(sizeof(t_msl), 1);
		if (*msl == NULL)
			return ;
		env_init(*msl, env, argv);//pasa la tabla de env a formato lista para no tener que hacer reallocs
		//check path
		msl[0]->sys = (t_system *)ft_calloc(sizeof(t_system), 1);
		if (msl[0]->sys == NULL)
			return ;
		msl[0]->parsing_utils = init_parsing(*msl);//la utils de parseo
		msl[0]->msl_pid = (pid_t)ft_getpid();//el pid del proceso
		// get_hostname();
		// get_user(*msl, env);
		// print_msl(msl[0]);
		signal_init();
	}
}

////////////////////////////////////////////////// HOSTNAME /////////////////////////////////////////////////////////


// void	get_hostname_fallbacks(t_msl *msl, char **target)
// {
// 	int	fallback;
// 	char *abs_path;

// 	fallback = 0;
// 	if (fallback == 0)
// 		fallback =+ get_hostnamedir(msl, &(msl->sys->host));
// 	if (fallback == 1)
// 	{
// 		abs_path = which_cmd(msl, "which", "hostname");
// 		if (abs_path)
// 			fallback =+ get_infcmds(msl, &(msl->sys->user), abs_path, NULL);
// 		else
// 			fallback++;
// 	}
// 	if (fallback == 2)
// 		*target = NULL;
// }


// int	get_hostnamedir(t_msl *msl, char **target)
// {
// 	int fd;
// 	char *hostname;

// 	fd = open("/etc/hostname", O_RDONLY);
// 	if (fd < 0)
// 		return (1);
// 	else
// 	{
// 		hostname = get_next_line(fd);
// 		if (hostname == NULL)
// 			return (NULL);
// 		hostname[ft_strlen(hostname) - 1] = '\0';
// 		*target = hostname;
// 		close (fd);
// 		return (0);
// 	}
// }

// //////////////////////////////////////////////////   USER  //////////////////////////////////////////////////

// void	get_user_fallbacks(t_msl *msl)
// {
// 	int	fallback;
// 	char	*abs_path;

// 	fallback = 0;
// 	abs_path = NULL;
// 	if (fallback == 0)
// 	{
// 		abs_path = which_cmd(msl, "which", "whoami");
// 		if (abs_path)
// 			fallback =+ get_infcmds(msl, &(msl->sys->user), abs_path, NULL);
// 		else
// 			fallback++;
// 	}
// 	if (fallback ==  1)
// 	{
// 		abs_path = which_cmd(msl, "which", "id");
// 		if (abs_path)
// 			fallback =+ get_infcmds(msl, &(msl->sys->user), abs_path, "-un");
// 		else
// 			fallback ++;
// 	}
// 	if (fallback == 2)
// 	{
// 		//aqui iria la opcion de usar el pwd ir al home y trimerat el nombre de la carpetaa (guarra)
// 		fallback++;
// 	}
// 	if (fallback == 3)
// 	{
// 		//aqui va la opcion de poner el del usuario heredado si lo hay del env
// 		fallback++;
// 	}
// 	if (fallback == 4)
// 	{
// 		//aqui va la opcion de dejarlo en nULL
// 		fallback++;
// 	}
// 	free(abs_path);
// }

// int	get_infcmds(t_msl *msl, char **target, char *command, char *args)
// {
// 	pid_t	pid;
// 	int		pipe_fds[2];
// 	int		ret;

// 	if (pipe(pipe_fds) == -1)
// 		ft_errerrno();
// 	pid = fork();
// 	if (pid < 0)
// 		ft_errerrno();
// 	if (pid == 0)
// 		execute_cmd(command, pipe_fds, ft_env_to_table(msl->own_env), NULL);
// 	if (pid > 0)
// 		ret = wait_childs5(pid);
// 	if (ret == 0)
// 	{
// 		close(pipe_fds[1]);
// 		*target = get_next_line(pipe_fds[0]);
// 		close(pipe_fds[0]);
// 		if (*target == NULL)
// 			return (1);
// 		(*target)[ft_strlen(*target) - 1] = '\0';
// 		return (ret);
// 	}
// 	close_fds(pipe_fds);
// 	return (ret);
// }

// char	*which_cmd(t_msl *msl, char *command, char *args)
// {
// 	pid_t	pid;
// 	int		pipe_fds[2];
// 	char	*where;

// 	if (pipe(pipe_fds) == -1)
// 		ft_errerrno();
// 	pid = fork();
// 	if (pid < 0)
// 		ft_errerrno();
// 	if (pid == 0)
// 		execute_cmd(command, pipe_fds, ft_env_to_table(msl->own_env), NULL);
// 	if (wait_childs5(pid))
// 	{
// 		close(pipe_fds[1]);
// 		where = get_next_line(pipe_fds[0]);
// 		close(pipe_fds[0]);
// 		if (where == NULL)
// 			return (NULL);
// 		where[ft_strlen(where) - 1] = '\0';
// 		return (where);
// 	}
// 	close_fds(pipe_fds);
// 	return (NULL);
// }

// void	execute_cmd(char *command, char *option, int *pipes, char **env)
// {
// 	char	**args;

// 	if (option == NULL)
// 	{
// 		args = (char **)malloc(sizeof (char *) * 2);
// 		if (args == NULL)
// 			return ;
// 		args[0] = ft_strdup(command);
// 	}
// 	else
// 	{
// 		args = (char **)malloc(sizeof (char *) * 3);
// 		if (args == NULL)
// 			return ;
// 		args[0] = ft_strdup(command);
// 		args[1] = ft_strdup(option);
// 	}
// 	args[2] = NULL;
// 	pipe_forward(pipes, 1, STDOUT_FILENO);
// 	if (execve(command, args, env) == -1)
// 	{
// 		free(args[0]);
// 		free(args[1]);
// 		ft_free_table(env);
// 		exit(1);
// 	}
// }

