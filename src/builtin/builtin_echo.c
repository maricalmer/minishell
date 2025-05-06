/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:40:59 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/11 22:55:45 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Implements the `echo` builtin command, supporting multiple `-n` flags      */
/* to suppress the trailing newline. Handles output redirection if specified  */
/* in the command structure. Parses arguments and writes them to the proper   */
/* file descriptor with appropriate spacing and newline logic. Always         */
/* returns 0, as echo does not fail under normal conditions.                  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(t_command *cmd)
{
	int		i;
	int		newline;
	t_files	*output;
	int		output_fd;

	i = 1;
	newline = 1;
	parse_echo_flags(cmd->args, &i, &newline);
	output = get_last_file(cmd->outfile);
	if (output)
		output_fd = output->fd;
	else
		output_fd = STDOUT_FILENO;
	print_arguments(cmd->args, i, newline, output_fd);
	return (0);
}

void	parse_echo_flags(char **args, int *i, int *newline)
{
	while (args[*i] && is_all_n_flag(args[*i]))
	{
		*newline = 0;
		(*i)++;
	}
}

int	is_all_n_flag(char *arg)
{
	int	j;

	if (arg[0] != '-')
		return (0);
	j = 1;
	while (arg[j] && arg[j] == 'n')
		j++;
	if (arg[j] == '\0')
		return (1);
	return (0);
}

void	print_arguments(char **args, int i, int newline, int output_fd)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], output_fd);
		if (args[i + 1])
			ft_putstr_fd(" ", output_fd);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", output_fd);
}
