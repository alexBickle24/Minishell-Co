

#include "../../inc/minishell.h"


/*
    Manejo centrailizado: Pra estonecesitariamos un manejador que peuda recibir argumentos o
    un puntero a la estrcutura de minishell, porque... para poder acceder a una job table donde
    tendriamos guardados todos los procesos hijos abiertos. Esto nos valdria para los hijos creados
    para el heredoc o para los hijos creados para ejecutar los comandos. Podriamos mandarles señales
    con kill para matarlos directamente (dejaria still reachable) o podrian tener su podrio manejador
    para controlar que pasa si reciben esta señal. (NO LO PODEMOS USAR->RESTRICION GLOBALES)

    Manejo descentralizado: Esta manera hace que cada proceso, hijos y padre tengan uss manejadores 
    independientes. Cuando reciben la señal no se redirige desde el padre con un manejador a los hijos
    sino que cada hijo tiene su propio manejador que recibe la señal directamente y hace lo que tenga
    que hacer cunado la reciba. (LA QUE VAMOS A USAR)

    Señales a manejar:
        -Control+C
        -Control+\
    
    Manejadores:
        -Padre:
            -Cambia el estado de la minishell a estado de Control+C: Esto nos vale de flag para cortar ejecucion
            en caso de que la señal llegase en la fase de lexeriazacion y parseo. Iria directamente a liberar los
            tockens creados.
            -Tiene que ignorar Cotrol+\ (si no lo manejamos explicitamente no lo ignorara)
            -Pone el exit estatus en el valor que sea. Si estaba en fase de ejecucion cuando recibe la señal da 
            igual porque se ejecute antes el manejador del padre o el del hijo, como los hijos tienen bloqueos que
            reciben el ultimo codigo de salida  establecen el exit estatus en ambos casos el error code estaria  bien
            configurado. A nivel de experiencia de ususario no se nota->ver strace.
            -No es necesario poner la flag SA_RESTART, esto es porque la ejecucion del codigo es lineal y no estamos usan
            do una manejador centralizado. En ese caso si que tendria sentido para que no se reiniciase la funcion waitpis
            y espera a que los hijos acaabase de hacer lo que tiene que hacer.
        -Hijos:
            -Cuando reciben la señal su manejador hace exit con el codifo de salida pertienete. Como no se reserva
            memeria dentro de los hijos solo daria los (still reachable de la mmeria recibida del padre). Al hijo tambien le 
            aafecta la señald de Ctrl^\. Pero al los padres no. Da un mensaje de salida ¿hay que manejarlo?
        -Hijo_heredoc:
            -Es como el del hijo pero no le afecta Control^\. Ademas el codigo de salida no es relevante por la arquitectura de 
            nuestra minishell pero en su bloqueador tambien lo podriamso recoger si fuese necesario.
        -*Nieto_heredoc: PENDING
        EXTRA: Meter el control+z [n] es el contador de veces que se ha llamado a esa señal Codigo de señal = 10
*/


void    signal_init(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    g_signal = S_INIT;
    sa_int.sa_handler = sig_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void    signal_init_childs(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = sig_child_handler;
    sigemptyset(&sa_int.sa_mask);//no bloquea señales
    sa_int.sa_flags = 0;//no esta en ningun modo especial
    sigaction(SIGINT, &sa_int, NULL);//La fucnion que jeecuta la señal segun preparacion de estrcutura previa
    sa_quit.sa_handler = sig_child_handler;//con esta opcion noestablecemos handler para este tipo de señal
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);//la funcoin que espera y ejecuta la señal segun configuracion de estrucutra previa
}

void    signal_init_heredoc(void)
{
	// signal(SIGINT, SIG_DFL);
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = sig_heredoc_handler;
    sigemptyset(&sa_int.sa_mask);//no bloquea señales
    sa_int.sa_flags = 0;//no esta en ningun modo especial
    sigaction(SIGINT, &sa_int, NULL);//La fucnion que jeecuta la señal segun preparacion de estrcutura previa
    sa_quit.sa_handler = SIG_IGN;//con esta opcion noestablecemos handler para este tipo de señal
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);//la funcoin que espera y ejecuta la señal segun configuracion de estrucutra previa
}

//por defecto readline que queda todolo que escribamos por pantalla y lo va filtrando y alñadiendo al historial de comandos
//es como un get next line pero con estrcuturas internas (comola del hostoria) y mas funcones por eso hac elos still reachable
//por qeu los guarda en variables estatuticas que apuntan a estrcuturas de datos 
void sig_handler(int signal)
{
    (void)signal;

	if (g_signal == S_EXECUTION)
	{
		g_signal = S_SIGINT;
    	write(1, "\n", 1);//para que lo que pinter readline, es decir elprompt sea en la siguiente linea de STDIN file
    	rl_replace_line("", 0);//para que loque hayamos escrito se sustituya por un caracteer vcacio
		rl_on_new_line();//no cmabia nada a efectos practicos creoque es solo para el conteo de lineas en el historial
	}
	else if (g_signal == S_INIT || g_signal == S_SIGINT)
	{
		g_signal = S_SIGINT;
		write(1, "\n", 1);//para que lo que pinter readline, es decir elprompt sea en la siguiente linea de STDIN file
		rl_replace_line("", 0);//para que loque hayamos escrito se sustituya por un caracteer vcacio
		rl_on_new_line();//no cmabia nada a efectos practicos creoque es solo para el conteo de lineas en el historial
		rl_redisplay();//hace un redisplay del propmpt
	}
	else if (g_signal == S_HEREDOC)
	{
		g_signal = S_SIGINT;
    	write(1, "^C", 2);//para que lo que pinter readline, es decir elprompt sea en la siguiente linea de STDIN file
    	write(1, "\n", 1);//para que lo que pinter readline, es decir elprompt sea en la siguiente linea de STDIN file
    	rl_replace_line("", 0);//para que loque hayamos escrito se sustituya por un caracteer vcacio
		rl_on_new_line();//no cmabia nada a efectos practicos creoque es solo para el conteo de lineas en el historial
	}
}

//se peuede hacer con signal o exit o kill. En al practica el manejador que
//de ell codigo de salida va a ser el manejador del proceso que sustituyamos
//con exeve quien lo de pero si no tiene heredara el que le demos asi que si
//que importa
void sig_child_handler(int signals)
{
    signal(signals, SIG_DFL);
}

//se puede hacer con kill (SIGKILL), con exit o con signal(sigDFL) ya que le codigo de salida nos da igual
//en este caso solo queremos libera la memeria reservada y cualquiera de las opciones vale.
void    sig_heredoc_handler(int signals)
{
	int i;

	i = -1;
	while (close(++i) == 0)
		close (i);
	exit (signals);
}

