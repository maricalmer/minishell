/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:56:17 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 19:40:59 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	reset signal handlers, setup redirections, validate the command
	and attempt to execute it; handle any execution errors appropriately */
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
			print_error_and_exit(cmd->cmd_name, "Permission denied", 126,
				shell);
		else
			print_error_and_exit(cmd->cmd_name, "command not found", 127,
				shell);
	}
}

/*	validate cmd by checking its existence, check if its a dir & if its exec;
	if a check fails, print appropriate error msg and exit with correct code */
void	validate_command(t_command *cmd, t_minishell *shell)
{
	struct stat	st;

	if (stat(cmd->path, &st) == -1)
	{
		if (errno == ENOENT)
			print_error_and_exit(cmd->cmd_name, "command not found", 127,
				shell);
		else
			print_error_and_exit(cmd->cmd_name, strerror(errno), 127, shell);
	}
	if (S_ISDIR(st.st_mode))
		print_error_and_exit(cmd->cmd_name, "Is a directory", 126, shell);
	if (access(cmd->path, X_OK) != 0)
	{
		if (errno == EACCES)
			print_error_and_exit(cmd->cmd_name, "Permission denied", 126,
				shell);
		else
			print_error_and_exit(cmd->cmd_name, strerror(errno), 126, shell);
	}
}

/*	print error msg to STDERR, free gc, exit with the given status code */
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

/*	wait for child process to finish, update last exit status based on
	child's exit status or termination signal, and setup prompt signals */
void	handle_parent_process(pid_t pid, t_minishell *shell)
{
	int	status;

	ignore_signal_handlers(shell);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
		shell->last_exit_status = 1;
	}
	else
	{
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->last_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit\n", STDERR_FILENO);
		}
		else
			shell->last_exit_status = 1;
	}
	setup_prompt_signals(shell);
}
