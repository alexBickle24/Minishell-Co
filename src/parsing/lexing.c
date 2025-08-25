
// void	set_parsdefaultvals(t_msl *msl);
// void	free_lexer(t_msl *msl, char all);
// void	lexer(t_msl *msl, char *line);
// void	addback_lex(t_msl *msl, t_lex *node);
// t_lex	*lex_newnode(int type, char *raw);
// void	ft_error_unexpect(char *simbol);
// int	get_unexpexted_errors1(int type, int infstat);
// int	redir_out(t_msl *msl, int *i, char *line, t_parsing *pars);
// int	redir_in(t_msl *msl, int *i, char *line, t_parsing *pars);
// int	pipe_op(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
// int	spaces(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
// int	info(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
// int	d_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
// int	s_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);



#include "../../inc/minishell.h"

// int g_signal = S_INIT;

int	s_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	char tmp;

	pars->infstat = WORD;
	if (pars->lexstat == NO_QUOTES && pars->lexstat != D_QUOTES)
	{
		if (*i == 0 || (*i > 0 && (pars->sep_op[line[*i - 1]] > 0 &&
			pars->sep_op[line[*i - 1]] < 5)))
		{
			pars->ptr = &line[*i];
		}
		pars->lexstat = S_QUOTES;
	}
	else if (pars->lexstat == S_QUOTES)
	{
		if (line[*i + 1] == '\0' || (pars->sep_op[line[*i + 1]] > 0 &&
			pars->sep_op[line[*i + 1]] < 5))
		{
			tmp = line[*i +1];
			line[*i +1] = '\0';
			addback_lex(msl, lex_newnode(pars->parstat, ft_strdup((const char *)pars->ptr)));
			line[*i+ 1] = tmp;
		}
		pars->lexstat = NO_QUOTES;
	}
	return((*i)++, 0);
}

int	d_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	char tmp;

	pars->infstat = WORD;
	if (pars->lexstat == NO_QUOTES && pars->lexstat != S_QUOTES)
	{
		if (*i == 0 || (*i > 0 && (pars->sep_op[line[*i - 1]] > 0 &&
			pars->sep_op[line[*i - 1]] < 5)))
		{
			pars->ptr = &line[*i];
		}
		pars->lexstat = D_QUOTES;
	}
	else if (pars->lexstat == D_QUOTES)
	{
		if (line[*i + 1] == '\0' || (pars->sep_op[line[*i + 1]] > 0 &&
			pars->sep_op[line[*i + 1]] < 5))
		{
			tmp = line[*i + 1];
			line[*i + 1] = '\0';
			addback_lex(msl, lex_newnode(pars->parstat, ft_strdup((const char *)pars->ptr)));
			line[*i + 1] = tmp;
		}
		pars->lexstat = NO_QUOTES;
	}
	return((*i)++, 0);
}

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
	char tmp;

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
			tmp = line[*i + 1];
			line[*i + 1] = '\0';
			addback_lex(msl, lex_newnode(pars->parstat, ft_strdup((const char *)pars->ptr)));
			line[*i + 1] = tmp;
		}
	}
	pars->infstat = WORD;
	return((*i)++, 0);
}

int	spaces(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	(void)msl;
	(void)line;
	(void)pars;
	return((*i)++, 0);
}

int	pipe_op(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	char tmp;

	if (pars->lexstat == NO_QUOTES)
	{	
		pars->parstat = T_PIPE;
		if (pars->infstat != WORD)
		{
			get_unexpexted_errors1(pars->parstat, pars->infstat);
			return (1);
		}
		else
		{
			pars->ptr = &line[*i];
			tmp = line[*i+1];
			line[*i+1] = '\0';
			addback_lex(msl, lex_newnode(T_PIPE, ft_strdup((const char *)pars->ptr)));
			line[*i+1] = tmp;
		}
		pars->infstat = OPERATOR;
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
		{
			get_unexpexted_errors1(pars->parstat, pars->infstat);
			return (1);
		}
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
		{
			get_unexpexted_errors1(pars->parstat, pars->infstat);
			return (1);
		}
		pars->infstat = REDIR;
	}
	else
		pars->infstat = WORD;
	return((void)msl, (*i)++, 0);
}


int	get_unexpexted_errors1(int type, int infstat)
{
	if ((infstat ==  INIT || infstat == REDIR) && type == T_PIPE)
		ft_error_unexpect("|");
	else if (infstat == OPERATOR || infstat == REDIR)
	{
		if (type == T_HEREDOC)
			ft_error_unexpect("<<");
		if (type == T_HEREDOC_S)
			ft_error_unexpect("<<-");
		if (type == T_INFILE)
			ft_error_unexpect("<");
		if (type == T_OUTFILE)
			ft_error_unexpect(">");
		if (type == T_APPEND)
			ft_error_unexpect(">>");
		if (type == T_PIPE)
			ft_error_unexpect("|");
	}
	return (1);
}

void	ft_error_unexpect(char *simbol)
{
	ft_putstr_fd("minishell: syntax error near unexpected token", 2);
	ft_putstr_fd(" `", 2);
	ft_putstr_fd(simbol, 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd("\n", 2);
}


t_lex	*lex_newnode(int type, char *raw)
{
	t_lex *node;

	node = (t_lex *)malloc(sizeof(t_lex));
	if (node == NULL)
		return (NULL);
	node->str = raw;
	node->len = ft_strlen(raw);
	node->type = type;
	node->next = NULL;
	return (node);
}

void	addback_lex(t_msl *msl, t_lex *node)
{
	t_lex *current;

	current = msl->lexer;
	if (current == NULL)
	{
		msl->lexer = node;
		return ;
	}
	while(current->next)
		current = current->next;
	current->next = node;
}

void	lexer(t_msl *msl, char *line)
{
	t_parsing *parser;
	unsigned char *line_ptr;
	int i;
	int err;

	i = 0;
	err = 0;
	line_ptr = (unsigned char *)line;
	parser = msl->parsing_utils;
	while (line[i])
	{
		err = parser->lex[(unsigned char)parser->sep_op[line_ptr[i]]](msl, &i, line_ptr, parser);
		// print_parser_state(parser, line[i], i);
		if (err)
			break;
	}
	print_lex(msl->lexer, parser);
	if (err)//proabaer esto
	{
		msl->pars_err = 1;
		free_lexer(msl, 1);
	}
	else
	{
		if (parser->infstat == REDIR)
		{
			ft_putstr_fd(NEWLINE_ERR, 2);
			free_lexer(msl, 1);
		}
		else if(parser->infstat == OPERATOR)
		{
			ft_putstr_fd(MIPIPE_ERR, 2);
			free_lexer(msl, 1);
		}
		else if (parser->lexstat != NO_QUOTES)
		{
			ft_putstr_fd(MIQUOTE_ERR, 2);
			free_lexer(msl, 1);
		}
	}
	free_lexer(msl, 1);//esto es solo para el tester
}

void	free_lexer(t_msl *msl, char all)
{
	t_lex *lexer;
	t_lex *tmp;

	lexer = msl->lexer;
	while (lexer)
	{
		if (all == 1)
			free (lexer->str);
		tmp = lexer->next;
		free(lexer);
		lexer = tmp;
	}
	msl->lexer = NULL;
	set_parsdefaultvals(msl);
}

void	set_parsdefaultvals(t_msl *msl)
{
	msl->parsing_utils->infstat = 0;
	msl->parsing_utils->lexstat = 0;
	msl->parsing_utils->parstat = 0;
	msl->parsing_utils->ptr = 0;
}

// int main(int argc, char **argv, char **env)
// {
// 	t_msl *msl;
// 	char *line;//la linea en bruto

// 	(void)argv;//para que no se queje el compilador
//     if (argc != 1)
// 		return (1);//si no hacemos el modo literal
//     minishell_init(&msl, env, argv);//inicamos la estrcutura de minishell y el manejados
// 	while (1)
// 	{
// 		line = readline(PROMPT);
// 		add_history(line);//exit tambien se mete al historial
// 		msl->clean_line = ft_strtrim(line, " \t\n\v\f\r");//por el mod literal de bash con control+V
// 		if (!msl->clean_line || ft_strncmp(msl->clean_line, "exit\0", 5) == 0)
// 		{
// 			ft_putstr_fd("exit\n", 2);
// 			free(msl->clean_line);
// 			msl->clean_line = NULL;//Esta linea es solo para poder printear los valores de la minishell
// 			free(line);
// 			break ;
// 		}
// 		// meter la linea en minishell (lexerizaciion y parseo)
// 		if (!line || !msl)
// 			printf("hola\n");
// 		lexer(msl, line);
// 		free(line);
// 		free(msl->clean_line);
// 		msl->clean_line = NULL;//Esta linea es solo para poder printear los valores de la minishell
// 		// printf("\n");
// 		// print_msl(msl);
// 	}
// 	free_msl(&msl);
// 	return (0);
// }