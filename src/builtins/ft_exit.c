#include "minishell.h"

int	ft_check_argc_exit(t_msl *msl)
{
	int	i;
	char	*flag;

	printf("Start check exit! . . .\n");
	if (ft_tokencounter(msl) > 3)
	{
		printf("ft_tokencounter > 3\n");
		return 1;
	}
	if (ft_tokencounter(msl) == 2)
	{
		i = 0;
		flag = ft_strdup(msl->tocken->pcmds->next->cmd);
		while (flag[i])
		{
			if (!ft_isdigit(flag[i]))
			{
				printf("exit: %s: numeric argument required\n", flag); // corregir
				free(flag);
				return 1;
			}
			i++;
		}
		free(flag);
	}
	return (0);
}

void	ft_exit(t_msl *msl)
{
	printf("Start exit! . . .\n");
	ft_putendl_fd("exit!!", 2);
	if (!ft_check_argc_exit(msl))
		return ;
		// ft_exit(msl);
		// free_tockens(msl);
	else
		ft_putendl_fd("exit: too many arguments", 2);
}