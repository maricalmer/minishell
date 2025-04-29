/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 04:24:57 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/17 23:30:10 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_sigint_at_child_heredoc(int sig);

void	setup_heredoc_signals(t_minishell *shell)
{
	setup_sigint_handler_at_heredoc(shell);
	setup_sigquit_handler(shell);
	setup_sigpipe_handler(shell);
}

void	setup_sigint_handler_at_heredoc(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_at_heredoc;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

void	handle_sigint_at_heredoc(int sig)
{
	rl_redisplay();
	g_received_signal = sig;
}

void	ignore_sigquit_at_heredoc(t_minishell *shell)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

void	reset_sigint_at_heredoc(t_minishell *shell)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	if (g_received_signal == 2)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

/*	signal handler for SIGINT (Ctrl+C) at prompt */
// void	handle_sigint_at_child_heredoc(int sig)
// {
// 	g_received_signal = sig;
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	write(STDOUT_FILENO, "Press enter", 11);
// }
