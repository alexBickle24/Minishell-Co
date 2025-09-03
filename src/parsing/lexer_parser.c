

#include "../../inc/minishell.h"

int g_signal = S_INIT;

void	lexer_parser(t_msl *msl, unsigned char *line)
{
	t_parsing *parser;
	int i;
	int err;

	if (g_signal != S_INIT)
		return ;
	i = 0;
	err = 0;
	parser = msl->parsing_utils;
	while (line[i])
	{
		err = parser->lex[parser->sep_op[line[i]]](msl, &i, line, parser);
		if (err)
			break;
	}
	// print_lex(msl->lexer, parser);
	if (err)
		free_lexer(msl, 1);
	else
		manage_last_state(msl, parser);
	set_parsdefaultvals(msl);
}

void	manage_last_state(t_msl *msl, t_parsing *parser)
{
	if (parser->infstat == REDIR)
			msl->pars_err = 1;
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


int main(int argc, char **argv, char **env)
{
	t_msl *msl;
	char *line;//la linea en bruto

	(void)argv;//para que no se queje el compilador
    if (argc != 1)
		return (1);//si no hacemos el modo literal
    minishell_init(&msl, env, argv);//inicamos la estrcutura de minishell y el manejados
	while (1)
	{
		line = readline(PROMPT);//leo la linea
		add_history(line);//exit tambien se mete al historial
		msl->clean_line = ft_strtrim(line, " \t\n\v\f\r");//por el mod literal de bash con control+V
		if (!msl->clean_line || ft_strncmp(msl->clean_line, "exit\0", 5) == 0)
		{
			ft_putstr_fd("exit\n", 2);
			free(msl->clean_line);
			msl->clean_line = NULL;//Esta linea es solo para poder printear los valores de la minishell
			free(line);
			break ;
		}
		// meter la linea en minishell (lexerizaciion y parseo)
		if (!line || !msl)
			printf("hola\n");
		// lexer(msl, line);
		// clean_expand_add_toexecuter(msl);
		interpreter_mode2(msl, (unsigned char *) msl->clean_line);
		free(line);
		free(msl->clean_line);
		msl->clean_line = NULL;//Esta linea es solo para poder printear los valores de la minishell
		// printf("\n");
		// print_msl(msl);
	}
	free_msl(&msl);
	return (0);
}

void interpreter_mode2(t_msl *msl, unsigned char *clean_line)
{
	// g_signal = S_INIT;
	lexer_parser(msl, clean_line);
	clean_expand_add_toexecuter(msl);//siq uitas este tienes que meter un free_lexer(msl, 1)
	// free_lexer(msl, 1);
	// print_tockens(msl);
	// sleep(4);
	executer(msl);
	// free_tockens(msl);//Cuanndo no tengo el ejecutor
	// executer(msl);
	// g_signal = S_INIT;
}
