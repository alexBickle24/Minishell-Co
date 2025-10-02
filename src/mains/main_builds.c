

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


	// printf("tocken -> %d\n", ft_tokencounter(msl));
	// if (!tmp->cmd)
	// {
	// 	printf("Vacio\n");
	// }
	// while(tmp)
	// {
	// 	printf("%s\n", tmp->cmd);
	// 	tmp = tmp->next;
	// }

t_env	*ft_lstnew_env(char *id, char *value, int alloc)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(t_env));
	new->id = id;
	new->value = value;
	if (alloc)
	{
		new->id = ft_strdup(id);
		new->value = ft_strdup(value);
		if (!new->id || !new->value)
		{
			ft_freeptr(new->id);
			ft_freeptr(new->value);
			ft_freeptr(new);
			return (NULL);
		}
	}
	new->next = NULL;
	return (new);
}

t_env	*ft_sort_env(t_env *own_env)
{
	t_env	*tmp;
	t_env	*swap;

	tmp = own_env;
	swap = ft_lstnew_env("", "", 0);
	while (own_env->next != NULL)
	{
		if (own_env->next && ft_strcmp(own_env->id, own_env->next->id) > 0)
		{
			swap->id = own_env->id;
			swap->value = own_env->value;
			own_env->id = own_env->next->id;
			own_env->value = own_env->next->value;
			own_env->next->id = swap->id;
			own_env->next->value = swap->value;
			own_env = tmp;
		}
		else
			own_env = own_env->next;
	}
	own_env = tmp;
	ft_freeptr(swap);
	return (tmp);
}

// Dudas con tmp->value[0]
void	ft_print_env(t_env *own_env)
{
	t_env	*tmp;

	tmp = own_env;
	while(tmp)
	{
		if (!ft_strcmp(tmp->id, "_"))
			printf("\r");
		else if (tmp->value[0])
			printf("declare -x %s\n", tmp->id);
		else
		{
			printf("declare -x %s", tmp->id);
			printf("=\"%s\n\"", tmp->value);
		}
		tmp = tmp->next;
	}
}

int	ft_check_export(char *cmd)
{
	int	i;
	char	*id;

	i = 0;
	id = ft_get_env_id(cmd);
	if (ft_isdigit(id[i]))
	{
		printf("export: %s not a valid identifier\n", cmd);
		ft_freeptr(id);
		return (0);
	}
	while (id[i])
	{
		if (ft_isalnum(id[i]) || id[i] == '_')
			i++;
		else
		{
			printf("export: %s not a valid identifier\n", cmd);
			ft_freeptr(id);
			return (0);
		}
	}
	ft_freeptr(id);
	return (1);
}
void	*ft_get_one_env_value(char *env, char *id)
{
	int		i;
	int		j;
// 	int		k;
	char	*value;

	if (!env || !id)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		printf("check -> %c\n",env[i]);
		if (env[i] == '=')
			break;
	}
	value = malloc(sizeof(char) * (ft_strlen(env) - i + 1));
	if (!value)
		return (NULL);
	j = 0;
	while (env[++i])
	{
		value[j++] = env[i];
	}
	printf("%d\n", i);
	printf("%ld\n", ft_strlen(env));
// 	while (env[++i])
// 	{
// 		if (ft_strncmp(env[i], id, ft_strlen_c(id)) == 0
// 			&& env[i][ft_strlen_c(id)] == '=')
// 		{
// 			j = ft_strlen_c(id) + 1;
// 			value = malloc(sizeof(char) * (ft_strlen(env[i]) - j + 1));
// 			if (!value)
// 				return (NULL);
// 			k = 0;
// 			while (env[i][j])
// 				value[k++] = env[i][j++];
// 			value[k] = '\0';
// 			return (value);
// 		}
// 	}
	return (NULL);
}

void	ft_add_env(t_msl *msl, char *cmd)
{
	//t_env	*tmp_env;
	char	*id;
	// char	*value;

	//tmp_env = msl->own_env;
	printf("%d\n", ft_tokencounter(msl));
	id = ft_get_env_id(cmd);
	ft_get_one_env_value(cmd, id); //duda
	// printf("id: %s - value: %s\n", id, value);
	// check value
	// env = lst_new
	// add back
	ft_freeptr(id);
	// ft_freeptr(value);
}

/*
 * Export es para añadir variables de entorno
 * 
 * Caso 1: export sin argc -> lista ordenada con declare
 */
void	ft_export(t_msl *msl)
{
	t_pcmds	*tmp;

	printf("Start ft_export . . .\n");
	if (ft_tokencounter(msl) == 1 && !ft_strcmp(msl->tocken->pcmds->cmd, "export"))
	{
		ft_print_env(ft_sort_env(msl->own_env));
	}
	else
	{
		tmp = msl->tocken->pcmds->next;
		while (tmp)
		{
			printf("-------------------\n");
			if (ft_check_export(tmp->cmd))
				ft_add_env(msl, tmp->cmd);
			else
				return ;
			tmp = tmp->next;
		}
	}
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

	ft_export(msl);

	free_tockens(msl);//Para liberar los tockens cunado no tengo executer
}