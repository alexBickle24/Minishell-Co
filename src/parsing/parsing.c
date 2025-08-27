
#include "../../inc/minishell.h"


//Funcion para recorrer la lista de lexer e ir añadiedno a la estrcutura de 
//ejecucio las palabras segun su etiqueta (tipo sintactico).
void	clean_expand_add_toexecuter(t_msl *msl)
{
	t_lex *lexer;
	t_lex	*tmp;
	t_tocken *current;

	printf("\n////////////////clean and expand//////////////////////////\n");
	lexer = msl->lexer;
	if (g_signal != S_INIT || lexer == NULL)
	{
		printf("no hay lexer que procesar\n");
		return ;
	}
	if (check_heredocs_and_newline_error(msl))
		return ;
	msl->tocken = list_new_tocken(1);
	current = msl->tocken;
	msl->total_tockens = 1;
	while (lexer)
	{	
		tmp = lexer->next;
		if (lexer->type == T_PIPE)
		{
			msl->total_tockens++;
			current->next = list_new_tocken(msl->total_tockens);
			printf("nuevo tocken con cnumero %d\n", msl->total_tockens);
			current = current->next;
			// list_addback_tocken(&(msl->tocken), current);//bajamos complejidad
			free(lexer->raw);
			free(lexer->str);
		}
		else if (lexer->type == T_CMD)
		{
			printf("lex tipo %d\n", lexer->type);
			expand_str(&(lexer->str), msl, &(lexer->len));
			adding_cmds(msl, current, lexer);
			print_pcmds(current->pcmds);
		}
		else if (lexer->type == T_INFILE || lexer->type == T_OUTFILE || lexer->type == T_APPEND)
		{
			printf("lex tipo %d\n", lexer->type);
			expand_str(&(lexer->str), msl, (&lexer->len));
			adding_files(msl, current, lexer);
			print_files(current->files);//
		}
		else if (lexer->type == T_HEREDOC || lexer->type == T_HEREDOC_S)
		{
			printf("lex tipo %d\n", lexer->type);
			adding_heredoc(msl, current, lexer);
			print_files(current->files);//

		}
		free(lexer);
		lexer = tmp;
	}
	msl->lexer = NULL;
}

//primer recorrido del string expando variables
void	expand_str(char **str, t_msl *msl, size_t *len)//FUNIONA
{
	int i;

	if (!str)
		return ;
	i = 0;
	while(1)
	{
		check_clean_quotes(msl, &((*str)[i]), 0);
		if ((*str)[i] == '$' && msl->parsing_utils->lexstat != S_QUOTES)
			dollar_expansion(str, &i, len, msl);
		else
			i++;
		if ((*str)[i] == '\0')
			break ;
	}
}


//segundo recorrido, compuebo si despues de la expansion habia espacios
//y ademas limpio comillas
void	adding_cmds(t_msl *msl, t_tocken *current, t_lex *lex)//FUNCOINA
{
	char  *start;
	char *str;
	char jump;

	str = lex->str;
	start = str;
	while(*str)
	{
		// printf("el valor de str es: %s\n", str);
		jump = check_clean_quotes(msl, str, 1);
		if (is_space(*str) && msl->parsing_utils->lexstat == NO_QUOTES)
		{
			*str = '\0';
			printf("ESPACION EN CMD: el valor de la cadena es: %s\n", start);
			list_addback_pcmds(&(current->pcmds), list_new_pcmds(ft_strdup(start)));
			str++;
			jump_separator(&str);
			start = str;
		}
		else if (jump == 1)
			str++;
	}
	list_addback_pcmds(&(current->pcmds), list_new_pcmds(ft_strdup(start)));
	free(lex->raw);
	free(lex->str);
}



void	adding_files(t_msl *msl, t_tocken *current, t_lex *lex)
{
	char ambiguos;
	char *str;
	char jump;

	ambiguos = 0;
	str = lex->str;
	while(*str)
	{
		jump = check_clean_quotes(msl, str, 1);
		if (is_space(*str) && msl->parsing_utils->lexstat == NO_QUOTES)
			ambiguos = 1;
		if (jump == 1)
			str++;
	}
	if (ambiguos == 1)
	{
		printf("ARCHIVO AMBIGUO: %s\n", lex->raw);
		list_addback_infiles(&(current->files), list_new_files(lex->raw, lex->type, ambiguos));
		free(lex->str);
	}
	else
	{
		printf("ARCHIVO NO AMBIGUO: %s\n", lex->str);
		list_addback_infiles(&(current->files), list_new_files(lex->str, lex->type, ambiguos));
		free(lex->raw);
	}
}

void	adding_heredoc(t_msl *msl, t_tocken *current, t_lex *lex)
{
	free(lex->str);
	if (lex->type == T_HEREDOC)
		lex->str = create_heredoc(msl, lex->raw, 0);
	else if (lex->type == T_HEREDOC_S)
		lex->str = create_heredoc(msl, lex->raw, 1);
	list_addback_infiles(&(current->files), list_new_files(lex->str, lex->type, 0));
	free(lex->raw);
}






/////////////////////////////////////////////PARA CASO DE NEW LINE ERROR///////////////////////////////////////////

//esto es para checkear e imitar el funcionamiento de bashe n caso de que sea necesario con el tema de ls
//creacion de heredocs antes del error de newline

char	check_heredocs_and_newline_error(t_msl *msl)
{
	t_lex	*lexer;

	if (msl->pars_err == 1)
	{
		lexer = msl->lexer;
		while(lexer)
		{
			if (lexer->type == T_HEREDOC || lexer->type == T_HEREDOC_S)
				heredoc_new_line_errror(msl, lexer);
			lexer = lexer->next;
		}
		new_line_err(msl);
		msl->pars_err = 0;
		return (1);
	}
	return (0);
}


//esto es para mostrar el mensaje de newline error en caso de que queramos imitar 
//el funcionamientos de los errores de nweline de bash en  fase deparse o despues de 
//crear el heredoc
char	new_line_err(t_msl *msl)
{
	char	error;

	error = 0;
	if (msl->pars_err == 1)
	{
		error = 1;
		ft_putstr_fd(NEWLINE_ERR, 2);
		free_lexer(msl, 1);//
		free_tockens(msl);// esta es por si creamos los tockens y manejamos el error especial
	}
	return (error);
}

//esto para el caso que tengamos que 
void	heredoc_new_line_errror(t_msl *msl, t_lex *lex)
{
	free(lex->str);
	if (lex->type == T_HEREDOC)
		lex->str = create_heredoc(msl, lex->raw, 0);
	else if (lex->type == T_HEREDOC_S)
		lex->str = create_heredoc(msl, lex->raw, 1);
}
