

#include "../inc/minishell.h"
/*
 * env: Es lo mismo que extern char **environ
  *         env[i] = "PATH=/usr/bin" (ejemplo)
  *         env[N] == NULL
 */

int g_signal = S_INIT;

void	evaluate_line(t_msl *msl, unsigned char *clean_line);
void	interactive_mode(t_msl *msl);

int main(int argc, char **argv, char **env)
{
	t_msl *msl;
	int		interpreter_exit;

    if (argc > 3)
		return (ft_errormini(), 1);
	minishell_init(&msl, env);
	if (argc == 1)
		interactive_mode(msl);
	else if (argc == 3)
	{
		if (!ft_strncmp(argv[1], "-c\0", 3))
		{
			msl->mode = 1;
			evaluate_line(msl, (unsigned char *)argv[2]);
			interpreter_exit = msl->exit_status;
			return (free_msl(&msl), interpreter_exit);

		}
		else
		{
			ft_errorargs(argv[1]);
			return (free_msl(&msl), 2);
		}
	}
	return (free_msl(&msl), 0);
}

void evaluate_line(t_msl *msl, unsigned char *clean_line)
{
	if (g_signal == S_SIGINT)
	{
		msl->exit_status = SIGINT + 128;
		g_signal = S_INIT;
	}
	lexer_parser(msl, clean_line);
	// print_lex(msl->lexer, parser);//para ver el lexer
	// free_lexer(msl, 1);//liberar el lexer
	clean_expand_add_toexecuter(msl);//siq uitas este tienes que meter un free_lexer(msl, 1)
	// print_tockens(msl);//para ver los tockens
	executer(msl);//si quitas este tienes que meter el free_tockens(msl)
	// free_tockens(msl);//Para liberar los tockens cunado no tengo executer
}

void	interactive_mode(t_msl *msl)
{
	char *line;//la linea en bruto

	msl->mode = 0;
	while (1)
	{
		set_ps1(msl, msl->sys);
		line = readline(msl->sys->ps1);//leo la linea
		add_history(line);//exit tambien se mete al historial
		msl->clean_line = ft_strtrim(line, " \t\n\v\f\r");//por el mod literal de bash con control+V
		if (!msl->clean_line)
		{
			ft_putstr_fd("exit\n", 2);
			free(msl->clean_line);
			msl->clean_line = NULL;
			free(line);
			break ;
		}
		evaluate_line(msl, (unsigned char *) msl->clean_line);
		free(line);
		free(msl->clean_line);
		msl->clean_line = NULL;
	}
}
