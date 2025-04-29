/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:43:11 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/16 21:46:37 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setup_pipe_signals(t_minishell *shell)
{
	setup_sigint_handler_at_pipe(shell);
	setup_sigquit_handler(shell);
	setup_sigpipe_handler(shell);
}

void	setup_sigint_handler_at_pipe(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_at_pipe;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

void	handle_sigint_at_pipe(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_redisplay();
	g_received_signal = sig;
}
