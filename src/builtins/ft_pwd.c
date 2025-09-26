#include "../inc/minishell.h"

/*
Test: pwd
 getcwd devuelve el directorio actual
 getcwd(buff, size) para reservar momoria pero si haces (NULL, 0)
 reserva memoria dinamica
*/
//Te he añadido el return para poder usarlo en le main principal
void	ft_freeptr(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

int	ft_pwd(t_msl *msl)
{
	ft_putendl_fd(msl->sys->pwd, 1);
	return(0);
}