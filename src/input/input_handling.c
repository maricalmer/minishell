/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:48:08 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/08 23:08:46 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles the main input loop for the shell.                                 */
/*  - Rejects command-line arguments and exits if any are given.              */
/*  - Reads user input, checks for signals, and handles edge cases.           */
/*  - Skips empty or syntactically incorrect commands.                        */
/*  - Processes valid input through lexing, parsing, and execution.           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_input_loop(int ac, char **av, t_minishell *shell)
{
	char	*input;

	handle_multiple_args(ac, av, shell);
	while (1)
	{
		input = read_user_input();
		if (g_received_signal)
		{
			shell->last_exit_status = SHELL_SIGNAL_BASE_STATUS
				+ g_received_signal;
			g_received_signal = 0;
		}
		if (input == NULL)
			break ;
		if (handle_empty_input(input))
			continue ;
		if (handle_syntax_error(input, shell))
			continue ;
		process_valid_input(input, shell);
	}
}

void	handle_multiple_args(int ac, char **av, t_minishell *shell)
{
	if (ac != 1 || av[1])
	{
		printf("minishell does not accept arguments\n");
		gc_free_all(shell->gc_head);
		exit(0);
	}
}

char	*read_user_input(void)
{
	char	*input;

	input = readline("minishell$> ");
	if (input == NULL)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		return (NULL);
	}
	return (input);
}

int	handle_empty_input(char *input)
{
	if (*input == '\0')
	{
		free(input);
		return (1);
	}
	return (0);
}

void	process_valid_input(char *input, t_minishell *shell)
{
	add_history(input);
	heredoc_scan(input, shell);
	run_lexer(input, shell);
	if (shell->tokens == NULL)
	{
		free(input);
		return ;
	}
	shell->ast_root = parse_expression(shell, MIN_PRECEDENCE_LVL);
	read_tree(shell->ast_root, shell);
	free(input);
}
