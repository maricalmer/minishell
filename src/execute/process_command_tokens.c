/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command_tokens.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 20:34:26 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 20:36:06 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	update cmd->args and cmd->cmd_name
	i: current index in cmd->args.
	return updated index */
int	process_builtin_cmd(t_command *cmd, t_token *token, t_minishell *shell,
		int i)
{
	cmd->cmd_name = gc_strdup(&shell->gc_head, token->value);
	if (!cmd->cmd_name)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	cmd->args[i] = gc_strdup(&shell->gc_head, token->value);
	if (!cmd->args[i])
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	return (i + 1);
}

/*	update cmd->args, cmd->cmd_name and cmd->path
	i: current index in cmd->args.
	return updated index */
int	process_extern_cmd(t_command *cmd, t_token *token, t_minishell *shell,
		int i)
{
	cmd->cmd_name = gc_strdup(&shell->gc_head, token->value);
	if (!cmd->cmd_name)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	cmd->args[i] = gc_strdup(&shell->gc_head, token->value);
	if (!cmd->args[i])
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	cmd->path = build_command_path(cmd->cmd_name, shell);
	return (i + 1);
}

/*	process redirect input token, update cmd->infile */
t_token	*process_redirect_in(t_command *cmd, t_token *token, t_minishell *shell)
{
	if (token->next && token->next->type == TOKEN_FILENAME)
	{
		add_infile_to_cmd(cmd, token->next->value, shell);
		if (shell->last_exit_status != 0)
			return (NULL);
		return (token->next->next);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		cmd->redirect_error_flag = 1;
		shell->last_exit_status = 2;
		return (token->next);
	}
}

/*	process redirect output token, update cmd->outfile */
t_token	*process_redirect_out(t_command *cmd, t_token *token,
		t_minishell *shell)
{
	if (token->next && token->next->type == TOKEN_FILENAME)
	{
		add_outfile_to_cmd(cmd, token->next->value, shell, 0);
		if (shell->last_exit_status != 0)
			return (NULL);
		return (token->next->next);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		cmd->redirect_error_flag = 1;
		shell->last_exit_status = 2;
		return (token->next);
	}
}

/*	process redirect append token, update cmd->outfile */
t_token	*process_redirect_append(t_command *cmd, t_token *token,
		t_minishell *shell)
{
	if (token->next && token->next->type == TOKEN_FILENAME)
	{
		add_outfile_to_cmd(cmd, token->next->value, shell, 1);
		if (shell->last_exit_status != 0)
			return (NULL);
		return (token->next->next);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		cmd->redirect_error_flag = 1;
		shell->last_exit_status = 2;
		return (token->next);
	}
}
