

#include "../../inc/minishell.h"

void	init_sep_op(unsigned char *sep_op)
{
	sep_op['\0'] = 1;
	sep_op['\t'] = 1;
	sep_op['\n'] = 1;
	sep_op['\v'] = 1;
	sep_op['\f'] = 1;
	sep_op['\r'] = 1;
	sep_op[' '] = 1;
	sep_op['>'] = 2;
	sep_op['<'] = 3;
	sep_op['|'] = 4;
	sep_op['\"'] = 5;//borrado
	sep_op['\''] = 5;
}

void	init_dollar_lim(char *dollar_limits, unsigned char *sep_op)
{
	int	i;
	int	max;

	i = -1;
	max = 255;
	while (++i <= max)
	{
		if (sep_op[i] != 0)
			dollar_limits[i] = sep_op[i];//print
		if (i >= '1' && i <= '9')
			dollar_limits[i] = 9;//borradp junto con numero
		if (ft_isalpha(i))
			dollar_limits[i] = 10;//caracteres asccii
	}
	dollar_limits['='] = 6;//pritn junto a caracter
	dollar_limits['?'] = 7;//sustiucion por valores de msl
	dollar_limits['$'] = 7;
	dollar_limits['!'] = 7;
	// dollar_limits['-'] = 7;
	// dollar_limits['#'] = 7;
	dollar_limits['0'] = 8;
}


void	init_jump_table(int (**f)(t_msl *, int *, unsigned char *, t_parsing *))
{
	f[0] = info;
	f[1] = spaces;
	f[2] = redir_out;
	f[3] = redir_in;
	f[4] = pipe_op;
	f[5] = quotes;
}


t_parsing	*init_parsing(t_msl *msl)
{
	t_parsing *pars;

	if (!msl)
		return (NULL);
	pars = NULL;
	if (msl->parsing_utils == NULL)
	{
		pars = (t_parsing *)ft_calloc(sizeof(t_parsing), 1);
		if (pars == NULL)
			return (NULL);
		init_sep_op(pars->sep_op);
		init_dollar_lim(pars->dollar_lim, pars->sep_op);
		init_jump_table(pars->lex);
	}
	return (pars);
}

void	set_parsdefaultvals(t_msl *msl)
{
	msl->parsing_utils->infstat = 0;
	msl->parsing_utils->lexstat = 0;
	msl->parsing_utils->parstat = 0;
	msl->parsing_utils->ptr = 0;
}

