#include <stdio.h>
#include <string.h>

/*
 * Comprobamos la flag -n, -nn o derivados
 */
int ft_is_flag(char *str)
{
	int i;

	if (str[0] != '-' || strlen(str) < 2)
		return 1;
	i = 1;
	while(str[i])
	{
		if (str[i] != 'n')
			return 1;
		i++;
	}
	return 0;
}

/*
 * Pintamos por pantalla, si hay -n no hacemos salto de linea
 * 
 * Pasos:
 *      1. Comprobamos la flag
 *      2. Escribirmos la palabra
 */
void    ft_echo(int argc, char **argv)
{
	if (ft_is_flag(argv[1]) == 0)
	{
		if (argc == 3)
			printf("%s", argv[2]);
	}
	else
	{
		if (argc == 2)
		{
			printf("%s\n", argv[1]);
		}
		else if (argc == 3)
		{
			printf("%s ", argv[1]);
			printf("%s\n", argv[2]);
		}
	}
}

int main(int argc, char **argv)
{
	if (argc == 2 || argc == 3)
		ft_echo(argc, argv);
	return 0;
}


/*
	argv = ft_pcmds_to_tab(tocken->pcmds)
	argz = ft_count_lines(tocken->cdm_tab[0])
*/