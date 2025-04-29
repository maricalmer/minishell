/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_creation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:46:35 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 06:55:05 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_files	*create_heredoc_file(char *delimiter, int is_quoted, t_minishell *shell)
{
	t_files	*new_file;

	new_file = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
	if (!new_file)
	{
		perror("malloc");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new_file->delim = delimiter;
	new_file->fd = -1;
	new_file->heredoc_quote = is_quoted;
	new_file->next = NULL;
	return (new_file);
}

t_list	*create_heredoc_list(t_minishell *shell)
{
	t_list	*new_list;

	new_list = gc_calloc(&shell->gc_head, 1, sizeof(t_list));
	if (!new_list)
	{
		perror("malloc");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new_list->content = NULL;
	new_list->next = NULL;
	return (new_list);
}

void	add_heredoc_to_list(t_files **head, t_files *new_file)
{
	t_files	*current;

	if (!*head)
		*head = new_file;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_file;
	}
}

void	add_list_to_outer_list(t_list **head, t_list *new_list)
{
	t_list	*current;

	if (!*head)
		*head = new_list;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_list;
	}
}
