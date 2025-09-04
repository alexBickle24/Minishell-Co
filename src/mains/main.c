


#include "../inc/minishell.h"
/*
 * env: Es lo mismo que extern char **environ
  *         env[i] = "PATH=/usr/bin" (ejemplo)
  *         env[N] == NULL
 */

int g_signal = S_INIT;

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
		if (g_signal == S_SIGINT)//esto puede ir en interpreter
		{
			msl->exit_status = SIGINT + 128;
			g_signal = S_INIT;
		}
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
	if (g_signal == S_SIGINT)
	{
		msl->exit_status = SIGINT + 128;
		g_signal = S_INIT;
	}
	lexer_parser(msl, clean_line);
	// print_lex(msl->lexer, parser);//para ver el lexer
	clean_expand_add_toexecuter(msl);//siq uitas este tienes que meter un free_lexer(msl, 1)
	// free_lexer(msl, 1);//liberar el lexer
	// print_tockens(msl);//para ver los tockens
	executer(msl);
	// free_tockens(msl);//Para liberar los tockens cunado no tengo executer
}



