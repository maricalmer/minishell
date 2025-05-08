/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:23:19 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 23:08:58 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Manages the creation and execution of processes involved in a pipe         */
/* operation. The file handles setting up pipe file descriptors, forking      */
/* child processes, and redirecting input/output between the processes. It    */
/* also waits for the processes to finish and updates the shell's exit status */
/* based on the result.                                                       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipe(t_ast_node *node, t_minishell *shell)
{
	int		fds[2];
	pid_t	pids[2];
	int		status_left;
	int		status_right;

	if (create_pipe_fds(fds))
		return ;
	shell->in_pipe = 1;
	setup_pipe_signals(shell);
	pids[0] = fork_left_child(fds, node->left, shell);
	pids[1] = fork_right_child(fds, node->right, shell);
	close(fds[READ_END]);
	close(fds[WRITE_END]);
	if (waitpid(pids[0], &status_left, 0) == -1)
		perror("waitpid");
	if (waitpid(pids[1], &status_right, 0) == -1)
		perror("waitpid");
	if (WIFEXITED(status_right))
		shell->last_exit_status = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
		shell->last_exit_status = SHELL_SIGNAL_BASE_STATUS
			+ WTERMSIG(status_right);
	setup_prompt_signals(shell);
	shell->in_pipe = 0;
}

int	create_pipe_fds(int fds[2])
{
	if (pipe(fds) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

pid_t	fork_left_child(int fds[], t_ast_node *node, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(fds[READ_END]);
		if (dup2(fds[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			gc_free_all(shell->gc_head);
			exit(EXIT_FAILURE);
		}
		close(fds[WRITE_END]);
		read_tree(node, shell);
		gc_free_all(shell->gc_head);
		exit(shell->last_exit_status);
	}
	return (pid);
}

pid_t	fork_right_child(int fds[], t_ast_node *node, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(fds[WRITE_END]);
		if (dup2(fds[READ_END], STDIN_FILENO) == -1)
		{
			perror("dup2");
			gc_free_all(shell->gc_head);
			exit(EXIT_FAILURE);
		}
		close(fds[READ_END]);
		read_tree(node, shell);
		gc_free_all(shell->gc_head);
		exit(shell->last_exit_status);
	}
	return (pid);
}
