/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:15:52 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/13 13:51:48 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles signal management for child processes. It includes functions to    */
/* reset signal handlers to default behaviors and to ignore certain signals   */
/* (e.g., SIGINT and SIGQUIT) during command execution, preventing the shell  */
/* from being interrupted by these signals. Proper error handling is provided */
/* in case of failure when setting signal actions.                            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_signal_handlers(t_minishell *shell)
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
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
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
}

void	ignore_signal_handlers(t_minishell *shell)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}
