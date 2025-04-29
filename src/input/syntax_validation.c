/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:29:20 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/18 14:09:15 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_syntax_error(char *input, t_minishell *shell)
{
	if (!validate_heredoc_delimiter(input) || !validate_delimiter_left(input)
		|| !validate_delimiter_right(input) || !validate_delimiter_middle(input)
		|| !validate_balanced_parenthesis(input)
		|| !validate_balanced_quotes(input)
		|| !validate_opening_parenthesis(input)
		|| !validate_closing_parenthesis(input)
		|| !validate_no_empty_parenthesis(input))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		shell->last_exit_status = 2;
		add_history(input);
		free(input);
		return (1);
	}
	if (!ft_strncmp(input, "''", 3) || !ft_strncmp(input, "\"\"", 5))
	{
		ft_putstr_fd("Command '' not found\n", 2);
		shell->last_exit_status = 127;
		add_history(input);
		free(input);
		return (1);
	}
	return (0);
}

int	validate_heredoc_delimiter(char *str)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = '\0';
	while (*str)
	{
		if (handle_quotes(str, &in_quotes, &quote_char))
			str++;
		else if (in_quotes)
			str++;
		else if (ft_strncmp(str, "<<", 2) == 0 && !check_heredoc_char(&str))
			return (0);
		else
			str++;
	}
	return (1);
}

int	validate_delimiter_left(const char *str)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = '\0';
	str = skip_whitespace_input((char *)str);
	while (*str)
	{
		if (handle_quotes((char *)str, &in_quotes, &quote_char))
			str++;
		else if (in_quotes)
			str++;
		else
			return (check_left_char(str));
		str++;
	}
	return (1);
}

int	validate_delimiter_right(const char *str)
{
	const char	*last_valid = NULL;

	while (*str != '\0')
	{
		if (*str == '|' || (*str == '&' && *(str + 1) == '&'))
		{
			last_valid = str;
			if (*str == '|')
				str++;
			else
				str += 2;
		}
		else
		{
			str++;
		}
	}
	if (last_valid)
	{
		str = skip_whitespace_input((char *)(last_valid + 1));
		if (*str == '\0')
			return (0);
	}
	return (1);
}

int	validate_delimiter_middle(const char *str)
{
	int			in_quotes;
	char		quote_char;
	const char	*new_str;

	in_quotes = 0;
	quote_char = '\0';
	while (*str != '\0')
	{
		new_str = process_delimiter_char(str, &in_quotes, &quote_char);
		if (!new_str)
			return (0);
		str = new_str;
	}
	return (1);
}
