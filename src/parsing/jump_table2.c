

#include "../../inc/minishell.h"

int	s_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	pars->infstat = WORD;
	if (pars->lexstat == NO_QUOTES)
	{
		pars->lexstat = S_QUOTES;
		if (*i == 0 || (*i > 0 && (pars->sep_op[line[*i - 1]] > 0 &&
			pars->sep_op[line[*i - 1]] < 5)))
		{
			pars->ptr = &line[*i];
		}
	}
	else if (pars->lexstat == S_QUOTES)
	{
		pars->lexstat = NO_QUOTES;
		if (line[*i + 1] == '\0' || (pars->sep_op[line[*i + 1]] > 0 &&
			pars->sep_op[line[*i + 1]] < 5))
		{
			create_new_lex(msl, i, line, pars);
			pars->parstat = T_CMD;
		}
	}
	return((*i)++, 0);
}

int	d_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	pars->infstat = WORD;
	if (pars->lexstat == NO_QUOTES)
	{
		pars->lexstat = D_QUOTES;
		if (*i == 0 || (*i > 0 && (pars->sep_op[line[*i - 1]] > 0 &&
			pars->sep_op[line[*i - 1]] < 5)))
		{
			pars->ptr = &line[*i];
		}
	}
	else if (pars->lexstat == D_QUOTES)
	{
		pars->lexstat = NO_QUOTES;
		if (line[*i + 1] == '\0' || (pars->sep_op[line[*i + 1]] > 0 &&
			pars->sep_op[line[*i + 1]] < 5))
		{
			create_new_lex(msl, i, line, pars);
			pars->parstat = T_CMD;
		}
	}
	return((*i)++, 0);
}

int	spaces(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	(void)msl;
	(void)line;
	(void)pars;
	return((*i)++, 0);
}

void	create_new_lex( t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	char	tmp;
	t_lex	*new_node;

	tmp = line[*i + 1];
	line[*i + 1] = '\0';
	new_node = lex_newnode(pars->parstat, ft_strdup((const char *)pars->ptr));
	addback_lex(msl, new_node);
	line[*i + 1] = tmp;
}
