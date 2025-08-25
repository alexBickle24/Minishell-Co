#include "../../inc/minishell.h"

void	expand_and_cleanstr(char **str, t_msl *msl);
void	dollar_expansion(char **str, int *i, size_t *len, t_msl *msl);
void	replace_dollar(char **str, int *i, size_t *len, t_msl *msl);
void	concatenate_strings(char **str, int *i, size_t *len, char *replace);
void	concatenate_strings2(char **str, int *i, size_t *len, char *replace);
void	expand_envvars(char **str, int *i, size_t *len, t_msl *msl);
void	parser2(t_msl *msl);

int g_signal = S_INIT;

void dollar_expansion(char **str, int *i, size_t *len, t_msl *msl)
{
	char	*dolim;
	unsigned char	*idx;

	idx = (unsigned char *)(*str);
	dolim = msl->parsing_utils->dollar_lim;
	if (dolim[idx[*i + 1]] >= 1 && dolim[idx[*i + 1]] < 5)//FUNCIONA
	{
		printf("Caso 1-4: dolim=%d, char=%c\n", dolim[idx[*i + 1]], (*str)[*i + 1]);
		(*i)++;
	}
	else if(dolim[idx[*i + 1]] == 5)//FUNCIONA
	{
		//caso fura de comillas
		printf("Caso 5: dolim=5, char=%c\n", (*str)[*i + 1]);
		ft_memmove(&((*str)[*i]), &((*str)[*i + 1]), *len - *i);
		(*len) --;
		//caso dentrol de comillas
			// (*i)++;
	}
	else if (dolim[idx[*i + 1]] == 6 || dolim[idx[*i + 1]] == 0)//FUNCIONA
	{
		printf("Caso 6: dolim=6, char=%c\n", (*str)[*i + 1]);
		(*i)++;
	}
	else if (dolim[idx[*i + 1]] >= 7  && dolim[idx[*i + 1]] <= 8)
	{
		printf("Caso 7-8: dolim=%d, char=%c\n", dolim[idx[*i + 1]], (*str)[*i + 1]);
		replace_dollar(str, i, len, msl);
	}
	else if (dolim[idx[*i + 1]] == 9)//FUNCIONA
	{
		printf("Caso 9: dolim=9, char=%c\n", (*str)[*i + 1]);
		ft_memmove(&((*str)[*i]), &((*str)[*i + 2]), *len - *i);
		*i = *i + 2;
		*len -= 2;
	}
	else if (dolim[idx[*i + 1]] == 10)//igual hayq ue meter un sialpha
	{
		printf("Caso envvar: dolim=0, char=%c\n", (*str)[*i + 1]);
		expand_envvars(str, i, len, msl);
	}
}

void replace_dollar(char **str, int *i, size_t *len, t_msl *msl)
{
	char	*replace = NULL;

	if ((*str)[*i + 1] == '$')//FUNCIONA
	{
		replace = ft_itoa(msl->msl_pid);
		printf("replace_dollar: caso '$', valor: %s\n", replace);
	}
	else if ((*str)[*i + 1] == '!')//FUNCIONA
	{
		replace = ft_itoa(msl->last_process);
		printf("replace_dollar: caso '!', valor: %s\n", replace);
	}
	else if ((*str)[*i + 1] == '?')//FUNCIONA
	{
		replace = ft_itoa(msl->exit_status);
		printf("replace_dollar: caso '?', valor: %s\n", replace);
	}
	else if ((*str)[*i + 1] == '0')
	{
		replace = ft_strdup(INTERPRETER_NAME);//FUNCIONA
		printf("replace_dollar: caso '0', valor: %s\n", replace);
	}
	else
	{
		printf("replace_dollar: caso desconocido, char: %c\n", (*str)[*i + 1]);
	}
	*len = *len - 2;
	concatenate_strings(str, i, len, replace);
}

//No hace falta proteger los itoa porque loa malloc no fallan nunca 
//ademas los valores que sustituyen estan inicilaizados al principio
void	concatenate_strings(char **str, int *i, size_t *len, char *replace)//FUCNIONA
{
	size_t	replace_len;
	char	*tmp;
	char	*tmp_after;
	char	*new;

	replace_len = ft_strlen(replace);
	(*str)[*i] = '\0';
	tmp = ft_strjoin(*str, replace);
	tmp_after = &((*str)[*i + 2]);
	new = ft_strjoin(tmp, tmp_after);
	free(replace);
	free(tmp);
	free(*str);
	*str = new;
	*len = *len + replace_len;
	*i = *i + replace_len;
}

void	expand_envvars(char **str, int *i, size_t *len, t_msl *msl)
{
	t_env	*env_node;
	char	tmp;
	size_t	i_tmp;
	char	*new;

	i_tmp = ++(*i);
	while (msl->parsing_utils->dollar_lim[(unsigned char)(*str)[i_tmp]] >= 8)
		i_tmp++;
	printf("Valor de dollar_lim[%c] = %d\n", (*str)[i_tmp], msl->parsing_utils->dollar_lim[(unsigned char)(*str)[i_tmp]]);
	printf("el valor del linite es %s\n", &(*str)[i_tmp]);
	tmp = (*str)[i_tmp];
	(*str)[i_tmp] = '\0';
	env_node = search_id_node(msl, &((*str)[*i]));
	(*str)[i_tmp] = tmp;
	(*str)[--(*i)] = '\0';
	*len = *len - (i_tmp - *i);
	if (env_node == NULL)
	{
		new = ft_strjoin(*str, &((*str)[i_tmp]));
		free(*str);
		*str = new;
	}
	else
	{
		*i = i_tmp;
		concatenate_strings2(str, i, len, env_node->value);
	}
}

void	concatenate_strings2(char **str, int *i, size_t *len, char *replace)
{
	size_t	replace_len;
	char	*tmp;
	char	*new;

	replace_len = ft_strlen(replace);
	tmp = ft_strjoin(*str, replace);
	new = ft_strjoin(tmp, &(*str)[(*i)]);
	*i = ft_strlen(new) - ft_strlen(&(*str)[(*i)]);
	free(tmp);
	free(*str);
	*str = new;
	*len = *len + replace_len;
}

void	expand_and_cleanstr(char **str, t_msl *msl)
{
	size_t len;
	int i;

	if (!str)
		return ;
	len = ft_strlen(*str);
	i = 0;
	while(1)
	{
		if ((*str)[i] == '$')
		{
			dollar_expansion(str, &i, &len, msl);
			printf("el valor de line[i] es: %c\n", (*str)[i]);
		}
		else
			i++;
		if ((*str)[i] == 0)
			break ;
	}
	printf("la linea despues de la expansion es %s con un len de %ld\n", *str, ft_strlen(*str));
	printf("el len de la linea es %ld\n", len);
	// clean_quotes(str);
	printf("La cadena despues de limpiar las comillas es %s\n", *str);
}

void parser2(t_msl *msl)
{
	char *line = ft_strdup(msl->clean_line);
	expand_and_cleanstr(&line, msl);
	free(line);//seria la linea del lexer ose boora y se copia o se hace logica para atraparla
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
		line = readline(PROMPT);
		add_history(line);//exit tambien se mete al historial
		msl->clean_line = line;
		if (!msl->clean_line || ft_strncmp(msl->clean_line, "exit\0", 5) == 0)
		{
			ft_putstr_fd("exit\n", 2);
			free(msl->clean_line);
			msl->clean_line = NULL;//Esta linea es solo para poder printear los valores de la minishell
			free(line);
			break ;
		}
		// meter la linea en tester de variables
		parser2(msl);
		// free(line);
		free(msl->clean_line);
		msl->clean_line = NULL;//Esta linea es solo para poder printear los valores de la minishell
		// printf("\n");
		// print_msl(msl);
	}
	free_msl(&msl);
	return (0);
}

