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

/* ************************************************************************** */
/*                                                                            */
/* Handles input and output redirections by duplicating file descriptors.     */
/* Ensures proper redirection for both standard input and output during       */
/* command execution, using file nodes representing the input and output      */
/* files. The function safely handles errors in redirection setup, exiting on */
/* failure.                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_files	*get_last_file(t_files *files)
{
	if (!files)
		return (NULL);
	while (files->next)
		files = files->next;
	return (files);
}

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
