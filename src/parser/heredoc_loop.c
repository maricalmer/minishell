/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:29:50 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 23:31:54 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* "close_all_heredocs(shell);" after "close(pipe[1]);"? */
		// if (g_received_signal == 2)
		// {
		// 	g_received_signal = 0;
		// 	break ;
		// }
void	heredoc_loop(t_minishell *shell, t_token *token, int *pipe,
		t_files *heredoc)
{
	char	*buffer;
	char	*delimiter;

	buffer = NULL;
	delimiter = token->next->value;
	close(pipe[0]);
	while (1)
	{
		buffer = readline("> ");
		if (is_heredoc_delimiter(buffer, delimiter))
			break ;
		if (buffer == NULL)
		{
			heredoc_warning_message();
			break ;
		}
		catch_heredoc_input(shell, buffer, pipe[1], heredoc);
	}
	close(pipe[1]);
	gc_free_all(shell->gc_head);
	rl_clear_history();
	exit(0);
}

int	is_heredoc_delimiter(const char *input, const char *delimiter)
{
	size_t	delimiter_len;

	if (!input || !delimiter)
		return (0);
	delimiter_len = ft_strlen(delimiter);
	if (ft_strncmp(input, delimiter, delimiter_len) == 0)
	{
		if (input[delimiter_len] == '\0' || input[delimiter_len] == '\n')
			return (1);
	}
	return (0);
}

/* "ft_putchar_fd('\n', fd);" might be unnecessary? */
void	catch_heredoc_input(t_minishell *shell, char *str, int fd,
		t_files *heredoc)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && !heredoc->heredoc_quote)
			i += expend_in_heredoc(shell, str + i, fd);
		else
		{
			ft_putchar_fd(str[i], fd);
			i++;
		}
	}
	ft_putchar_fd('\n', fd);
	return ;
}

int	expend_in_heredoc(t_minishell *shell, char *str, int fd)
{
	int		length;
	char	save;
	char	*expanded_var;

	length = 1;
	while (str[length] && str[length] != '$' && !ft_iswhitespace(str[length]))
		length++;
	save = str[length];
	str[length] = '\0';
	expanded_var = get_env_variable(str + 1, shell->env);
	if (expanded_var)
		ft_putstr_fd(expanded_var, fd);
	str[length] = save;
	return (length);
}

char	*get_env_variable(const char *var_name, char **env)
{
	size_t	name_len;
	char	*current_var;

	if (!var_name || !env)
		return (NULL);
	name_len = ft_strlen(var_name);
	while (*env)
	{
		current_var = *env;
		if (ft_strncmp(current_var, var_name, name_len) == 0
			&& current_var[name_len] == '=')
			return (current_var + name_len + 1);
		env++;
	}
	return (NULL);
}
