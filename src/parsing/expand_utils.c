

#include "../../inc/minishell.h"

//No hace falta proteger los itoa porque loa malloc no fallan nunca 
//ademas los valores que sustituyen estan inicilaizados al principio
void	concatenate_strings(char **str, int *i, size_t *len, char *replace)//FUCNIONA
{
	size_t	replace_len;
	char	*tmp;
	char	*tmp_after;
	char	*new;

	replace_len = ft_strlen(replace);
	(*str)[*i] = '\0';
	tmp = ft_strjoin(*str, replace);
	tmp_after = &((*str)[*i + 2]);
	new = ft_strjoin(tmp, tmp_after);
	free(replace);
	free(tmp);
	free(*str);
	*str = new;
	*len = *len + replace_len;
	*i = *i + replace_len;
}

void	concatenate_strings2(char **str, int *i, size_t *len, char *replace)
{
	size_t	replace_len;
	char	*tmp;
	char	*new;

	replace_len = ft_strlen(replace);
	tmp = ft_strjoin(*str, replace);
	new = ft_strjoin(tmp, &(*str)[(*i)]);
	*i = ft_strlen(new) - ft_strlen(&(*str)[(*i)]);
	free(tmp);
	free(*str);
	*str = new;
	*len = *len + replace_len;
}