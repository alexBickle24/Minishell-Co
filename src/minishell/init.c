/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:23:48 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 15:00:17 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Initializes the main structure of the shell.
 * 
 * This function allocates and initializes the `t_msl` structure, which holds
 * the core data of the shell. It sets up the environment, parsing utilities,
 * built-in commands, and signal handling.
 * 
 * - Converts the environment variables from a table to a linked list.
 * - Sets the shell mode (`interactive` or `non-interactive`).
 * - Initializes system-related data (e.g., user, hostname, home directory).
 * - Sets default environment variable values.
 * - Initializes the parsing utilities and the shell's process ID.
 * - Configures signal handling for the shell.
 * 
 * @param msl Pointer to the pointer of the main structure to be initialized.
 * @param env Array of environment variables passed to the shell.
 * @param mode The mode of the shell (`interactive` or `non-interactive`).
 */

void	minishell_init(t_msl **msl, char **env, char mode)
{
	if (msl != NULL)
	{
		*msl = (t_msl *)ft_calloc(sizeof(t_msl), 1);
		if (*msl == NULL)
			return ;
		ft_env_table_to_list(*msl, env);
		(*msl)->mode = mode;
		init_system(*msl);
		set_env_default_values(*msl);
		if ((*msl)->builts == NULL)
			(*msl)->builts = (t_builts *)ft_calloc(sizeof(t_builts), 1);
		msl[0]->parsing_utils = init_parsing(*msl);
		msl[0]->msl_pid = (pid_t)ft_getpid();
		signal_init();
	}
}

/**
 * @brief Initializes system-related data for the shell.
 * 
 * This function sets up system-related data in the `t_system` structure,
 * including the user, hostname, home directory, global path, and current
 * working directory. It also sets the shell prompt (`PS1`) based on the
 * user and hostname.
 * 
 * - Allocates memory for the `t_system` structure.
 * - Retrieves fallback values for the user and hostname if they are not set.
 * - Retrieves the home directory and global path.
 * - Initializes the current working directory (`PWD`).
 * - Sets the `PS1` prompt to include the user and hostname.
 * 
 * @param msl Pointer to the main structure of the shell.
 */

void	init_system(t_msl *msl)
{
	if (msl == NULL)
		return ;
	if (msl->sys == NULL)
	{
		msl->sys = (t_system *)ft_calloc(sizeof(t_system), 1);
		if (msl->sys == NULL)
			return ;
		user_fallbacks(msl);
		hostname_fallbacks(msl, &(msl->sys->host));
		get_home(msl, &(msl->sys->home));
		get_global_path(&(msl->sys->g_path));
		init_pwd(msl);
		msl->sys->ps1_hostuser = set_ps1_hostuser(msl->sys);
	}
}

/**
 * @brief Sets default values for essential environment variables.
 * 
 * This function ensures that critical environment variables are set to
 * default values if they are not already defined. These variables include:
 * - `HOME`: The user's home directory.
 * - `SHLVL`: The shell level.
 * - `PWD`: The current working directory.
 * - `PATH`: The system path for executable files.
 * - `_`: The last argument of the last executed command.
 * - `LESSCLOSE` and `LESSOPEN`: Variables used by the `less` command.
 * 
 * This function is particularly useful in cases where the environment
 * variables have been cleared in the parent process, or if `minishell`
 * is initialized with `env -i` (an empty environment) from the parent
 * shell's command interpreter. It ensures that the shell has the necessary
 * environment variables to function correctly.
 * 
 * @param msl Pointer to the main structure of the shell.
 */

void	set_env_default_values(t_msl *msl)
{
	check_home_message(msl);
	set_shlvl(msl);
	set_pwd(msl);
	set_path(msl);
	set_last_arg_init(msl);
	set_lessclose(msl);
	set_lessopen(msl);
}

/**
 * @brief Ensures the `PATH` environment variable is set.
 * 
 * This function checks if the `PATH` variable is defined in the environment.
 * If it is not, it creates a new `PATH` variable using the global path
 * retrieved during system initialization and adds it to the shell's
 * environment list.
 * 
 * @param msl Pointer to the main structure of the shell.
 */

void	set_path(t_msl *msl)
{
	t_env	*path_node;
	char	*glo_path;

	glo_path = msl->sys->g_path;
	path_node = search_id_node(msl, "PATH");
	if (path_node == NULL)
	{
		path_node = list_new_ownenv(ft_strdup("PATH"), ft_strdup(glo_path));
		if (!path_node)
			return ;
		list_addback_env(path_node, &(msl->own_env));
	}
}

/**
 * @brief Initializes the current working directory (`PWD`).
 * 
 * This function retrieves the current working directory using `getcwd` and
 * stores it in the `t_system` structure. If `getcwd` fails (e.g., due to
 * inaccessible parent directories), it sets the `PWD` to `"."` and displays
 * an error message.
 * 
 * This function is particularly useful if `minishell` is initialized from
 * a directory whose parent directory has been deleted, ensuring that the
 * shell can still operate with a valid `PWD`.
 * 
 * - If `getcwd` succeeds, the current directory is stored in `msl->sys->pwd`.
 * - If `getcwd` fails, an error message is printed, and `PWD` is set to `"."`.
 * 
 * @param msl Pointer to the main structure of the shell.
 */

void	init_pwd(t_msl *msl)
{
	msl->sys->pwd = getcwd(NULL, 0);
	if (msl->sys->pwd == NULL)
	{
		ft_putstr_fd("shell-init: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		msl->sys->pwd = ft_strdup(".");
	}
}
