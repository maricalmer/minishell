/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:54:22 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 19:36:51 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles the `export` builtin command by setting or validating environment  */
/* variables. Supports key-only exports and key=value pairs. If no arguments  */
/* are passed, it prints the current environment in export format. Includes   */
/* error handling for invalid identifiers and returns appropriate status.     */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(char **args, t_minishell *shell)
{
	int	i;
	int	error_found;
	int	result;

	if (!args[1])
		return (print_export_without_args(shell));
	error_found = 0;
	i = 1;
	while (args[i])
	{
		result = process_export_argument(args[i], shell);
		if (result != 0)
			error_found = 1;
		i++;
	}
	return (error_found);
}

int	process_export_argument(const char *arg, t_minishell *shell)
{
	char	*value;
	char	*name;

	value = ft_strchr(arg, '=');
	if (!value)
	{
		if (!is_valid_identifier(arg))
			return (print_export_error(arg));
		return (0);
	}
	*value = '\0';
	value++;
	name = (char *)arg;
	if (!is_valid_identifier(name))
		return (print_export_error(arg));
	return (set_env_variable(name, value, shell));
}

int	is_valid_identifier(const char *arg)
{
	if (!ft_isalpha(*arg) && *arg != '_')
		return (0);
	arg++;
	while (*arg)
	{
		if (!ft_isalnum(*arg) && *arg != '_')
			return (0);
		arg++;
	}
	return (1);
}

int	print_export_error(const char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}
