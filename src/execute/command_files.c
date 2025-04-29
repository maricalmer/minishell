/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 20:42:48 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 22:44:04 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	create new file node for infile or outfile */
t_files	*create_new_file_node(t_minishell *shell)
{
	t_files	*new_file;

	new_file = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
	if (!new_file)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new_file->delim = NULL;
	new_file->next = NULL;
	return (new_file);
}

/*	add new file node to file list in cmd */
void	append_file_node(t_files **file_list, t_files *new_file)
{
	t_files	*current;

	if (!(*file_list))
		*file_list = new_file;
	else
	{
		current = *file_list;
		while (current->next)
			current = current->next;
		current->next = new_file;
	}
}

void	add_infile_to_cmd(t_command *cmd, char *filename, t_minishell *shell)
{
	t_files	*new_infile;

	new_infile = create_new_file_node(shell);
	new_infile->fd = open(filename, O_RDONLY, 0644);
	if (new_infile->fd < 0)
	{
		perror(filename);
		shell->last_exit_status = 1;
		cmd->redirect_error_flag = 1;
		new_infile->fd = open("/dev/null", O_RDONLY);
	}
	append_file_node(&cmd->infile, new_infile);
}

/*	add an outfile to the command.
	append_flag: flag indicating whether to append (1) or truncate (0) */
void	add_outfile_to_cmd(t_command *cmd, char *filename, t_minishell *shell,
		int append_flag)
{
	t_files	*new_outfile;

	new_outfile = create_new_file_node(shell);
	if (append_flag)
		new_outfile->fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		new_outfile->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_outfile->fd < 0)
	{
		perror(filename);
		shell->last_exit_status = 1;
		cmd->redirect_error_flag = 1;
		new_outfile->fd = open("/dev/null", O_WRONLY);
	}
	append_file_node(&cmd->outfile, new_outfile);
}
