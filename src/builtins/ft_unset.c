#include "../inc/minishell.h"

void	ft_delete_env(t_msl *msl, char *id)
{
	t_env	*tmp;

	tmp = msl->own_env;
	while (tmp)
	{
		if (ft_strncmp(tmp->id, id, ft_strlen(id)) == 0)
		{
			ft_freeptr(tmp->value);
			ft_freeptr(tmp->id);
			ft_freeptr(tmp->next);
			tmp->next = tmp->next->next;
			// Delete
			return ;
		}
		tmp = tmp->next;
	}
}
/*
 *	Unset: Eliminamos variables de entorno
*/
void	ft_unset(t_msl *msl)
{
	char	*varname;

	printf("Start unset . . . \n");
	// numero de argumentos
	if (ft_tokencounter(msl) != 2)
	{
		perror("Error de argumentos");
		return ;
	}
	varname = ft_cdpath(msl);
	printf("%s\n", varname);
	ft_delete_env(msl, varname);
	free(varname);
	return ;
}