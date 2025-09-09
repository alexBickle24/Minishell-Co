

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
    minishell_init(&msl, env);//inicamos la estrcutura de minishell y el manejados
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
	Check Builtins:
		x pwd
		s env
		o cd
		o echo
		o exit
		o export
		o unset
*/
// void	ft_freeptr(void *ptr)
// {
// 	if (!ptr)
// 		return ;
// 	free(ptr);
// 	ptr = NULL;
// }
void	ft_edit_env(t_msl *msl, char *id, char *value)
{
	t_env	*tmp;

	tmp = msl->own_env;
	while (tmp)
	{
		if (ft_strncmp(tmp->id, id, ft_strlen(id)) == 0)
		{
			ft_freeptr(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
}

/*
	Cambiar de directorio
	Podemos recibir una ruta o cambiar a HOME 
*/
void	ft_cd(t_msl *msl)
{
	char	*oldpwd;
	char	*home;
	char	*newpwd;

	printf("_______________________\n\nTest ft_cd:\n");
	ft_pwd(); // control
	// Si tenemos mas de 2 argumentos fuera
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd");
		return ;
	}
	ft_edit_env(msl, "OLDPWD", oldpwd);
	ft_freeptr(oldpwd);
	// estudiar argumentos 1 o 0
	home = getenv("HOME");
	if (!home)
	{
		perror("home");
		return ;
	}
	if (chdir(home) == -1)
	{
		perror("cd");
		return ;
	}
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("getcwd");
		return ;
	}
	ft_edit_env(msl, "PWD", newpwd);
	ft_freeptr(newpwd);
	ft_pwd(); // control
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

	ft_cd(msl);

	free_tockens(msl);//Para liberar los tockens cunado no tengo executer
}