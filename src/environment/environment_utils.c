/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:10:19 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 20:05:07 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**duplicate_env(char **envp, t_minishell *shell)
{
	char	**new_env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	new_env = gc_calloc(&shell->gc_head, i + 1, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_env[i] = gc_strdup(&shell->gc_head, envp[i]);
		if (!new_env[i])
			return (NULL);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

/* set an environment variable */
int	set_env_variable(const char *name, const char *value, t_minishell *shell)
{
	int		index;
	char	*new_var;

	index = find_env_index(name, shell);
	new_var = create_env_string(name, value, shell);
	if (!new_var)
		return (1);
	if (index != -1)
		shell->env[index] = new_var;
	else
	{
		if (add_env_variable(new_var, shell) != 0)
			return (1);
	}
	return (0);
}

/* create environment string in the format NAME=VALUE */
char	*create_env_string(const char *name, const char *value,
		t_minishell *shell)
{
	char	*env_string;

	env_string = gc_strjoin(&shell->gc_head, name, "=");
	if (!env_string)
		return (NULL);
	env_string = gc_strjoin(&shell->gc_head, env_string, value);
	if (!env_string)
		return (NULL);
	return (env_string);
}

int	add_env_variable(char *new_var, t_minishell *shell)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	while (shell->env[len])
		len++;
	new_env = gc_calloc(&shell->gc_head, len + 2, sizeof(char *));
	if (!new_env)
		return (1);
	i = 0;
	while (i < len)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[len] = new_var;
	new_env[len + 1] = NULL;
	shell->env = new_env;
	return (0);
}
