#include "../../inc/minishell.h"

void	user_fallbacks(t_msl *msl);
void	execute_cmd(char *command, char *option, int *pipes, char **env);
int		get_infcmds(t_msl *msl, char **target, char *command, char *args);
char	*which_cmd(t_msl *msl, char *command, char *args);
void	hostname_fallbacks(t_msl *msl, char **target);
int	get_hostnamedir(char **target);
void	create_ps1(t_msl *msl);


void minishell_init(t_msl **msl, char **env, char **argv)
{
	if (msl != NULL)
	{
	   *msl = (t_msl *)ft_calloc(sizeof(t_msl), 1);
		if (*msl == NULL)
			return ;
		env_init(*msl, env, argv);//pasa la tabla de env a formato lista para no tener que hacer reallocs
		msl[0]->sys = (t_system *)ft_calloc(sizeof(t_system), 1);
		if (msl[0]->sys == NULL)
			return ;
		user_fallbacks(*msl);
		printf("el valor de user es %s\n", msl[0]->sys->user);//
		hostname_fallbacks(*msl, &(msl[0]->sys->host));
		printf("el valor del hostname A es %s\n", msl[0]->sys->host);
		// create_ps1(*msl);
		msl[0]->parsing_utils = init_parsing(*msl);//la utils de parseo
		msl[0]->msl_pid = (pid_t)ft_getpid();//el pid del procesos
		// print_msl(msl[0]);
		signal_init();
	}
}

////////////////////////////////////////////////// HOSTNAME /////////////////////////////////////////////////////////

// void	create_ps1(t_msl *msl)
// {
// 	char	*ps1;

// 	ps1 = ft_strjoin("\033[1;32m", msl->sys->user); // Nombre en verde
// 	ps1 = ft_strjoin(ps1, "@");
// 	ps1 = ft_strjoin(ps1, msl->sys->host); // Hostname en verde
// 	ps1 = ft_strjoin(ps1, "\033[0m"); // Reset color
// 	ps1 = ft_strjoin(ps1, ":"); // Dos puntos en blanco
// 	ps1 = ft_strjoin(ps1, "\033[1;34m"); // Ruta en azul
// 	ps1 = ft_strjoin(ps1, getcwd(NULL, 0));
// 	ps1 = ft_strjoin(ps1, "\033[0m"); // Reset color

// 	// Asignar el prompt al sistema
// 	msl->sys->ps1 = ps1;
// }

void	hostname_fallbacks(t_msl *msl, char **target)
{
	int	fallback;
	char *abs_path;

	fallback = 0;
	if (fallback == 0)
	{
		fallback =+ get_hostnamedir(&(msl->sys->host));
		printf("el valor del hostname A es %s\n", msl->sys->host);
	}
	if (fallback == 1)
	{
		abs_path = which_cmd(msl, "/usr/bin/which", "hostname");
		if (abs_path)
			fallback =+ get_infcmds(msl, &(msl->sys->user), abs_path, NULL);
		else
			fallback++;
	}
	if (fallback == 2)
		*target = NULL;
}


int	get_hostnamedir(char **target)
{
	int fd;
	char *hostname;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return (1);
	else
	{
		hostname = get_next_line(fd);
		if (hostname == NULL)
			return (1);
		hostname[ft_strlen(hostname) - 1] = '\0';
		*target = hostname;
		close (fd);
		return (0);
	}
}

// //////////////////////////////////////////////////   USER  //////////////////////////////////////////////////

void	user_fallbacks(t_msl *msl)
{
	int	fallback;
	char	*abs_path;
	t_env *node;

	fallback = 0;
	abs_path = NULL;
	if (fallback == 0)
	{
		abs_path = which_cmd(msl, "/usr/bin/which", "whoami");
		if (abs_path)
			fallback += get_infcmds(msl, &(msl->sys->user), abs_path, NULL);
		else
			fallback++;
	}
	if (fallback ==  1)
	{
		abs_path = which_cmd(msl, "/usr/bin/which", "id");
		if (abs_path)
			fallback += get_infcmds(msl, &(msl->sys->user), abs_path, "-un");
		else
			fallback ++;
	}
	if (fallback == 2)
	{
		//aqui iria la opcion de usar el pwd ir al home y trimerat el nombre de la carpetaa (guarra)
		fallback++;
	}
	if (fallback == 3)
	{
		node = search_id_node(msl,"USER");
		if (node == NULL)
			fallback++;
		else
			msl->sys->user = ft_strdup(node->value);
	}
	if (fallback == 4)
	{
		msl->sys->user = NULL;
		fallback++;
	}
	free(abs_path);
}

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

	if (option == NULL)
	{
		args = (char **)malloc(sizeof (char *) * 2);
		if (args == NULL)
			return ;
		args[0] = ft_strdup(command);
		args[1] = NULL;
	}
	else
	{
		args = (char **)malloc(sizeof (char *) * 3);
		if (args == NULL)
			return ;
		args[0] = ft_strdup(command);
		args[1] = ft_strdup(option);
		args[2] = NULL;
	}
	pipe_forward(pipes, 1, STDOUT_FILENO);
	if (execve(command, args, env) == -1)
	{
		ft_free_table(env);
		ft_free_table(args);
		exit(1);
	}
}

