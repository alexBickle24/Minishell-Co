

#include "../../inc/minishell.h"

t_tocken    *list_new_tocken(int position)
{
	t_tocken   *new_node;

	new_node = (t_tocken *)malloc(sizeof(t_tocken));
	if (!new_node)
		return(NULL);
	new_node->index = position;
	new_node->error_file = 0;
	new_node->files = NULL;
	new_node->redir_in = NULL;
	new_node->redir_out = NULL;
	new_node->pcmds = NULL;
	new_node->cmd_tb = NULL;
	new_node->env_tb = NULL;
	new_node->error_cmd = 0; 
	new_node->next = NULL;
	//Puedo crear la pipe en esta funcion depende de la ejecucion y si decido crearla enparseo o en ejecucion (oarseo es lo mas facil)
	return (new_node);
}

t_files    *list_new_files(char *file, int type)
{
	t_files   *new_node;

	new_node = (t_files *)malloc(sizeof(t_files));
	if (!new_node)
		return(NULL);
	new_node->file_name = file;
	new_node->type = type;
	new_node->next = NULL;
	return (new_node);
}

t_pcmds    *list_new_pcmds(char *cmd)
{
	t_pcmds   *new_node;

	new_node = (t_pcmds *)malloc(sizeof(t_pcmds));
	if (!new_node)
		return(NULL);
	new_node->cmd = cmd;
	new_node->next = NULL;
	return (new_node);
}

void   list_addback_tocken(t_tocken **list, t_tocken *new_node)
{
	t_tocken   *current;

	if (!new_node)
		return ;
	current = *list;
	if (!*list)
		*list = new_node;
	else
	{
		while(current->next)
			current = current->next;
		current->next = new_node;
	}
}

void    list_addback_infiles(t_files **list, t_files *new_node)
{
	t_files   *current;

	if (!new_node)
		return ;
	current = *list;
	if (!*list)
		*list = new_node;
	else
	{
		while(current->next)
			current = current->next;
		current->next = new_node;
	}
}

void    list_addback_pcmds(t_pcmds **list, t_pcmds *new_node)
{
	t_pcmds   *current;

	if (!new_node)
		return ;
	current = *list;
	if (!*list)
		*list = new_node;
	else
	{
		while(current->next)
			current = current->next;
		current->next = new_node;
	}
}
