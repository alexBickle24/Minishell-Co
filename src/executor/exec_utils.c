

#include "../../inc/minishell.h"


/*
	Funcion para buscar y ejecutar el comando
	**Revisar: Casos de ./ -- .sh y scripts -- directorios (hechi)
	**Revisar: Mensajes de error. (hecho)
	**Aprender: Bases de los scripts y .sh (como crearlos e iniciarse->>AUTOMATIZACION DE TAREAS)
	*Como porlas señales no puedo cancelar con kill no tengo reseravar memeria desde 
	*los hujos por lo que se hace un evaluate_cmd y configuo el mensaje de error, hay tambie se
	*pasa la lista enlazada a tabla.

	**CONCLUSION: Al fianla por el tema de al memoria dentro de lso proceos y la imposibilidad
	*de crear un job table y acceder a ella desde el manejador de señales centralizado
	*para matarlos con la señales de Ctrl+C y Ctrl+\ nohay que reservar memeria dentro de los
	*procesos hijos. Entonces solo queda generar una variable de error_cmd y checkearla desde el
	*proceso hijo. Como errno estara configurado usamos las funciones de error para hacer el 
	*tipo de error en caso de que lo haya y generar el exit_code o ejecutar el comando.

	//1: Esto es el caso de que nos den un ruta de un binario o de un script con permisos de ejecucion del sistema
	(cuidado con el shebang)->errores de permision denied o no suchfile or directory
	//2: Esto es caso de que nos den un comando (binario) o script sin su ruta. Cunado nos lo dan asi busacsmos is esta
	en el path. Si no esta retornamos el error de comand not found. Si no esta no retornamos el error de comando not found
	u si esta pasamos a //3 que comprobaria la rutas para saber si tiene los permisos, ya sabemos que existe por descarte

	Tener en cuenta: Shell no deja crear archivos con .. ni ./ no / de nombre asiq ue con eso no hay problema. Realemete si
	 se pueden usar los . y .. para crear archivos lo que pasa es que los crea en modo oculto, asi que 
	sabiendo eso se podria cambiar los ft_strncmp pot un ft_strchar(strn "/")

	Tema de los scripts: Los scripts se pueden ejecutar de dos maneras.
		- Si el archivo tiene permisos de ejecucion y elegimos y en la linea de shebang que nos dice
		el interprete a usar por el sistema para ejecutarlos
*/


void evaluate_tocken_cmds_errors(t_tocken *c_tocken, t_msl *msl)
{
	char	*path;

	if (!c_tocken->pcmds)
		return ;
	if (!c_tocken->cmd_tb)
		c_tocken->cmd_tb = ft_pcmds_to_table(c_tocken->pcmds);
	if (!c_tocken->env_tb)
		c_tocken->env_tb = ft_env_to_table(msl->own_env);
	// if (!ft_strncmp(c_tocken->cmd_tb[0], "./", 2) || !ft_strncmp(c_tocken->cmd_tb[0], "/", 1) || !ft_strncmp(c_tocken->cmd_tb[0], "../", 2))//1
	// {
	// 	if (access(c_tocken->cmd_tb[0], F_OK | X_OK) == -1)
	// 		c_tocken->error_cmd = 1;
	// 	return ;
	// }
	if (ft_strchr(c_tocken->cmd_tb[0], '/') != 0)//1
	{
		if (access(c_tocken->cmd_tb[0], F_OK | X_OK) == -1)
			c_tocken->error_cmd = 1;
		return ;
	}
	else
	{
		path = check_path(c_tocken->cmd_tb[0], c_tocken->env_tb);//2
		if (!path)
		{
			c_tocken->error_cmd = 2;
			return ;
		}
		free(c_tocken->cmd_tb[0]);
		c_tocken->cmd_tb[0] = path;
		if (access(path, X_OK) == -1)//3
			c_tocken->error_cmd = 1;
	}
}


char *check_path(char *x_file, char **env)
{
	char	**absolute_paths_table;
	char	*absolute_paths;
	char	*path;

	if (!x_file || !env)
		return (NULL);
	absolute_paths = get_env_value("PATH", env);
	if (!absolute_paths)
		return (NULL);
	absolute_paths_table = ft_split(absolute_paths, ':');
	if (!absolute_paths_table || !absolute_paths[0])
		return (NULL);
	path = find_exe_file(absolute_paths_table, x_file);
	if (!path)
		return (ft_free_table(absolute_paths_table), NULL);
	ft_free_table(absolute_paths_table);
	return (path);
}

char	*get_env_value(const char *key_value, char **env)
{
	int		i;
	size_t	key_len;
	char	*new_value;

	if (!key_value)
		return (NULL);
	key_len = ft_strlen(key_value);
	if (key_len == 0 || env == NULL)
		return (NULL);
	new_value = ft_strjoin(key_value, "=");
	if (new_value == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if ((ft_strncmp(new_value, env[i], key_len + 1)) == 0)
		{
			free(new_value);
			return ((env[i] + key_len + 1));
		}
		i++;
	}
	free(new_value);
	return (NULL);
}

char	*find_exe_file(char **posible_paths, char *x_file)
{
	char	*relative_path;
	char	*aux;
	int		i;

	if (!posible_paths || !x_file)
		return (NULL);
	i = 0;
	while (posible_paths[i])
	{
		aux = ft_strjoin(posible_paths[i], "/");
		if (!aux)
			return (NULL);
		relative_path = ft_strjoin(aux, x_file);
		if (!relative_path)
		{
			free(aux);
			return (NULL);
		}
		if (access(relative_path, F_OK) != -1)
			return (free(aux), relative_path);
		i++;
		free(aux);
		free(relative_path);
	}
	return (NULL);
}



///FUNCION DE ANTES DE SABER QUE NO PUEDO RESERVAR MEMERIA EN PROCESO HIJO POR LAS SEÑALES

// void	check_exe(char *x_file, char **env, char **orders_list)
// {
// 	char	*path;

// 	//compurebo si x_files es una ruta al directorio actual, o una ruta absoluta
// 	if (!ft_strncmp(x_file, "./", 2) || !ft_strncmp(x_file, "/", 1) || !ft_strncmp(x_file, "..", 2))
// 	{
// 		if (access(x_file, F_OK | X_OK) == -1)
// 			ft_error_exes(x_file);//126 o 127
// 		else
// 			execve(x_file, orders_list, env);
// 	}
// 	path = check_path(x_file, env);
// 	if (!path)
// 	{
// 		if (access(x_file, F_OK) == 0)
// 			ft_error_cmd(x_file);//127
// 	}
// 	if (access(path, X_OK) == -1)
// 		ft_error_exes(path);//126 o 127
// 	else
// 		if (execve(path, orders_list, env) == -1)
// 			ft_execve_error();
// }