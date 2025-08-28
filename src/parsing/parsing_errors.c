

#include "../../inc/minishell.h"

char	check_nwl_error(t_msl *msl)
{
	t_lex	*lexer;

	if (msl->pars_err == 1)
	{
		lexer = msl->lexer;
		while(lexer)
		{
			if (lexer->type == T_HEREDOC || lexer->type == T_HEREDOC_S)
				create_heredoc_nwlerr(msl, lexer);
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
void	create_heredoc_nwlerr(t_msl *msl, t_lex *lex)
{
	free(lex->str);
	if (lex->type == T_HEREDOC)
		lex->str = create_heredoc(msl, lex->raw, 0);
	else if (lex->type == T_HEREDOC_S)
		lex->str = create_heredoc(msl, lex->raw, 1);
}
