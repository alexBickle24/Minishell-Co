

#include "../../inc/minishell.h"

int	quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	int ret;

	if (line[*i] == '\'')
		ret = s_quotes(msl, i, line, pars);
	if (line[*i] == '\"')
		ret = d_quotes(msl, i, line, pars);
	return (ret);
}

int	info(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	if (pars->lexstat == NO_QUOTES)
	{
		if (*i == 0 || (*i > 0 && (pars->sep_op[line[*i - 1]] > 0 &&
			pars->sep_op[line[*i - 1]] < 5)))
		{
			pars->ptr = &line[*i];
		}
		if (line[*i + 1] == '\0' || (pars->sep_op[line[*i + 1]] > 0 &&
			pars->sep_op[line[*i + 1]] < 5))
		{
			create_new_lex(msl, i, line, pars);
			pars->parstat = T_CMD;
		}
	}
	pars->infstat = WORD;
	return((*i)++, 0);
}


int	pipe_op(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	if (pars->lexstat == NO_QUOTES)
	{	
		pars->parstat = T_PIPE;
		if (pars->infstat != WORD)
			return (get_unexpexted_errors1(pars->parstat, pars->infstat));
		else
		{
			pars->ptr = &line[*i];
			create_new_lex(msl, i, line, pars);
		}
		pars->infstat = OPERATOR;
		pars->parstat = T_CMD;
	}
	else
		pars->infstat = WORD;
	return((*i)++, 0);
}

int	redir_out(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	if (pars->lexstat == NO_QUOTES)
	{
		pars->parstat = T_OUTFILE;
		if (line[*i + 1] == '>')
		{
			pars->parstat = T_APPEND;
			(*i)++;
		}
		if (pars->infstat == REDIR)
			return (get_unexpexted_errors1(pars->parstat, pars->infstat));
		pars->infstat = REDIR;
	}
	else
		pars->infstat = WORD;
	return((void)msl, (*i)++, 0);
}

int	redir_in(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	if (pars->lexstat == NO_QUOTES)
	{
		pars->parstat = T_INFILE;
		if (line[*i + 1] == '<')
		{
			(*i)++;
			if (line[*i + 1] == '-')
			{
				pars->parstat = T_HEREDOC_S;
				(*i)++;
				if (pars->sep_op[line[*i + 1]] == 0 || pars->sep_op[line[*i + 1]] == 5)
					pars->ptr = &line[*i + 1];
			}
			else
				pars->parstat = T_HEREDOC;
		}
		if (pars->infstat == REDIR)
			return (get_unexpexted_errors1(pars->parstat, pars->infstat));
		pars->infstat = REDIR;
	}
	else
		pars->infstat = WORD;
	return((void)msl, (*i)++, 0);
}

