/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 08:57:29 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/16 17:10:31 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	check if cmd path exists, handle errors if not or exec cmd by forking a
	child process */
void	execute_external(t_command *cmd, char **env, t_minishell *shell)
{
	if (!cmd->path)
	{
		handle_command_not_found(cmd->cmd_name, shell);
		return ;
	}
	fork_and_execute(cmd, env, shell);
}

/* print an error message and set last exit status to 127 */
void	handle_command_not_found(char *cmd_name, t_minishell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	shell->last_exit_status = 127;
}

/*	fork current process; in child process, exec external cmd;
	in parent process, wait for child and update exit status */
void	fork_and_execute(t_command *cmd, char **env, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_command_child(cmd, env, shell);
	else if (pid > 0)
		handle_parent_process(pid, shell);
	else
	{
		perror("minishell: fork");
		shell->last_exit_status = 1;
	}
}
