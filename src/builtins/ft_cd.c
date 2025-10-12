#include "../inc/minishell.h"

void	ft_edit_env(t_msl *msl, char *id, char *value)
{
	t_env	*tmp;

	tmp = msl->own_env;
	while (tmp)
	{
		if (ft_strncmp(tmp->id, id, ft_strlen(id)) == 0)
		{
			ft_freeptr(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
}

char *ft_cdpath(t_msl *msl)
{
	t_pcmds *tmp;
	char	*path;
	int 	i;

	tmp = msl->tocken->pcmds;
	if (ft_tokencounter(msl) != 2)
	{
		perror("Error de argc\n");
	}
	i = 0;
	while (tmp)
	{
		i++;
		if (i == 2)
			path = ft_strdup(tmp->cmd);
		tmp = tmp->next;
	}
	return path;
}

int	ft_tokencounter(t_msl *msl)
{
	t_pcmds	*tmp;
	int i;

	i = 0;
	tmp = msl->tocken->pcmds;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
