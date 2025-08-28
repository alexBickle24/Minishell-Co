

#include "../../inc/minishell.h"

int have_quotes(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

char *clean_quotes(char *str)
{
	char *tmp;
	int lensrc;
	int i;

	tmp = str;
	if (!tmp)
		return (NULL);
	lensrc = (int)ft_strlen(str);
	i = 0;
	while(tmp[i])
	{
		if (tmp[i] == '\'' || tmp[i] == '\"')
		{
			ft_memmove(&tmp[i], &tmp[i + 1], lensrc - i);
			lensrc--;
		}
		i++;
	}
	return(str);
}

char	*jump_caracter(char *str, char caracter)
{
	while (str && *str && *str == caracter)
		str++;
	return(str);
}

void	jump_separator(char **str)
{
	while (str && *str && is_space(**str))
		(*str)++;
}

int is_space(char c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	return (0);
}