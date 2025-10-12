#include "minishell.h"

t_env	*ft_lstnew_env(char *id, char *value, int alloc)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(t_env));
	new->id = id;
	new->value = value;
	if (alloc)
	{
		new->id = ft_strdup(id);
		new->value = ft_strdup(value);
		if (!new->id || !new->value)
		{
			ft_freeptr(new->id);
			ft_freeptr(new->value);
			ft_freeptr(new);
			return (NULL);
		}
	}
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back_env(t_env **msl_env, t_env *new_env)
{
	t_env	*tmp_env;

	if (!*msl_env)
	{
		*msl_env = new_env;
		return ;
	}
	tmp_env = *msl_env;
	while (tmp_env->next)
		tmp_env = tmp_env->next;
	tmp_env->next = new_env;
}

t_env	*ft_sort_env(t_env *own_env)
{
	t_env	*tmp;
	t_env	*swap;

	tmp = own_env;
	swap = ft_lstnew_env("", "", 0);
	while (own_env->next != NULL)
	{
		if (own_env->next && ft_strcmp(own_env->id, own_env->next->id) > 0)
		{
			swap->id = own_env->id;
			swap->value = own_env->value;
			own_env->id = own_env->next->id;
			own_env->value = own_env->next->value;
			own_env->next->id = swap->id;
			own_env->next->value = swap->value;
			own_env = tmp;
		}
		else
			own_env = own_env->next;
	}
	own_env = tmp;
	ft_freeptr(swap);
	return (tmp);
}

void	ft_print_env(t_env *own_env)
{
	t_env	*tmp;

	tmp = own_env;
	while(tmp)
	{
		if (!ft_strcmp(tmp->id, "_"))
            ft_putendl_fd("\r", 1);
		else if (tmp->value[0])
		{
            ft_putstr_fd("declare -x ", 1);
            ft_putstr_fd(tmp->id, 1);
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(tmp->value, 1);
            ft_putendl_fd("\"", 1);
		}
		else
        {
            ft_putstr_fd("declare -x ", 1);
            ft_putendl_fd(tmp->id, 1);
        }
		tmp = tmp->next;
	}
}

/*
ft_putstr_fd(tmp->id, 1);
ft_putchar_fd('=', 1);
ft_putendl_fd(tmp->value, 1);
*/
int	ft_check_export(char *cmd)
{
	int	i;
	char	*id;

	i = 0;
	id = ft_get_env_id(cmd);
	if (ft_isdigit(id[i]))
	{
        ft_putstr_fd("export : ", 1);
        ft_putstr_fd(cmd, 1);
        ft_putendl_fd("not a valid identifier", 1);
		ft_freeptr(id);
		return (0);
	}
	while (id[i])
	{
		if (ft_isalnum(id[i]) || id[i] == '_')
			i++;
		else
		{
			printf("export: %s not a valid identifier\n", cmd); //--------------------- printf
			ft_freeptr(id);
			return (0);
		}
	}
	ft_freeptr(id);
	return (1);
}
char	*ft_get_one_env_value(char *env, char *id)
{
	int		i;
	int		j;
	char	*value;

	if (!env || !id)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		if (env[i] == '=')
			break;
	}
	value = malloc(sizeof(char) * (ft_strlen(env) - i + 1));
	if (!value)
		return (NULL);
	j = 0;
	while (env[++i])
		value[j++] = env[i];
	value[j] = '\0';
	return (value);
}

int	ft_check_id(t_msl *msl, char *id, char *value)
{
	t_env *tmp_env;

	tmp_env = msl->own_env;
	while (tmp_env)
	{
		if (!ft_strcmp(tmp_env->id, id))
		{
			ft_freeptr(tmp_env->id);
			tmp_env->id = ft_strdup(id);
			ft_freeptr(tmp_env->value);
			tmp_env->value = ft_strdup(value);
			return (1);
		}
		tmp_env = tmp_env->next;
	}
	return (0);
}

void	ft_add_env(t_msl *msl, char *cmd)
{
	char	*id;
	char	*value;
	t_env	*new_env;

	if (!ft_strrchr(cmd, '='))
		return ;
	id = ft_get_env_id(cmd);
	value = ft_get_one_env_value(cmd, id);
	if (!ft_check_id(msl, id, value))
	{
		new_env = ft_lstnew_env(id, value, 1);
		ft_lstadd_back_env(&msl->own_env, new_env);
		ft_freeptr(id);
		ft_freeptr(value);
		return ;
	}
	ft_freeptr(id);
	ft_freeptr(value);
	return ;
}


void	ft_export(t_msl *msl)
{
	t_pcmds	*tmp;

	if (ft_tokencounter(msl) == 1 && !ft_strcmp(msl->tocken->pcmds->cmd, "export"))
	{
		ft_print_env(ft_sort_env(msl->own_env));
	}
	else
	{
		tmp = msl->tocken->pcmds->next;
		while (tmp)
		{
			if (ft_check_export(tmp->cmd))
				ft_add_env(msl, tmp->cmd);
			else
				return ;
			tmp = tmp->next;
		}
	}
}