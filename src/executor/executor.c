
#include "../../inc/minishell.h"

/*
	*BRIEF: Esto es el ejecutador(core) de las ordenes ya Tockenizadas
	Lo separando en 3 las Fucniones de ejecucion, mediante el index y 
	el contador de tockens de la estructura proncipal controlo en cuantas
	de ellas tengo que entrar
	*Maneras alternativas de hacerlo:
		+Pipes fuera del nodo:
			-Creando las pipes en las propias funciones y que esten en la
			estrcutura (mas dificil de liberar).
		+Pipes_fd en los nodos: (mas facil de liberar)
			-Con recurisvidad y una unica fucncion de ejecutar proceso (pipes
			en el nodo)
			-Cambiando el manejador e incluyendo checko de señales si
			en vez de pipes (|), tengo (;).
			- En una sola funcion
			-En tres funciones
		+Forma de hacerlo de frack;
		
*/

void    executer(t_msl *msl)
{
	if (msl && g_signal == S_INIT)//temer que la lista de tockens exista o msl errro (probar a ver como se comporta waitchild)
	{
		if (msl->total_tockens == 1 && is_builtin(msl->tocken))
			only_builtin(msl);
		else 
			execute_orders(msl);
	}
	//Funcion para el caso de señal activad aantes de ejecucion (evalua la vairable global)
	g_signal = S_INIT;
	msl->pars_err = 0;
	//Funcion de liberacion de memeria y cierre de fds de p_tockens
	free_tockens(msl);
}

void    only_builtin(t_msl *msl)
{
	t_tocken    *c_tocken;
	char        append_mode;

	append_mode = 0;
	c_tocken = msl->tocken;
	if (c_tocken->redir_out && c_tocken->redir_out->type == T_APPEND)
		append_mode = 1;
	check_create_redirs(c_tocken, c_tocken->files);
	if(c_tocken->redir_in->ambiguos || tunel_in_file(c_tocken->redir_in->file_name) != 0)
	{
		ft_error_redirs(c_tocken->redir_in->file_name, c_tocken->redir_in->ambiguos);
		c_tocken->error_file = 1;
	}
	if(c_tocken->redir_out->ambiguos || tunel_out_file(c_tocken->redir_out->file_name, append_mode) != 0)
	{
		ft_error_redirs(c_tocken->redir_out->file_name, c_tocken->redir_out->ambiguos);
		c_tocken->error_file = 1;
	}
	if (c_tocken->error_file != 1)
		exec_builtin(msl, msl->tocken, is_builtin(msl->tocken));
	else
		msl->exit_status = 1;
}

void execute_orders(t_msl *msl)
{
	t_tocken *c_tocken;

	c_tocken = msl->tocken;
	while (c_tocken)
	{
		execute_childs(c_tocken, msl);
		c_tocken = c_tocken->next;
	}
	wait_childs3(msl);
}

void execute_childs(t_tocken *c_tocken, t_msl *msl)
{
	pid_t pid;
	int   s_builtin;

	s_builtin = is_builtin(c_tocken);
	check_create_redirs(c_tocken, c_tocken->files);
	if (!s_builtin)
		evaluate_tocken_cmds_errors(c_tocken, msl);
	if(msl->total_tockens - c_tocken->index != 0)
		create_pipes(c_tocken->next);
	pid = fork();
	if (pid < 0)
		ft_errerrno();
	if (pid == 0)
	{   
		fordward_in(c_tocken);//Se cierran los fds de la pipe de lectura (la anterior), esta en el tock
		fordward_out(c_tocken);//se cierran fds de la pipe de escritura (la siguiente)
		signal_init_childs();//sepone despues porque sino no se liberan los fds
		if (c_tocken->error_file != 0)//error de exitencia o permisos de archivos (corta ejecuion)
			exit (1);//podriao liberar tambien toda la memeri ade copy on write
		if (c_tocken->pcmds)//di hay cmd o building y no se ha cortado la ejecucion por fallo (file)
			cmd_vs_builtin(c_tocken, s_builtin);
		else//en caso de que no haya cmd
			exit(0);//podriamo liberar la memeria tambien de copy on write
	}
	if (c_tocken->index != 1)
		close_fds(c_tocken->pipe_fds);
	c_tocken->pid = pid;
}

void cmd_vs_builtin(t_tocken *c_tocken, int builtin)
{
	if (builtin == 1)
	{
		// ft_echo(c_tocken->cmds, msl->own_env);
	}
	else if (builtin == 2)//#
	{
		// ft_cd(c_tocken->cmds, msl->own_env);
	}
	else if (builtin == 3)
	{
		// ft_pwd();
	}
	else if (builtin == 4)//#
	{
		// ft_export(c_tocken->cmds, msl->own_env);
	}
	else if (builtin == 5)//
	{
		// ft_unset(c_tocken->cmds, msl->own_env);
	}
	else if (builtin == 6)
	{
		// ft_env(msl->own_env);
	}
	else if (builtin == 7)//#
	{
		// ft_exit(c_tocken->cmds, msl);
	}
	else
	{
		exec_cmd(c_tocken);
	}
}


































// //TAMBIEN ESTA LA FORMA DE HACERLO DE FRACK->ELMESTRO

// ///FORMA DE HACERLO ADAPATADA A MI PIPEX, PERO USANDO LAS ESTRCUTURAS PARA
// //LAS PIPES Y REDIRECCIONES


// //Areglar el iterations count
// //arreglar  nombres para que sean mas profesionales
// void executer(t_msl *msl, t_tocken *msl)
// {
//     t_tocken *current_order;
//     int iterations_count;

//     iterations_count = 1;
//     current_order = msl->tockens_list;
//     first_process(current_order, msl);
//     while(current_order->t_index <= msl->total_tockens - 1)
//     {
//         link_process(current_order, msl);
//         current = current->next;
//         iterations_count++;
//     }
//     last_process(current_order, iterations_countm msl);
//     //CAPTURADOR DE SEÑALES DESDE EL PADRE Y LIMIPIEXZA DE TOCKENS
//     //MEMORIA Y CERRAR FDS DE TOCKENS
// 	id = waitpid(-1, &status, 0);
// 	while (id > 0)
// 		id = waitpid(-1, &status, 0);
// 	close_fds(NULL);
// }

// void first_process(t_tocken *c_tocken, t_msl *msl)
// {
//     pid_t pid;

//     if(total_tockens - index != 0)
//         create_pipe(c_tocken->next->pipe);
//     evaluate_tocken_redirs(c_tocken->redirs)//La puedo llamar desde el padre o desde el hijo
//     pid = fork();
//     if (pid < 0)
//         ft_putstr_fd(strerro(errno), 2);
//     if (pid == 0)
//     {
// 	    fordward_in(c_tocken, msl);
//         fordward_out(c_tocken, msl);
//         if (!c_tocken->cmd[0])//esto lo puedo subir un parde lineas
//             exec_cmd();//comproebar si es biulting o comds
//         else
//             exit(0);
		
//     }
// }

// void	link_pipes(t_tocken *c_tocken, t_msl *msl)
// {
// 	int			id;

//     create_pipe(c_tocken->next);
//     evaluate_tocken_redirs(c_tocken->redirs)
//     id = fork();
//     if (id < 0)
//         ft_putstr_fd(strerro(errno), 2);
//     if (id == 0)
//     {
// 		fordward_in(c_tocken, msl);
//         fordward_out(c_tocken, msl);
//         if (!c_tocken->cmd[0])
//             exec_cmd();//Se puede subir arriba
//         else
//             exit(0);
//     }
// }

// void	last_process(t_tocken *c_tocken, t_msl *msl)
// {
// 	pid_t	id;
// 	int		fd_out;

//     if (iterations_count > msl->total_tockens)
//         return ;
// 	evaluate_tocken_redirs(c_tocken->redirs)
//     id = fork();
//     if (id < 0)
//         ft_error(NULL, NULL, NULL, NULL);
//     if (id == 0)
//     {
// 		fordward_in(c_tocken, msl);
//         fordward_out(c_tocken, msl);
//         if (!c_tocken->cmd[0])
//             exec_cmd();
//         else
//             exit(0);
//     }
// }