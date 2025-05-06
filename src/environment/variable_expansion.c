/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:33:39 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/29 16:44:13 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles variable expansion during lexing by replacing $VAR or $? tokens    */
/* with their corresponding values from the shell environment or exit code.   */
/* Includes utilities to parse variable names, retrieve their values, and     */
/* append them to the current buffer using garbage-collected memory.          */
/* Supports special case expansion for the exit status variable `$?`.         */
/* Ensures robustness with fallback to empty string if variable is undefined. */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_variable_expansion(t_lexer *lexer, char **buffer, t_minishell *shell)
{
	char	*var_name;
	char	*var_value;
	int		status;

	advance_lexer_char(lexer);
	var_name = collect_variable_name(lexer, shell);
	if (!var_name)
		return (0);
	var_value = get_variable_value(var_name, shell);
	if (!var_value)
		var_value = "";
	status = append_to_buffer(buffer, var_value, shell);
	return (status);
}

char	*collect_variable_name(t_lexer *lexer, t_minishell *shell)
{
	char		*var_name;
	const char	*current_pos = lexer->str + lexer->pos;
	int			name_length;

	if (lexer->current_char == '?')
	{
		var_name = gc_strdup(&shell->gc_head, "?");
		advance_lexer_char(lexer);
		return (var_name);
	}
	else
	{
		name_length = get_variable_name_length(current_pos);
		if (name_length == 0)
			return (gc_strdup(&shell->gc_head, ""));
		var_name = gc_calloc(&shell->gc_head, name_length + 1, sizeof(char));
		if (!var_name)
			return (NULL);
		ft_strlcpy(var_name, current_pos, name_length + 1);
		lexer->pos += name_length;
		lexer->current_char = lexer->str[lexer->pos];
		return (var_name);
	}
}

int	get_variable_name_length(const char *str)
{
	int	length;

	if (*str == '?')
		return (1);
	length = 0;
	while (str[length] && (ft_isalnum(str[length]) || str[length] == '_'))
		length++;
	return (length);
}

char	*get_variable_value(const char *var_name, t_minishell *shell)
{
	char	*exit_status_str;
	int		i;
	size_t	len;

	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		exit_status_str = gc_itoa(&shell->gc_head, shell->last_exit_status);
		if (!exit_status_str)
			return ("");
		return (exit_status_str);
	}
	i = 0;
	len = ft_strlen(var_name);
	while (shell->env[i] != NULL)
	{
		if (ft_strncmp(shell->env[i], var_name, len) == 0
			&& shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return ("");
}

int	append_to_buffer(char **buffer, const char *str, t_minishell *shell)
{
	*buffer = gc_strjoin(&shell->gc_head, *buffer, str);
	if (!*buffer)
		return (0);
	return (1);
}
