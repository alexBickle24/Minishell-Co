/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_alex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:54:28 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/26 01:13:03 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


int	ft_argscounter(t_pcmds *pcmds)
{
	t_pcmds	*tmp;
	int i;

	i = 0;
	tmp = pcmds;
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

int	ft_cd(t_msl *msl, t_pcmds *pcmds)
{
	int		nbr_tockens;
	char	succes;
	char	*old_pwd;

	old_pwd = ft_strdup(msl->sys->pwd);
	nbr_tockens = ft_argscounter(pcmds);
	if (nbr_tockens > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (nbr_tockens == 1)
		succes = go_home(msl);
	else if (nbr_tockens == 2)
		succes = one_arg_cases(msl, pcmds);
	if (succes == 0)
		set_pwd_oldpwd(msl, old_pwd);
	else
		free(old_pwd);
	if (succes == 2)
		return (0);
	return(succes);
}

//la voy a hacer tipo char para poder hacerla bien 
int one_arg_cases(t_msl *msl, t_pcmds *pcmds)
{
	char	*path;

	path = pcmds->next->cmd;
	if (ft_strncmp(path, ".\0", 2) == 0)
		return(stay_case(path, msl));
	else if (ft_strncmp(path, "..\0", 3) == 0)
		return (parent_case(path, msl));
	else if (ft_strncmp(path, "-\0", 2) == 0)
		return (go_oldpwd(msl));
	else
	{
		if (chdir(path) == -1)
		{
			ft_cderrors(errno, path, 0);
			return (1);
		}
		else
		{
			free(msl->sys->pwd);
			msl->sys->pwd = getcwd(NULL, 0);
			return(0);
		}
	}
}

int	go_home(t_msl *msl)
{
	t_env	*home_node;

	home_node = search_id_node(msl, "HOME");
	if (home_node == NULL)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	else
	{
		if (home_node->value[0] == '\0')
			return (1);
		else
		{
			if (chdir(home_node->value) == -1)
			{
				ft_cderrors(errno, home_node->value, 0);
				return (1);
			}
			else
			{
				free(msl->sys->pwd);
				msl->sys->pwd = ft_strdup(home_node->value);
				return (0);
			}
		}
	}
}

int	go_oldpwd(t_msl *msl)
{
	t_env	*oldpwd;

	oldpwd = search_id_node(msl, "OLDPWD");
	if (oldpwd != NULL)
	{
		if (chdir(oldpwd->value) == -1)
		{
			ft_cderrors(errno, oldpwd->value, 0);
			return (1);
		}
		else
		{
			free(msl->sys->pwd);
			msl->sys->pwd = ft_strdup(oldpwd->value);
			ft_putendl_fd(msl->sys->pwd, 1);
			return (0);
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return(1);
	}
}

int	stay_case(char *path, t_msl *msl)
{
	char	*tmp;
	char	*cwd;
	
	cwd = getcwd(NULL, 0);
	if (cwd == NULL && is_broken_pwd(msl))
		return (1);
	if (cwd == NULL && errno == ENOENT)//directorio actual borado
	{
		ft_cderrors(errno, path, 1);
		tmp = ft_strjoin(msl->sys->pwd, "/");
		free(msl->sys->pwd);
		msl->sys->pwd = ft_strjoin(tmp, path);
		free(tmp);
		return(0);
	}
	else
	{
		if (chdir(path) == -1)
		{
			ft_cderrors(errno, path, 0);
			free(cwd);
			return (1);
		}
		else
		{
			free(msl->sys->pwd);
			msl->sys->pwd = cwd;
			return(0);
		}
	}
}

int	parent_case(char *path, t_msl *msl)
{
	char	*tmp;
	int flag;
	
	if (is_broken_pwd(msl))
	{
		if (ft_strlen(msl->sys->pwd) >= 2)
		{
			tmp = ft_strjoin(msl->sys->pwd, "/");
			free(msl->sys->pwd);
			msl->sys->pwd = ft_strjoin(tmp, path);
			free(tmp);
			return(0);
		}
		if (ft_strlen(msl->sys->pwd) == 1 && *(msl->sys->pwd) != '/')
		{
			free(msl->sys->pwd);
			msl->sys->pwd = ft_strdup("..");
			return (0);
		}
	}
	if (msl->sys->pwd[ft_strlen(msl->sys->pwd) - 1] == '.')
		return(search_first_parent(msl));
	flag = check_parent(msl);
	if (flag == 1)
	{
		unlink_message();
		tmp = ft_strjoin(msl->sys->pwd, "/");
		free(msl->sys->pwd);
		msl->sys->pwd = ft_strjoin(tmp, path);
		free(tmp);
		return(0);
	}
	else if (flag == 0)
	{
		if (chdir(path) == -1)
		{
			ft_cderrors(errno, path, 0);
			return (1);
		}
		else
		{
			free(msl->sys->pwd);
			msl->sys->pwd = getcwd(NULL, 0);
			return(0);
		}
	}
	else if (flag == 2)
		return (2);
	return (0);
}


int	search_first_parent(t_msl *msl)
{
	struct stat st;
	char	*tmp1;
	char	*pwd;
	
	pwd = ft_strdup(msl->sys->pwd);
	while (1)
	{
		tmp1 = ft_strrchr(pwd, '/');
		if (tmp1 != pwd)//directorio raiz
			*tmp1 = '\0';
		else if ((tmp1 == pwd))
		{
			if (chdir("/") == -1)
			{
				ft_cderrors(errno, "..", 0);
				free(pwd);
				return (1);
			}
			else
			{
				free(msl->sys->pwd);
				msl->sys->pwd = ft_strdup("/");
			}
			break ;
		}
		if (stat(pwd, &st) != -1)
		{
			if (chdir(pwd) == -1)
			{
				ft_cderrors(errno, "..", 0);
				free(pwd);
				return (1);
			}
			else
			{
				free(msl->sys->pwd);
				msl->sys->pwd = ft_strdup(pwd);
			}
			break ;
		}
	}
	free(pwd);
	return (0);
}

int	check_parent(t_msl *msl)
{
	char	*bar;
	struct stat st;

	bar = ft_strrchr(msl->sys->pwd, '/');
	if (bar == NULL)
		return (3);
	if (bar == msl->sys->pwd && bar[1] == 0)//para ver si estamos en el root
		return (2);
	else
	{
		if (bar != msl->sys->pwd)//si estamos en la capa del arbol que esta justo despues del root
			*bar = '\0';
		if (stat(msl->sys->pwd, &st) == -1)
		{
			*bar = '/';
			return (1);
		}
		else
		{
			*bar = '/';
			return (0);
		}
	}
}


void	set_olpwd(t_msl *msl, char *oldpath)
{
	t_env	*old_pwd;

	old_pwd = search_id_node(msl, "OLDPWD");
	if (!old_pwd)
	{
		if (msl->builts->unst_oldpwd == 0)
		{
			old_pwd = list_new_ownenv(ft_strdup("OLDPWD"), oldpath);
			if (!old_pwd)
			{
				free(oldpath);
				return ;
			}
			list_addback_env(old_pwd, &(msl->own_env));
		}
		else
			free(oldpath);
	}
	else
	{
		free(old_pwd->value);
		old_pwd->value = oldpath;
	}
}

void set_pwd_oldpwd(t_msl *msl, char *oldpath)
{
	t_env *pwd;
	t_env *old_pwd;

	printf("[DEBUG] Entering set_pwd_oldpwd\n");
	printf("[DEBUG] oldpath: %s\n", oldpath);

	if (!oldpath)
	{
		printf("[ERROR] oldpath is NULL\n");
		return;
	}

	if (!msl->sys->pwd)
	{
		printf("[ERROR] msl->sys->pwd is NULL\n");
		return;
	}

	pwd = search_id_node(msl, "PWD");
	if (!pwd)
	{
		free(oldpath);
		old_pwd = search_id_node(msl, "OLDPWD");
		if (old_pwd)
		{
			free(old_pwd->value);
			old_pwd->value = (char *)ft_calloc(sizeof(char), 1);
		}
		return;
	}
	else
	{
		free(pwd->value);
		pwd->value = ft_strdup(msl->sys->pwd);
		set_olpwd(msl, oldpath);
	}
	printf("[DEBUG] Exiting set_pwd_oldpwd\n");
}

void	unlink_message(void)
{
	ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2 );
	ft_putstr_fd("cannot access parent directories: No such file or directory\n", 2);
}

void	ft_cderrors(int value, char *path, char flag)
{
	if (value == ENOTDIR)
	{
		ft_putstr_fd("minishell: cd: ", 2),
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Not a directory\n", 2);
	}
	if (value == ENOENT)
	{
		if (flag)
			unlink_message();
		else
		{
			ft_putstr_fd("minishell: cd: ", 2),
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
	}
	if (value == EACCES)
	{
		ft_putstr_fd("minishell: cd: ", 2),
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permision denied\n", 2);
	}
}

int	is_broken_pwd(t_msl *msl)
{
	int	i;

	printf("%s\n", msl->sys->pwd);
	i = 0;
	while (msl->sys->pwd[i] != '\0')
	{
		if (msl->sys->pwd[i] != '.' && msl->sys->pwd[i] != '/')
			return (0);
		i++;
	}
	return (1);
}
