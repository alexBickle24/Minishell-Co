

#include "../inc/minishell.h"
/*
 * env: Es lo mismo que extern char **environ
  *         env[i] = "PATH=/usr/bin" (ejemplo)
  *         env[N] == NULL
 */

int g_signal = S_INIT;
void	interpreter_mode_builds(t_msl *msl, unsigned char *clean_line);

/*
	El prompt que puedes usar es el siguiente;
	<nombre del building> <argumento> <arguemnto1> .... 
*/
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
			msl->clean_line = NULL;
			free(line);
			break ;
		}
		interpreter_mode_builds(msl, (unsigned char *)msl->clean_line);
		free(line);
		free(msl->clean_line);
		msl->clean_line = NULL;
	}
	free_msl(&msl);
	return (0);
}


/*
	El prompt que puedes usar es el siguiente;
	<nombre del building> <argumento> <arguemnto1> .... 
*/

void	interpreter_mode_builds(t_msl *msl, unsigned char *clean_line)
{
	//a esto no le hagas caso
	if (g_signal == S_SIGINT)
	{
		msl->exit_status = SIGINT + 128;
		g_signal = S_INIT;
	}

	//estas dos funciones van a parsear tu linea->la que escribes por readline:
	lexer_parser(msl, clean_line);
	clean_expand_add_toexecuter(msl);

	print_tockens(msl);//para ver los tockens (la puedes comentar)

	//aqui iria tu funcion building (ej): ft_echo(msl, msl->tocken->pcmds)
	//msl:la cabecera (contiene el env y la variable sys)
	//tocken->pcmds; los argumentos es decir lo que vas a parsear. Ej: linea-> echo palabra1 palabra2 palabra3

	free_tockens(msl);//Para liberar los tockens cunado no tengo executer
}