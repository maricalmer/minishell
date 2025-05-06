/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:47:34 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 05:22:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Manages signal handling for the shell's prompt, configuring handlers for   */
/* SIGINT, SIGQUIT, and SIGPIPE to ensure appropriate behavior during user    */
/* input. SIGINT triggers a prompt reset, while SIGQUIT and SIGPIPE are       */
/* ignored to prevent interruption. Proper error handling is included for     */
/* signal setup.                                                              */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_prompt_signals(t_minishell *shell)
{
	setup_sigint_handler(shell);
	setup_sigquit_handler(shell);
	setup_sigpipe_handler(shell);
}

void	setup_sigint_handler(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_at_prompt;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

void	handle_sigint_at_prompt(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_received_signal = sig;
}

void	setup_sigquit_handler(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

void	setup_sigpipe_handler(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}
