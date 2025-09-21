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
	char	*argc_pth;

	ft_pwd(); // control (borrar)
	nbr_tokens = ft_tokencounter(msl); // Numero de tokens
	if (nbr_tokens > 2) // Si es mas de dos: ERROR
	{
		perror("minishell: cd: too many arguments\n");
		return ;
	}
	oldpwd = getcwd(NULL, 0); // Obtenemos el cwd

	ft_edit_env(msl, "OLDPWD", oldpwd); // editamos los env
	ft_freeptr(oldpwd);
	if (nbr_tokens == 1)
	{
		home = getenv("HOME");//*search_id_node(t_msl *msl, char *id)
		if (!home)
		{
			perror("minishell: cd: HOME not set\n");
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
		//Caso 1: Cuando nos encntramos de argumento la ~ y no exxiste la variable de 
		//entorno home te leva a la ruta que esta en la estrcutura "msl->sys->home"
		//Cas0 2: Cuando enontramos un gion vamos a la ruta que esta en la variable de entorno OLDPWD
		//si la variable no existe o esta vacia se muestra mensaje de error 
		printf("test: cd x\n");
		argc_pth = ft_cdpath(msl);
		newpwd = getcwd(NULL, 0);
		if (!newpwd)
		{
			perror("getcwd");
			return ;
		}
		if (chdir(argc_pth) == -1)
		{
			perror("cd");
			ft_freeptr(newpwd);
			free(argc_pth);
			return ;
		}
		ft_edit_env(msl, "PWD", newpwd);
		ft_freeptr(newpwd);
		free(argc_pth);
	}
	ft_pwd(); // control
}

//casos de creacion del OLDPWD


// ////usage para crear nodos 
// t_env *env;

// env = list_new_ownenv(ft_strdup(id), ft_strdup(value));
// list_addback_env(t_env *node, &(msl->own_env));

// ///searh_id_node-> oara sacar nodos 

//casosode creacion de PWD
//si no existe PWD-> se deja OLDPWD vacio
//si existe se pone OLDPWD con el valor de PWD