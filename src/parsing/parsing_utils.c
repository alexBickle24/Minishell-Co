#include "../../inc/minishell.h"
#include <stdio.h> // Añadido para printf

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


char	check_clean_quotes(t_msl *msl, char *str, char clean)
{
	char jump;

	jump = 1;
	if (*str == '\'')
		jump = check_clean_squotes(msl, str, clean);
	if (*str == '\"')
		jump = check_clean_dquotes(msl, str, clean);
	return (jump);
}

char	check_clean_squotes(t_msl *msl, char *str, char clean)
{
	t_parsing *pars;

	pars = msl->parsing_utils;
	if (pars->lexstat == NO_QUOTES)
	{
		pars->lexstat = S_QUOTES;
		if (clean)
			ft_memmove(str, str + 1, ft_strlen(str));
		return(0);
	}
	else if (pars->lexstat == S_QUOTES)
	{
		pars->lexstat = NO_QUOTES;
		if (clean)
			ft_memmove(str, str + 1, ft_strlen(str));
		return(0);
	}
	return(1);
}

char	check_clean_dquotes(t_msl *msl, char *str, char clean)
{
	t_parsing *pars;

	pars = msl->parsing_utils;
	if (pars->lexstat == NO_QUOTES)
	{
		pars->lexstat = D_QUOTES;
		if (clean)
			ft_memmove(str, str + 1, ft_strlen(str));
		return(0);
	}
	else if (pars->lexstat == D_QUOTES)
	{
		pars->lexstat = NO_QUOTES;
		if (clean)
			ft_memmove(str, str + 1, ft_strlen(str));
		return(0);
	}
	return(1);

}