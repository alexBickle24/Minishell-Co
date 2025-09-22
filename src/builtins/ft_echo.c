#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((const unsigned char) s1[i] - (const unsigned char) s2[i]);
		i++;
	}
	return (0);
}

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
	tmp = tmp->next;
	if (!ft_strcmp(tmp->cmd, "-n"))
	{
		printf("Caso -n\n");
		tmp = tmp->next;
		while (tmp)
		{
			ft_putstr_fd(tmp->cmd, 2);
			if (tmp->next)
				ft_putchar_fd(',', 1);
			tmp = tmp ->next;
		}
	}
	else
	{
		printf("Caso simple\n");
		while (tmp)
		{
			ft_putstr_fd(tmp->cmd, 2);
			if (tmp->next)
				ft_putchar_fd(',', 1);
			tmp = tmp ->next;
		}
		ft_putchar_fd('\n', 1);
	}
}