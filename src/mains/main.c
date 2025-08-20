


#include "../inc/minishell.h"
/*
 * env: Es lo mismo que extern char **environ
  *         env[i] = "PATH=/usr/bin" (ejemplo)
  *         env[N] == NULL
 */


int g_signal = S_INIT;

// int main(int argc, char **argv, char **env)
// {
//     t_msl msl;

//     if (argc != 1)
//         return (1);
//     ft_environ_init(env);
//     signal_init();
//     //rl_catch_signals = 0; Desactibva handlers por defecto de readline
//     while (1)
//     {
//         msl.input = readline(PROMPT); // Muestra un prompt
//         if (!msl.input) //Ctrl + D
//         {
//             printf("exit\n");
//             break ;
//         }
//         if (*msl.input)
//             add_history(msl.input); //Guarda (char*) en el historial
//         free(msl.input);
//     }
//     (void)argv;
//     return(0);
// }

int main(int argc, char **argv, char **env)
{
	t_msl *msl;
	char *line;//la linea en bruto
	char *clean_line;//esto es por el modo literal de la shell Control+V

    if (argc != 1)
		return (1);//si no hacemos el modo literal
	msl = NULL;
	//esto seria todo en el modo interactivo
    minishell_init(msl,env);//inicamos la estrcutura de minishell y el manejados
	while (1)
	{
		line = readline(PROMPT);
		add_history(line);//exit tambien se mete al historial
		clean_line = ft_strtrim(line, " \t\n\v\f\r");//esto no estoy muy seguro
		if (!clean_line || ft_strncmp(clean_line, "exit\0", 5) == 0)
		{
			ft_putstr_fd("\nexit", 2);
			free(clean_line);
			free(line);
			break ;
		}
		//meter la linea en dtrcutura de minishell
		msl->clean_input = clean_line;
		// meter la linea en minishell (lexerizaciion y parseo)
		interpreter_mode(msl);
		free(line);
		free(clean_line);
	}
	//limpiarmos la estrcutrua de msl
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
	char **comands;
	int num_lines;
	int num_comands;
	int i;
	int control;
	int position;
	t_tocken *c_tockens;

	i = 0;
	control = 0;
	num_lines = 0;
	position = 0;
	comands = ft_split(msl->clean_input, ' ');
	if (!comands || *comands[0])
		return ;
	while (comands[i])
	{
		if(control == 0)
		{
			position +=1;
			c_tockens = list_new_tocken(position);
			list_add_back_tocken(&(msl->tocken), c_tockens);
			msl->total_tockens ++;
			list_addback_pcmds(&(c_tockens->pcmds), list_new_pcmds(comands[i]));
		}
		if (control == 1)
			list_addback_pcmds(&(c_tockens->pcmds), list_new_pcmds(comands[i]));
		if (control == 2)
			list_addback_infiles(&(c_tockens->files), list_new_files(comands[i], INFILE));
		if (control == 3)
			list_addback_infiles(&(c_tockens->files), list_new_files(comands[i], INFILE));
		if (control == 4)
		{
			list_addback_infiles(&(c_tockens->files), list_new_files(comands[i], OUTFILE));
			control = -1;
		}
		control++;
		i++;
	}
}

// void print_tocken(t_tocken *tok)
// {

// }
