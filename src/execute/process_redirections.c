/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:01:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/18 01:59:50 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	traverses linked list to find the last node, i.e. the last file
	return pointer to last file node, or NULL if the list is empty */
t_files	*get_last_file(t_files *files)
{
	if (!files)
		return (NULL);
	while (files->next)
		files = files->next;
	return (files);
}

/*	do input & output redirects by dup fds for input & output specified in cmd
	infile: pointer to last input file node
	outfile: pointer to last output file node */
void	setup_redirections(t_files *infile, t_files *outfile,
		t_minishell *shell)
{
	if (infile)
	{
		if (dup2(infile->fd, STDIN_FILENO) == -1)
		{
			perror("dup2 infile");
			gc_free_all(shell->gc_head);
			exit(1);
		}
	}
	if (outfile)
	{
		if (dup2(outfile->fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 outfile");
			gc_free_all(shell->gc_head);
			exit(1);
		}
	}
}
