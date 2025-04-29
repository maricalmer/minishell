/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:18:42 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/09 12:47:42 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_heredoc_token_in_input(char **input, t_minishell *shell,
		t_list **outer_list, t_list **current_list)
{
	char	*delimiter;
	int		is_quoted;
	t_files	*new_heredoc;

	(*input) += 2;
	while (ft_iswhitespace(**input))
		(*input)++;
	delimiter = get_delimiter(*input, shell);
	is_quoted = is_quoted_delimiter(delimiter);
	new_heredoc = create_heredoc_file(delimiter, is_quoted, shell);
	if (!*current_list)
	{
		*current_list = create_heredoc_list(shell);
		add_list_to_outer_list(outer_list, *current_list);
	}
	add_heredoc_to_list((t_files **)&(*current_list)->content, new_heredoc);
}

void	handle_pipe_token(char **input, t_list **current_list)
{
	*current_list = NULL;
	if (**input == '&')
		(*input)++;
}

void	heredoc_scan(char *input, t_minishell *shell)
{
	t_list	*outer_list;
	t_list	*current_list;

	outer_list = NULL;
	current_list = NULL;
	while (*input)
	{
		if (ft_strncmp(input, "<<", 2) == 0)
			handle_heredoc_token_in_input(&input, shell, &outer_list,
				&current_list);
		if (*input == '|' || (*input == '&' && *(input + 1) == '&'))
			handle_pipe_token(&input, &current_list);
		input++;
	}
	shell->heredocs = outer_list;
}

char	*get_delimiter(char *input, t_minishell *shell)
{
	int		i;
	char	*delimiter;

	delimiter = gc_strdup(&shell->gc_head, input);
	if (!delimiter)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (delimiter[i] && !ft_iswhitespace(delimiter[i]) && delimiter[i] != '|'
		&& delimiter[i] != '&')
		i++;
	delimiter[i] = '\0';
	return (delimiter);
}

int	is_quoted_delimiter(char *delimiter)
{
	int	i;

	i = 0;
	if (!delimiter)
		return (0);
	while (delimiter[i])
	{
		if (delimiter[i] == '"' || delimiter[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}
