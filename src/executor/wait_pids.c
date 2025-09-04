
#include "../../inc/minishell.h"

/*
	*Estas fucnoines no me dan el error code del ultimo
	comando en ejecutarse sino el error code del ultimo comando en
	acabar. El que quiero recoger es el del ultimo en ejecutarse
*/

//error_code->trsnformar(p)->captutrar->error_code
void wait_childs1(t_msl *msl)
{
	int status;
	pid_t pid;
	
	pid = waitpid(-1, &status, 0);
	while(pid != -1 && errno != EINTR)
		pid = waitpid(-1, &status, 0);
	msl->exit_status = WEXITSTATUS(status);
}

//Señal_code->captura(p)->transformar->error_code
void wait_childs2(t_msl *msl)
{
	int status;
	pid_t pid;
	
	pid = waitpid(-1, &status, 0);
	while(pid > 0 && errno != EINTR)
		pid = waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		msl->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		msl->exit_status = WTERMSIG(status) + 128;
}

//Obtiene el pid del proceso buscandolo en la carpeta /proc/self/stat
//Siempre es el primer dato le capreta es un dato tipo int ademas esta
//limitado por el sistema. 
int ft_getpid(void)
{
	int fd;
	char buffer[MLEN_INT];

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	if (read(fd, buffer, MLEN_INT) == -1)
		return (-1);
	close (fd);
	return(ft_atoi(buffer));
}


void wait_childs3(t_msl *msl)
{
	t_tocken *c_tocken;
	pid_t pid;
	int status;
	
	c_tocken = msl->tocken;
	while (c_tocken->next)
		c_tocken = c_tocken->next;
	while(1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid > 0)
			msl->last_process = pid;
		if (pid == c_tocken->pid)
		{
			if (WIFEXITED(status))
				msl->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				msl->exit_status = WTERMSIG(status) + 128;
		}
		if (pid < 0 && errno != EINTR)
			break ;
	}
}

void wait_heredoc(void)
{
	pid_t pid;
	
	pid = waitpid(-1, NULL, 0);
	while(pid != -1 && errno != EINTR)
		pid = waitpid(-1, NULL, 0);
}

void wait_childs4(t_msl *msl, pid_t pid_heredoc)
{
	pid_t pid;
	int status;
	
	while(1)
	{
		pid = waitpid(pid_heredoc, &status, 0);
		if (pid == pid_heredoc)
		{
			if (WIFEXITED(status))
			{
				g_signal = WEXITSTATUS(status);
				if (WEXITSTATUS(status) == 2)
					msl->exit_status = WEXITSTATUS(status) + 128;
			}
		}
		if (pid < 0 && errno != EINTR)
			break ;
	}
}


int wait_childs5(pid_t pid_child)
{
	pid_t pid;
	int status;
	int ret;

	ret = 0;
	while(1)
	{
		pid = waitpid(pid_child, &status, 0);
		if (pid == pid_child)
		{
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) != 0)
					ret = 1;
				else
					ret = 0;
			}
		}
		if (pid < 0 && errno != EINTR)
			break ;
	}
	return (ret);
}