/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:56:17 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/08 23:08:32 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles the execution flow for child and parent processes in command       */
/* execution. Manages redirections, validates commands, and handles errors    */
/* during execve. Ensures proper signal handling and error reporting for      */
/* command execution. Parent process waits for child termination and updates  */
/* the shell's exit status. Provides cleanup and graceful exit upon           */
/* encountering errors like permissions.                                      */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command_child(t_command *cmd, char **env, t_minishell *shell)
{
	t_files	*infile;
	t_files	*outfile;

	reset_signal_handlers(shell);
	infile = get_last_file(cmd->infile);
	outfile = get_last_file(cmd->outfile);
	setup_redirections(infile, outfile, shell);
	validate_command(cmd, shell);
	if (execve(cmd->path, cmd->args, env) == -1)
	{
		if (errno == EACCES || errno == ENOEXEC)
			print_error_and_exit(cmd->cmd_name, "Permission denied",
				SHELL_STATUS_CMD_NO_EXEC, shell);
		else
			print_error_and_exit(cmd->cmd_name, "command not found", 
				SHELL_STATUS_CMD_NOT_FOUND, shell);
	}
}

void	validate_command(t_command *cmd, t_minishell *shell)
{
	struct stat	st;

	if (stat(cmd->path, &st) == -1)
	{
		if (errno == ENOENT)
			print_error_and_exit(cmd->cmd_name, "command not found",
				SHELL_STATUS_CMD_NOT_FOUND, shell);
		else
			print_error_and_exit(cmd->cmd_name, strerror(errno),
				SHELL_STATUS_CMD_NOT_FOUND, shell);
	}
	if (S_ISDIR(st.st_mode))
		print_error_and_exit(cmd->cmd_name, "Is a directory",
			SHELL_STATUS_CMD_NO_EXEC, shell);
	if (access(cmd->path, X_OK) != 0)
	{
		if (errno == EACCES)
			print_error_and_exit(cmd->cmd_name, "Permission denied",
				SHELL_STATUS_CMD_NO_EXEC, shell);
		else
			print_error_and_exit(cmd->cmd_name, strerror(errno),
				SHELL_STATUS_CMD_NO_EXEC, shell);
	}
}

void	print_error_and_exit(char *cmd_name, char *message,
		int exit_code, t_minishell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	gc_free_all(shell->gc_head);
	exit(exit_code);
}

void	handle_parent_process(pid_t pid, t_minishell *shell)
{
	int	status;

	ignore_signal_handlers(shell);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
		shell->last_exit_status = SHELL_STATUS_GENERAL_ERROR;
	}
	else
	{
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->last_exit_status = SHELL_SIGNAL_BASE_STATUS
				+ WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit\n", STDERR_FILENO);
		}
		else
			shell->last_exit_status = SHELL_STATUS_GENERAL_ERROR;
	}
	setup_prompt_signals(shell);
}
