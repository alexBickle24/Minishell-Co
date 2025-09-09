#include "../inc/minishell.h"

/*
Test: pwd
 getcwd devuelve el directorio actual
 getcwd(buff, size) para reservar momoria pero si haces (NULL, 0)
 reserva memoria dinamica
*/
void	ft_freeptr(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

void	ft_pwd(void)
{
	char *pwd;

	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, 1);
	ft_freeptr(pwd);
}