

#include "../inc/minishell.h"

int g_signal = S_INIT;

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
		msl->clean_line = ft_strtrim(line, " \t\n\v\f\r");//por el mod literal de bash con control+V
		if (!msl->clean_line || ft_strncmp(msl->clean_line, "exit\0", 5) == 0)
		{
			ft_putstr_fd("exit\n", 2);
			free(msl->clean_line);
			msl->clean_line = NULL;//Esta linea es solo para poder printear los valores de la minishell
			free(line);
			break ;
		}
		// meter la linea en minishell (lexerizaciion y parseo)
		interpreter_mode(msl);//esto deberia llamarse process_line
		free(line);
		free(msl->clean_line);
		msl->clean_line = NULL;//Esta linea es solo para poder printear los valores de la minishell
		// printf("\n");
		// print_msl(msl);
	}
	free_msl(&msl);
	return (0);
}


//Funciones de parseo y ejecucion
void	interpreter_mode(t_msl *msl)
{
	//Funcion de parseo 1.0 para probar ejecutador (comentar, es solo para testear la ejecucion)
	parser1(msl);
	//Ejecutador
	executer(msl);
}

/*
	*esta funcion recibe la linea y crea tres comandos
*/

void	parser1(t_msl *msl)
{
	printf("entra en parser1\n");
	char **comands;
	int i;
	int control;
	int position;
	t_tocken *c_tockens;
	t_files *heredoc;
	char *file_name;

	i = 0;
	control = 0;
	position = 0;
	comands = ft_split(msl->clean_line, ' ');
	if (!comands || !*comands[0])
		return ;
	while (comands[i])
	{
		if (control == 0)
		{
			position += 1;
			c_tockens = list_new_tocken(position);
			list_addback_tocken(&(msl->tocken), c_tockens);
			msl->total_tockens ++;
			if (ft_strncmp(comands[i], "null\0", 5) != 0)
				list_addback_pcmds(&(c_tockens->pcmds), list_new_pcmds(ft_strdup(comands[i])));
		}
		if (control == 1 && ft_strncmp(comands[i], "null\0", 5) != 0)
			list_addback_pcmds(&(c_tockens->pcmds), list_new_pcmds(ft_strdup(comands[i])));
		if (control == 2 && ft_strncmp(comands[i], "null\0", 5) != 0)
			list_addback_infiles(&(c_tockens->files), list_new_files(ft_strdup(comands[i]), INFILE));
		if (control == 3 && ft_strncmp(comands[i], "null\0", 5) != 0)
		{
			file_name = create_heredoc(msl, comands[i], 1);//desactiavda por defecto
			heredoc = list_new_files(ft_strdup(file_name), HEREDOC);
			list_addback_infiles(&(c_tockens->files), heredoc);
			free(file_name);
		}
		if (control == 4)
		{
			if (ft_strncmp(comands[i], "null\0", 5) != 0)
				list_addback_infiles(&(c_tockens->files), list_new_files(ft_strdup(comands[i]), OUTFILE));
			control = -1;
		}
		control++;
		i++;
		//funcion de limpieza de tockens
	}
	print_tockens(msl);
	ft_free_table(comands);
}

