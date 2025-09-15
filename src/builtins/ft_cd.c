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

/*
	Cambiar de directorio
	Podemos recibir una ruta o cambiar a HOME 
*/
void	ft_cd(t_msl *msl)
{
	char	*oldpwd;
	char	*home;
	char	*newpwd;
	int	nbr_tokens;
	char	*path;

	ft_pwd(); // control (borrar)
	nbr_tokens = ft_tokencounter(msl); // Numero de tokens
	if (nbr_tokens > 2) // Si es mas de dos: ERROR
	{
		perror("Mas de 2 tokens\n");
		return ;
	}
	oldpwd = getcwd(NULL, 0); // Obtenemos el cwd
	if (!oldpwd)
	{
		perror("getcwd");
		return ;
	}
	ft_edit_env(msl, "OLDPWD", oldpwd); // editamos los env
	ft_freeptr(oldpwd);
	if (nbr_tokens == 1)
	{
		home = getenv("HOME");
		if (!home)
		{
			perror("home");
			return ;
		}
		if (chdir(home) == -1)
		{
			perror("cd");
			return ;
		}
	}
	else
	{
		printf("test: cd x\n");
		path = ft_cdpath(msl);
		newpwd = getcwd(NULL, 0);
		if (!newpwd)
		{
			perror("getcwd");
			return ;
		}
		if (chdir(path) == -1)
		{
			perror("cd");
			ft_freeptr(newpwd);
			free(path);
			return ;
		}
		ft_edit_env(msl, "PWD", newpwd);
		ft_freeptr(newpwd);
		free(path);
	}
	ft_pwd(); // control
}
