/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:09:18 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/18 19:10:20 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* main function to handle exit command */
int	handle_exit_command(char **args, t_minishell *shell)
{
	int		exit_status;

	if (ft_strncmp(args[0], "exit", 4) != 0)
		return (0);
	if (shell->in_pipe == 0)
		printf("exit\n");
	exit_status = handle_exit_with_arguments(args, shell);
	if (exit_status >= 0 && exit_status <= 255)
	{
		rl_clear_history();
		gc_free_all(shell->gc_head);
		exit(exit_status);
	}
	return (1);
}

/* handle exit command with arguments */
int	handle_exit_with_arguments(char **args, t_minishell *shell)
{
	int	arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1)
		return (shell->last_exit_status);
	else if (arg_count == 2)
		return (handle_numeric_argument(args[1], shell));
	else
	{
		if (is_numeric_argument(args[1]))
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
			shell->last_exit_status = 1;
			return (-1);
		}
		else
		{
			print_numeric_argument_required(args[1]);
			shell->last_exit_status = 2;
			return (2);
		}
	}
}

/* handle numeric argument for exit */
int	handle_numeric_argument(char *arg, t_minishell *shell)
{
	char	*cleaned_arg;
	char	*trimmed_arg;

	cleaned_arg = remove_surrounding_quotes(arg, shell);
	if (!cleaned_arg)
		return (2);
	trimmed_arg = gc_strtrim(&shell->gc_head, cleaned_arg, " \t\n\v\f\r");
	if (!trimmed_arg)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
	if (is_numeric_argument(trimmed_arg))
		return ((unsigned char)ft_atol(trimmed_arg));
	else
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
}

char	*remove_surrounding_quotes(char *arg, t_minishell *shell)
{
	size_t	len;
	char	first_char;

	if (!arg)
		return (NULL);
	len = ft_strlen(arg);
	if (len > 1)
	{
		first_char = arg[0];
		if ((first_char == '\'' || first_char == '\"') && arg[len
				- 1] == first_char)
			return (gc_substr(&shell->gc_head, arg, 1, len - 2));
	}
	return (gc_strdup(&shell->gc_head, arg));
}

/* check if a string represents a numeric argument */
int	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}
