/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:18:42 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 22:53:28 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Initializes heredoc processing by setting up a pipe and forking a child.   */
/* The child reads heredoc input; the parent waits and handles interruptions. */
/* Signal handlers are configured to ensure proper shell behavior.            */
/* Associates an available heredoc fd with its corresponding file entry.      */
/* Gracefully handles SIGINT during heredoc to maintain shell state.          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_heredoc(t_minishell *shell, t_token *token)
{
	int		fd[2];
	t_files	*heredoc;

	if (create_heredoc_pipe(fd) == -1)
		return ;
	setup_sigquit_handler(shell);
	setup_sigint_handler(shell);
	heredoc = save_heredoc_fd(shell->heredocs, fd[0]);
	fork_heredoc_child(shell, token, fd, heredoc);
}

int	create_heredoc_pipe(int fd[2])
{
	if (pipe(fd) < 0)
		return (-1);
	return (0);
}

void	fork_heredoc_child(t_minishell *shell, t_token *token, int fd[2],
		t_files *heredoc)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		reset_sigint_at_heredoc(shell);
		ignore_sigquit_at_heredoc(shell);
		heredoc_loop(shell, token, fd, heredoc);
	}
	else if (pid > 0)
		handle_heredoc_parent(pid, shell, fd);
	else
		perror("minishell: fork");
}

void	handle_heredoc_parent(pid_t pid, t_minishell *shell, int fd[2])
{
	int	status;

	setup_heredoc_signals(shell);
	if (waitpid(pid, &status, 0) == -1)
		perror("minishell: waitpid");
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		shell->sigint_heredocs = 1;
		shell->last_exit_status = SHELL_STATUS_SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		close_all_heredocs(shell);
	}
	setup_prompt_signals(shell);
	close(fd[1]);
}

t_files	*save_heredoc_fd(t_list *heredoc_list, int new_fd)
{
	t_list	*outer;
	t_files	*inner;

	if (!heredoc_list)
		return (NULL);
	outer = heredoc_list;
	while (outer)
	{
		inner = (t_files *)outer->content;
		while (inner)
		{
			if (inner->fd == -1)
			{
				inner->fd = new_fd;
				return (inner);
			}
			inner = inner->next;
		}
		outer = outer->next;
	}
	return (NULL);
}
