/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:32:58 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 00:26:10 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	initialize command based on tokens */
void	init_command(t_command *cmd, t_token *node_tokens, t_minishell *shell)
{
	int	arg_count;
	int	i;

	initialize_command_struct(cmd);
	cmd->has_heredoc = 0;
	cmd->redirect_error_flag = 0;
	arg_count = count_arg_tokens(node_tokens);
	i = 0;
	cmd->args = gc_calloc(&shell->gc_head, arg_count + 2, sizeof(char *));
	if (!cmd->args)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	while (node_tokens)
		node_tokens = process_token(cmd, node_tokens, shell, &i);
	cmd->args[i] = NULL;
	shell->cmd = cmd;
}

void	initialize_command_struct(t_command *cmd)
{
	cmd->cmd_name = NULL;
	cmd->path = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
}

int	count_arg_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_ARGUMENT)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

/*	process a single token and update cmd struct
	i: pointer to current index in cmd->args.
	return the next token to process */
t_token	*process_token(t_command *cmd, t_token *token, t_minishell *shell,
		int *i)
{
	if (token->type == TOKEN_BUILTIN_CMD)
	{
		*i = process_builtin_cmd(cmd, token, shell, *i);
		return (token->next);
	}
	else if (token->type == TOKEN_EXTERN_CMD)
	{
		*i = process_extern_cmd(cmd, token, shell, *i);
		return (token->next);
	}
	else if (token->type == TOKEN_ARGUMENT)
	{
		*i = process_argument(cmd, token, shell, *i);
		return (token->next);
	}
	else if (token->type == TOKEN_REDIRECT_IN)
		return (process_redirect_in(cmd, token, shell));
	else if (token->type == TOKEN_HEREDOC)
		return (handle_heredoc_token(cmd, token, shell));
	else if (token->type == TOKEN_REDIRECT_OUT)
		return (process_redirect_out(cmd, token, shell));
	else if (token->type == TOKEN_REDIRECT_APPEND)
		return (process_redirect_append(cmd, token, shell));
	return (token->next);
}

int	process_argument(t_command *cmd, t_token *token, t_minishell *shell, int i)
{
	cmd->args[i] = gc_strdup(&shell->gc_head, token->value);
	if (!cmd->args[i])
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	return (i + 1);
}
