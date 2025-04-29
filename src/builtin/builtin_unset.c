/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:37:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/29 12:38:47 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* handle unset built-in command */
int	builtin_unset(char **args, t_minishell *shell)
{
	int	i;
	int	return_status;
	int	result;

	return_status = 0;
	i = 1;
	while (args[i])
	{
		result = process_unset_argument(args[i], shell);
		if (result != 0)
			return_status = result;
		i++;
	}
	return (return_status);
}

/* process a single unset argument */
int	process_unset_argument(const char *arg, t_minishell *shell)
{
	if (!is_valid_env_name(arg))
	{
		print_builtin_error("unset", "invalid variable name");
		return (1);
	}
	return (unset_env_variable(arg, shell));
	return (0);
}

/* check if string is valid environment variable name;
	i.e. name should start with alpha or '_' and contain only alnum or '_' */
int	is_valid_env_name(const char *name)
{
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

/* remove an environment variable from env list */
int	unset_env_variable(const char *name, t_minishell *shell)
{
	int	index;
	int	i;

	index = find_env_index(name, shell);
	if (index == -1)
		return (0);
	i = index;
	while (shell->env[i])
	{
		shell->env[i] = shell->env[i + 1];
		i++;
	}
	return (0);
}

/* find the index of a variable in the environment array */
int	find_env_index(const char *name, t_minishell *shell)
{
	int		i;
	char	*var_name;

	i = 0;
	while (shell->env[i])
	{
		var_name = ft_strchr(shell->env[i], '=');
		if (var_name && ft_strncmp(shell->env[i], name, var_name
				- shell->env[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}
