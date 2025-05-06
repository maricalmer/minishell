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

/* ************************************************************************** */
/*                                                                            */
/* Implements the `cd` builtin command, handling directory changes and        */
/* updating environment variables (PWD, OLDPWD). Handles edge cases such as   */
/* too many arguments or missing HOME variable. Internally manages cwd        */
/* retrieval, argument parsing, directory switching, and env updates.         */
/* Returns 0 on success, 1 on any error.                                      */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	get_current_directory(char *cwd)
{
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("minishell: getcwd");
		return (1);
	}
	return (0);
}

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

int	change_directory(char *path)
{
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

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
