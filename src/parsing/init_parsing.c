

#include "../../inc/minishell.h"

void	init_separators(char *separators)
{
	separators['\0'] = 1;
	separators['\t'] = 1;
	separators['\n'] = 1;
	separators['\v'] = 1;
	separators['\f'] = 1;
	separators['\r'] = 1;
	separators[' '] = 1;
}

void	init_special_op(char *operators)
{
	operators['>'] = 2;
	operators['<'] = 3;
	operators['|'] = 4;
}

void	init_dollar_lim(char *dollar_limits, char *separators, char *operators)
{
	int	i;
	int	max;

	i = 0;
	max = 255;
	while (i <= max)
	{
		if (separators[i] != 0)
			dollar_limits[i] = separators[i];//print
		if (operators[i] != 0)
			dollar_limits[i] = operators[i];//print
		if (i >= '1' && i <= '9')
			dollar_limits[i] = 9;//borradp junto con numero
		i++;
	}
	dollar_limits['\"'] = 5;//borrado
	dollar_limits['\''] = 5;
	dollar_limits['='] = 6;//pritn junto a caracter
	dollar_limits['?'] = 7;//sustiucion por valores de msl
	dollar_limits['$'] = 7;
	dollar_limits['!'] = 7;
	dollar_limits['0'] = 8;
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
		init_separators(pars->separators);
		init_special_op(pars->special_op);
		init_dollar_lim(pars->dollar_lim, pars->separators, pars->special_op);
	}
	return (pars);
}
