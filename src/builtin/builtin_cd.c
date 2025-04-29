/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:52:29 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 00:51:04 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	change cwd based on provided args; handle cases with no args provided or
	when argument is '~', directing the shell to the user's home directory */
int	builtin_cd(char **args, t_minishell *shell)
{
	int		status;
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	*path;

	if (cd_too_many_arguments(args))
		return (1);
	status = get_current_directory(cwd);
	if (status != 0)
		return (1);
	oldpwd = cwd;
	status = get_target_path(args, shell, &path);
	if (status != 0)
		return (1);
	status = change_directory(path);
	if (status != 0)
		return (1);
	status = update_environment(oldpwd, shell);
	if (status != 0)
		return (1);
	return (0);
}

/* cwd: buffer to store current working directory */
int	get_current_directory(char *cwd)
{
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("minishell: getcwd");
		return (1);
	}
	return (0);
}

/*	path: pointer to store the target path */
int	get_target_path(char **args, t_minishell *shell, char **path)
{
	if (args[1] == NULL || ft_strncmp(args[1], "~", 2) == 0)
	{
		*path = get_variable_value("HOME", shell);
		if (!*path || ft_strlen(*path) == 0)
		{
			print_builtin_error("cd", "HOME not set");
			return (1);
		}
	}
	else
		*path = args[1];
	return (0);
}

/*	change cwd to target directory path */
int	change_directory(char *path)
{
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

/* update OLDPWD (previous working dir) & PWD env variables */
int	update_environment(char *oldpwd, t_minishell *shell)
{
	char	cwd[PATH_MAX];

	if (set_env_variable("OLDPWD", oldpwd, shell) != 0)
		return (1);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: getcwd");
		return (1);
	}
	if (set_env_variable("PWD", cwd, shell) != 0)
		return (1);
	return (0);
}
