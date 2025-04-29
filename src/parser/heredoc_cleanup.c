/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:33:28 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 19:24:50 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_heredoc_list(t_minishell *shell)
{
	t_files	*tmp;
	t_files	*buf;
	t_list	*old_head;
	t_list	*last;

	if (!shell->heredocs || !shell->heredocs->content)
		return ;
	tmp = (t_files *)(shell->heredocs->content);
	while (tmp)
	{
		buf = tmp->next;
		close(tmp->fd);
		tmp = buf;
	}
	old_head = shell->heredocs;
	last = ft_lstlast(shell->heredocs);
	shell->heredocs = shell->heredocs->next;
	last->next = old_head;
	old_head->next = NULL;
}

void	close_all_heredocs(t_minishell *shell)
{
	t_list	*outer_node;
	t_files	*inner_node;

	outer_node = shell->heredocs;
	while (outer_node)
	{
		inner_node = (t_files *)outer_node->content;
		while (inner_node)
		{
			if (inner_node->fd != -1)
			{
				if (close(inner_node->fd) == -1)
					perror("minishell: close heredoc fd");
				inner_node->fd = -1;
			}
			inner_node = inner_node->next;
		}
		outer_node = outer_node->next;
	}
}

void	heredoc_warning_message(void)
{
	printf("minishell: warning: here-document at current ");
	printf("line delimited by end-of-file (wanted specified delimiter)\n");
}
