

#include "../../inc/minishell.h"

//FUNCION PARA CERRAR PIPES
void	close_fds(int *pipe_ports)
{
	if (pipe_ports)
	{
		close(pipe_ports[0]);
		close(pipe_ports[1]);
	}
}

//FUNCION DE LIBERACION DE TABLAS
void	ft_free_table(char **ptr)
{
	int	i;

	if (ptr == NULL)
		return ;
	i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
	ptr = NULL;
}


//Libera la lista enlazada de files
void	free_tocken_files(t_tocken *tocken)
{
	t_files *files;
	t_files *tmp;

	files = tocken->files;
	if (!files)
		return ;
	while (files)
	{
		if (files->type == HEREDOC && files->file_name)
			unlink(files->file_name);
		if (files->file_name)
			free(files->file_name);
		tmp = files;
		files = files->next;
		free(tmp);
	}
	tocken->files = NULL;
}

//libera la lista enlazada de cmds y hace que apunte a NULL
void free_tocken_cmds(t_tocken *tocken)
{
    t_pcmds *pcmds;
    t_pcmds *tmp;

    pcmds = tocken->pcmds;
    while (pcmds)
    {
        tmp = pcmds->next;
        if (pcmds->cmd)
            free(pcmds->cmd);
        free(pcmds);
        pcmds = tmp;
    }
    tocken->pcmds = NULL;
}



//Libera la lista enlazada de tockens y ahce que apunte a NULL
//pone el numero de tockens a cero
void free_tockens(t_msl *msl)
{
	t_tocken *c_tocken;
	t_tocken *tmp;

	if (!msl)
		return ;
	c_tocken = msl->tocken;
	while (c_tocken)
	{
		if (c_tocken->cmd_tb)
			ft_free_table(c_tocken->cmd_tb);
		if (c_tocken->env_tb)
			ft_free_table(c_tocken->env_tb);
		free_tocken_cmds(c_tocken);
		free_tocken_files(c_tocken);
		c_tocken->redir_in = NULL;
		c_tocken->redir_out = NULL;
		tmp = c_tocken;
		c_tocken = c_tocken->next;
		free(tmp);

	}
	msl->tocken = NULL;
	msl->total_tockens = 0;
}

