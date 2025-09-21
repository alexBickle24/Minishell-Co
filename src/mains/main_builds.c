

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

/*
	Para consultar el nuemro de argumentos dentro de cada funcion,
	dentro de cada nodo cmds estara la primera. por defecto el primero sera
	el nombre del building. Es decir cuando escribo por la minishell:

	prompt: cd /etc/pssword
	La lista de cmds se llenara despues del parseo con ;
	<cmds->file = cd> NEXT <cmds->file = /etc/pssword> NEXT <cmds == NULL>

	Ejemplo de env: NO ADMITE ARGUMENTOS
	int	ft_env(t_msl *msl, t_pcmds *cmds)
	{
		t_pmcds *current;
		int control;

		control = 0;
		current = cmds;
		while (current != NULL)
		{
			if (control => 1)
			{
				//imprimir mensaje de error
				//return (1); //calor de error
			}
			control++;
			current = current->next; //para ir a siguiente nodod
		}
	}

	Ejemplo de cd: ADMITE ARGUMNETOS PERO SOLO UNO
	int	ft_cd(t_msl *msl, t_pcmds *cmds)
	{
		t_pmcds *current;
		char	*ruta;
		int control;
		
		control = 0;
		ruta = NULL;
		current = cmds;
		while (current != NULL)
		{
			if (control = 1)
				ruta = current->file;
			if (control > 1)
			{
				//imprimir mensaje de error
				//return (1); //calor de error
			}
			control++;
			current = current->next; //para ir a siguiente nodod
		}
		//aqui ya podrias trabajar con el valor de la ruta: si ruta == NULL (cd sin argumentos), si ruta != NULL (ese es el valor de la ruta)
	}

	Ejemplo de export: ADMITE X ARGUMENTOS
	int	ft_export(t_msl *msl, t_pcmds *cmds)
	{
		t_pmcds *current;
		char	*ruta;
		int control;
		
		control = 0;
		ruta = NULL;
		current = cmds;
		while (current != NULL)
		{
			if (control = 0 && current->next = NULL)
			{
				//funcoin de imprimir lista de env de manera ordenada 
				//return ;
			}
			if (control > 1)
			{
				ft_funcion_add_env(current->file);
			}
			control++;
			current = current->next; //para ir a siguiente nodod
		}
		return (0);
	}

	EN AMBOS CASOS CUANDO ESTAS EN CONTROL = 0, ESTAS EN EL PRIMER NODOD DE LA LISTA PCMDS
	QUE ES EL QUE CONTITNE EL NOMBRE DEL PROPIO BUILTING, NO LONECESITAS, PORQUE ENTRAR O NO ENTRAR
	EN LA FUNCION DEL BUILTING DEPENDE DE LA EJECUCION.
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


/*
 * ft_printexport() 
 * La diferencia con env es que export imprime las variables no declaradas y ordenada
 * Falta ordenarlo!!!!!!!!!!
 */
// void	ft_printexport(t_msl *msl)
// {
// 	t_env	*tmp;

// 	printf("Start ft_printexport\n");
// 	tmp = msl->own_env;
// 	while(tmp)
// 	{
// 		printf("id: %s -> ", tmp->id);
// 		printf("value: %s\n", tmp->value);
// 		tmp = tmp->next;
// 	}
// }

// /*
//  * Chequea si el argumento es valido para export
// */
// int	ft_checkexport(char *str)
// {
// 	char	*id;
// 	int	i;

// 	printf("Start ft_checkexport!\n");
// 	// extraemos el nombre antes del '='
// 	id = ft_get_env_id(str);
// 	i = 0;
// 	if (ft_isdigit(id[i]))
// 	{
// 		free(id);
// 		return (0);
// 	}
// 	while (id[i])
// 	{
// 		if (!ft_isalnum(id[i]) && id[i] != '_')
// 		{
// 			free(id);
// 			return (0);
// 		}
// 		i++;
// 	}
// 	printf("Ok\n");
// 	free(id);
// 	return (1);
// }

// /*
//  * 
//  */
// void	ft_addexport(t_msl *msl, char *str)
// {
// 	char	*id;
// 	char	*value;

// 	if (!ft_strrchr(str, '='))
// 		return ;
// 	id = ft_get_env_id(str);
// 	//value = ft_get_env_value(str);      ------> next
// 	free(id);
// 	free(value);
// }

// /*
//  * Exportar variables de entorno, si no hay variables las mostramos
//  */
// void	ft_export(t_msl *msl)
// {
// 	t_pcmds	*tmp;

// 	printf("Strat ft_export\n");
// 	printf("counter token: %d\n", ft_tokencounter(msl));
// 	if (ft_tokencounter(msl) == 1)
// 		ft_printexport(msl);
// 	else
// 	{
// 		tmp = msl->tocken->pcmds;
// 		tmp = tmp->next;
// 		while(tmp)
// 		{
// 			ft_checkexport(tmp->cmd);
// 			printf("add tocken: %s\n", tmp->cmd);
// 			tmp = tmp->next;
// 		}
// 	}
// }

/*
	El prompt que puedes usar es el siguiente;
	<nombre del building> <argumento> <arguemnto1> .... 
*/

void	ft_echo(t_msl *msl)
{
	t_pcmds	*tmp;

	printf("Start echo! . . .\n");
	tmp = msl->tocken->pcmds;
	if (ft_tokencounter(msl) == 1)
	{
		printf("\n");
		return ;
	}
	printf("tocken -> %d", ft_tokencounter(msl));
	// if (!tmp->cmd)
	// {
	// 	printf("Vacio\n");
	// }
	// while(tmp)
	// {
	// 	printf("%s\n", tmp->cmd);
	// 	tmp = tmp->next;
	// }
}

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

	//ft_unset(msl);
	ft_echo(msl);

	free_tockens(msl);//Para liberar los tockens cunado no tengo executer
}